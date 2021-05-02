[![Build](https://github.com/coditva/Jazz/actions/workflows/build.yml/badge.svg)](https://github.com/coditva/Jazz/actions/workflows/build.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/coditva/jazz/badge)](https://www.codefactor.io/repository/github/coditva/jazz)

# Jazz
_Let's write an OS!_

## About the project
**Jazz** is an Operating System that I have decided to write from scratch as a proof of concept to understand the OS by doing.

## Documentation
Most of the documentation is inside individual files.
Apart from that, the development 'transcript' can be found [here](https://coditva.github.io/Jazz/).
Although I am trying to make it as elaborate as possible, it might not be complete.
Also, it is still unverified _n00b_ knowledge.
Don't trust unless you verify. And if you find an error, do report it.

## Development
### Build
```bash
make deps       # to build cross-compiler
make kernel     # to build kernel
make all        # to build everything
```
Most of the work is in kernel itself and the standalone kernel can be build from
inside the `kernel/` directory.
```bash
cd kernel/
make            # build the kernel
make qemu       # run the kernel on qemu
make gdb        # run a gdb session with qemu
```

### Linting
It is recommended to use `clang-tidy` as the linting tool to keep the codebase
clean:
```bash
make lint       # run clang-tidy on source files
```

### Troubleshooting
**Building dependencies fails with 
`make -j 8 g++: internal compiler error: Killed (program cc1plus)` error**
This might be because you ran out of memory due to `make` running build in parallel. Try to change the option `-j 8` to `-j 2` and build again.

### Debug
Aside from `qemu` and `gdb`, there is extensive logging on the serial port which
can be accessed at `/tmp/jazz_serial1.log` file when `qemu` is started from the
makefile as `make qemu`.

## Todo
The todo/plan can be found [here](https://github.com/coditva/Jazz/blob/master/TODO).

## License
[MIT](https://github.com/coditva/Jazz/blob/master/LICENSE)
&copy; 2018-present [Utkarsh Maheshwari](https://github.com/coditva)
