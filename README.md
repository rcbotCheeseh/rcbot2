## RCBot2 v1.00 for Windows and Linux (TF2, HL2:DM, DOD:S)

Note: Those are beta builds recompiled by [APG] Clan for RCBot2 v1.00, and those builds are recompiled for newer Metamod Source engine.

### Information
This is a fork of the official RCBot2 plugin written by Cheeseh, which can be found [here](http://rcbot.bots-united.com/). <br />
This repository is mainly used for the Linux release of his plugin, but it also works on Windows.

### Installation
A complete guide for installing this plugin can be found at the official RCBot2 forums over [here](http://rcbot.bots-united.com/forums/index.php?showtopic=1967). <br />
This thread is also maintained by me.

### Build
* Go to `linux_sdk`
* Type `make -f Makefile.rcbot2 vcpm`
* Type `make -f Makefile.rcbot2 genmf` (this will generate the Makefiles)
* Edit `Makefile.rcbot2` and `Makefile.HPB_bot2_*` according to your needs
  * Most of the time you will only need to modify Line 21 and 55 of `Makefile.rcbot2`
* Type `make -f Makefile.rcbot2 all -j4 2> error.log`
* Check the `error.log`
