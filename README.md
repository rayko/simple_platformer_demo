# simple_platformer_demo

A small 2D game built from scratch with C++ and SFML library, that showcases
a simple game engine using ECS arquitecture.

The game itself is a simple 2D platformer, where the player navigates a few
different levels collecting coins. Upon collecting all coins in a level, the
player can activate the exit leaver, that requires said coins, and a door to
the next level will open.

The repo contains everything necessary to compile and run the game, including
all necessary assets.

# Building

To build the project, follow the SFML requirements setup from [SFML Documentation](https://www.sfml-dev.org/tutorials/3.1/getting-started/cmake/#customize-the-cmake-project-and-executable-names),
basically the Debian/Ubuntu dependencies:

```bash
sudo apt update
sudo apt install \
    libxrandr-dev \
    libxcursor-dev \
    libxi-dev \
    libudev-dev \
    libfreetype-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libfreetype-dev
```

The project was built using `clang`, `cmake` and `ninja`, with C++ 20 standard.
Be sure to have the necessary building tools to compile it. There are some helper
scripts provided to help:

- `scripts/setup` will prepare the `build/` directory, runs this once or when you clear `build/` dir
- `scripts/build` will run `cmake` to build the project
- `scripts/run` will execute the resultant binary in `build/bin`, as a followup of `scripts/build`

After cloning, use the setup scripts and copy the contents of `assets/` to the `build/bin`
location:

```
cp -r assets/* build/bin/
```

Build and run.

# Config File

There a few different configuration files required to run, all of them included in
the `assets/` folder to copy over the `build/bin` dir.

`assets.txt` defines the textures to use, animations and fonts. The animations
definitions are the only tricky ones, but the config file has some documentation
on what each value is.

`level<n>.txt`, `editor_level.txt` and `test_level.txt` define actual levels. The
latter two are specific for the editor and a quick test level I had around.

`configs.txt` defines some game engine configurations, primarily the list of
levels and order they play.

Feel free to mess around with these. Following the overal layout makes it easily
editable. You can load your own textures, add in new tiles and change levels.

No extra spaces are allowed among values in any config file, otherwise it can crash.
Every single value in a config file should be a string of characters without spaces,
since the space character is the value delimiter.

# Features

Primarily the main feature in this little game enigne is the ECS implementation,
which facilitates composing different game mechanics and functions on top of the
main classes.

- Simple main menu
- Entity management
- Rendering layers
- Player interactions (exit switch)
- "Cinematics" via timed events (played at level finish)
- Baisc physics and AABB collision detections
- Simple interactible UI elements (editor panels)
- Built-in level editor
- Some modding capabilities (mostly decorative)
- Simple sprite animations

### Editor

The game also includes its own editor which reads `assets.txt` to load tiles, and
can be used to create new levels or edit existing ones.

It's a little rough but functional. To use it, simply run the game and visit the "Editor"
menu entry. By opening the editor, if a level file called `editor_level.txt` does
not exists, it will create it with some defaults. If it exists, it will simply
load the file.

To make a new level delete or rename `editor_level.txt` and re-enter the editor
from the game menu.

Keys:

- `WASD` - Move around
- `F1` - Show/Hide map grid
- `F2` - Show/Hide front decorations
- `F3` - Show/Hide regular tiles
- `F4` - Show/Hide background decorations
- `F5` - Save to `editor_level.txt`
- `ESC` - Exit to main menu
- `Left Click` - Place tile/object or select items in panels
- `Right Click` - Remove tile

It's pretty straight forward, simply select tiles, layer and draw on the screen
to place tiles. Front and back decorations have no ingame effects, other than
be shown on screen. Everything "solid" need to be in the "Normal Tile" layer.

There are 3 special objects:

- Player spawn
- Exit Switch
- Exit Door

Only one of each can exist per level, so placing any of them on the map will
remove any other instances. Select any of them from the "Objects" panel to
place them, these ignore layer and are always placed on the "Normal Tile" layer.

Select "Tile" in the "Objects" panel to go back to placing tiles.

The "Tiles" panel lists all available tiles defined from `assets.txt`. Scroll
up or down in it to reveal more items.

Once the editor level is saved, it can be played quickly from the "Play Editor Level"
menu entry, as a single world level. This allows you to go back and forth between
playing the level and editing it.

Once a level is done, you can rename it to something else, or "level<n>.txt" where
`n` is a number, and add it to the list in `configs.txt`. Doing so and using
the regular "Play" menu entry, will start at the first one defined, and progress
to the next one when cleared, until there are no more levels in the list.

# Credits

All textures/fonts were custom made by me.
