# Ojo

Ojo is an open soure gtk media player for linux.

![Ojo](/data/OjoScreenshot.png)

## Installation

### Dependencies

#### Ubuntu (18.04.3)
```bash
sudo apt install gcc git meson pkg-config libgtk-3-dev vlc libvlc-dev
```

#### Manjaro (18.1.0)
```bash
sudo pacman -Sy gcc git meson pkg-config gtk3 vlc
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

Copyright 2019 FreaxMATE

Licensed under the terms of the GPLv3 license: https://www.gnu.org/licenses/gpl-3.0.html


