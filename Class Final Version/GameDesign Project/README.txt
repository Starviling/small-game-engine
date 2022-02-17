The spawn zone sits on the small central platform of the screen
The death zone is all platforms
The translation zone is to the right
Replay system does not support speed up or slow down (only supports pause)
To execute replay record, press r
To terminate recording and start replaying, press t
To pause the game, press space
To speed up the game, press .
To slow down the game, press ,
Press C to raise a scripted event to handle
Press F to shoot projectile with server (SPACE INVADER ONLY)

--------------------------------------------------------------------------------------------------

Extra Info: Client version of the space invaders will only display the positions of objects at
their locations upon join. The Client can send position information to the server while the server
returns some information about the objects. The implemtation effort to reflect on both server and
client was abandoned to save time.

The space invaders doesn't have a full implementation of a game over - Game needs to be restarted
to play again. While the invaders suicide against the planet, they don't trigger the game over if
they do - they only do when colliding with the player.

--------------------------------------------------------------------------------------------------

My project set up followed the SMFL tutorial pretty closely - Using the 32bit version
Properties - Linker - Input - Additional Dependencies
- All configurations: libzmq-v141-mt-4_3_3;ws2_32.lib;ogg.lib;vorbis.lib;vorbisfile.lib;vorbisenc.lib;flac.lib;openal32.lib;winmm.lib;freetype.lib;opengl32.lib;<different options>
- Debug configurations: libzmq-v141-mt-4_3_3;ws2_32.lib;ogg.lib;vorbis.lib;vorbisfile.lib;vorbisenc.lib;flac.lib;openal32.lib;winmm.lib;freetype.lib;opengl32.lib;sfml-main-d.lib;sfml-graphics-s-d.lib;sfml-window-s-d.lib;sfml-system-s-d.lib;sfml-network-s-d.lib;sfml-audio-s-d.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)
- Release configurations: libzmq-v141-mt-4_3_3;ws2_32.lib;ogg.lib;vorbis.lib;vorbisfile.lib;vorbisenc.lib;flac.lib;openal32.lib;winmm.lib;freetype.lib;opengl32.lib;sfml-main.lib;sfml-graphics-s.lib;sfml-window-s.lib;sfml-system-s.lib;sfml-network-s.lib;sfml-audio-s.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)

Properties - Linker - General
- Additional directories attached from SFML-2.5.1\lib
- \libzmq-updated		(4.3.3)

Properties - C/C++ - Additional Include Directories
- SFML-2.5.1\include
- \libzmq-updated		(4.3.3)
- nlohmann-json

Properties - C/C++ - Preprocessor Definitions
- SFML_STATIC;WIN32;NDEBUG;_CONSOLE;%(PreprocessorDefinitions)

--------------------------------------------------------------------------------------------------

The libraries for nlohmann-json, libzmq-v141-mt-4_3_3, boost, and SFML are being used.

These were the only settings I changed in my environment. If I have anything done incorrectly,
a comment or email would be nice since I'd really like to not overlook small errors.

I don't believe I have anything else special that may complicate the compilation/execution
process. If I am, please let me know so that I can avoid the same mistake.

Thanks,
Stephen Wang (sywang)