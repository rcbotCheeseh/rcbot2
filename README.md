# RCBot2 for Windows and Linux (TF2, HL2:DM, DOD:S)

## Information
This is a fork of Cheeseh's RCBot2 repository. This aims to build on top of it to add features along with possible stability fixes.
Note that this is mainly intended for TF2.

[Installation](http://rcbot.bots-united.com/forums/index.php?showtopic=1967)

## Building
### Windows
Make sure to have Visual Studio 2013 installed.
Other than that, it should compile fine, even with the latest Visual Studio 2017.

### Linux
Run ./compile.sh in 'linux_sdk'

Alternatively:
* Go to `linux_sdk`
* Type `make -f Makefile.rcbot2 vcpm`
* Type `make -f Makefile.rcbot2 genmf` (this will generate the Makefiles)
* Edit `Makefile.rcbot2` and `Makefile.HPB_bot2_*` according to your needs
  * Most of the time you will only need to modify Line 21 and 55 of `Makefile.rcbot2`
* Type `make -f Makefile.rcbot2 all -j4 2> error.log`
* Check the `error.log`

---

Make sure to check out the [bots-united.com discord](https://discord.gg/BbxR5wY) for support and to stay updated.