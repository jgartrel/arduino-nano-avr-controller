# AVR Controller Project

This is a simple project designed to control a Harmon Kardon AVR 320 receiver based on sensing the status of an automatic S/PDIF switch using the Arduino Nano (ATmega328P with CH340).

### Getting started
1.  Start the Vagrant Dev Box:
    ```
    vagrant up
    vagrant ssh
    ```
2.  Connect your Elegoo Arduino Nano (ATmega328P with CH340) to the USB port

### Building
1.  From within the Vagrant Dev Box `vagrant ssh`:
    ```
    cd /vagrant
    make && sudo make upload && sudo make monitor
    ```

NOTE: use: `ctrl-a \` to quit from screen


### Useful Info

The following links were useful in putting this example project together:
*  [Arduino CLI Hacking](http://www.raspberryvi.org/stories/arduino-cli.html)
*  [Screen Quick Reference](http://aperiodic.net/screen/quick_reference)
*  [Arduino and Linux TTY](https://playground.arduino.cc/Interfacing/LinuxTTY)
*  [USB on a Vagrant Box](https://sonnguyen.ws/connect-usb-from-virtual-machine-using-vagrant-and-virtualbox/)

This project was tested with:
*  Ubuntu 18.04.1 LTS (GNU/Linux 4.15.0-29-generic x86_64)
*  Ubuntu Package: arduino-core, Version: 2:1.0.5+dfsg2-4.1
*  Ubuntu Package: arduino-mk, Version: 1.5.2-1
*  Arduino HW: Elegoo Arduino Nano v3.0 (ATmega328P with CH340)
*  Vagrant, Version: 2.2.2
*  Virtual Box, Version: 5.2.22
*  Virtual Box Extention Pack, Version: 5.2.22
*  VM Box: geerlingguy/ubuntu1804 Version: 1.0.5
