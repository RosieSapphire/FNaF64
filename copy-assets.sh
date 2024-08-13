#!/bin/sh

PROGNAME=$0
FNAF_IMG_DIR=$1/Images
FNAF_SND_DIR=$1/Sounds

# Make sure we are running in the root FNaF64 repo directory
if ! test -d assets/
then
	echo "$PROGNAME: You are not running this program inside the 'FNaF64' repository's root directory. If this is by possible accidental deletion, please run 'git reset --hard'. If '.git/' is missing, you will have to reclone the repository from 'https://github.com/RosieSapphire/FNaF64.git'"
	exit 1
fi

# Check that we have an directory passed in
if [ $# -ne 1 ]
then
	echo "Usage: $PROGNAME [image-audio-dir]"
	exit 1
fi

# Remove all previous assets
rm -rf assets/ci8 assets/ci4 assets/i4 assets/*.wav filesystem/

# Convert ci8 textures
for f in $(cat refs/ci8.txt)
do
	mkdir -p assets/ci8
	cp -rf $FNAF_IMG_DIR/$f assets/ci8/$f 2> /dev/null
	if [ "$?" -ne 0 ]
	then
		echo "$PROGNAME: No FNaF assets found in provided folder, or certain vital assets are missing."
		exit 1
	fi
	convert assets/ci8/$f -resize 33.33% assets/ci8/$f
	echo "GFX CI8 '$FNAF_IMG_DIR/$f' -> 'assets/ci8/$f'"
done

# Convert ci4 textures
for f in $(cat refs/ci4.txt)
do
	mkdir -p assets/ci4
	cp -rf $FNAF_IMG_DIR/$f assets/ci4/$f
	convert assets/ci4/$f -resize 33.33% assets/ci4/$f
	convert assets/ci4/$f -gravity center \
		-crop 320x240+0+0 +repage assets/ci4/$f
	echo "GFX CI4 '$FNAF_IMG_DIR/$f' -> 'assets/ci8/$f'"
done

# Convert i4 textures
for f in $(cat refs/i4.txt)
do
	mkdir -p assets/i4
	cp -rf $FNAF_IMG_DIR/$f assets/i4/$f
	convert assets/i4/$f -resize 33.33% assets/i4/$f
	convert assets/i4/$f -resize 320x240\! assets/i4/$f
	echo "GFX I4 '$FNAF_IMG_DIR/$f' -> 'assets/ci8/$f'"
done

# Special exceptions

# Freddy Faces
it=0
for i in 431.png 440.png 441.png 442.png
do
	echo "Cropping Freddy's Title Screen Face Sprite $it"
	convert assets/ci8/$i -gravity east \
		-crop 320x240+0+0 +repage assets/ci8/$i
	it=$((it+1))
done

# Golden Freddy
echo "Cropping Golden Freddy Sprite in Room"
convert assets/ci8/548.png -gravity center \
	-crop 320x240+0+0 +repage assets/ci8/548.png

# Newspaper, Paychecks 1 & 2, and Notice of Termination (in that order)
echo "Cropping Newspaper"
convert assets/ci8/574.png -gravity center \
	-crop 320x240+0+0 +repage assets/ci8/574.png
echo "Cropping Paycheck 1"
convert assets/ci8/210.png -gravity center \
	-crop 320x240+0+0 +repage assets/ci8/210.png
echo "Cropping Paycheck 2"
convert assets/ci8/522.png -gravity center \
	-crop 320x240+0+0 +repage assets/ci8/522.png
echo "Cropping Termination Slip"
convert assets/ci8/523.png -gravity center \
	-crop 320x240+0+0 +repage assets/ci8/523.png

# Camera Flip Animation
it=0
for i in $(cat refs/camflip.txt)
do
	echo "Resizing Camera Flip Animation Frame $it"
	convert assets/ci8/$i -resize 320x240\! assets/ci8/$i
	it=$((it+1))
done

# Convert Audio
# First, remove spaces from the filenames. Fuck spaces.
rename 's/ /_/g' $FNAF_SND_DIR/*
echo "Removing Spaces from Audio Files"
for i in $(cat refs/audio.txt)
do
	ffmpeg -i $FNAF_SND_DIR/$i -ar 22050 -ac 1 assets/$i > /dev/null 2>&1
	echo "SFX WAV16 '$FNAF_SND_DIR/$i' -> 'assets/$i'"
done
