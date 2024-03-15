#include "composents.h"

composents composent;

void setup() {
  Serial.begin(9600);
  Serial.println("Lancement de l'app");

   //// Initialize BME280 sensor, LED and LCD with backlight color
   if(composent.setup()!=0)
   {
      Serial.println("device error");
   }

}

void loop() {
  //for two LED
  // Read analog sensor value from pin A0
   composent.setPotentiometreValue(analogRead(A0));
   // Print sensor value to serial monitor

   Serial.println(composent.getPotentiometreValue()); 
  

   //LED control
  composent.controlLED(400);

   // LCD updates
   unsigned long currentMillisLCD = millis();
   composent.updateLCD(currentMillisLCD);
}