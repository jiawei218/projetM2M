#include "composents.h"

//LED object for controlling LEDs
ChainableLED leds(7, 8, 2);

/** 
* @brief constructor, variable initialization
*/
composents::composents()
{
    previousMillis=0;
    previousMillisLCD=0;
    sensorValue=0;
    currentDisplayState = TEMPERATURE;

}

/**
 * @brief destructor
*/
composents::~composents()
{
}

/**
 * 
 * @brief BME280 objet, LED objet and lcd objet initialization
 * @return 0 or -1
*/
int composents::setup()
{
    // Initialize BME280 sensor
   if(!bme280.init()){
      // Print error message if BME280 initialization fails
      return -1;
   }
   // Initialize LED strip
   leds.init();
   // Initialize LCD with backlight color
   lcd.begin(16, 2);
   lcd.setRGB(colorR, colorG, colorB);
   return 0;
}

/**
 * 
 * @param currentMillisLCD
 * @brief Update the content displayed on the LCD
*/
void composents::updateLCD(unsigned long currentMillisLCD)
{
    if (currentMillisLCD - previousMillisLCD >= intervalLCD) {
      previousMillisLCD = currentMillisLCD;


   // Switch between displaying temperature, humidity, and pressure on LCD
   switch (currentDisplayState) {
      case TEMPERATURE:
         lcd.clear();
         lcd.print("Temperature :");
         lcd.setCursor(0, 1);
         lcd.print(bme280.getTemperature());
         lcd.print("C");
         currentDisplayState = HUMIDITY;
         break;


      case HUMIDITY:
         lcd.clear();
         lcd.print("Humidity :");
         lcd.setCursor(0, 1);
         lcd.print(bme280.getHumidity());
         lcd.print("%");
         currentDisplayState = PRESSURE;
         break;


      case PRESSURE:
         lcd.clear();
         lcd.print("Pressure :");
         lcd.setCursor(0, 1);
         lcd.print(bme280.getPressure());
         lcd.print("Pa");
         currentDisplayState = TEMPERATURE;
         break;
   }}
}

/**
 * @param value
 * @brief Determine whether LED1 is on or off according to 
 *       the value and parameters of the potentiometer, 
 *       and change the brightness of LED2.
*/
void composents::controlLED(int value){
      // LED1 control based on sensor value
   if (sensorValue > value) {
      // Set LED1 color to blue if sensor value is high
      leds.setColorRGB(0, 0, 0, 255); 
   } else {
      // Turn off LED1 if sensor value is low
      leds.setColorRGB(0, 0, 0, 0); 
   }
      // LED2 control based on sensor value
   leds.setColorRGB(1, sensorValue / 4 , 0 , 0);
}


/**
 * @param value
 * @brief Set the value of the potentiometre
*/
void composents::setPotentiometreValue(int value)
{
    sensorValue=value;
}

/**
 * @brief get the value of the potentiometre
 * @return sensorValue
*/
int composents::getPotentiometreValue()
{
    return sensorValue;
}