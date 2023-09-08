#!/bin/sh

FNAF_IMG_DIR=$1
FNAF_AUD_DIR=$2

if [ "$#" -ne 2 ]
then
	echo "Usage: ./copy_assets.sh [image-dir] [audio-dir]"
	exit 1
fi

rm -rf assets/ci8 assets/ci4 assets/i4 assets/*.wav filesystem/

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

# Special exceptions

# Freddy Faces
for i in 431.png 440.png 441.png 442.png
do
	convert assets/ci8/$i -gravity east \
		-crop 320x240+0+0 +repage assets/ci8/$i
done

# Golden Freddy
convert assets/ci8/548.png -gravity center \
	-crop 320x240+0+0 +repage assets/ci8/548.png

# Newspaper
convert assets/ci8/574.png -gravity center \
	-crop 320x240+0+0 +repage assets/ci8/574.png

# Camera Flip Animation
for i in $(cat refs/camflip.txt)
do
	convert assets/ci8/$i -resize 320x240\! assets/ci8/$i
done

# Sound
rename 's/ /_/g' $FNAF_AUD_DIR/*
for i in $(cat refs/audio.txt)
do
	ffmpeg -i $FNAF_AUD_DIR/$i -ar 22050 -ac 1 assets/$i
done
