#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define relay_pin 2


#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif

const char *ssid     = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/json", "{\"options\":[{\"option\":\"long-press\",\"url\":\"/long-press\"},{\"option\":\"press\",\"url\":\"/press\"}]}");
}

void handlePress() {
  server.send(202, "text/json", "{\"result\":\"acceted\", \"option\":\"press\"}");
  digitalWrite(relay_pin, LOW);
  delay(100);
  digitalWrite(relay_pin, HIGH);
}


void handleLongPress() {
  server.send(202, "text/json", "{\"result\":\"acceted\", \"option\":\"long-press\"}");
  digitalWrite(relay_pin, LOW);
  delay(6000);
  digitalWrite(relay_pin, HIGH);
}

void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}


void setup(void) {
  pinMode(relay_pin , OUTPUT);
  digitalWrite(relay_pin, HIGH);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  server.on("/", handleRoot);
  server.on("/press", handlePress);
  server.on("/long-press", handleLongPress);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
