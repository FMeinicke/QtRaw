# QtRaw

A Qt image plugin for loading raw files, via libraw. Once installed, it enables all Qt applications to load raw files produced by most digital cameras.

This is a fork from the [mardy/qtraw](https://github.com/mardy/qtraw) repsitory with the intention to make the QtRaw Plugin usable under Windows as well as Linux. 

## Installation

### Linux
The QtRaw plugin depends on Qt and libraw. In order to build it, make sure you have the necessary development packages installed. Under Ubuntu, this can be achieved by running these commands: 

    sudo apt-get install libraw-dev qtbase5-dev

for building with Qt 5, or 

    sudo apt-get install libraw-dev libqt4-dev

for building with Qt 4.
Alternatively, you can find the libraw source code at
http://www.libraw.org/download

Once the dependencies are set up, then clone the repository as usual with

    git clone https://github.com/FlorianMeinicke/QtRaw.git  

The following commands will build and install the plugin into your system:

      qmake  
      make  
      sudo make install  


### Windows
Unfortunately on Windows the build process is not as easy as on Linux. Therefore I tried to simplify it as much as possible. What I ended up with simplifies the build to a minimum number of steps. (If you find another easier way of building QtRaw just let me know.) 
First of all clone the repository with

      git clone --recursive https://github.com/FlorianMeinicke/QtRaw.git  

This will automatically clone the LibRaw and rawspeed repositories as well. After that you need to apply the patches provided in the `patches` directoy of the repository. This will apply all the changes that are necessary to build with MinGW under Windows. In the `patches/rawspeed` directory of the repository run

      git apply --ignore-space-change --ignore-whitespace ..\patches\rawspeed.patch  

and in the `patches/LibRaw` directory run the same command but with the `LibRaw.patch` instead.  

The next step is to get all dependencies that are required by LibRaw and rawspeed. All dependencies need to go in the `third-party` directory. However if you prefer to store your dependencies somewhere else you can also do that but you have to change all paths that point to the `third-party` directory in the `rawspeed.pro` and the `libraw.pro` file. Follow the instructions below to get all dependencies:
1. Download the `libjpeg` installer for gcc (MinGW) from https://sourceforge.net/projects/libjpeg-turbo/. Execute it and select the `libjpeg-turbo` directory as destination folder.
2. Download the `iconv-1.9.2.win32.zip`, `zlib-1.2.5.win32.zip` and `libxml2-2.7.8.win32.zip` archives from https://www.zlatkovic.com/pub/libxml/ and extract them to the `iconv`, `zlib` and the `libxml2` directory respectively.

Finally you are ready to build QtRaw. Open the `qtraw.pro` file with QtCreator. Go into the 'Projects' tab on the left and add a build step. Select 'Make' and give it `install` as 'Make arguments'. Then just hit 'Build' and that's it. 

If you prefer the command line, just run

      mkdir build && cd build  
      qmake ..   
      mingw32-make -j<number_of_cpu_cores>  
      mingw32-make install  

Substitute `<number_of_cpu_cores>` with the number of CPUs your PC has.  
If everything worked correctly, all Qt applications should be able to load and display raw camera files.

This was tested with Qt 5.11.1 on a Windows 10 machine. I'll test if and how this procedure is applicable with the MSVC compiler as well.
