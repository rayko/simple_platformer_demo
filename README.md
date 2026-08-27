# simple_platformer_demo

TODO

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

The project was built using `clang`, `cmake` and `ninja`, using C++ 20 standard.
Be sure to have the necessary building tools to compile it. There are some helper
scripts provided to help:

- `scripts/setup` will prepare the `build/` directory, runs this once or when you clear `build/` dir
- `scripts/build` will run `cmake` to build the project
- `scripts/run` will execute the resultant binary in `build/bin`, as a followup of `scripts/build`

After cloning, use the setup scripts and copy the contents of `assets/` on the `build/bin`
location:

```
cp -r assets/* build/bin/
```

Build and run.

# Config File

TODO

# Running

Ensure the `build/bin` location has the assets needed before trying to run. Simply
call `scripts/run` or navigate to the binary's location and run it.

TODO

# Credits

TODO

- Font source: [1001freefonts.com](https://www.1001freefonts.com/carbon-block.font)
