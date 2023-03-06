# Wearable-Ai-Assistant
I used an esp32, rasbperry pi, &amp; and a adafruit tft to create an wearble ai-assistant powered by chat GPT (Davinci-03)

# Instructions
The instructions are simply if you have some experience working with electronics, it requires three main parts, and the rest are optional:

Parts:

1 - Spi 2.4 inch ILI9341 display by adafruit: https://www.adafruit.com/product/2478 

2 - Esp32, I reccomend this one: https://www.adafruit.com/product/3405

3 - Raspbery Pi, I used the Zero W: https://www.raspberrypi.com/products/raspberry-pi-zero-w/

4 (Optional) - Battery pack, so you can actually carry it around: https://www.adafruit.com/product/354

5 (Optional) - Switch, conserve battery life: https://www.adafruit.com/product/3220
  
Steps:
 
  Those are all the parts, so the secound thing to do is connect the tft to the esp32. Follow this guide: https://learn.adafruit.com/adafruit-2-4-color-tft-touchscreen-breakout/spi-wiring-test

  Secound, you'll want to setup the raspberry pi headless, if your unaware on how to do that, this is a good tutorial: https://www.tomshardware.com/reviews/raspberry-pi-headless-setup-how-to,6028.html

  Third, you'll want to setup your esp32. Follow this tutorial: https://learn.adafruit.com/adafruit-huzzah32-esp32-feather

  Fourthly, Upload the py to thonny, and connect your desired headphones to the pi. I used bluetooth headphones for ease of acess. Then, upload the ino sketch   to the esp32. 

  Lastly, connect the usb port of your raspberry pi tto the usb port of the esp32, and your essentially finished.
