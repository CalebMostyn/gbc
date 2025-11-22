# gbc
Original Game Boy (DMG) Emulator Written in C

# Architecture
.. here is where I may include details on the DMG hardware and my implementation of it

# Specs
.. specs of the software

Windowing and GUI are handled using [raylib](https://github.com/raysan5/raylib/) and [raygui](https://github.com/raysan5/raygui/). File dialogs for desktop versions are handled using [tinyfiledialogs](https://sourceforge.net/projects/tinyfiledialogs/).

# Requirements
To build completely locally, `raylib` and `raygui` and their dependencies are required, which may be available via the package manager depending on the platform ([see here](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux)) or you may have to build it manually from source. To build locally with the build image, docker is required.

To get the latest versions of the external libraries (other than `raylib`):

```bash
curl -L https://sourceforge.net/projects/tinyfiledialogs/files/latest/download -o tinyfd.zip && \
unzip tinyfd.zip && \
mv tinyfiledialogs/tinyfiledialogs.c lib/ && \
mv tinyfiledialogs/tinyfiledialogs.h lib/ && \
rm tinyfd.zip && rm -rf tinyfd/ && \
git clone https://github.com/raysan5/raygui.git && \
mv raygui/src/raygui.h lib/ && \
rm -rf raygui 
```
# Build
Building locally can be assisted with the build docker image and build script:

```bash
# Build an instance of the image
docker build -t ghcr.io/calebmostyn/gbc-build:<version> .
# Run the build process from within a container
./build-local.sh
```

# Usage
.. pertinant details on using the emulator (settings, controls, etc)

# Resources
- Emulation
    - [Basics of Emulation](http://www.xsim.com/papers/Bario.2001.emubook.pdf)
- Game Boy Resources
    - [Game Boy emulator written in Rust](https://rylev.github.io/DMG-01/public/book/introduction.html)
    - [Game Boy: Complete Technical Reference](https://gekkio.fi/files/gb-docs/gbctr.pdf)
    - [Game Boy architecture analysis](https://www.copetti.org/writings/consoles/game-boy/)
    - [Nintendo's Game Boy programming manual](https://archive.org/details/GameBoyProgManVer1.1)
    - [Pandocs Game Boy Dev Documentation](https://gbdev.io/pandocs/)
    - [RealBoy, emulating the Game Boy core](https://realboyemulator.wordpress.com/posts/) 
    - [Game Boy CPU manual](https://realboyemulator.wordpress.com/wp-content/uploads/2013/01/gbcpuman.pdf)
    - [Game Boy opcode table](https://izik1.github.io/gbops/)
    - [Game Boy Memory Map](http://gameboy.mongenel.com/dmg/asmmemmap.html)
    - [Game Boy docs (cycle-accurate)](https://github.com/AntonioND/giibiiadvance/blob/master/docs/TCAGBD.pdf)
- raylib
    - [raylib website](https://www.raylib.com/index.html)
    - [Quick Software GUI with raylib](https://youtu.be/KSKzaeZJlqk)
