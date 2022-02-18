# Bolt-Industries
Welcome to the Bolt Industries Github Repo.

Here you will find firmware for the Pico 87 Mechanical Keyboard, as well as the open source hardware. 

There are two firmwares that can be used on te Pico 87 Mechanical keyboard. 
The first is BMK. BMK stands for Bolt Industries Mechanical Keyboard. It is a 
custom built firmware written in the Arduino IDE. If you are comfortable using the 
Arduino IDE, then You will probably like BMK. To install it, simply download the 
sketch BMK.ino and follow the directions in the sketch. 

KMK is a mechanical keyboard firmware that runs on circuit python. 
Installing KMK is a little more involved.
1. Download and flash circuit python to your Pico. https://circuitpython.org/board/raspberry_pi_pico/
2. Download and unzip the KMK firmware from the KMK repo. https://github.com/KMKfw/kmk_firmware
3. Plug in your keyboard without holding down the bootsel button, and a drive called circuit python should appear. 
4. Copy boot.py from the KMK firmware, and main.py from this repository onto your circuit python drive (your pico) 
5. Your keyboard should immidatly start working!

In the hardware file, you will find a the schematic and PCB's in .PDF form as well as .JSON that can be imported into Easy EDA. 

![image](https://user-images.githubusercontent.com/58665565/154625246-d543506a-e4ff-4449-9799-5506421c1dbc.png)
