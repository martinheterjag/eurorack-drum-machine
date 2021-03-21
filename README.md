# eurorack-drum-machine

This project uses cross-pi-gcc-9.2.0-0 to cross compile to a Rasberry Pi Zero. I build on an Ubuntu VM that i use for Linux development.

To build and install run the following commands:

``` bash
cd Sampleplayer/Builds/LinuxMakefile
source envars
make
# Change 'pi' and 'home/pi' to correct username, if not using the default username for your Raspberry Pi
scp build/Sampleplayer pi@pizero:/home/pi
```

I have set up systemd to start the sampleplayer binary by adding the systemd script `sampleplayer.service` to `/etc/systemd/system` on the raspberry pi zero and enabled it with `systemctl enable sampleplayer` (root access needed, so you probably need to run it with sudo).
