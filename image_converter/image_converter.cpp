//
// image_converter.cpp - image converter implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "image_converter.h"
#include "../error/win_error.h"
#include "../form_common.h"

#include <wincodec.h>
#pragma comment (lib, "Windowscodecs.lib")

#include <comdef.h>	// for _com_error

using liblec::lecui::safe_release;

// create a stream object initialized with the data from an executable resource
IStream* create_stream_on_resource(
	HMODULE h_module,
	LPCTSTR lp_name,
	LPCTSTR lp_type,
	std::string& error) {
	// initialize return value
	IStream* ip_stream = nullptr;

	DWORD resource_size = NULL;
	HGLOBAL hglb_image = nullptr;
	LPVOID pv_source_resource_data = nullptr;
	HGLOBAL hgbl_resource_data = nullptr;
	LPVOID pv_resource_data = nullptr;

	// find the resource
	HRSRC hrsrc = FindResource(h_module, lp_name, lp_type);
	if (hrsrc == nullptr)
		goto Return;

	// load the resource
	resource_size = SizeofResource(h_module, hrsrc);
	hglb_image = LoadResource(h_module, hrsrc);
	if (hglb_image == nullptr)
		goto Return;

	// lock the resource, getting a pointer to its data
	pv_source_resource_data = LockResource(hglb_image);
	if (pv_source_resource_data == nullptr)
		goto Return;

	// allocate memory to hold the resource data
	hgbl_resource_data = GlobalAlloc(GMEM_MOVEABLE, resource_size);
	if (hgbl_resource_data == nullptr)
		goto Return;

	// get a pointer to the allocated memory
	pv_resource_data = GlobalLock(hgbl_resource_data);
	if (pv_resource_data == nullptr)
		goto FreeData;

	// copy the data from the resource to the new memory block
	CopyMemory(pv_resource_data, pv_source_resource_data, resource_size);
	GlobalUnlock(hgbl_resource_data);

	// create a stream on the HGLOBAL containing the data
	if (SUCCEEDED(CreateStreamOnHGlobal(hgbl_resource_data, TRUE, &ip_stream)))
		goto Return;

FreeData:
	// couldn't create stream; free the memory
	GlobalFree(hgbl_resource_data);

Return:
	if (ip_stream == nullptr)
		error = get_last_error();

	// no need to unlock or free the resource
	return ip_stream;
}

// Load a PNG image from the specified stream (using Windows Imaging Component)
IWICBitmapSource* load_bitmap_from_stream(
	IStream* ip_image_stream,
	float scale,
	std::string& error) {
	// initialize return value
	IWICBitmapSource* ip_bitmap = nullptr;

	// load WIC's PNG decoder
	IWICBitmapDecoder* ip_decoder = nullptr;

	UINT nFrameCount = 0;

	IWICBitmapFrameDecode* ip_frame = nullptr;
	IWICImagingFactory* p_factory = nullptr;
	IWICBitmapScaler* ip_scaler = nullptr;

	// create factory
	HRESULT result = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&p_factory));

	if (FAILED(result)) {
		_com_error err(result);
		error = liblec::lecui::convert_string(err.ErrorMessage());
		goto Return;
	}

	// Create a decoder for the stream.
	result = p_factory->CreateDecoderFromStream(
		ip_image_stream,
		NULL,
		WICDecodeMetadataCacheOnLoad,
		&ip_decoder
	);

	if (FAILED(result)) {
		_com_error err(result);
		error = liblec::lecui::convert_string(err.ErrorMessage());
		goto ReleaseDecoder;
	}

	// check for the presence of the first frame in the bitmap
	result = ip_decoder->GetFrameCount(&nFrameCount);

	if (FAILED(result) || nFrameCount != 1) {
		_com_error err(result);
		error = liblec::lecui::convert_string(err.ErrorMessage());
		goto ReleaseDecoder;
	}

	// load the first frame (i.e., the image)
	result = ip_decoder->GetFrame(0, &ip_frame);

	if (FAILED(result)) {
		_com_error err(result);
		error = liblec::lecui::convert_string(err.ErrorMessage());
		goto ReleaseDecoder;
	}

	if (scale == 1.f) {
		// convert the image to 32bpp BGRA format with pre-multiplied alpha
		//   (it may not be stored in that format natively in the PNG resource,
		//   but we need this format to create the DIB to use on-screen)
		result = WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, ip_frame, &ip_bitmap);
	}
	else {
		UINT width, height;
		ip_frame->GetSize(&width, &height);

		result = p_factory->CreateBitmapScaler(&ip_scaler);

		if (FAILED(result)) {
			_com_error err(result);
			error = liblec::lecui::convert_string(err.ErrorMessage());
			goto ReleaseDecoder;
		}

		result = ip_scaler->Initialize(
			ip_frame,
			static_cast<UINT>(width * scale),
			static_cast<UINT>(height * scale),
			WICBitmapInterpolationModeHighQualityCubic
		);

		if (FAILED(result)) {
			_com_error err(result);
			error = liblec::lecui::convert_string(err.ErrorMessage());
			goto ReleaseDecoder;
		}

		// convert the image to 32bpp BGRA format with pre-multiplied alpha
		//   (it may not be stored in that format natively in the PNG resource,
		//   but we need this format to create the DIB to use on-screen)
		result = WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, ip_scaler, &ip_bitmap);
	}
	safe_release(&ip_frame);

	if (FAILED(result)) {
		_com_error err(result);
		error = liblec::lecui::convert_string(err.ErrorMessage());
	}

ReleaseDecoder:
	safe_release(&ip_decoder);
	safe_release(&p_factory);
	safe_release(&ip_scaler);
Return:
	return ip_bitmap;
}

// Creates a 32-bit DIB from the specified WIC bitmap
HBITMAP create_HBITMAP(IWICBitmapSource* ipBitmap) {
	// initialize return value
	HBITMAP h_bmp = nullptr;

	void* pv_image_bits = nullptr;

	// get image attributes and check for valid image
	UINT width = 0, height = 0;
	if (FAILED(ipBitmap->GetSize(&width, &height)) || width == 0 || height == 0)
		goto Return;

	// prepare structure giving bitmap information (negative height indicates a top-down DIB)
	BITMAPINFO bminfo;
	ZeroMemory(&bminfo, sizeof(bminfo));
	bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bminfo.bmiHeader.biWidth = width;
	bminfo.bmiHeader.biHeight = -((LONG)height);
	bminfo.bmiHeader.biPlanes = 1;
	bminfo.bmiHeader.biBitCount = 32;
	bminfo.bmiHeader.biCompression = BI_RGB;

	// create a DIB section that can hold the image
	{
		HDC hdcScreen = GetDC(nullptr);
		h_bmp = CreateDIBSection(hdcScreen, &bminfo, DIB_RGB_COLORS, &pv_image_bits, nullptr, 0);
		ReleaseDC(nullptr, hdcScreen);
	}

	if (h_bmp == nullptr)
		goto Return;

	// extract the image into the HBITMAP
	{
		const UINT cbStride = width * 4;
		const UINT cbImage = cbStride * height;
		if (FAILED(ipBitmap->CopyPixels(nullptr, cbStride, cbImage, static_cast<BYTE*>(pv_image_bits)))) {
			// couldn't extract image; delete HBITMAP
			DeleteObject(h_bmp);
			h_bmp = nullptr;
		}
	}

Return:
	return h_bmp;
}

HBITMAP image_converter::png_to_argb(HMODULE h_module, int id_png, float scale, std::string& error) {
	HBITMAP h_bmp = nullptr;

	// load the PNG image data into a stream
	IStream* ip_image_stream = create_stream_on_resource(h_module, MAKEINTRESOURCE(id_png), L"PNG", error);

	if (ip_image_stream == nullptr)
		goto Return;

	// load the bitmap with WIC
	{
		IWICBitmapSource* ip_bitmap = load_bitmap_from_stream(ip_image_stream, scale, error);

		if (ip_bitmap == nullptr)
			goto ReleaseStream;

		// create a HBITMAP containing the image
		h_bmp = create_HBITMAP(ip_bitmap);
		safe_release(&ip_bitmap);
	}

ReleaseStream:
	safe_release(&ip_image_stream);

Return:
	return h_bmp;
}
