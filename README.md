# PES_Final_Project
Code for Final Project for PES, ECEN-5813, Fall 2021 - Due Date >> 12/13/2021 @ 11:59:59 PM MDT

# Author: TAHER UJJAINWALA </br>

Contact me if you are facing issue in execution of the code @ tauj5361@colorado.edu

# Project source details </br>
-PES_Final_Project.c --> Main application script initializes the Four Tones which is then generated using DAC and ADC  </br>
-accelerometer.c --> This file contains all the functions to read the Accelerometer values </br>
-adc.c --> This file contains all the ADC implementation, reading the sample sampled by DAC and Performing analysis and prints the values </br>
-command_line.c --> Reads the data entered by used on terminal and sends the string for further processing </br>
-command_process.c --> Decode the data received from serial and defines the commands line interface functions </br>
-hexdump.c --> prints hex dump from the starting address till the length defined by user </br>
-sysclock. --> This file initialized the system clock </br>
-UART.c --> Initialization function, interrupt handler and standard printf() and getchar() function implementation. Also Initializes UART1 </br>
-cbfifo.c --> This file implements circular buffer functionalities - enqueue, dequeue, length, capacity and reset </br>
-test_cbfifo.c --> Runs the test cases to make Circular buffer solid </br>
-delay.c --> This files runs the hard loop for delay using NOP (No operation) and approximates the requires delay </br>
-gpio.c --> This files contains all the functions required to run LED, Initialize the GPIO for Read-Write and runs Init Test functions </br>
-i2c.c --> This file contains all the Initialization done for I2C bus protocol </br>
-test_i2c.c --> Runs the test case to check the I2C bus and MMA sensor </br>
-

# Project Comments </br>
1) System clock either 48MHz or 24Mhz to drive the I2C and UART peripherals </br>

2) Delay functions to general a request and response frame needed for the RS-485 bus protocol </br>

3) UART0 for command processing to take input from user "Author", "Temp", "Dump", “I2Cdata”, “EMData” and perform lexical analysis and call the required function to display the data on serial terminal </br>

4) I2C initialization to read the Inertial sensor data of X,Y and Z plane </br>

5) UART1 or UART2 for RS-485 data polling from 3 phase meter to develop request-response based algorithm to read data from Modbus meter and display on terminal </br>

6) BONUS - ADC Interfacing of Internal Temperature sensor on channel 26 with Calibration is implemented </br>

# Command processing (Reading input from user to read the values) – User interface </br>
•	Using the concept of A6, command processing technique will be created for the user to poll the data in interrupt mode. User can poll the I2C MMA data as well as Energy meter data by typing I2CData and EMData </br>
•	Displaying I2C accelerometer data either polling or interrupt. I2C driver will be developed to read the X,Y and Z axis data </br>
•	Displaying energy meter data using MAX485 IC from the meter using request response functions </br>
•	BONUS - Implemented ADC to read the Internal Temperature sensor value on channel 26 </br>

# Inertial Measurement using inbuilt accelerometer sensor – MMA8451Q I2C sensor </br>
•	Bare metal coding to initialize and read the accelerometer values </br>

# Serial communication </br>
•	Interfacing UART1 with RS-485 module to read the meter data </br>
•	Developing UART driver to read and write the Modbus data (Read Holding Register) </br>


# Technologies my project demonstrate </br>
•	Concept of command processor  </br>
•	Concept of developing i2C driver </br>
•	UART0 driver to read user input – interrupt based </br>
•	UART1 driver for RS-485 protocol implementation </br>
•	ADC driver to read internal temperature sensor </br>


# Anticipated learning to develop my project </br>
•	I2C driver development </br>
•	UART driver development </br>
•	16 bit CRC development - Online CRC Calculator (tahapaksu.com) </br>
•	Modbus reference - Modbus Protocol (modbustools.com) </br>
•	Software sources - Modscan64 1.0 Download - ModScan64.exe (informer.com) </br>
•	Software sources - RTU/ASCII Master Test Software | Simply Modbus Software </br>
•	Difference between Modbus RTU and Modbus ASCII  </br>
•	Internet of Things Industry </br>


# Developement Details </br>
Software used Developed using MCUExpresso IDE 7.2.0 on Windows 10. </br>
DEBUG Mode - Prints DEBUG Messages to UART terminal at Baud Rate of 9600 and 8N1. </br>
To see the console messages in UART mode change to UART in quick settings and open a new Terminal with 9600 Baud Rate and 8N1 settings </br>
Command Processor works on UART0

# HEX converter reference </br>
https://www.scadacore.com/tools/programming-calculators/online-hex-converter/ </br>

# Video Demo Link </br>
https://drive.google.com/file/d/1okvpnWN0ck3bXVbYMABQSMDsK49VCS6T/view?usp=sharing </br>
