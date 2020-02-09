# QtRaw

A Qt image plugin for loading raw files, via libraw. Once installed, it enables all Qt applications to load raw files produced by most digital cameras.

This is a fork from the [mardy/qtraw](https://github.com/mardy/qtraw) repsitory (which has now moved to [GitLab](https://gitlab.com/mardy/qtraw)) with the intention to make the QtRaw Plugin usable under Windows as well as Linux. 

# Installation

## Linux

The QtRaw plugin depends on Qt and LibRaw. In order to build it, make sure you have the necessary development packages installed. Under Ubuntu, this can be achieved by running these commands: 
```bash
$ sudo apt-get install libraw-dev qtbase5-dev
```
for building with Qt 5, or 
```bash
$ sudo apt-get install libraw-dev libqt4-dev
```
for building with Qt 4.
Alternatively, you can find the libraw source code at
http://www.libraw.org/download

Once the dependencies are set up, then clone the repository as usual with
```bash
$ git clone https://github.com/FMeinicke/QtRaw.git  
```
The following commands will build and install the plugin into your system:
```bash
$ mkdir build && cd build
$ qmake ..
$ make -j$(nproc)
$ sudo make install
```

## Windows
Unfortunately on Windows the build process is not as easy as on Linux. Therefore I tried to simplify it as much as possible. What I ended up with simplifies the build to a minimum number of steps. (If you find another easier way of building QtRaw just let me know.) 
First of all clone the repository with
```cmd
> git clone --recursive https://github.com/FMeinicke/QtRaw.git  
```
This will automatically clone the LibRaw, rawspeed, and pugixml repositories as well. After that you need to apply the patches provided in the `patches` directoy of the repository. This will apply all the changes that are necessary to build with MinGW under Windows. In the `rawspeed` directory of the repository run
```cmd
> git apply --ignore-space-change --ignore-whitespace ..\patches\rawspeed.patch  
```
Do the same for `LibRaw` and `pugixml` as well.  

The next step is to get all dependencies that are required by LibRaw and rawspeed. All dependencies need to go in the `third-party` directory. However if you prefer to store your dependencies somewhere else you can also do that but you have to change all paths that point to the `third-party` directory in the `rawspeed.pro` and the `libraw.pro` file. Follow the instructions below to get all dependencies:
1. Download the `libjpeg` 64-bit installer for gcc (MinGW) from https://sourceforge.net/projects/libjpeg-turbo/files/2.0.4/libjpeg-turbo-2.0.4-gcc64.exe/download. Execute it and select the `libjpeg-turbo` directory as destination folder.
2. Download the `zlib` 64-bit archive from https://www.zlatkovic.com/pub/libxml/64bit/zlib-1.2.8-win32-x86_64.7z and extract them to the `zlib` directory.

Finally you are ready to build QtRaw. Open the `qtraw.pro` file with QtCreator. Go into the 'Projects' tab on the left and add a build step. Select 'Make' and give it `install` as 'Make arguments'. Then just hit 'Build' and that's it. 

If you prefer the command line, just run
```cmd
> mkdir build & cd build  
> qmake ..   
> mingw32-make -j<number_of_cpu_cores>  
> mingw32-make install  
```

Substitute `<number_of_cpu_cores>` with the number of CPUs your PC has.  
If everything worked correctly, all Qt applications should be able to load and display raw camera files.

> #### Note:
>This was tested with Qt 5.13.1 on a Windows 10 machine using MinGW 7.3.0 64-bit compiler. For 32-bit systems use the master branch instead.

# Getting started
The project contains a simple example application that was taken from one of the Qt Examples. It is basically a simplified version of the [imageviewer](https://github.com/qt/qtbase/tree/5.12/examples/widgets/widgets/imageviewer) example. The example shows how you can use a `QImageReader` object to read an image from a file:
```cpp
QImageReader Reader{FileName};
const auto NewImage = Reader.read();

if (NewImage.isNull())
{
    QMessageBox::critical(this, QGuiApplication::applicationDisplayName(),
                          tr("Cannot load %1: %2")
                          .arg(QDir::toNativeSeparators(FileName), Reader.errorString()));
    return false;
}
m_Image = NewImage;
m_ImageLabel->setPixmap(QPixmap::fromImage(m_Image));
```
