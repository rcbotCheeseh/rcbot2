# RCBot2 for Windows and Linux (TF2, HL2:DM, DOD:S)

## Information
This is a fork of Cheeseh's RCBot2 repository. This aims to build on top of it to add features along with possible stability fixes.
Note that this is mainly intended for TF2.

[Installation](http://rcbot.bots-united.com/forums/index.php?showtopic=1967)

## Building
### Windows
Make sure you have Visual Studio 2013 installed.
Other than that it should compile fine, even with the latest Visual Studio 2019 - as long you only use the VS2013 C++ compiler.

### Linux
Make sure you have GCC 4.8 as well as G++ 4.8 installed - newer versions are not compatible.

* Go to `linux_sdk`
* `make -f Makefile.rcbot2 vcpm`
* `make -f Makefile.rcbot2 genmf` (this will generate the Makefiles)
* `make -f Makefile.rcbot2 all -j4`
And a `RCBot2Meta_i486.so` should appear in the folder.
---

Make sure to check out the [Bots United Discord](https://discord.gg/BbxR5wY) for support and to stay updated.
