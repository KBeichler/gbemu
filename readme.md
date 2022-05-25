# Gameboy Emulator in C 

Bad implementation of a GameBay Classic emualtor in C. Graphics is done via SDL2.

**earning project**

#### Overview
The system consits of one main .c file. All other functions are declared and defined in .h files. 


## Architecture

The plan right now is to keep the system as modular as possible. the idea is to port it to an embedded system in the future.

The main aprts are:
* **CPU**
  * main cpu emulation. Implements all the opcodes, registers and clock
* **MEM**
  * handels all the memory access. That means access to the game ROM (MBC support is planned), RAM, optional RAM
* **IO Device**
  * handles user input. close conenction to MEM
* **PPU**
  * draw background, window and sprites to VRAM
* **Display**
  * right now SDL2 is used to display the VRAM, but different implementations are planned (possible SPI display)

Some kind of debugging interface is planned, main focus on memory inspection.

## Used resources

[Blargg's Gameboy hardware test ROMs. Originally hosted at http://blargg.parodius.com/gb-tests/](https://github.com/retrio/gb-test-roms)

[Pandocs](https://gbdev.io/pandocs/)

[Everything else](https://github.com/gbdev/awesome-gbdev)

  



