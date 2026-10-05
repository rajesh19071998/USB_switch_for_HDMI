# CH32V003A4M6 Switch and LED Controller

Five active-low switches control five LEDs. The TM1637 display shows `L1` through
`L5` for the lowest-numbered active LED, or `OFF` when all LEDs are off.

| Signal | GPIO |
| --- | --- |
| SW1, SW2, SW3, SW4, SW5 | PC1, PC2, PC3, PC4, PC6 |
| LED1, LED2, LED3, LED4, LED5 | PC0, PA2, PA1, PD6, PD5 |
| TM1637 CLK, DIO | PC7, PD4 |

Connect each switch between its GPIO and ground; internal pull-ups hold an open
switch high. Connect the TM1637 `VCC` to the board supply and its `GND` to the
board ground. Keep `PD1` reserved for the WCH-Link SWIO programming connection.