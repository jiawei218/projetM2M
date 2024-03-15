#include "composents.h"

composents composent;

// Setup function
void setup() {
  Serial.begin(9600);
  Serial.println("Lancement de l'app"); // App launch message

  if(composent.setup()!=0) {
    Serial.println("Device error");
  }

}

// Main loop
void loop() {
    composent.run();
}
