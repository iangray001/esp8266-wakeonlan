# Wake-on-LAN on the ESP 8266
A small Wake-on-LAN server powered by the ESP8266.

## Description
[Wake-on-LAN](https://en.wikipedia.org/wiki/Wake-on-LAN) is an Ethernet standard that is used to turn on a computer with a network message. It useful for allowing servers to sleep when idle to conserve power, yet still always be remotely accessible.

## Use
When programmed to an ESP module such as the [esp01](http://esp8266.co.uk/modules/esp-01/) the ESP will boot, associate with a wifi network, and create an HTTP server. You can then go to the ESP's IP address with a web browser to instruct it to send a Wake-on-LAN packet. 

## Building
Before building, set credentials for the target wifi network in `settings.h`.

The best way to build this project is with [platformio](http://platformio.org/), which is a build system for network-enabled microcontrollers, or with [Arduino IDE](https://www.arduino.cc/en/main/software).

To use it with Arduino IDE simply install it and add [ESP8266 additional board manager URL](http://arduino.esp8266.com/package_esp8266com_index.json), then install it with the board manager, load the project and flash the board. Basically follow the instructions [in this post](http://www.whatimade.today/esp8266-easiest-way-to-program-so-far/) until the COM port selection.

platformio is built in Python an [can be installed](http://docs.platformio.org/en/latest/installation.html) using [pip](https://pip.pypa.io/en/stable/installing/) as follows:

    pip install -U platformio

Once installed, clone this repository and in the clone issue the following:

    platformio run

platformio will read the `platformio.ini` file, which tells it which board to build for. Currently this is the [esp01](http://esp8266.co.uk/modules/esp-01/). It will download the necessary toolchain and build the code.

## Programming
If you have [NodeMCU devkit board](https://hackerstribe.com/wp-content/uploads/2015/07/NodeMCU_DEVKIT_1.0-300x200.jpg) you can simply use [esptool](https://github.com/espressif/esptool) to flash it and then write software as on any Arduino device.
After you've used esptool, just follow the instructions [in this post](http://www.whatimade.today/esp8266-easiest-way-to-program-so-far/) until the COM port selection and then compile and load your code. 

That's the easiest way currently (2017).

In general you can use a programming rig. There are many examples of this available, like the one described [in this post](http://www.whatimade.today/esp8266-easiest-way-to-program-so-far/) after the COM port selection.

Essentially you make the following connections:

* `TX` and `RX` of a USB-Serial converter to the ESP.
* `GND` to ground of the USB-Serial converter.
* `PWR` to a 3.3v, 300mA supply. (Note an Arduino's 3v3 pin does not provide enough current. Try a 3.3v linear regulator like the LD1117)
* `GPIO0` through a momentary-action push-to-make button to ground.
* `reset` through a momentary-action push-to-make button to ground.

Start the ESP in programming  mode by holding `reset` and `GPIO0` to ground, releasing `reset`, and then releasing `GPIO0`. Then issue the following command:

    platformio run -t upload

platformio will build the project if necessary, and then upload the binary to the flash of the ESP. The ESP will reset after programming and should be operational.
