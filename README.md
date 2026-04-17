<<<<<<< HEAD
# ZILO-SYS
An entire OS written from scratch inspired by Mainframe z/OS && VM/ESA systems.

This OS is inspired in nanobyte OS, in fact it contains part of it's code.
=======
![Logo](https://github.com/LuisGloria/ZILO-SYS/blob/main/Logo.png)
This OS has been inspired on the mainframe systems like z/OS && VM/ESA for example,
The bootloader and kernel are originally from [nanobyte_os](https://github.com/nanobyte-dev/nanobyte_os/), but with my own aditions and libs.

## New in version 1.3b:
* VGA Mode (not used yet)
* Paging
* Shell updates
* Return Codes (retc)
* PIT driver

* Note, this is still a beta OS, might be unstable, things may and will change and it's more of a hobby project rather than an actual usable system.
>>>>>>> 9880b65 (Version 1.3b)

## Prerequisites

The project requires a Unix-like environment to build. If you are using Windows you can either install WS or run a Linux virtual.
Recommend using [WLS](https://learn.microsoft.com/en-us/windows/wsl/install).

<<<<<<< HEAD
Tools needed to build:
=======
You need the following tools to build:
>>>>>>> 9880b65 (Version 1.3b)

* `make`
* `nasm`
* `mtools`
* dependencies to build a GCC cross compiler (see the section below)
* `qemu-system-x86` for testing
* `bochs-x bochsbios vgabios` for debugging
* your preferred text editor

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

2. Run `make toolchain`, this should download and build the required tools (binutils and GCC). If you encounter errors during this step, you might have to modify `build_scripts/config.mk` and try a different version of **binutils** and **gcc**. Using the same versions as in your distribution's repositories is your best bet. Run `make clean-toolchain-all` to delete all toolchain related files and start over. 

3. Run `make`.

## Running with qemu

* run `./run.sh`

## Debugging with bochs

* run `./debug.sh`

**Troubleshooting**: Bochs has proven to be pretty unreliable. Check [this article](https://github.com/nanobyte-dev/nanobyte_os/wiki/Frequent-issues#bochs-doesnt-work) for some troubleshooting tips.
<<<<<<< HEAD
=======

## Links

* [Twitter/X](https://x.com/LG10sys)
>>>>>>> 9880b65 (Version 1.3b)
