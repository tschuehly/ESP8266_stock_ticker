#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <keyfile.h>
const char* ssid = SSID_ ;
const char* password = WLAN_PW;
const char* host = "api.thingspeak.com";
int pinCS = 0;                          //D3 wird mit CS verbunden
int numberOfHorizontalDisplays = 4;     //Anzahl der Module Horizontal
int numberOfVerticalDisplays = 1;       //Anzahl der Module Vertikal
float Preisfloat = 0;
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
String thingspeakAPI = "/apps/thinghttp/send_request?api_key=R26TO1A5LDNB31ZY";
int helligkeit = 1;             //Helligkeit 0 bis 15

void setup() {
  Serial.begin(115200);
  delay(10);

  matrix.setIntensity(helligkeit); //Helligkeit von Display einstellen
  matrix.setRotation(0, 1);        //Erste DOT Matrix Drehen
  matrix.setRotation(1, 1);        //Zweite DOT Matrix Drehen
  matrix.setRotation(2, 1);        //Dritte DOT Matrix Drehen
  matrix.setRotation(3, 1);        //Vierte DOT Matrix Drehen

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
   Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int find_text(String needle, String haystack,int from) {
  int foundpos = -1;
  if (haystack.length()<needle.length())
    return foundpos;
  for (int i = from; (i < haystack.length() - needle.length()); i++) {
    if (haystack.substring(i,needle.length()+i) == needle) {
      foundpos = i;
      return foundpos;
    }
  }
  return foundpos;
}

void loop() {
  delay(5000);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }


  // This will send the request to the server
    client.print(String("GET ") + thingspeakAPI + " HTTP/1.0\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(1500);

  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    int start_loc= find_text("text-nowrap\">",line,0);
    int end_loc=find_text("<span>",line,0);
    String Preis;
    if (start_loc>0 && end_loc>0)
    {
      Serial.print("Preis:  ");
      for (int i=start_loc+13;i<end_loc;i++)
      {
        Preis.concat(line[i]);
        //Serial.print(line[i]);
      }
      Serial.println(Preis);
      Serial.println("");
      matrix.fillScreen(LOW);
      int k =0;
      for(int i=0;i<26;i +=6){
        if(k==2){
          i=i-1;
        }else if(k==3){
          i-=1;
        }
        matrix.drawChar(i, 0, Preis[k], HIGH, LOW, 1);
        
        k = k+1;
        if(k==5){
        k = 0;
        }
        matrix.write(); //Display beschreiben
      }
      Preis = "";
    }
  }
}
