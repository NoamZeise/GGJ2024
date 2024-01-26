# Global Game Jam 2024



# setup

Cmake is required for building on all platforms.

### linux with apt on debian based systems
vulkan tools
```
$ sudo apt-get install vulkan-tools
```
vulkan loader / validation layers / spriv compilers
```
$ sudo apt-get install libvulkan-dev vulkan-validationlayers-dev spirv-tools
```
test vulkan works
```
$ vkcube
```
additional libraries
```
$ sudo apt-get install libfreetype-dev libsndfile1-dev libasound-dev portaudio19-dev
```

### windows

* download [libsndfile](http://www.mega-nerd.com/libsndfile/#Download) compile and put in your lib and include directories, and distrubute dll with your binaries

* download [portaudio](http://files.portaudio.com/docs/v19-doxydocs/compile_windows.html) compile and put in your lib and include directories, and distrubute dll with your binaries

You may need to specify `sndfile_DIR` and `portaudio_DIR` with the cmake folders of those libraries ( ie where the `portaudioConfig.cmake` files are). SO you would pass `-Dportaudio_DIR=/your/path/to/portaudio/cmake/folder` to cmake when building.

Download the [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/), for getting the vulkan validation layers and the tools for compiling shaders into spirv.

# Build

```
git clone --recurse-submodules https://github.com/NoamZeise/GGJ2024.git
cd GGJ2024
mkdir build && cd build
cmake ..
cmake --build .
```
Then the built binary should be in /build/src/. Note that PortAudio and Libsndfile dlls wont be built with this, so on windows you'll need to copy the dlls for those into the same path as the example binary.

# FAQ

### Common Build Errors

* Missing <vulkan/...> error
pass `-D VULKAN_HEADERS_INSTALL_DIR=/your/path/to/your/installed/vulkan/headers`
to cmake when generating this project

### Enabling other 3D model formats

To use formats other than those enabled by default, you must set `ASSIMP_BUILD_XYZ_IMPORTER` to true, where `XYZ` is your format, before loading the cmake files for this project.

For example to enable the blend format, you would have `set(ASSIMP_BUILD_BLEND_IMPORTER TRUE)` somewhere in your cmake file before calling `add_subdirectory(Graphics-Environment)`. Check the [assimp](https://assimp.org/) docs for more info on supported formats.

You should then be able to load these newly enabled formats the same as you load the default ones.

### Proper export settings for 3D modelling software

When exporting your models, ensure you are using relative paths to textures.
Also ensure that models are exported with the Z Up direction option.

# Todo list:
bugs:
* instability reported on some AMD GPUs (unable to test this directly)

features:
* define new shader pipelines outside of render
* better model material support (right now just diffuse colour)
