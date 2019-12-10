/*
 * ESP32 AJAX Demo
 * Updates and Gets data from webpage without page refresh
 * https://circuits4you.com
 */
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <TinyGPS++.h>
#include "server_of_pages.h"
#include "main.h"
#include "EEPROM.h"
#include <AsyncElegantOTA.h>

bool toogle = false;
#define led_status 13
TinyGPSPlus gps;
extern AsyncElegantOtaClass AsyncElegantOTA;

unsigned long speed_limited = 15;
String ssid = "Over Speed Alarm";
String password = "88888888";


void setup(void)
{
  Serial.begin(115200);
  Serial2.begin(9600);
  EEPROM.begin(100);
  if (EEPROM.readByte(0) == 0x11)
  {
      speed_limited = EEPROM.readInt(1);
      ssid = EEPROM.readString(10).c_str();
      password = EEPROM.readString(20).c_str();
    Serial.println("Cargando Valores configurados: ");
    Serial.println("SpeedLimit: " + String(speed_limited));
    Serial.println("Ficha: " + String(ssid));
    Serial.println("password: " + String(password));

  }  
  else
  {
    Serial.println("cargando valores predeterminados:");
  }
  
  
  pinMode(buzin_pin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(buzin_pin ,HIGH);
  digitalWrite(LED_BUILTIN ,HIGH);

  wifi_ini();
  server_pages(); 


}
void loop(void)
{
  smartDelay(1000);

   AsyncElegantOTA.loop();

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));

  buzin_alert();


}

void smartDelay(unsigned long ms)
{
  unsigned long start = millis();

  do
  {
    toogle = !toogle;
    digitalWrite(led_status, toogle);
    while (Serial2.available() >= 0)
      gps.encode(Serial2.read());
  } while (millis() - start < ms);
}

void wifi_ini()
{

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid.c_str(), password.c_str());

  //ESP32 connects to your wifi -----------------------------------
  // WiFi.mode(WIFI_STA); //Connectto your wifi
  // WiFi.begin(ssid, password);

  Serial.println("Connecting to ");
  Serial.print(ssid);

  //Wait for WiFi to connect
  // while(WiFi.waitForConnectResult() != WL_CONNECTED){
  //     Serial.print(".");
  //   }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); //IP address assigned to your ESP
                                  //----------------------------------------------------------------
}


void buzin_alert()
{
  
  if (gps.speed.kmph() > speed_limited)
  {
    digitalWrite(buzin_pin, LOW);
  }
  else
  {
    digitalWrite(buzin_pin, HIGH);
  }

  
}