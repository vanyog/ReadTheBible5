#!/bin/sh

rm -rf Bible.app
rm -rf update.app
rm -f build/*.*

qmake -spec macx-g++
make
cd update
qmake -spec macx-g++
make
cd ..