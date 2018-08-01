#include <Arduino.h>
#include <SmartLeds.h>

const int LED_COUNT = 144;
const int DATA_PIN = 13;
const int CHANNEL = 0;

// SmartLed -> RMT driver (WS2812/WS2812B/SK6812/WS2813)
// SmartLed leds( LED_WS2812, LED_COUNT, DATA_PIN, CHANNEL, DoubleBuffer );

const int CLK_PIN = 14;
// APA102 -> SPI driver
Apa102 leds(LED_COUNT, CLK_PIN, DATA_PIN, DoubleBuffer);

void setup() {
//   Serial.begin(9600);  
}

uint8_t hue = 0;

void showGradient() {
    hue+=5;
    // Use HSV to create nice gradient
    for ( int i = 0; i != LED_COUNT; i++ )
        leds[ i ] = Hsv{ static_cast< uint8_t >( hue * i ), 255, 5 };
	leds.show();
    // Show is asynchronous; if we need to wait for the end of transmission,
    // we can use leds.wait(); however we use double buffered mode, so we
    // can start drawing right after showing.
}

void showRgb() {
    leds[ 0 ] = Rgb{ 255, 0, 0 };
    leds[ 1 ] = Rgb{ 0, 255, 0 };
    leds[ 2 ] = Rgb{ 0, 0, 255 };
    leds[ 3 ] = Rgb{ 0, 0, 0 };
    leds[ 4 ] = Rgb{ 255, 255, 255 };
    leds.show();
}

void loop() {
    // Serial.println("New loop");
    
	showGradient();
}