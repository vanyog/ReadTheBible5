#!/bin/sh

rm -r build/rel

mkdir -p build/rel/usr/bin
mkdir -p build/rel/usr/share/icons/hicolor/512x512/apps
cp build/Bible5 build/rel/usr/bin/
chmod +x build/rel/usr/bin/Bible5
cp Bible5.desktop build/rel/
cp images/Logo_ikon_300x300.png build/rel/Bible5.png
cp images/Logo_ikon_300x300.png build/rel/usr/share/icons/hicolor/512x512/apps/Bible5.png
~/Downloads/linuxdeployqt-continuous-x86_64.AppImage build/rel/Bible5.desktop -appimage
