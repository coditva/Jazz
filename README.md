[![Build](https://github.com/coditva/Jazz/actions/workflows/build.yml/badge.svg)](https://github.com/coditva/Jazz/actions/workflows/build.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/coditva/jazz/badge)](https://www.codefactor.io/repository/github/coditva/jazz)

# Jazz
_A small hobby OS_

## About the project
**Jazz** is an Operating System that I have decided to write from scratch to
understand an OS.

## Design Goals
The design goal of this project is to make a very light operating system with
minimal microkernel. A light microkernel means an easy to maintain OS where
each module is easy to test and update. This is also in line with the principle
of separation of mechanism and policy.

## Development
### Build
```bash
make deps       # to build cross-compiler
make kernel     # to build kernel
make all        # to build everything
```

Most of the work is in kernel itself and the standalone kernel can be built
from inside the `kernel/` directory.
```bash
cd kernel/
make            # build the kernel
make qemu       # run the kernel on qemu
make gdb        # run a gdb session with qemu
```

### Troubleshooting
**Building dependencies fails with error: `make -j 8 g++: internal compiler
error: Killed (program cc1plus)`**
This might be because you ran out of memory due to `make` running build in
parallel. Try to change the option `-j 8` to `-j 2` and build again.

### Debug
Aside from `qemu` and `gdb`, there is extensive logging on the serial port
which can be accessed at `/tmp/jazz_serial1.log` file when `qemu` is started
from the makefile as `make qemu`.

## Todo
The todo/plan can be found
[here](https://github.com/coditva/Jazz/blob/master/TODO).

## License
[MIT](https://github.com/coditva/Jazz/blob/master/LICENSE)
&copy; 2018-present [Utkarsh Maheshwari](https://github.com/coditva)
