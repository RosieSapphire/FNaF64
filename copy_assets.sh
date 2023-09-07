#!/bin/sh

FNAF_IMG_DIR=$1

if [ "$#" -ne 1 ]
then
	echo "Usage: ./copy_assets.sh [fnafdumpdir]"
	exit 1
fi

for f in $(cat refs/ci8.txt)
do
	mkdir -p assets/ci8
	cp -rvf $FNAF_IMG_DIR/$f assets/ci8/$f
	convert assets/ci8/$f -resize 33.33% assets/ci8/$f
done

for f in $(cat refs/ci4.txt)
do
	mkdir -p assets/ci4
	cp -rvf $FNAF_IMG_DIR/$f assets/ci4/$f
	convert assets/ci4/$f -resize 33.33% assets/ci4/$f
	convert assets/ci4/$f -gravity center \
		-crop 320x240+0+0 +repage assets/ci4/$f
done

for f in $(cat refs/i4.txt)
do
	mkdir -p assets/i4
	cp -rvf $FNAF_IMG_DIR/$f assets/i4/$f
	convert assets/i4/$f -resize 33.33% assets/i4/$f
	convert assets/i4/$f -resize 320x240\! assets/i4/$f
done

# Freddy Faces
for i in 431.png 440.png 441.png 442.png
do
	convert assets/ci8/$i -gravity east \
		-crop 320x240+0+0 +repage assets/ci8/$i
done
