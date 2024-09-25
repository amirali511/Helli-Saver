# Helli-Saver
![](HelliSaver.png)
A Simple Screen Saver For Linux.

## Description
Helli-Saver is a simple screen saver for Linux that does not let the screen be empty with simple and elegant rectangles.

## Setup
Install the dependencies `xcb`, `xcb_keysyms`, `x11`, `c`, `meson`, `ninja`, and `bash`.

## Installation or Uninstalling
To install run `configure.sh` or
```bash
meson setup build;
ninja -C build;
sudo ninja -C build install;
```

To uninstall run `configure.sh` or
```bash
sudo ninja -C build uninstall;
rm -R build;
```

## Run
Run it in a separate vt or in a desktop environment using:
```bash
# Desktop Env
hellisaver
# VT
startx hellisaver
```