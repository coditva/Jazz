# Serial Port I/O

## What
Serial ports are ancient ports which are/were used to connect external devices.
They are also known as COM ports.

## How
- Initialize the ports.
- Send data to write to it.

## See it in action
Enable output to serial ports:
- Edit `kernel/config.h` to enable outputting to serial ports:
```c
/* uncomment this line: */
# define DEBUG_TO_SERIAL
```
Compile and run:
```bash
cd kernel
make qemu
```
Check the output of `/tmp/jazz_serial1_XXXXXX.log`:
```bash
cat /tmp/jazz_serial1_*

# Initializing Serial ports... OK
# Initializing IDT... OK
# Initializing ISRs... OK
# Initialization complete.
```
