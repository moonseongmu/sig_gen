# Microcontroller based direct digital synthesis signal generator

Copyright Moon Seongmu 2026
Licensed under GPL 3.0-or-later

## How this repo is structured

Core functionality for signal generation is in /sig_gen_core. The files in /sig_gen_core do not call any board specific functions. /interface/interface.h defines the functions that the files in /sig_gen_core can call, as well as the functionality that the board specifiic microcontroller code must implement. Board level code in in /boards. To port the code to a new microcontroller/board, implement the functions needed to start up the micrcontroller and the functions in interface.h

# build instrucitons

Setup meson build directory with cross file specified

```
meson setup build --cross-file cross_file.txt
cd build
meson compile
meson compile flash
```