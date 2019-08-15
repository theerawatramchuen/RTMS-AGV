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
#define TIME_TO_SLEEP  60        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0; // 1st time after power up
RTC_DATA_ATTR float T0 = 0;
RTC_DATA_ATTR float T1 = 0;
RTC_DATA_ATTR float T2 = 0;
RTC_DATA_ATTR float T3 = 0;
RTC_DATA_ATTR float T4 = 0;
RTC_DATA_ATTR float R0 = 0;
RTC_DATA_ATTR float R1 = 0;
RTC_DATA_ATTR float R2 = 0;
RTC_DATA_ATTR float R3 = 0;
RTC_DATA_ATTR float R4 = 0;

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


  if(bootCount == 0) // Run this only the first time after Power Up !!!
  {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(1000);
      Serial.println("Connecting to WiFi..");
      failedConnectCounter = failedConnectCounter + 1;
      if (failedConnectCounter > 10) 
         {
          Serial.print("Failed Connect "); Serial.print(ssid);
          Serial.println(" over 10 time after Powerup");

          delay(500);
          esp_deep_sleep_enable_timer_wakeup(5000000);
          esp_deep_sleep_start();
         }
    }
    Serial.println("Connected to the WiFi network");
    Serial.println(WiFi.localIP());           //แสดง IP
    Serial.println(WiFi.macAddress());
    bootCount = bootCount + 1;

    String url ="http://172.16.2.78/realtime/register/register.php?";
    url+= "&mac="+String(WiFi.macAddress());
    url+= "&id=AGV-2/P";
    url+= "&sid=AGV2";
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
Serial.println(".");Serial.println(".");Serial.println(".");


if(bootCount == 1) 
{
  sensor1Data = dhtSensor1.getTempAndHumidity();
  T4 = sensor1Data.temperature;
  R4 = sensor1Data.humidity;
  Serial.print("T4 ");
  Serial.println(T4);
  Serial.print("R4 ");
  Serial.println(R4);
}

if(bootCount == 2) 
{
  sensor1Data = dhtSensor1.getTempAndHumidity();
  T3 = sensor1Data.temperature;
  R3 = sensor1Data.humidity;
  Serial.print("T3 ");
  Serial.println(T3);
  Serial.print("R3 ");
  Serial.println(R3);
}

if(bootCount == 3) 
{
  sensor1Data = dhtSensor1.getTempAndHumidity();
  T2 = sensor1Data.temperature;
  R2 = sensor1Data.humidity;
  Serial.print("T2 ");
  Serial.println(T2);
  Serial.print("R2 ");
  Serial.println(R2);
}

if(bootCount == 4) 
{
  sensor1Data = dhtSensor1.getTempAndHumidity();
  T1 = sensor1Data.temperature;
  R1 = sensor1Data.humidity;
  Serial.print("T1 ");
  Serial.println(T1);
  Serial.print("R1 ");
  Serial.println(R1);
}

if(bootCount == 5) 
{ 
  sensor1Data = dhtSensor1.getTempAndHumidity();
  T0 = sensor1Data.temperature;
  R0 = sensor1Data.humidity;
  Serial.print("T0 ");
  Serial.println(T0);
  Serial.print("R0 ");
  Serial.println(R0);
}

if(bootCount == 6) // Run this every 5 mins
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(1000);
      Serial.println("Connecting to WiFi..");
      failedConnectCounter = failedConnectCounter + 1;
      if (failedConnectCounter > 10) 
         {
          Serial.println("Failed Connect Prod_WIFI over 10 time for BOOTCOUNT = 6");
          delay(500);
          esp_deep_sleep_enable_timer_wakeup(5000000);
          esp_deep_sleep_start();
         }
    }
    Serial.println("Connected to the WiFi network");
    Serial.println(WiFi.localIP());           //แสดง IP
    Serial.println(WiFi.macAddress());
    bootCount = 1;
    // T0=1;T1=1;T2=1;T3=1;T4=1;R0=1;R1=1;R2=1;R3=1;R4=1;  // For testing only
    Analog();
    Digital();
    Serial.println("Updated 5 sampling data to server..done.");
    Serial.println(".");Serial.println(".");Serial.println(".");
}
delay(500);
Serial.print("Bootcount = ");
Serial.println(bootCount);
esp_deep_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
Serial.println("Goto deep sleep mode...bye");
bootCount = bootCount + 1;
delay(500);
esp_deep_sleep_start();  // ***************  IoT Goto DeepSleep Mode **********************

}

//************ RTMS Analog ***************
//void Analog(float T0, float T1, float T2, float T3, float T4, float R0, float R1, float R2, float R3, float R4) 
void Analog() 
{
  String analog ="http://172.16.2.78/realtime/analog_v2.php?";
  analog+="&id=AGV-2/P";
  analog+="&a1="+String(T0,1);
  analog+="&a2="+String(R0,1);
  analog+="&a3="+String(T1,1);
  analog+="&a4="+String(R1,1);
  analog+="&a5="+String(T2,1);
  analog+="&a6="+String(R2,1);
  analog+="&a7="+String(T3,1);
  analog+="&a8="+String(R3,1);
  analog+="&a9="+String(T4,1);
  analog+="&a10="+String(R4,1);
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
  Serial.println(String(T0,1)+" "+String(T1,1)+" "+String(T2,1)+" "+String(T3,1)+" "+String(T4,1));
  Serial.print("RH % = ");
  Serial.println(String(R0,1)+" "+String(R1,1)+" "+String(R2,1)+" "+String(R3,1)+" "+String(R4,1));

}

//************* RTMS Digital ******************
void Digital(){
  String digital ="http://172.16.2.78/realtime/update.php?";
  digital+="&id=AGV-2/P";
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
