# ESP32 + NodeCMU firmware for Sniper target. 

--------------------------------------------
## Libraries

PlatformIO lib repo:  
https://registry.platformio.org/

Web Server:
https://registry.platformio.org/libraries/esphome/ESPAsyncWebServer-esphome

JSON:
https://registry.platformio.org/libraries/bblanchon/ArduinoJson

--------------------------------------------
## Project is divided into folders:
* include - all headers are here
* src - all cpp files are here

LED pin layout can be seen in ledService.cpp, default:
* red pin - pin D6
* green pin - pin D7

WiFi setup can be seen in wifiService.cpp :
* ssid: "Sniper"
* password: "bulletproof"
* IP: 192.168.4.1

Web server can be seen in serverService.cpp :
 * web server port: 80

--------------------------------------------
## How to build:

Build this project with "Build" option in PlatformIO plugin in VS Code.

--------------------------------------------
## How to run:

1. Upload web server files("data" folder) - use PlatformIO task "Upload Filesystem Image" 
2. Run this project on ESP chip with "Upload" option in  plugin in VS Code.

--------------------------------------------
## Changing web server files ("data" folder)

Regular upload DO NOT change this files.
To upload new web page data:

In PlatformIO plugin run task "Erase Flash" and then run "Upload Filesystem Image" and "Upload" tasks.

--------------------------------------------
## Output checkout

Logs, can be viewed using VS Code plugin PlatformIO "Monitor" option.
Monitor baud (bitrate) value can be found in sniperMain.cpp, default: 115200.

--------------------------------------------
## Chaning partitions size (usefull when no space for data folder)

In `platformio.ini` 

change ldscript name to one from this list:  
https://github.com/esp8266/Arduino/tree/master/tools/sdk/ld  

or make custom:
```
board_build.ldscript = custom.ld
```
where `custom.ld` file need to be in main project directory.