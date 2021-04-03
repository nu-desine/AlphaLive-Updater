# AlphaLive Updater #

AlphaLive Updater is a simple GUI utility application that installs software updates for [AlphaLive](http://www.alphasphere.com/alphalive/), the [AlphaSphere](http://www.alphasphere.com/)'s official software application. It has been designed to be launched directly from AlphaLive, and takes the update data that was downloaded through AlphaLive and moves all the files to the correct places. 

AlphaLive Updater has been programmed using the [JUCE](https://juce.com) cross-platform C++ library. It includes an Xcode project for building on macOS, and a Visual Studio 2019 project for building on Windows. JUCE also easily allows for Linux makefile's to be built.

You can download the latest pre-built binaries of the application for macOS and Windows from the [AlphaLive repository](https://github.com/nu-desine/AlphaLive).

## Build Requirements ##

To build AlphaLive Updater on macOS you will need to download and install [Xcode](https://developer.apple.com/xcode/).

To build AlphaLive Updater on Windows you will need to download and install [Visual Studio](https://visualstudio.microsoft.com/downloads/) with the C++ SDK.

## Building ##

For all platforms, use the 'AlphaLive Updater.jucer' Projucer file to configure the project for your system.

On Windows, the application must be built to [run using administrator permissions](https://docs.microsoft.com/en-us/cpp/build/reference/manifestuac-embeds-uac-information-in-manifest?view=msvc-160) in order for the software updating functionality to work correctly.

## License ##

AlphaLive Updater is licensed under the GPLv3. See gpl-3.0.txt for details.

AlphaLive and the AlphaSphere have been developed by [nu desine](https://www.watershed.co.uk/studio/residents/nu-desine).

The following libraries are included in the source code:

+  The [JUCE](https://juce.com) library which is licensed under the GPLv3. 


