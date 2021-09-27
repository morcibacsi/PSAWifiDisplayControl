# PSA WiFi display controller

### What is it ?
If you remove your factory fitted head-unit (RD4/RD43/RD45) from your car made by PSA Group you will loose the ability to set up the date and time on the multifunction display. This application creates a wifi access point where you can connect with your smart phone. It contains a captive portal so as soon as you are connected a message gets displayed on your phone to log in. If you tap the message a browser opens with the buttons which you can use to set up your display.

It also adds the ability to change the display mode by pressing the Seek Up and Seek Down buttons simultaneously on the radio remote stalk.

Here is a video on youtube to see how it works:

[![WATCH IT ON YOUTUBE](https://github.com/morcibacsi/PSAWifiDisplayControl/raw/master/images/youtube_preview.png)](https://www.youtube.com/watch?v=8nrd7SJgO-M "Watch it on YouTube")

- The SSID of the access point is: **PSA display AP**
- The password is: **99999999**

Of course you can modify this in the config.h file.

You can use this hardware to interface the CAN bus: [PSAVanCanBridgeHW][psavancanbridgehw]

### Schema

![schema](https://github.com/morcibacsi/PSAWifiDisplayControl/raw/master/images/schema.jpg)

### Building the project

To build the project you will need the following:

 - Arduino IDE with ESP32 board support installed
 - Libraries installed from the Used libraries section

### Used libraries

- [Arduino abstract serial][lib_abstract_serial] (tested version - commit: cda61dd)
- [ArduinoJson v6.13.0][lib_arduino_json]
- [AsyncTCP v1.1.1][lib_async_tcp]
- [ESPAsyncWebServer  v1.2.3][lib_esp_async_webserver]

[lib_abstract_serial]: https://github.com/computergeek125/arduino-abstract-serial
[lib_arduino_json]: https://github.com/bblanchon/ArduinoJson.git
[lib_async_tcp]: https://github.com/me-no-dev/AsyncTCP
[lib_esp_async_webserver]: https://github.com/me-no-dev/ESPAsyncWebServer
[psavancanbridgehw]: https://github.com/morcibacsi/PSAVanCanBridgeHW
