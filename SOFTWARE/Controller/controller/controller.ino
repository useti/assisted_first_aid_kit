#include <GPRS_Shield_Arduino.h>

#define GPRS_WAIT_TIMEOUT 3000
#define TARGET_PHONE_COMMAND "ATD+79030000000;\r\n"
char *LAT[6] = {"3","0","0","0","0","0"};
char *LON[6] = {"5","3","0","0","0","0"};

GPRS gprs(Serial1);
 
void setup()
{ 
  Serial.begin(9600);
  Serial1.begin(9600);
  
  gprs.powerOn();
  
  while (!gprs.init()) {
    Serial.print("GPRS Init error\r\n");
    delay(GPRS_WAIT_TIMEOUT);
  }
  Serial.println("GPRS init success");

  
}

bool first_run = true;

void send_position()
{
  delay(3000);
  // init send
  sim900_send_cmd("AT");
  Serial.print("AT");
  // send lat
  sim900_send_cmd("+VTS=A;");
  Serial.print("+VTS=A");
  for(int i = 0; i< 6; i++){
    sim900_send_cmd("+VTS=");
    Serial.print("+VTS=");
    sim900_send_cmd(LAT[i]);
    Serial.print(LAT[i]);
    sim900_send_cmd(";");
    Serial.print(";");
  }
  sim900_send_cmd("+VTS=*;");
  Serial.print("+VTS=*;");
  // send lon
  sim900_send_cmd("+VTS=B;");
  Serial.print("+VTS=B");
  for(int i = 0; i< 6; i++){
    sim900_send_cmd("+VTS=");
    Serial.print("+VTS=");
    sim900_send_cmd(LON[i]);
    Serial.print(LON[i]);
    sim900_send_cmd(";");
    Serial.print(";");
  }
  sim900_send_cmd("+VTS=#;");
  Serial.print("+VTS=#;\r\n");
  if(sim900_check_with_cmd("\r\n","OK\r\n",CMD,30,30000))
  {
    Serial.println("Coordinates sent");
  }
}

void set_call()
{
  if (sim900_check_with_cmd("AT+DDET=1\r\n","OK\r\n",CMD)){
    if(sim900_check_with_cmd("AT+COLP=1\r\n","OK\r\n",CMD)) {
      if(sim900_check_with_cmd(TARGET_PHONE_COMMAND,"OK\r\n",CMD, DEFAULT_TIMEOUT * 6, DEFAULT_INTERCHAR_TIMEOUT * 20)) {
        Serial.println("Call init");
        delay(500);
        send_position();
      }
      else{
        Serial.println("Something went wrong");
      }
    }
  }
}
 
void loop()
{
  if(first_run)
  {
    first_run = false;
    set_call();
  }
  serialPCread();
  serialGPRSread();
}
 
void serialPCread()
{
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      Serial1.write(Serial.read());
    }
  }
}
 
void serialGPRSread()
{
    if (Serial1.available() > 0) {
      while (Serial1.available() > 0) {
        Serial.write(Serial1.read());
    }
  }
}
