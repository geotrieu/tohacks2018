#include <LiquidCrystal.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <DHT.h>
#include <DHT_U.h>

WiFiServer server(80);
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
DHT dht(14, DHT11);

/*int temps[1500];
int humid[1500];
int numTemps = 0;*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Connect to WiFi
  WiFi.begin("x", "x");
  //WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint 
  //WiFi.softAP("milk", "0000000000"); // Provide the (SSID, password); . 
  server.begin();
  
  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  //print a new line, then print WiFi connected and the IP address
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.localIP());
  /*IPAddress HTTPS_ServerIP= WiFi.softAPIP(); // Obtain the IP of the Server 
  Serial.print("Server IP is: "); // Print the IP to the monitor window 
  Serial.println(HTTPS_ServerIP);*/
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
}

void loop() {
  WiFiClient client = server.available();
/*
  temps[numTemps] = (dht.readTemperature() - 3);
  humid[numTemps] = (dht.readHumidity());
  numTemps++;*/
  
  if (!client) {
    delay(1000);
    return; 
  } 
  //Looking under the hood 
  Serial.println("Somebody has connected :)");
  String request = client.readStringUntil('\r');
  Serial.println(request);
  Serial.println(request.indexOf("/TOG"));
  if (request.indexOf("/TOG") != -1){
    digitalWrite(5, HIGH);
    delay(25);
    digitalWrite(5, LOW);
    delay(25);
    digitalWrite(5, HIGH);
    delay(25);
    digitalWrite(5, LOW);
  }
  else if (request.indexOf("favicon.ico") != -1) {
    client.stop();
    delay(11);
    return;
  }

  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  
  s += "<!DOCTYPE HTML>";
  s += "<html>";
  s +=   "<head>";
  s +=     "<title>GV IoT Control Center</title>";
  s +=     "<link rel=icon type=image/png href=http://geovillageva.com/favicon.ico>";
  s +=     "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'";
  s +=   "</head>";
  s +=   "<body>";
  s +=     "<div class=container>";
  s +=       "<h1 class=\"col-md-10\">GV IoT Control Center!";
  s +=       "<div class=row>";
  s +=         "<h3><b><u>Light Settings (On D1)</b></u></h3>";
  s +=         "<div class=col-sm>";
  s +=           "<input type=button class=\"btn btn-success\" value=Toggle onclick=location.href='/TOG'>";
  s +=         "</div>";
  s +=       "</div>";
  s +=       "<br>";
  s +=       "<div class=row>";
  s +=         "<h3><u><b>Temperature Sensor (On D5)</b></u></h3>";
  s +=         "<div class=col-sm>";
  s +=           "<h4>Temperature: ";
  s +=           (dht.readTemperature() - 3);
  s +=           " Degrees Celcius</h4>";
  s +=         "</div>";
  s +=         "<div class=col-sm>";
  s +=           "<h4>Humidity: ";
  s +=           dht.readHumidity();
  s +=           "%</h4>";
  s +=         "</div>";
  s +=       "</div>";
  s +=       "<div class=row>";
  s +=         "<h3><u><b>Soil Humdity Sensor (On A0)</b></u></h3>";
  s +=         "<div class=col-sm>";
  s +=           "<h4>Humidity: ";
  s +=           map(analogRead(A0), 1024, 0, 0, 100);
  s +=           "%</h4>";
  s +=         "</div>";
  s +=       "</div>";
  s +=     "</div>";
  s +=   "</body>";
  s += "</html>";
  s += "<script> setTimeout(function(){ window.location.reload(1);}, 1000);</script>";
  client.flush();
  client.print(s);
  delay(10);
  Serial.println("Client disconnected");
}
