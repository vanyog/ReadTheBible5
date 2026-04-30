#!/bin/sh

echo "Този скрипт копира всички необходими за работа на Компютърна Библия 5 файлове в директория rel."
echo "Искате ли да продължите (y/n)?"
read answer
if [ "$answer" != "y" ]; then
    echo "Прекратено."
    exit 1
fi

rm -rf rel

mkdir rel

cp -R build/Qt_6_11_0_for_macOS-Release/Bible5.app rel
~/Qt/6.11.0/macos/bin/macdeployqt rel/Bible5.app

echo "All done"
exit
