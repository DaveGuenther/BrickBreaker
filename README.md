# SDL-Template

Use this to create a basic started SDL project that cross-compiles in Linux and Windows.

## Linux Configuration (I use Manjaro):
This is the primary development side and uses clang++, VS Code (OSS), and lldb.  It also uses google profiling tools.  Build targets are located in <code>build/linux</code>.  Manjaro Linux should come with SDL installed, but you may have to install SDL2_image as well.


## Win32
This should only be used to build the Win32 target.  To do so, you need MSys2, clang++ (through MSys), git (through Msys), make (through MSys).  Build targets are located in <code>build/win32</code>


## SDL Configuration:
On both platforms, you'll need the install SDL 64 bit libraries and development libraries.  Linux should be simple with a package manager, but there are several steps for windows.  At the time this readme.md was edited, the versions were SDL2 version 2.26.1 and SDL2_image version 



#### SDL2 releases: https://github.com/libsdl-org/SDL/releases/

#### SDL2_image releases: https://github.com/libsdl-org/SDL_image/releases
Get the file ending in "-win32-x64.zip" for both the binaries and the development libraries (find "devel" in the filename for development libraries).

Extract development libraries to c:\SDL\

Extract binaries to build/win32/ overwriting the older ones already there

Update the makefile include path if necessary to identify the correct version in c:\SDL\
