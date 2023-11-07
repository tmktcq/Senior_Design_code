#include <ESP8266WiFi.h>

#include <WiFiClient.h>
#include "Pushsafer.h"

// Initialize Wifi connection to the router
char ssid[] = "XXX";     // your network SSID (name)
char password[] = "XXXX"; // your network key

// Pushsafer private or alias key
#define PushsaferKey "XXX"

/*WiFiClientSecure client;*/
WiFiClient client;
Pushsafer pushsafer(PushsaferKey, client);
struct PushSaferInput input;
void setup() {

  pinMode(D1, INPUT); 
  pinMode(D4, OUTPUT); 

  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: \n");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pushsafer.debug = true;

  // Take a look at the Pushsafer.com API at
  // https://www.pushsafer.com/en/pushapi
  

  input.message = "Carbon Monoxide has reached an unsafe level";
  input.title = "Warning!";
  input.sound = "8";
  input.vibration = "1";
  input.icon = "1";
  input.iconcolor = "#FFCCCC";
  input.priority = "1";
  input.device = "a";
  input.url = "https://www.pushsafer.com";
  input.urlTitle = "Open Pushsafer.com";
  input.picture = "";
  input.picture2 = "";
  input.picture3 = "";
  input.time2live = "";
  input.retry = "";
  input.expire = "";
  input.confirm = "";
  input.answer = "";
  input.answeroptions = "";
  input.answerforce = "";
  
  // client.stop();

}

void loop() 
{

  uint8_t status = digitalRead(D1); 
  if(status == HIGH)
  {
    Serial.print("Sending Message to Homeowner\n"); 
    delay(100);
    Serial.println(pushsafer.sendEvent(input));
    Serial.println("Sent");
  }

}