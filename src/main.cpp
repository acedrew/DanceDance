#include <Arduino.h>
#include <SmartLeds.h>

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
int lastLoop = 0;
int printTime = 250;
uint8_t hue = 0;
int positions[SPEED_STEPS];
int speedIndex = 0;
int speed = 0;


Rgb color1 = {255,0,0};
Rgb color2 = {0,0,0};
Rgb color3 = {0,0,0};
Rgb color4 = {0,0,0};

void display();

// SmartLed -> RMT driver (WS2812/WS2812B/SK6812/WS2813)
// SmartLed leds( LED_WS2812, LED_COUNT, DATA_PIN, CHANNEL, DoubleBuffer );
// APA102 -> SPI driver
SmartLed led1( LED_WS2812, LED_COUNT, LED1_DATA, LED1_CHANNEL, DoubleBuffer );
SmartLed led2( LED_WS2812, LED_COUNT, LED2_DATA, LED2_CHANNEL, DoubleBuffer );
SmartLed led3( LED_WS2812, LED_COUNT, LED3_DATA, LED3_CHANNEL, DoubleBuffer );
SmartLed led4( LED_WS2812, LED_COUNT, LED4_DATA, LED4_CHANNEL, DoubleBuffer );


void setup() {
    memset(positions, 0, SPEED_STEPS);
    pinMode(COUNT_PIN, INPUT);
    Serial.begin(115200);  
    display();
}

void display() {
    led4.show();
    led3.show();
    led2.show();
    led1.show();
}

void updateLeds() {}
    for (int i=0; i<LED_COUNT; i++) {
        led1[ i ]  = (i-position) % 3 == 0 ? color1 : Rgb(0,0,0);
    }
    for (int i=0; i<LED_COUNT; i++) {
        led2[ i ]  = (i-position) % 2 == 0 ? color2 : Rgb(0,0,0);
    }
    for (int i=0; i<LED_COUNT; i++) {
        led3[ i ]  = (i-position) % 4 == 0 ? color3 : Rgb(0,0,0);
    }
    for (int i=0; i<LED_COUNT; i++) {
        led4[ i ]  = (i-position) % 3 == 0 ? color4 : Rgb(0,0,0);
    }
}

void updatePosition() {
    position++;
    positions[speedIndex] = millis() - positions[speedIndex -1];
    speedIndex++;
    if(speedIndex >= SPEED_STEPS) {
        speedIndex = 0;
    }
    int speedSum = 0;
    for(int i; i < SPEED_STEPS; i++){
        speedSum += positions[i];
    }
    speed = (int)(128 * (1.0 / ((float)(speedSum) / SPEED_STEPS)));

}


void loop() {
    // Serial.println("New loop");
    if(millis() > lastLoop + printTime){
        lastLoop = millis();
        int sensor = analogRead(COUNT_PIN);
        Serial.printf("sensor value is: %u\n", sensor);
    }
    
    int data = analogRead(COUNT_PIN);
    if(position % 15 == 0) {
        color1 = Hsv(hue, 255, speed);
        if(millis() % 2 == 0) {
            hue += 5;
        }
    }

    if(position % 50 == 0) {
        color2 = Hsv(hue + 128, 255, speed);
        if(millis() % 10 == 0) {
            hue += 20;
        }
    }

    if(position % 10 == 0) {
        color3 = Hsv(hue+64, 255, speed);
    }

    if(position % 5 == 0) {
        color4 = Hsv(hue, 255, speed);
        hue++;
    }


    if (data > HIGH_THRESHOLD && !isHigh) {
        updatePosition();
        isHigh = true;
        updateLeds();
        // Serial.println(position);
    }
    if (data < LOW_THRESHOLD && isHigh) {
        updatePosition();
        isHigh = false;
        updateLeds();
        // Serial.println(position);
    }
    display();
}