# FissionFailure64 for Nintendo64

The GreenGlo power plant is in dire need of your help!

N64 game developed for the N64brew Jam #2

Support 1 player and Rumble Pak.

![title](./resources/screenshots/title.png?raw=true)
![station1](./resources/screenshots/station1.png?raw=true)
![station2](./resources/screenshots/station2.png?raw=true)
![station3](./resources/screenshots/station3.png?raw=true)
![credits](./resources/screenshots/credits.png?raw=true)

## Note

This ROM file has been tested to work on real Nintendo 64 hardware using the
[EverDrive-64 by krikzz](http://krikzz.com/). It should also work with
[64drive by retroactive](http://64drive.retroactive.be/).

This ROM uses [libdragon](https://github.com/DragonMinded/libdragon) instead of
the proprietary SDK used by official licensed Nintendo software.

## Building

### Using Docker

All development can be done using Docker. It's the easiest way to build the ROM on Windows and MacOS.

 * Install [Docker](https://docker.com)
 * Run `make` to produce the debug `FissionFailure64.z64` ROM file.
 * Run `make release` to produce the release `FissionFailure64-release.z64` ROM file.

### On Linux

* Install [libdragon development toolchain](https://github.com/DragonMinded/libdragon)
* Install build tools used by the asset pipeline: `imagemagick`.
* Run `make` to produce the debug `FissionFailure64.z64` ROM file.
* Run `make release` to produce the release `FissionFailure64-release.z64` ROM file.

## Testing

* Install [ares](https://ares-emu.net/).
* Run `make ares` to build and launch the debug ROM.
* Run `make ares-release` to build and launch the release ROM.

## Thanks

* Thanks to [Jennifer Taylor](https://github.com/DragonMinded) for libdragon.
