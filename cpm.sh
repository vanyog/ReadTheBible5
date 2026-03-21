#!/bin/sh

echo "Този скрипт копира всички необходими за работа на Компютърна Библия 5 файлове в директория rel."
echo "Искате ли да продължите (y/n)?"
read answer
if [ "$answer" != "y" ]; then
    echo "Прекратено."
    exit 1
fi

ZIPF="Bible5.3.1-mac.zip"

rm -f "$ZIPF"
rm -rf rel

mkdir rel

cp -R Bible5.app rel
~/Qt/6.10.2/macos/bin/macdeployqt rel/Bible5.app

cd rel
zip -rq -9 ../"$ZIPF" .

echo "All done"
