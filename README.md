![Logo](https://github.com/LuisGloria/ZILO-SYS/blob/main/Logo.png)

# ZILO-SYS
This OS has been inspired on the mainframe systems like z/OS && VM/ESA for example,
The bootloader and kernel are originally from [nanobyte_os](https://github.com/nanobyte-dev/nanobyte_os/), but with my own aditions and libs.

## New in version 1.7:
* A lotta things

* Note, this is still a beta OS, might be unstable, things may and will change and it's more of a hobby project rather than an actual usable system.
For example. editing datasets is still unstable and VGA graphics mode is not implemented yet (might never do it...)

## Prerequisites

The project requires a Unix-like environment to build. If you are using Windows you can either install WS or run a Linux virtual.
Recommend using [WLS](https://learn.microsoft.com/en-us/windows/wsl/install).

Tools needed to build:
* `make`
* `nasm`
* `mtools`
* dependencies to build a GCC cross compiler (see the section below)
* `qemu-system-x86` for testing
* `bochs-x bochsbios vgabios` for debugging
* `Open Watcom v2 16-bit compiler` for bootloader compilation

## Building

1. Install the following dependencies:

```
# Ubuntu, Debian:
$ sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo \
                   nasm mtools qemu-system-x86
           
# Fedora:
$ sudo dnf install gcc gcc-c++ make bison flex gmp-devel libmpc-devel mpfr-devel texinfo \
                   nasm mtools qemu-system-x86
```

2. Install the [Open watcom v2 compiler](<github.com/open-watcom/open-watcom-v2>). 
NOTE: Install the x86 installer since the x64 is known to segfault in the installation and has some issues.

3. Run `make toolchain`, this should download and build the required tools (binutils and GCC). If you encounter errors during this step, you might have to modify `build_scripts/config.mk` and try a different version of **binutils** and **gcc**. Using the same versions as in your distribution's repositories is your best bet. Run `make clean-toolchain-all` to delete all toolchain related files and start over. 

4. Run `make`.

## Running with qemu

* run `./run.sh`

## Debugging with bochs

* run `./debug.sh`

**Troubleshooting**: Bochs has proven to be pretty unreliable. Check [this article](https://github.com/nanobyte-dev/nanobyte_os/wiki/Frequent-issues#bochs-doesnt-work) for some troubleshooting tips.

## Links

* [Twitter/X](https://x.com/LG10sys)

# IMPORTANT!

Once again, this OS has [nanobyte_os](https://github.com/nanobyte-dev/nanobyte_os/) code at it's core, go support nanobyte :3

Also, if you're gonna contribute, do NOT delete the thingamabob, it is very import and it looks for the doodab, and the kernel needs his doodab.

Also, if it spats out any error of $XDG_RUNTIME not being set, remove any lines in ./run.sh about audio.
