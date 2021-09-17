#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <WebSocketsServer.h>
#include "device.h"

#ifndef APSSID
#define APSSID "Esp_AP"
#define APPSK  "1234567890"
#endif


/* Set these to your desired credentials. */
const char *ssid_AP = APSSID;
const char *password_AP = APPSK;
const int led = 2;

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void StartWebsocket(){
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}
int NUM = -1;
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    NUM = num;
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        
        // send message to client
        webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
        {
            Serial.printf("[%u] get Text: %s\n", num, payload);
            String message = (char*)payload;
            if (message =="on"){
              digitalWrite(led,LOW);
              webSocket.broadcastTXT("LED ON");
            }
            else if(message =="off"){
              digitalWrite(led,HIGH);
              webSocket.broadcastTXT("LED OFF");
            }

            // send message to client
            // webSocket.sendTXT(num, "message here");

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;
        }
        case WStype_BIN:
            Serial.printf("[%u] get binary length: %u\n", num, length);
            hexdump(payload, length);

            // send message to client
            // webSocket.sendBIN(num, payload, length);
            break;
    }
}

bool test_wifi();

void setup_wifi_AP(){
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid_AP, password_AP);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

void setup() {
  Serial.begin(115200);
  SPIFFS.begin();
  EEPROM.begin(512);
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
  setup_wifi_AP();
  byte val;
  int i=0;
  String ssid_wifi="";
  String pass_wifi="";
  do{
    val = EEPROM.read(i);
    if (val!=0){
      ssid_wifi += char(val);
    }
    i++;
  }while (val!=0 && i<10);
  Serial.print("ssid_wifi: ");
  Serial.println(ssid_wifi);
  i=10;
  do{
    val = EEPROM.read(i);
    if (val!=0){
      pass_wifi += char(val);
    }
    i++;
  }while (val!=0);
  Serial.print("pass_wifi: ");
  Serial.println(pass_wifi);
  WiFi.begin(ssid_wifi.c_str(), pass_wifi.c_str());

  if (test_wifi())
  {
    Serial.print("Kết nối thành công tới ssid_wifi: ");
    Serial.println(ssid_wifi);
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.on("/", handleRoot);
    server.on("/dangnhap.html",handleDangNhap);
    server.on("/ghi.html", handleGhi);
    server.begin();
    Serial.println("Start Websocket"); 
    StartWebsocket(); 
    return;
  }
  else
  {
    setup_wifi_AP();
    server.on("/", handleRoot);
    server.on("/dangnhap.html",handleDangNhap);
    server.on("/ghi.html", handleGhi);
    server.begin();
    Serial.println("HTTP server started");    
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    server.handleClient();
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  
}

unsigned long now = 0;
char buf_1[70];
char buf_2[70];
void loop() {
  server.handleClient();
  webSocket.loop();
  if (millis() - now >1000){
    now = millis();
    int _temp = getTemp();
    int _humidity = getHumidity();
    Serial.print("temp: ");
    Serial.println(_temp);
    Serial.print("humidity: ");
    Serial.println(_humidity);
    sprintf(buf_1, "temp:%d", _temp);  
    webSocket.sendTXT(NUM, buf_1);
    sprintf(buf_2, "humidity:%d", _humidity);  
    webSocket.sendTXT(NUM, buf_2);
  }
}

void handleRoot() {
  server.send(200, "text/html", "<h1>YOU ARE CONNECTED</h1>");
}

void handleDangNhap() {
  File f = SPIFFS.open("/dangnhap.html","r");
  String debugLogData ="";
  while (f.available()){
            debugLogData += char(f.read());
          }
  int i=0;
  byte val;
  String ssid="";
  String pass="";
  do{
    val = EEPROM.read(i);
    if (val!=0){
      ssid += char(val);
    }
    i++;
  }while (val!=0 && i<10);
  i=10;
  do{
    val = EEPROM.read(i);
    if (val!=0){
      pass += char(val);
    }
    i++;
  }while (val!=0);
  debugLogData.replace("abcdefgh",ssid);
  debugLogData.replace("123456789",pass);
  server.send(200, "text/html", debugLogData);  
}
void handleGhi() {
  String ssid = server.arg("ssid");
  String pass = server.arg("pass");
  if (ssid.length()>0 && pass.length()>0){
    for (int i = 0; i < 512; i++) {
      EEPROM.write(i, 0);
    }
  }
  int i;
  for (i=0;i<ssid.length();i++){
    EEPROM.write(i,ssid[i]);
  }
  EEPROM.write(i,0);
  for (i=0;i<pass.length();i++){
    EEPROM.write(10+i,pass[i]);
  }
  EEPROM.write(10+i,0);
  if(EEPROM.commit()){
    server.send(200, "text/html", "<HTML><HEAD></HEAD><BODY>GHI VAO EEPROM THANH CONG</BODY></HTML>");
    Serial.println("Esp restart sau: ");
    for (int t =0;t<5;t++){
      Serial.println(t);
      delay(500);
    }
    ESP.restart();
  }
  else{
    server.send(200, "text/html", "<HTML><HEAD></HEAD><BODY>GHI VAO EEPROM FAILED</BODY></HTML>");
  }
}

bool test_wifi(){
  int k = 0;
  Serial.println("Chờ kết nối Wifi");
  while ( k < 20 ) {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(500);
    Serial.print(".");
    k++;
  }
  Serial.println("");
  Serial.println("Hết thời gian chờ");  
  return false;
}
