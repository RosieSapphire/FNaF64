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

This will be done inside of [WSL](https://aka.ms/wsl), so if you don't have that set up, you should do so now.

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
./copy-assets.sh tmp
```

Remove the temporary directory.
```bash
rm -r tmp/
```

After this, you should be good to build the ROM.
```bash
make -j4
```

Now you can run it, or put it on an N64 Flashcart via
SD card or with the [UNFLoader developed by Buu342](https://github.com/buu342/N64-UNFLoader).

Have fun!

# In-Game Debugger
## Info
Five Nights at Freddy's 64 now has a built-in on-screen debugger. It allows you to push a bunch of variables or constants to a debug buffer, and prints them out every frame. So far it only supports ints and floats, so it's very basic, but if you are working on a Libdragon project, the `debug_view.h/debug_view.c` files can just be copied into your Libdragon project and used according to the following instructions.
## Usage
To enable the game's debugger, make sure to go to `src/debug_view.c` and find `#define DEBUG_MODE` and change it to 0 to disable it, or 1 to enable it.

To use the debugger for FNaF, all you have to do is
### If you use this for a separate Libdragon project,
make sure to call
- `debug_view_init()` to initialize (this MUST be done after `dfs_init()`, since it loads a font from the filesystem [Which you will also have to change the path to in `debug_view.c` to load your custom font {or you can just copy the `debug_font.ttf` in `assets/custom/` to your project's asset directory, and update the path accordingly}]).
- `debug_view_update()` every frame to update the debug viewer. Make sure to pass in the button that can be used to toggle the menu on and off (in FNaF64, it's the L button), and the delta/frame time for updating the blinking animation of the red debug symbol at the bottom right.
- `debug_view_draw()` to actually draw all the values to the screen. Make sure this is done AFTER `rdpq_attach_xxx` has been drawn, and make sure it's the LAST function called BEFORE `rdpq_detach_show()`, since you don't want it to be covered up by everything else in the game.
- `debug_view_terminate()` to unload the debug font and disable it. Usually this is done along with de-allocating everything else in the game at the very end upon closing (technically this is optional since it all gets flushed anyway when the console resets).

# To-Do List
| Task | Description | Priority |
|--|--|--|
| Death Moaning | When a robot gets into your room and your camera is up, you should hear moaning | Medium |
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
