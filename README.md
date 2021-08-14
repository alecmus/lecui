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
[liblec libraries](https://github.com/alecmus/liblec). It enables you to make a simple gui app
with minimal code but also gives you the power to build a marvel if you need to.

### Wiki
The library has a wiki available [here](https://github.com/alecmus/lecui/wiki). The wiki has details about how to use the library, examples, guidelines and more.

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
The library is used in the [pc_info](https://github.com/alecmus/pc_info) and [spotlight_images](https://github.com/alecmus/spotlight_images) apps and
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

### Building
Refer to the [wiki](https://github.com/alecmus/lecui/wiki/Installation).

### Linking to the Library
Refer to the [wiki](https://github.com/alecmus/lecui/wiki).

### Deploying your Application
If it's the 32 bit build you will need to deploy it with lecui32.dll in the same folder. If it's a
64 bit build use the lecui64.dll.
