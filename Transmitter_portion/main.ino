#include "wifi_module.h"
#include "speaker_module.h"
#include "push_button_module.h"
#include "CO_snsr_module.h"
#include "rf_transmitter.h"

#define TEST_BUTTON         ( 2 )
#define DEBUG               ( 1 )
#define WIFI_SIGNAL_PIN     ( 7 )
#define RF_TRANSMITTER_PIN  ( 12 )


void setup()
{
  pinMode(TEST_BUTTON, INPUT); 
  pinMode(11, OUTPUT); 
  pinMode(WIFI_SIGNAL_PIN, OUTPUT); 
  pinMode(RF_TRANSMITTER_PIN, OUTPUT); 

  /* Initially Set the pin to low */
  digitalWrite(WIFI_SIGNAL_PIN, LOW); 

  /* initalize RF transmitter */
  init_RF_Transmitter(); 

  Serial.begin(9600);                       
  Serial.print("Calibrating...\ ");                
  Ro = MQCalibration(MQ_PIN);                       //Calibrating the sensor. Please  make sure the sensor is in clean air                                                 //when  you perform the calibration                    
  Serial.print("Calibration is done...\ "); 
}
  
void loop()
{

  /* Gets the CO Sensor Value */
  float sensorValue = analogRead(MQ_PIN);

  if( DEBUG )
  {
    Serial.print("Sensor Value: "); 
    Serial.print(sensorValue); 
    Serial.print("\n"); 
  }

  if( (MQGetGasPercentage((float)(MQRead(MQ_PIN) / Ro), GAS_CO) >= TRIGGER_VALUE) || (digitalRead(TEST_BUTTON) == HIGH) )
  {
    Serial.print("Triggering Events to ensure CO safety\n"); 
    
    /* Play Sound */
    send_Sound();

    /* Send Wifi Signal */
    transmit_wifi_message();

    /* Send RF transmission */  
    send_RF_Signal(); 

  }

  delay(200);
}
