# JUCE_Shiftee
 If it bits, it shifts, for a low, flat rate.
 
 ![A screenshot of the plugin; two horizontal sliders labeled "Gain" and "Bit Offset" respectively.](https://i.imgur.com/zHtT0Nh.png)
 
This plugin is something I thought of that I haven't seen anywhere before. Yes, everyone and their mother has bit crushers, with adjustable bit rate and bit depth, but don't you want to be able to play with those bits a little more? Move them around a little? \
That's where this plugin comes into play.

Digital Audio is made up of samples; 32-bit (4-byte) floating point numbers (meaning the decimal point can move based on the precision needed) ranging from -1 to +1 .

To manipulate a digital sound, it is as simple as changing these numbers.

This plugin works by copying the float bits (1s and 0s) from each sample into an unsigned integer with a fixed width of 32 bits, or 4 bytes (a `uint32_t`). This means that no errors occur, as the data is all the correct size, yet now the bits can be played with using bitwise operators, as it is considered an integer. This plugin shifts the bits to the left and then ands that with the original bits, as that had the most versatility musically (shifting right required a lot of doing just to get something usable). The integer bits are then copied back into the float bits of the sample, causing the sound to change.

This plugin uses the [JUCE](https://github.com/juce-framework/JUCE) library.

### Using the plugin
To use the plugin you will need an audio plugin host, like a DAW, that can load plugins from custom paths. \
If that doesn't sound too daunting, head to the [releases page](https://github.com/LensPlaysGames/Shiftee/releases) to download the latest and greatest; check back if you have any issues.

Double click on a slider to reset it to default.

#### Windows
FL Studio requires that all `.vst3` plugins be placed in a single specific folder: \
`C:\Program Files\Common Files\VST3\` \
and adding custom paths to a `.vst3` or `.vst3` directory isn't an option. 

Over in Ableton, custom `.vst3` directories are specifiable, so it's easy to find or add the right place to put it.

#### MacOSx, Linux
I have not tested anything on macOSx or Linux yet so if you have any comments, concerns, or criticisms, be sure to leave an [issue](https://github.com/LensPlaysGames/Shiftee/issues) or make a [pull request](https://github.com/LensPlaysGames/Shiftee/pulls).

### Building the plugin
If you: are a developer and want to edit the source code and actually build the thing, this project uses CMake.

Simply run the `build_cmake.bat` included when you clone the repository, or run CMake through the command line with arguments of your choice.

This will result in a project to open for whatever build system is default for your system. 

On Windows, this is Visual Studio 2022. \
On MacOSx, this is XCode. \
On Linux, this is Makefile.

Once the project is built using CMake, simply open it up and build the VST3 project (or whatever project you are targeting at the moment).

This will create the `.vst3` that can be loaded by any modern audio plugin host.

If you are having trouble finding the plugin once built, you are not alone. For some reason JUCE defaults it to kind of a crazy path. \
For me, on Windows 10 using Visual Studio 2022, that path is `Path/To/PluginRepository/build/Shiftee/ShifteePlugin_artefacts/DebugOrRelease/VST3/Shiftee.vst3/Contents/x86_64-win/` \
Your mileage may vary.