#!/bin/sh

echo "make -f Makefile.rcbot2 vcpm"
make -f Makefile.rcbot2 vcpm > compile.log

echo ""
echo "make -f Makefile.rcbot2 genmf"
make -f Makefile.rcbot2 genmf >> compile.log

echo ""
echo "make -f Makefile.rcbot2 all -j4"
make -f Makefile.rcbot2 all -j4 >> compile.log

echo "Done. Check compile.log"
