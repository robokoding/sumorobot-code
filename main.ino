#include"FS.h"
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

const char* ssid = "...";
const char* password = "...";

ESP8266WebServer server(80);

void streamFile(const String &fileName, const String &contentType) {
  File file = SPIFFS.open(fileName, "r");
  size_t sent = server.streamFile(file, contentType);
  file.close();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("wemos");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready for OTA");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Initializing SPIFFS ... ");
  if (SPIFFS.begin()) Serial.println("ok");
  else Serial.println("error");

  Serial.println("Checking SPIFFS files ...");
  static String files[12][2] =
  {{"/index.html", "text/html"},
   {"/blockly_compressed.js", "application/javascript"},
   {"/blocks_compressed.js", "application/javascript"},
   {"/sumorobot_compressed.js", "application/javascript"},
   {"/msg/js/en.js", "application/javascript"},
   {"/media/1x1.gif", "image/gif"},
   {"/media/click.mp3", "audio/mpeg"},
   {"/media/delete.mp3", "audio/mpeg"},
   {"/media/quote0.png", "image/png"},
   {"/media/quote1.png", "image/png"},
   {"/media/sprites.png", "image/png"},
   {"/media/sprites.svg", "image/svg+xml"}};
  for (int i = 0; i < 12; i++) {
    Serial.print(files[i][0]);
    if (SPIFFS.exists(files[i][0])) {
      Serial.println(" found");
    } else Serial.println(" missing");
  }

  server.on ("/", []() { streamFile(files[0][0], files[0][1]); });
  server.on (files[1][0].c_str(), []() { streamFile(files[1][0], files[1][1]); });
  server.on (files[2][0].c_str(), []() { streamFile(files[2][0], files[2][1]); });
  server.on (files[3][0].c_str(), []() { streamFile(files[3][0], files[3][1]); });
  server.on (files[4][0].c_str(), []() { streamFile(files[4][0], files[4][1]); });
  server.on (files[5][0].c_str(), []() { streamFile(files[5][0], files[5][1]); });
  server.on (files[6][0].c_str(), []() { streamFile(files[6][0], files[6][1]); });
  server.on (files[7][0].c_str(), []() { streamFile(files[7][0], files[7][1]); });
  server.on (files[8][0].c_str(), []() { streamFile(files[8][0], files[8][1]); });
  server.on (files[9][0].c_str(), []() { streamFile(files[9][0], files[9][1]); });
  server.on (files[10][0].c_str(), []() { streamFile(files[10][0], files[10][1]); });
  server.on (files[11][0].c_str(), []() { streamFile(files[11][0], files[11][1]); });

  server.begin();
	Serial.println ( "HTTP server started" );
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
}
