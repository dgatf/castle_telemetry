# Castle Serial Link ESC telemetry to FrSky Smartport

Send Castle Serial Link ESC telemetry to Frsky Smartport using an Arduino Pro Mini 168 or 328P (3.3v or 5v)

## ESC telemetry

Interfacing the ESC with [Castle Serial Link](http://www.castlecreations.com/en/serial-link-010-0121-00) it is possible to send the telemetry to Smartport protocol using an Arduino

## Wiring:

 - For Pro Mini 3.3V: SmartPort Vcc to Arduino RAW. For Pro Mini 5V: SmartPort Vcc to Arduino Vcc
 - SmartPort Gnd to Arduino Gnd
 - Smartport Signal to Arduino PIN_SMARTPORT_RX (8)
 - Smartport Signal to R1 (4.7k)
 - R1 (4.7k) to Arduino PIN_SMARTPORT_TX (11)


<img src="./images/castle_link1.png" width="600">


## Adjust RPM sensor value

- Blades/poles: number of pair of poles * main gear teeth  
- Multiplies: pinion gear teeth


## Configuration

The configuration is modified with a lua script (openTx 2.2 or higher)

<img src="./images/escSp2.png">

Options:

- ESC protocol. HobbyWing Platinum V3, HobbyWing Platinum V4/Hobbywing Flyfun V5 or PWM signal
- Battery. For voltage divider
- PWM. To generate PWM output from ESC serial  (for obbywing Flyfun V5)

Copy the file escSp.lua to the TELEMETRY folder in the sdcard of the Tx and add ther script to a telemetry screen (Script->escSp)

## Flash to Arduino

Using Arduino IDE copy folder *esc_smartport* and open *esc_smartport.ino*. Select board *Arduino Pro or Pro Mini*, processor *ATMega168 or ATMega328P (3.3V 8MHz or 5V 16MHz)* and flash

## [Video](https://youtu.be/Mby2rlmAMlU)
