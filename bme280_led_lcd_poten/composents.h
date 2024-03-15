#ifndef composents_H
#define composents_H

#include "Seeed_BME280.h"
#include "rgb_lcd.h"
#include <ChainableLED.h>


/**
 * @file composents.h
 * @author Jiawei CHEN and Hugo Lin
 * @brief Controller for BME280 capture, LED, LCD and potentiomètre
*/


// Number of LEDs
#define NUM_LEDS 2

// RGB values for LCD backlight color
const int colorR = 0;
const int colorG = 123;
const int colorB = 0;

// Enum to represent the state of information displayed on the LCD
enum DisplayState {
   TEMPERATURE,
   HUMIDITY,
   PRESSURE
};

const long interval = 500; // Blink interval in milliseconds

const long intervalLCD = 2000; // LCD update interval in milliseconds


class composents
{
private:
    // BME280 sensor object for temperature, humidity, and pressure
    BME280 bme280; // I2C

    // LCD object for displaying information
    rgb_lcd lcd; // I2C

    //analog sensor value from Potentiomètre
    int sensorValue;

    // Variables for timing LED blink and LCD update intervals
    unsigned long previousMillis;
    unsigned long previousMillisLCD;
    // Initial display state
    DisplayState currentDisplayState;

public:
    composents();
    ~composents();
    int setup();
    void updateLCD(unsigned long currentMillisLCD);
    void controlLED(int value);
    void setPotentiometreValue(int value);
    int getPotentiometreValue();


};


#endif