# Ojo

Ojo is an open soure gtk media player for linux.

![Ojo](/data/OjoScreenshot.png)

## Installation

### Dependencies

 - gcc
 - meson
 - pkg-config
 - gtk3
 - vlc
 - libvlc

#### Debian 10.2.0/Ubuntu (18.04.3)
```bash
sudo apt install gcc meson pkg-config libgtk-3-dev vlc libvlc-dev
```

#### Manjaro (18.1.0)
```bash
sudo pacman -Syu gcc meson pkg-config gtk3 vlc
```

### Install from source
```bash
git clone https://github.com/FreaxMATE/Ojo.git
cd Ojo/
meson build && cd build
ninja
sudo ninja install
```

## License

Copyright 2020 FreaxMATE

Licensed under the terms of the GPLv3 license: https://www.gnu.org/licenses/gpl-3.0.html


