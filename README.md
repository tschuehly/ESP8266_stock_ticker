# ESP8266_stockticker
A Stock Ticker powered by a ESP8266 and a MAX7219 LED Matrix

# Installation

Connect the Esp and the Matrix:

ESP     LED Matrix
3v  <-> VCC
GND <-> GND
D3  <-> CS
D5  <-> CLK

Create an account on [thingspeak](https://thingspeak.com/) and create a [ThingHTTP](https://thingspeak.com/apps/thinghttp). There you have to paste the URL from the stock site you want to watch, in my case [finanzen.net](https://www.finanzen.net/aktien/AMD-Aktie) and then get the XPATH from the Price Element. Then you copy the API URL into the thingspeakAPI String.
