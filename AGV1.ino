#include <Arduino.h>
#include <esp_deep_sleep.h>
#include <DHTesp.h>
#include <WiFi.h>
#include <HTTPClient.h>

#ifndef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP32 ONLY!)
#error Select ESP32 board.
#endif


#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  58        /* Time ESP32 will go to sleep (in seconds) */


RTC_DATA_ATTR int bootCount = 0;
int failedConnectCounter = 0;

DHTesp dhtSensor1;
int dhtPin1 = 26;  /** Pin number for DHT11 2 data pin */

TempAndHumidity sensor1Data; 

const char* ssid ="Prod_WIFI";  
const char* password ="";
 
void setup() 
{
  Serial.begin(115200);
  dhtSensor1.setup(dhtPin1, DHTesp::DHT22);
  delay(4000);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    failedConnectCounter = failedConnectCounter + 1;
    if (failedConnectCounter > 10) 
       {
        Serial.println("Failed Connect Prod_WIFI over 10 time");
        esp_deep_sleep_enable_timer_wakeup(5000000);
        esp_deep_sleep_start();
       }
  }
 
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());           //แสดง IP
  Serial.println(WiFi.macAddress());

  if(bootCount == 0) // Run this only the first time
  {
    bootCount = bootCount + 1;
    String url ="http://172.16.2.78/realtime/register/register.php?";
    url+= "&mac="+String(WiFi.macAddress());
    url+= "&id=AGV-1/P";
    url+= "&sid=AGV1";
    url+= "&oper=Wirebond";
    url+= "&lc=11WB9";
    url+= "&resp=Wanit";
    url+= "&ext=1016";
    url+= "&ip="+WiFi.localIP();
    url+= "&un=Unit";
    url+= "&doeff=100";
    url+= "&rs=";
    url+= "&mod=AGV";
    url+= "&Xlc=01";
    url+= "&Ylc=01";
    url+= "&fwver="; //Specify the URL
 
  
    if ((WiFi.status() == WL_CONNECTED))   //Check the current connection status
    { 
     HTTPClient http;
     http.begin(url);
     int httpCode = http.GET();
     //Make the request
     String payload = http.getString();
     Serial.println(httpCode);
     Serial.println(payload);
     Serial.println("Get content from " + url);
     http.end(); //Free the resources
    }
  }
delay(500);
Analog(); 
Digital();
Serial.print("Bootcount = ");
Serial.println(bootCount);
esp_deep_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
Serial.println("Goto deep sleep mode...bye");
delay(500);
esp_deep_sleep_start();  // ***************  IoT Goto DeepSleep Mode **********************
}

//************ RTMS Analog ***************
void Analog() 
{
  sensor1Data = dhtSensor1.getTempAndHumidity();
  String analog ="http://172.16.2.78/realtime/analog_v2.php?";
  analog+="&id=AGV-1/P";
  analog+="&a1="+String(sensor1Data.temperature,2);
  analog+="&a2="+String(sensor1Data.humidity,1);
  analog+="&a3=";
  analog+="&a4=";
  analog+="&a5=";
  analog+="&a6=";
  analog+="&a7=";
  analog+="&a8=";
  analog+="&a9=";
  analog+="&a10=";
  analog+="&a11=";
  analog+="&a12=";
  analog+="&a13=";
  analog+="&a14=";
  analog+="&a15=";
  analog+="&a16=";
  analog+="&a17=";
  analog+="&a18=";
  analog+="&a19=";
  analog+="&a20=";
  if ((WiFi.status() == WL_CONNECTED)) //Check the current connection status
  { 
    HTTPClient http;
    http.begin(analog);
    int httpCode = http.GET();
    //Make the request
    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
    Serial.println("Get content from " + analog);
    http.end(); //Free the resources
  }
  Serial.print("Temp = ");
  Serial.println(sensor1Data.temperature);
  Serial.print("RH % = ");
  Serial.println(sensor1Data.humidity);
}

//************* RTMS Digital ******************
void Digital(){
  String digital ="http://172.16.2.78/realtime/update.php?";
  digital+="&id=AGV-1/P";
  digital+="&k=";
  digital+="&val=100000";
  digital+="&upderr=";
  digital+="&tid=";
  digital+="&spm=";
  digital+="&fwver=";
  digital+="&gr1=0";
  digital+="&gr2=0";
  digital+="&io1=1";
  digital+="&io2=0";
  digital+="&io3=0";
  digital+="&io4=0";
  digital+="&io5=0";
  digital+="&io6=0";
  digital+="&io7=0";
  digital+="&io8=0";
  digital+="&io9=100";
  digital+="&io10=0";
  digital+="&io11=0";
  digital+="&io12=0";
  digital+="&io13=0";
  digital+="&io14=0";
  digital+="&io15=0";
  digital+="&io16=0";
  if ((WiFi.status() == WL_CONNECTED))   //Check the current connection status
  { 
     HTTPClient http;
     http.begin(digital);
     int httpCode = http.GET();
     String payload = http.getString();
     Serial.println(httpCode);
     Serial.println(payload);
     Serial.println("Get content from " + digital);
     http.end(); //Free the resources
  }
}


void loop()
{
}
