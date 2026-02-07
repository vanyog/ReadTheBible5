#!/bin/sh

rm -rf rel
mkdir rel

cp -R Bible.app rel
cp Bible_bg.qm rel
cp Bible_mk.qm rel

mkdir rel/htdocs
cp htdocs/copying.txt rel/htdocs
cp htdocs/readme-bg.html rel/htdocs
cp htdocs/readme-en.html rel/htdocs
cp htdocs/style.css rel/htdocs

mkdir rel/htdocs/help
cp htdocs/help/*.jpg rel/htdocs/help
cp htdocs/help/*.png rel/htdocs/help
cp htdocs/help/help.html rel/htdocs/help

mkdir rel/htdocs/images
cp htdocs/images/*.* rel/htdocs/images

mkdir rel/images
cp images/Logo_2a.png rel/images

mkdir rel/data
cp data/style.css rel/data

mkdir rel/data/bibles
cp data/bibles/php-bible.css rel/data/bibles
cp data/bibles/list-u.txt rel/data/bibles
cp data/bibles/Bible_Structure.txt rel/data/bibles
