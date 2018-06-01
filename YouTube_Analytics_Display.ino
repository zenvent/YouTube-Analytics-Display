// YouTube Analytics Display
// by zac@zenvent.com 2018

// Libraries
#include <YoutubeApi.h>            // https://github.com/witnessmenow/arduino-youtube-api
#include <ArduinoJson.h>           // https://github.com/bblanchon/ArduinoJson
#include <TM1637Display.h>         // https://github.com/avishorp/TM1637
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>


// Display Pins
#define DISPLAY_PIN_CLK 5
#define DISPLAY_PIN_DIO 16
#define API_KEY "YOUR_API_KEY"
#define CHANNEL_ID "YOUR_CHANNEL_ID"
char ssid[] = "SSID";
char password[] = "PASS";
unsigned long api_call_delay = 60000;
unsigned long api_last_call;

//Init
WiFiClientSecure client;
YoutubeApi api(API_KEY, client);
TM1637Display display(DISPLAY_PIN_CLK, DISPLAY_PIN_DIO);

void setup() {
  Serial.begin(9600);
  
  //Wifi Setup
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  //Set Display Max Brightness
  display.setBrightness(0x0f);
}

void loop() {
  if (millis() > api_last_call + api_call_delay || api_last_call == 0)  {
    if(api.getChannelStatistics(CHANNEL_ID)) {
      Serial.println("---------Stats---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);
      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);
      Serial.print("Comment Count: ");
      Serial.println(api.channelStats.commentCount);
      Serial.print("Video Count: ");
      Serial.println(api.channelStats.videoCount);
      display.showNumberDec(int(api.channelStats.subscriberCount));
    }
    api_last_call = millis();
  }
}
