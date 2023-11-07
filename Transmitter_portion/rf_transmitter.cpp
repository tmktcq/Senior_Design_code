#include "rf_transmitter.h"

RH_ASK driver; 

void init_RF_Transmitter()
{
  driver.init();
}

void send_RF_Signal()
{
    const char *msg = "UNSAFE_LEVELS"; 
    driver.send((uint8_t*) msg, strlen(msg)); 
    driver.waitPacketSent(); 
    Serial.print("RF Transmission Sent\n"); 
    delay(200); 
}