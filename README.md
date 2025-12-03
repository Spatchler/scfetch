# Simple C Fetch

![](.github/bars.png)![](.github/nobars.png)

A very simple single file C fetch that I put together in a rush.
The approach to the code was get it working as fast as possible and running as fast as possible so don't think the code is any good.

For me this performs similar to nitch and faster than fastfetch so guess you could say its fast.

This has only been tested on Arch Linux but everything should work except for the package count. To make it work
you will need to edit the _PACKAGE_PATH marco in the file to the appropriate path.

### Usage:
```sh
scfetch [paddingLeft] [boxWidth]
```
Minimum width with bars is 31 without is 30.
Maximum width with bars is 389 without is 456.

(Pipe into echo for very slightly better performance)

### Default:
```sh
scfetch 0 35
```

### Build with gcc:
(Remove the macros for the elements you dont want, remove the _BARS macro if you want ram and swap to display as text)

There is also a _PROFILING macro that times all the modules.

You can also change the colours by changing the colour macros inside the file using [`ansi escapes`](https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797#color-codes).

```sh
gcc -D_KERNEL\
    -D_UPTIME\
    -D_SHELL\
    -D_RAM\
    -D_SWAP\
    -D_PROCESSES\
    -D_PACKAGES\
    -D_USER\
    -D_HOSTNAME\
    -D_DISTRO\
    -D_BARS\
    main.c -o scfetch
```

![](.github/screenshot.png)

