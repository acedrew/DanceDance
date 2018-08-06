# DanceDance
Project in development to drive a wall/curtain of WS281x LED strands based on kinetic inputs from tangible sensors.
This project is a collaborative work of the Cha Art/Dev Collective

# Assumptions:
Needs to drive 2500 LEDs, arranged as 50 strips of 50 LEDs
Using ESP32 MCUs driving 8 channels of output each
Controlling at 800Khz, each strip state refresh takes ~2-3ms to send
Two strategies for controlling 50 strips:
* Use 7 MCUs to drive 8 channels each at full 4-500hz refresh rate
* Use 4 MCUs with multiplexed outputs via Output enable on level shifters to drive 16 strips each at 1/2 refresh rate 200hz or so
* Running longer strands is not an option, as there's no return path from the end of each strand to the next one the way they're hanging
Sensor inputs:
* Stationary bicycle, either a thrift-store exercise bike, or a custom stationary art bike
* Accelerometer powered Cymbals or Maracas
* Piezo or Microphone powered stomp platform?
* Loadcell violin?
# Current Status:
* Handcranked "pedal simulator" working with infrared edge detection encoder, 
currently running in loop, should move to dedicated pulse counting peripheral
* Maraca accelerometer working, using MMA7660 3 axis accelerometer, which only 
provides 6 bit resolution, and saturates at 1.5G, proving the concept but not 
appropriate for the application. MPU6050 6 axis accelerometers with programmable
range ordered for final application
* Single MCU control of 8 strands of lights, next step is to test multiplexing using existing level shifter.
