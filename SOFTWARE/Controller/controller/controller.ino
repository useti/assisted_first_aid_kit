#include <SoftwareSerial.h>
#include <GPRS_Shield_Arduino.h>

GPRS gprs(Serial1);

void setup() {
  gprs.powerOn();
  
  Serial.begin(9600); 
  while (!Serial) {
    
  }
  while (!gprs.init()) {
    
    delay(1000);
    Serial.print("Init error\r\n");
  }
  //gprs.sendSMS("+79263995140", "Hello SMS from Amperka!");

  gprs.powerOff();
}

void loop() {
  // put your main code here, to run repeatedly:

}
