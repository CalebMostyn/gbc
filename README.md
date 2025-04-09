# gbc
Original Game Boy (DMG) Emulator Written in C

# Architecture
.. here is where I may include details on the DMG hardware and my implementation of it

# Specs
.. specs of the software

Initially I am eyeing [raylib](https://github.com/raysan5/raylib) as a library to handle much of the windowing and graphics. `raylib` is fairly simple, written in C, and should provide a straightforward interface to create a GUI and emulated LCD display for the Game Boy. `raylib` also can be compiled targeting WebAssembly, which is making me consider a webhosted version of the emulator as a possibility.

# Requirements
`raylib` and its dependencies are required, which may be available via the package manager depending on the platform ([see here](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux)) or you may have to build it manually from source.

Also using `tinyfiledialogs` for lightweight, cross-platform file dialogs.

# Build
.. details on the build process

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