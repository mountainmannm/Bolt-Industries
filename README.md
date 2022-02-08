# Bolt-Industries
The Bolt Industries repo contains the firmware for the Pico 87 mechanical keyboard. 
In this repository you will find two diferant firmwares that can run on the Pico 87 
mechanical keyboard. BMK stands for Bolt Industries Mechanical Keyboard. It is a 
custom built firmware written in the Arduino IDE. If you are comfortable using the 
Arduino IDE, then You will probably like BMK. To install it, simply download the 
sketch BMK.ino and follow the directions in the sketch. 

KMK is a more conventional mechanical keyboard firmware that runs on circuit python. 
Installing KMK is a little more involved.
1. Download and flash circuit python to your Pico. https://circuitpython.org/board/raspberry_pi_pico/
2. Download and unzip the KMK firmware on your local drive.
3. Plug in your keyboard without holding down the bootsel button, and a drive called circuit python should appear. 
4. Copy boot.py from the KMK firmware, and main.py from this repository onto your circuit python drive (your pico) 
5. Your keyboard should immidatly start working!
