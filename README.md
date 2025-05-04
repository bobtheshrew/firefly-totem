# firefly-totem

Voltage Cut Off Instructions
Double press the “+” button, and when the decimal point is flashing, press the “+” or “-" button to set the disconnect voltage.
Double press the “-" button, and when the decimal point is flashing, press the “+” or “-" button to set the difference value.
Long press the “+” button, and when the LED screen is flashing, press the “+” or “-" button to calibrate the input voltage.
Long press the “-" button, and then press the “+” or “-" button to set the delay value T. That is to say, when the output voltage reaches the reconnect value, it will delay T minutes, the load reconnects. The delay time range from 000 to 010 minutes, and it displays like: 000 or 001 or 005 or 010. Note that no decimal point is displayed at this time.
Press the “+” button to view disconnect voltage.
Press the “-" button to view difference value.

*What is the "difference value"?

The value between the disconnect voltage and the reconnect voltage. That is to say, difference value = reconnect voltage - disconnect voltage
For example
1. How to set if I want it to stop outputting when the voltage is lower than 11V and start when the voltage is over 11.6V?
Long press “+” and the LED starts flashing, press the “+” or “-" button to set the value as 11V. After the flashing stops, long press “-" and when the LED is flashing, press the “+” or “-" button to set the difference value as 0.6V.
2. How to set if I want it to stop outputting when the battery voltage is lower than 12.5V and when it is over 14.8V, it starts outputting after 2 minutes?
Double press “+” button to set it as 12.5 and then double press “-" button to set the difference value as 2.3 (because 14.8V-12.5=2.3V). And then press and hold on the “-" button, set T=2.

COLOR INDEX;
https://make.wordpress.org/core/files/2021/02/wordpress-admin-color-palette-WP57.png