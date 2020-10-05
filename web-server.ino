#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "VIVOFIBRA-9B6D";
const char* password = "EAEA8F9B6D";

ESP8266WebServer server(80);

const int led = 2;

void handleRoot() {
  digitalWrite(led, LOW);

  String textoHTML;

  textoHTML = "Ola!! Aqui &eacute; o <b>ESP8266</b> falando! ";
  textoHTML += "Porta A0: ";
  textoHTML += analogRead(A0);
   
  server.send(200, "text/html", textoHTML);
  digitalWrite(led, HIGH);
}

void handleNotFound(){
  
  digitalWrite(led, HIGH);
  
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST" ;
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  server.send(404, "text/plain", message);
  digitalWrite(led, LOW);
  
}

void setup(void){
  
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
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

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  
}

void loop(void){
  server.handleClient();
}
