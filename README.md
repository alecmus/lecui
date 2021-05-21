# lecui
lecui user interface library

<p>
  <img alt="Lines of code" src="https://img.shields.io/tokei/lines/github/alecmus/lecui">
  <img alt="GitHub issues" src="https://img.shields.io/github/issues-raw/alecmus/lecui">
  <img alt="GitHub closed issues" src="https://img.shields.io/github/issues-closed-raw/alecmus/lecui">
</p>

#### Release Info
<p>
  <img alt="GitHub release (latest by date)" src="https://img.shields.io/github/v/release/alecmus/lecui">
  <img alt="GitHub Release Date" src="https://img.shields.io/github/release-date/alecmus/lecui">
  <img alt="GitHub all releases" src="https://img.shields.io/github/downloads/alecmus/lecui/total">
</p>

#### Commit Info
<p>
  <img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/alecmus/lecui">
  <img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/y/alecmus/lecui">
</p>

### About the Library
The lecui user interface library is designed for the rapid development of modern, efficient and
easy to maintain C++ applications with a graphical user interface (gui). It is part of the
[liblec libraries](https://github.com/alecmus/liblec).

### Documentation
The library uses embedded XML-style documentation. An HTML version of this documentation is available [here](https://alecmus.github.io/lecui).

### Successor to the cui framework
The lecui user interface library is designed to be a successor to the older
[cui framework](https://github.com/alecmus/cui). But why? It features newer technologies,
greater flexibility and a more robust architecture. Below are some of the most important
differences between the two:

1. Accelerated graphics - while the former uses raw winapi, lecui uses accelerated graphics.
2. Transparency channels - while all the widgets of the former are inherently opaque, lecui
supports transparent widgets due to the more modern technologies that it harnesses.
3. Containers - the former only has a single type of container (page) while the latter
features a myriad of containers for a much more flexible design (pages, panes, tab panes, etc.).
All containers support recursion; they can have not only widgets but also other containers,
and the containers in them can also contain containers and so forth. This makes it possible to
design advanced, compact modular user interfaces with layers. Every container supports
transparency as well through the use of an alpha channel.
4. Scroll bars - the way the former was designed from the word go does not support the overlap
of widgets beyond the page or form; lecui supports this and vertical and/or horizontal
scrollbars are automatically triggered to permit the user to scroll. These scroll bars are
supported for all containers.

### Usage Examples
The library is used in the [pc_info](https://github.com/alecmus/pc_info) app and
various useful usage examples are available in the
[lecui demo](https://github.com/alecmus/lecui_demo) repository. These range from minimalist hello world
examples all the way up to advanced multi-page and multi-layer apps.

Various screenshots are available in the following places:

Summary Description | Location
------------------- | ------------------------------------
lecui_show          | https://github.com/alecmus/files/tree/master/liblec/lecui/screenshots/lecui_show
lecui_minimal       | https://github.com/alecmus/files/tree/master/liblec/lecui/screenshots/lecui_minimal

### Prebuilt Binaries
Prebuilt binaries of the library can be found under
[releases](https://github.com/alecmus/lecui/releases).

### Compile-time Dependencies
The library has the following compile-time dependencies:
1. boost C++ libraries
* assumes boost 1.72 prebuilt binaries are in C:\local\libs\boost_1_72_0
* download prebuilt boost binaries [here](https://sourceforge.net/projects/boost/files/boost-binaries/)
* For example, the binaries for boost 1.72 are [here](https://sourceforge.net/projects/boost/files/boost-binaries/1.72.0/)
* For Microsoft Visual Studio 2019 you can download the file boost_1_72_0-msvc-14.2-64.exe (64 bit)
and boost_1_72_0-msvc-14.2-32.exe (32 bit).
* if the boost c++ library is installed elsewhere you will need to change the Microsoft Visual Studio project
properties under Properties - C/C++ - General - Additional Include Directories and also under
Properties - Linker - General - Additional Library Directories.

### Building
Create a folder '\liblec' and clone the repository into it such that it resides in 'liblec\lecui'. Open the Microsoft Visual Studio Solution file liblec\lecui\lecui.sln. Select Build -> Batch Build, then select the desired configurations of the given four:
1. Debug x86
2. Relese x86 (32 bit Release Build)
3. Debug x64
4. Release x64 (64 bit Release Build)

Build.

Three folders will be created in the \liblec directory, namely bin, lib and include. Below is a description of these subdirectories.

1. bin - contains the binary files. The following files will be created:

File            | Description
--------------- | ------------------------------------
lecui32.dll    | 32 bit release build
lecui64.dll    | 64 bit release build
lecui32d.dll   | 32 bit debug build
lecui64d.dll   | 64 bit debug build

2. lib - contains the static library files that accompany the dlls. The files are named after the respective dlls.
3. include - contains the include files

### Linking to the Library

#### Microsoft Visual Studio
Open your project's properties and for All Configurations and All Platforms set the following:
1. C/C++ -> General -> Additional Include Directories -> Edit -> New Line ... add \liblec\include
2. Linker -> General -> Additional Library Directories -> Edit -> New Line ... add \liblec\lib
3. Debugging -> Environment -> Edit ... add PATH=\liblec\bin;PATH%

Now you can use the required functions by calling #include <liblec/lecui/...>

Build.

### Using the library
The smallest possible gui app is as follows:

```
#include <liblec/lecui/form.h>

int main() {
	liblec::lecui::form("Form").message("Hello world!");
	return 0;
}
```

The above code displays the following form:<br/><br/>
![](https://github.com/alecmus/files/blob/master/liblec/lecui/screenshots/lecui_1.0.0_screenshot_01.PNG?raw=true)

Surely, it doesn't get easier than that!

For more sophisticated examples refer to the section 'Usage Examples' above.

And that's the point of this library: to allow you to make a simple gui app
with minimal code but also give you the power to build a marvel if you need to.

### Deploying your Application
If it's the 32 bit build you will need to deploy it with lecui32.dll in the same folder. If it's a
64 bit build use the lecui64.dll.
