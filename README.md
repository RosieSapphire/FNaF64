# Five Nights at Freddy's 64
![Artwork by Rosie Sapphire, Quilt and Spooky Илюхa](https://github.com/RosieSapphire/FNaF64/blob/main/coverart.png?raw=true)

A near 1:1 port of Five Nights at Freddy's 1 to the Nintendo 64.
Unfortunately, for the time being, it is unplayable in it's current state
due to needing to add the ability to supply your own copy of the game and
extract the assets, [similar to the SM64 Decomp Project.](https://github.com/n64decomp/sm64)

I would like to give a special thanks to [Rasky](https://github.com/rasky) and
[Buu342](https://github.com/buu342) for helping me along my Nintendo 64 development
journey, and I'm really happy with this so far.

# YouTube Tutorial
For those of you who are more visual learners, [here is a tutorial](https://youtu.be/cSOmlyvK9gQ)
I made for how to do everything necessary to compile the ROM.

# Prerequisites
## Software
* [CTFAK 2.0](https://github.com/CTFAK/CTFAK2.0)
* [WSL](https://learn.microsoft.com/en-us/windows/wsl/install)
* [Ares](https://ares-emu.net)

Note: Currently CTFAK really only works on Windows from my understanding.

## Extracting Assets
Once you have CTFAK installed, open it up and enter the path to your
legally obtained Five Nights at Freddy's executable file.
Hit enter with the `Parameters` section empty, and it will load
all the data from the game.

Once it's ready, press 3 to use the `Image Dumper`, and then
press 4 to use the `Sound Dumper`. After both of these processes
are finished, you can hit 0 to exit the program.

# Compilation
## Dependencies
* [ImageMagick](https://imagemagick.org) (for image compression)
* [Libdragon](https://github.com/DragonMinded/libdragon) (N64 API)
* [FFMPEG](https://ffmpeg.org/download.html) (for audio compression)
* Rename (Install using your package manager of choice on WSL)

## Converting Assets and Compiling
In order for the assets of Five Nights at Freddy's to fit on a Nintendo 64, they
have to be compressed, both in color depth and image size. With the help of
a handy-dandy tool I wrote in bash, which utilizes ImageMagick, we can convert
the textures after copying them to a temporary directory.

This will be done inside of WSL, so if you don't have that set up, you should do so now.

Install all the dependencies.

```bash
sudo apt install imagemagick ffmpeg rename build-essential libpng-dev git
```

Download the .deb file from the [Libdragon Releases](https://github.com/DragonMinded/libdragon/releases) and install it.

```bash
sudo dpkg -i gcc-toolchain-mips64-x86_64.deb
```

Clone the repository.

```bash
git clone --recursive https://github.com/RosieSapphire/FNaF64.git fnaf
```

Move into the Libdragon directory and build it.

```bash
cd fnaf/libdragon/ && ./build.sh
```

Move back into the main directory and copy the assets.

```bash
cd .. && mkdir tmp/
```

```bash
cp -rvf /mnt/c/<CTFAK Directory>/Dumps/Five\ Nights\ at\ Freddys/* tmp/
```

Run the script to convert the assets.
```bash
./copy_assets.sh tmp/Images/ tmp/Sounds/
```

Remove the temporary directory.
```bash
rm -r tmp/
```

After this, you should be good to build the ROM.
```bash
make -j
```

Now you can run it, or put it on an N64 Flashcart via
SD card or with the [UNFLoader developed by Buu342](https://github.com/buu342/N64-UNFLoader).

Have fun!

# To-Do List
| Task | Description | Priority |
|--|--|--|
| Death Moaning | When a robot gets into your room and your camera is up, you should hear moaning | Medium |
| Static Deltatime | Currently, the game uses a dynamic deltatime, which is fine, but there are better ways. | Medium |
| Random for Pic | There is a random value that dictates what you'll see in certain cameras | Low |
| Background Ambience | I haven't added ambience to the nights yet | Low |
| Goofy Mode | I actually don't know what I wanna do with this. lol | Really Low |

# Controls
| Button | Menu Action | Game Action |
|--|--|--|
| A | Select | Toggle Lights |
| B | N/A | Toggle Doors |
| C | Change Selection | Switch Cameras |
| R | Toggle Settings | Toggle Camera |
| L | N/A | Toggle Camera |
| Z | Delete Save | Toggle Camera |
| DPad | Change Selection | Switch Cameras |
| Start | Select | Exit Game |
