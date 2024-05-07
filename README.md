# Backroom's Escape Room Microcontroller Code

## D1-RF

D1-RF is code for the MFRC522 readers on the WeMos D1 revision 2. This uses multiple readers and compares the rfid tags to the solution which is a spesific tag to reader with SPI standard.

```
Notes
- Each reader needs 13-26 mA/3.3V DC so project needs external power for each reader

```

#### Update
- Will need to add button to solve puzzle in-case something goes wrong.


## D1-MATRIX

D1-Matrix is code for the led pattern puzzle. There is a preset light pattern that corresponds to spesific buttons with one button that replays the pattern and clears what order the buttons were pressed.


## RELAY-Test (Nano Atmega168)
Test code used to trigger relay. Based on Nano3

