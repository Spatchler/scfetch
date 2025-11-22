# Simple C Fetch

![](.github/focusedScreenshot.png)

A very simple single file C fetch that I put together in a rush because fastfetch was too slow.
The approach to the code was get it working as fast as possible on top of that this was my first time using C
from a C++ backgronud so don't think the code is any good.

This has only been tested on Arch Linux but everything should work except for the package count. To make it work
you will need to edit the _PACKAGE_PATH marco in the file to the appropriate path.

### Usage:
```sh
scfetch [paddingLeft] [boxWidth]
```

(Pipe into echo for very slightly better performance)

### Default:
```sh
scfetch 0 35
```

### Build with gcc:
(Remove the macros for the elements you dont want, remove the _BARS macro if you want ram and swap to display as text)

```sh
gcc -D_KERNEL -D_UPTIME -D_SHELL -D_RAM -D_SWAP -D_PROCESSES -D_PACKAGES -D_USER -D_HOSTNAME -D_DISTRO -D_BARS main.c -o scfetch
```

![](.github/screenshot.png)

