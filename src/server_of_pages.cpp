#include "server_of_pages.h"
#include <WiFiClient.h>
//#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <TinyGPS++.h>
#include "main.h"
#include <FS.h>
#include <SPIFFS.h>
#include <EEPROM.h>
#include <Hash.h>
#include "AsyncElegantOTA.h"

String estado = "cargado";

 
extern unsigned long speed_limited;
extern String ssid;
extern String password;

String main_page = "<!DOCTYPE html>\n"
              "<html>\n"
              "\n"
              "<head>\n"
              "  <title>Alerma de Exceso de Velocidad</title>\n"
              "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
              "  <link rel=\"icon\" href=\"data:,\">\n"
              "  <link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n"
              "  <link rel=\"stylesheet\" href=\"bootstrap.min.css\">\n"
              "\n"
              "</head>\n"
              "\n"
              "<body>\n"
              "    <h1 class=\"text-center mt-4\">Configurador de Exceso de Velocidad GPS</h1>\n"
              "\n"
              "  <div class=\"container\">\n"
              "    <div class=\"card-body\">\n"
              "      <div class=\"row mt-4 d-flex justify-content-center\">\n"
              "        <div class=\"card-container col-md-6 py-4 px-4\">\n"
              "          <div class=\"form-group row justify-content-center\">\n"
              "            <div class=\"form-group row justify-content-between\">\n"
              "              <a href=\"/on\"><button class=\"button\">ON</button></a>\n"
              "              <a href=\"/off\"><button class=\"button button2\">OFF</button></a>\n"
             "  <a href=\"/read\"><button class=\"button button3\">Ver Config</button></a>\n"
              "            </div>\n"
              "          </div>\n"
              "        </div>\n"
              "      </div>\n"
              "      <div class=\"container\">\n"
              "        <div class=\"card-body\">\n"
              "          <div class=\"row mt-4 d-flex justify-content-center\">\n"
              "            <div class=\"card-container col-md-6 py-4 px-4\">\n"
              "              <div class=\"form-group row justify-content-center\">\n"
              "                <p>Buzzin</p>\n"
              "                <p>\n"
              "                  <span class=\"sensor-labels\" class=\"col-sm-2 col-form-label col-form-label-lg\">Velcidad: </span>\n"
              "                  <span id=\"velocidad\">%VELOCIDAD%</span>\n"
              "                  <sup class=\"units\">Km/h</sup>\n"
              "                </p>\n"
              "                <p>\n"
              "                  <span class=\"sensor-labels\" class=\"col-sm-2 col-form-label col-form-label-lg\">Longitud: </span>\n"
              "                  <span id=\"longitud\">%LOGITUD GPS%</span>\n"
              "                  "
              "                </p>\n"
              "                <p>\n"
              "                  <span class=\"sensor-labels\" class=\"col-sm-2 col-form-label col-form-label-lg\">Latitud: </span>\n"
              "                  <span id=\"latitud\">%LATITUD GPS%</span>\n"
              "                 "
              "                </p>\n"
              "              </div>\n"
              "            </div>\n"
              "          </div>\n"
              "        </div>\n"
              "      </div>\n"
              "      <div class=\"container\">\n"
              "        <div class=\"card-body\">\n"
              "          <div class=\"row mt-4 d-flex justify-content-center\">\n"
              "            <div class=\"card-container col-md-6 py-4 px-4\">\n"
              "              <div class=\"form-group row justify-content-center\">\n"
              "                <form action=\"/config.html\">\n"
              "                  <div class=\"form-group row justify-content-between\">\n"
              "                    <label for=\"speed\" class=\"col-sm-2 col-form-label col-form-label-lg\">Velocidad</label>\n"
              "                    <input type=\"text\" id=\"speed\" name=\"speed\" class=\"col-md-5 form-control\" placeholder=\"\">\n"
              "                  </div>\n"
              "                  <div class=\"form-group row justify-content-between\">\n"
              "                    <label for=\"wifi\" class=\"col-sm-2 col-form-label col-form-label-lg\">Nombre</label>\n"
              "                    <input type=\"text\" id=\"wifi\" name=\"name\" class=\"col-md-5 form-control\" placeholder=\"\">\n"
              "                  </div>\n"
              "                  <div class=\"form-group row justify-content-between\">\n"
              "                    <label for=\"password\" class=\"col-sm-2 col-form-label col-form-label-lg\">Password</label>\n"
              "                    <input type=\"text\" id=\"password\" name=\"password\" class=\"col-md-5 form-control\"\n"
              "                      placeholder=\"\">\n"
              "                  </div>\n"
              "\n"
              "                  <div class=\"form-group row justify-content-between\">\n"
            "                    <div class=\"col-md-4\">\n"
              "                      <button type=\"submit\"  class=\"button\">Guardar</button>\n"
              "                    </div>\n"
              "                  </div>\n"
              "\n"
              "                </form>\n"
              "\n"
              "\n"
              "</body>\n"
              "<script>\n"
              "  setInterval(function () {\n"
              "    var xhttp = new XMLHttpRequest();\n"
              "    xhttp.onreadystatechange = function () {\n"
              "      if (this.readyState == 4 && this.status == 200) {\n"
              "        document.getElementById(\"velocidad\").innerHTML = this.responseText;\n"
              "      }\n"
              "    };\n"
              "    xhttp.open(\"GET\", \"/velocidad\", true);\n"
              "    xhttp.send();\n"
              "  }, 10000);\n"
              "\n"
              "  setInterval(function () {\n"
              "    var xhttp = new XMLHttpRequest();\n"
              "    xhttp.onreadystatechange = function () {\n"
              "      if (this.readyState == 4 && this.status == 200) {\n"
              "        document.getElementById(\"longitud\").innerHTML = this.responseText;\n"
              "      }\n"
              "    };\n"
              "    xhttp.open(\"GET\", \"/longitud\", true);\n"
              "    xhttp.send();\n"
              "  }, 10000);\n"
              "\n"
              "  setInterval(function () {\n"
              "    var xhttp = new XMLHttpRequest();\n"
              "    xhttp.onreadystatechange = function () {\n"
              "      if (this.readyState == 4 && this.status == 200) {\n"
              "        document.getElementById(\"latitud\").innerHTML = this.responseText;\n"
              "      }\n"
              "    };\n"
              "    xhttp.open(\"GET\", \"/latitud\", true);\n"
              "    xhttp.send();\n"
              "  }, 10000);\n"
              "</script>\n"
              "\n"
              "</html>";

String style = "\n"
               "\n"
               "html {\n"
               "  font-family: Arial;\n"
               "  display: inline-block;\n"
               "  margin: 0px auto;\n"
               "  text-align: center;\n"
               "}\n"
               "h1 {\n"
               "  color: #0F3376;\n"
               "  padding: 2vh;\n"
               "}\n"
               "p {\n"
               "  font-size: 1.5rem;\n"
               "}\n"
               ".button {\n"
               "  display: inline-block;\n"
               "  background-color: #008CBA;\n"
               "  border: none;\n"
               "  border-radius: 4px;\n"
               "  color: white;\n"
               "  padding: 16px 40px;\n"
               "  text-decoration: none;\n"
               "  font-size:18px;\n"
               "  margin: 2px;\n"
               "  cursor: pointer;\n"
               "}\n"
               ".button2 {\n"
               "  background-color: #f44336;\n"
               "}\n"
                              ".button3 {\n"
               "  background-color: #616069;\n"
               "}\n"
               ".units {\n"
               "  font-size: 1.2rem;\n"
               " }\n"
               ".sensor-labels {\n"
               "  font-size: 1.5rem;\n"
               "  vertical-align:middle;\n"
               "  padding-bottom: 15px;\n"
               "}\n"
               "\n"
               ".card-container {\n"
               "  border-radius: 10px;\n"
               "  -webkit-box-shadow: 0px 2px 8px -2px rgba(0,0,0,0.65);\n"
               "  -moz-box-shadow: 0px 2px 8px -2px rgba(0,0,0,0.65);\n"
               "  box-shadow: 0px 2px 8px -2px rgba(0,0,0,0.65);\n"
               "}\n"
               "\n"
               ".navbar-color {\n"
               "  background-color: #325D88;\n"
               "}\n"
               "\n"
               ".nav-item-color {\n"
               "  color: #ffffff;\n"
               "}\n"
               "\n"
               ".d-flex-pers {\n"
               "  display: flex;\n"
               "  justify-content: space-between;\n"
               "}\n"
               "\n"
               "@media (max-width: 768px) {\n"
               "  .d-flex-pers {\n"
               "      display: unset;\n"
               "  }\n"
               "\n"
               "  .card-container {\n"
               "      margin: 0 15px;\n"
               "  }\n"
               "}\n"
               "\n"
               ""
               "input[type=text], select {\n"
               "  width: 100%;\n"
               "  padding: 12px 20px;\n"
               "  margin: 8px 0;\n"
               "  display: inline-block;\n"
               "  border: 1px solid #ccc;\n"
               "  border-radius: 4px;\n"
               "  box-sizing: border-box;\n"
               "}\n"
               "\n"
               "input[type=submit] {\n"
               "  width: 100%;\n"
               "  background-color: #4CAF50;\n"
               "  color: white;\n"
               "  padding: 14px 20px;\n"
               "  margin: 8px 0;\n"
               "  border: none;\n"
               "  border-radius: 4px;\n"
               "  cursor: pointer;\n"
               "}\n"
               "\n"
               "input[type=submit]:hover {\n"
               "  background-color: #45a049;\n"
               "}\n"
               "\n"
               "div {\n"
               "  border-radius: 5px;\n"
               "  background-color: #f2f2f2;\n"
               "  padding: 20px;\n"
               "}";



extern TinyGPSPlus gps;
AsyncWebServer server(80);
AsyncElegantOtaClass AsyncElegantOTA;
   
String ledState;

void server_pages()
{
  

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        //request->send(SPIFFS, "/index.html", String(), false, processor);
        request->send(200, "text/html", main_page);
        Serial.println("new client serving root page");
    });

    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("new client serving root styles");
        // request->send(SPIFFS, "/style.css", "text/css");
        request->send(200, "text/css", style);
    });

    //    // Route to load style.css file
    // server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     Serial.println("new client serving root styles");
    //    // request->send(SPIFFS, "/style.css", "text/css");
    //     request->send(200, "text/css", bootstrap );

    // });

    // Route to set GPIO to HIGH
    server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("new client serving root on");
               digitalWrite(buzin_pin, LOW);
        digitalWrite(LED_BUILTIN, LOW); 
        estado = "ON";
         request->send(200, "text/html", main_page ); });

    // Route to set GPIO to LOW
    server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("new client serving root off");
        digitalWrite(buzin_pin, HIGH);
           digitalWrite(LED_BUILTIN, HIGH);

        estado = "OFF";
        request->send(200, "text/html", main_page);
    });
    
    server.on("/velocidad", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("new client serving root velocidad");
        request->send_P(200, "text/plain", String((long)gps.speed.kmph()).c_str());
    });

    server.on("/longitud", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("new client serving root velocidad");

        request->send_P(200, "text/plain", String((long)gps.location.lng()).c_str());
    });

    server.on("/latitud", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("new client serving root latitud");

        request->send_P(200, "text/plain", String((long)gps.location.lat()).c_str());
    });

        server.on("/read", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("new client serving root latitud");

        request->send(200, "text/html", configuredPage( speed_limited, ssid, password ));
    });


    server.on("/config.html", HTTP_GET, [](AsyncWebServerRequest *request) {

        int paramsNr = request->params();
        Serial.println(paramsNr);

        for (int i = 0; i < paramsNr; i++)
        {

            AsyncWebParameter *p = request->getParam(i);
            Serial.print("Param name: ");
            Serial.println(p->name());
            Serial.print("Param value: ");
            Serial.println(p->value());
            Serial.println("------");

            if (p->name().equals("speed") > 0)
            {
                Serial.println("Save Speed limit: " + p->value());

                EEPROM.writeInt(1, p->value().toInt());
                EEPROM.writeByte(0,0x11);
                EEPROM.commit();
            }
            if (p->name().equals("name") > 0)
            {
                Serial.println("Save ficha name: " + p->value());

                EEPROM.writeString(10, p->value());
                EEPROM.writeByte(0,0x11);
                EEPROM.commit();
            }

             if (p->name().equals("password") > 0)
            {
                Serial.println("Save pass: " + p->value());
                EEPROM.writeByte(0,0x11);
                EEPROM.writeString(20, p->value());
                EEPROM.commit();
            }


        }
    });

    
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am ESP32.");
  });

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA

    server.begin();
}

String processor(const String &var)
{
    Serial.println(var);
    if (var == "STATE")
    {
        if (digitalRead(buzin_pin))
        {
            ledState = "ON";
        }
        else
        {
            ledState = "OFF";
        }
        Serial.print(ledState);
        return ledState;
    }
    else if (var == "VELOCIDAD")
    {
        return String((long)gps.speed.kmph());
    }
    else if (var == "LONGITUD")
    {
        return String((long)gps.location.lng());
    }
    else if (var == "LATITUD")
    {
        return String((long)gps.location.lat());
    }
}



String configuredPage(unsigned long speed_limited , String name_config, String pass_config )
{
return
 "      <div class=\"container\">\n"
"        <div class=\"card-body\">\n"
"          <div class=\"row mt-4 d-flex justify-content-center\">\n"
"            <div class=\"card-container col-md-6 py-4 px-4\">\n"
"              <div class=\"form-group row justify-content-center\">\n"
"                \n"
"                </p>\n"
"                <p>\n"
"                  <span class=\"sensor-labels\" class=\"col-sm-2 col-form-label col-form-label-lg\">Velcidad: </span>\n"
"                  <span id=\"velocidad_config\">"+String(speed_limited)+"</span>\n"
"                </p>\n"
"                <p>\n"
"                  <span class=\"sensor-labels\" class=\"col-sm-2 col-form-label col-form-label-lg\">Ficha: </span>\n"
"                  <span id=\"name_config\">"+String(name_config)+"</span>\n"
"                </p>\n"
"                <p>\n"
"                  <span class=\"sensor-labels\" class=\"col-sm-2 col-form-label col-form-label-lg\">Pass: </span>\n"
"                  <span id=\"pass_config\">"+String(pass_config)+"</span>\n"
"                </p>\n"
"              </div>\n"
"            </div>\n"
"          </div>\n"
"        </div>\n"
"      </div>";

}
