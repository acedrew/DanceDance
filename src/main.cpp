#include <Arduino.h>
#include <SmartLeds.h>
#include <Wire.h>
#include "MMA7660.h"
MMA7660 accelerometer;

#define HIGH_THRESHOLD 2800
#define LOW_THRESHOLD 2000
#define COUNT_PIN 26

#define LED1_DATA 13
#define LED1_CHANNEL 0
#define LED2_DATA 12
#define LED2_CHANNEL 1
#define LED3_DATA 14
#define LED3_CHANNEL 2
#define LED4_DATA 27
#define LED4_CHANNEL 3

#define SPEED_STEPS 10


const int LED_COUNT = 100;
const int CHANNEL = 0;

int position = 0;
bool isHigh = false;
int printLoop = 0;
int printTime = 250;
uint8_t hue = 0;
uint8_t hue1 = 0;
uint8_t hue2 = 0;
uint8_t hue3 = 0;
uint8_t hue4 = 0;

int speedPosition = 0;
int speedSum = 0;
uint8_t speed = 0;
int speedLoop = 0;
int speedTime = 100;


Rgb color1 = {255,0,0};
Rgb color2 = {0,0,0};
Rgb color3 = {0,0,0};
Rgb color4 = {0,0,0};

/* accelerometer memory */
int8_t x = 0;
int8_t y = 0;
int8_t z = 0;

void display();

// SmartLed -> RMT driver (WS2812/WS2812B/SK6812/WS2813)
// SmartLed leds( LED_WS2812, LED_COUNT, DATA_PIN, CHANNEL, DoubleBuffer );
// APA102 -> SPI driver
SmartLed led1( LED_WS2812, LED_COUNT, LED1_DATA, LED1_CHANNEL, DoubleBuffer );
SmartLed led2( LED_WS2812, LED_COUNT, LED2_DATA, LED2_CHANNEL, DoubleBuffer );
SmartLed led3( LED_WS2812, LED_COUNT, LED3_DATA, LED3_CHANNEL, DoubleBuffer );
SmartLed led4( LED_WS2812, LED_COUNT, LED4_DATA, LED4_CHANNEL, DoubleBuffer );


void setup() {
    pinMode(COUNT_PIN, INPUT);
    delay(10);
    accelerometer.init();
    delay(10);
    Serial.begin(115200);  
    display();
}

void display() {
    led4.show();
    led3.show();
    led2.show();
    led1.show();
}

void updateLeds() {
    for (int i=0; i<LED_COUNT; i++) {
        led1[ i ]  = (i-position) % 3 == 0 ? Rgb(Hsv(hue1, 255, speed)) : Rgb(0,0,0);
    }
    for (int i=0; i<LED_COUNT; i++) {
        led2[ i ]  = (i-position) % 3 == 0 ? Rgb(Hsv(hue2, 255, speed)) : Rgb(0,0,0);
    }
    for (int i=0; i<LED_COUNT; i++) {
        led3[ i ]  = (i-position) % 3 == 0 ? Rgb(Hsv(hue3, 255, speed)) : Rgb(0,0,0);
    }
    for (int i=0; i<LED_COUNT; i++) {
        led4[ i ]  = (i-position) % 3 == 0 ? Rgb(Hsv(hue4, 255, speed)) : Rgb(0,0,0);
    }
}
void checkAcc() {
    if (millis() % 4 == 0) {
        accelerometer.getXYZ(&x, &y, &z);
        // Serial.printf("accx = %u accy = %u accz = %u\n", x, y, z);
        if(abs(x) >= 30 || abs(y) >= 30 || abs(z) >= 30) {
            speed = 255;
        }
    }
}

void updatePosition() {
    position++;
}


void loop() {
    int loopMillis = millis();
    checkAcc();
    // Serial.println("New loop");
    if(loopMillis > printLoop + printTime){
        printLoop = loopMillis;
        int sensor = analogRead(COUNT_PIN);
        Serial.printf("accx = %i accy = %i accz = %i; Raw Crank = %u, speed = %u\n", x, y, z, sensor, speed);
        // Serial.printf("sensor value is: %u speed is %u\n", sensor, speed);
    }

    if(loopMillis > speedLoop + speedTime) {
        speedLoop = loopMillis;
        int positionDelta = (position - speedPosition) * 13;
        speedSum += positionDelta;

        // speed = speedSum < 255 ? speedSum : 255;
        speed = speed < 255 - positionDelta ? speed += positionDelta : 255;

        // speedSum = speedSum - 10 > 0 ? speedSum - 10 : 0;
        int speedDiv = (int)((float)(speed) / 10.0);
        int speedInc = speedDiv > 0 ? speedDiv  : 1;
        speed = speed - speedInc > 0 ? speed - speedInc : 0;
        speedPosition = position;
        updateLeds();
        display();
    }
    
    int data = analogRead(COUNT_PIN);
    if(position % 15 == 0) {
        // color1 = Hsv(hue, 255, speed);
        hue1 = hue;
        if(loopMillis % 2 == 0) {
            hue += 5;
        }
    }

    if(position % 50 == 0) {
        hue2 = hue + 128;
        // color2 = Hsv(hue + 128, 255, speed);
        if(loopMillis % 10 == 0) {
            hue += 20;
        }
    }

    if(position % 10 == 0) {
        hue3 = hue + 64;
        // color3 = Hsv(hue+64, 255, speed);
    }

    if(position % 5 == 0) {
        hue4 = hue;

        // color4 = Hsv(hue, 255, speed);
        hue++;
    }


    if (data > HIGH_THRESHOLD && !isHigh) {
        updateLeds();
        display();
        updatePosition();
        isHigh = true;
        // Serial.println(position);
    }
    if (data < LOW_THRESHOLD && isHigh) {
        updateLeds();
        display();
        updatePosition();
        isHigh = false;
        // Serial.println(position);
    }
}