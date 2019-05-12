# Implementing kprintf

[This commit](https://github.com/coditva/Jazz/commit/b9b4fcd33a5f8c07028d90c12cd1ac8bcac1e1c7) adds our `kprintf` implementation.


## What
Our first step towards kernel functions is the `kprintf` function which is like a limited `printf` function in C. It can take variable arguments and print them according to the format specified. Only `char`, `char *` and `int` are supported as of now.

## Why
We need a function that we can use anytime to debug our kernel as we build it by printing out values to check on the run. This function might/will come in handy.


## See it in action

```bash
cd kernel
make qemu
```
