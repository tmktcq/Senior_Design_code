#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver; 
uint8_t buff[32]; 
uint8_t bufflen = sizeof(buff); 

void setup() {
  // put your setup code here, to run once:
  pinMode(11, INPUT); 
  Serial.begin(9600); 
  driver.init(); 
  pinMode(2, OUTPUT); 
}

String str; 
void loop() {
  if(driver.recv(buff,&bufflen))
  {
    str = (char*)buff; 
    if(str == "UNSAFE_LEVELS")
    {
      Serial.print("Opening Garage Door\n"); 
      digitalWrite(2, HIGH); 
      delay(200); 
      digitalWrite(2, LOW); 
    }
  }
}
