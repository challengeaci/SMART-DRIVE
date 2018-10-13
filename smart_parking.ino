#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SSD1306_LCDHEIGHT 64
#define trigpin D7
#define echopin D3
#define trig2 D5
#define echo2 D6

#define OLED_MOSI  D8
#define OLED_CLK   D4
#define OLED_DC    D1
#define OLED_CS    D2
#define OLED_RESET D0


// OLED display TWI address
#define OLED_ADDR   0x3C
#define SSD1306_128_64
#define SSD1306_LCDHEIGHT 64

Adafruit_SSD1306 display(OLED_MOSI,OLED_CLK,OLED_DC,OLED_RESET,OLED_CS);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
 
//-------- Customise these values -----------
const char* ssid = "GRIET1 2.4G";
const char* password = "abcd1234";
 
#define ORG "ypiq3v"
#define DEVICE_TYPE "sample"
#define DEVICE_ID "123456"
#define TOKEN "afCO8_SEmKv0b(RaYV"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/park/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);
String oledmsg,ol;
void oledDisplay(String message);

void setup() {
 Serial.begin(9600);
  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
  pinMode(trig2,OUTPUT);
  pinMode(echo2,INPUT);
 
 Serial.println();
 Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
 display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
 display.clearDisplay();
 display.display();
 ol="Smart parking system";
 display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setTextWrap(false);
  display.startscrollleft(0x00, 0x0F);
  display.println(ol);
  display.display();
 }
 
void loop() 
{
display.clearDisplay();
int duration,distance;
int duration1,distance1;

digitalWrite(trigpin,HIGH);
delay(1000);
digitalWrite(trigpin,LOW);
duration=pulseIn(echopin,HIGH);
distance=(duration/2)*0.0343;
Serial.print("distance ");
Serial.println(distance);

digitalWrite(trig2,HIGH);
delay(1000);
digitalWrite(trig2,LOW);
duration1=pulseIn(echo2,HIGH);
distance1=(duration1/2)*0.0343;
Serial.print("distance1 ");
Serial.println(distance1);

if(distance<10)
{
  //display.clearDisplay();
  Serial.println("already parking place1 is fill");
  display.setCursor(0,10);
  oledmsg="slot1:allocated";
  oledDisplay(oledmsg);
}

if(distance>=10)
{
  //display.clearDisplay();
  Serial.println("parking place1 is Available");
  display.setCursor(0,10);
  oledmsg="slot1:available";
  oledDisplay(oledmsg);
}

if(distance1<10)
{
  Serial.println("already parking place2 is fill");
  //display.clearDisplay();
  display.setCursor(0,20);
  oledmsg="slot2:allocated";
  oledDisplay(oledmsg);
}

if(distance1>=10)
{
  Serial.println("parking place2 is Available");
  //display.clearDisplay();
  display.setCursor(0,20);  
  oledmsg="slot2:available";
  oledDisplay(oledmsg);
}
PublishData(distance,distance1);
}

void PublishData(int distance, int distance1){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"slot1\":";
  payload += distance;
  payload+="," "\"slot2\":";
  payload += distance1;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
void oledDisplay(String message)
  {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setTextWrap(false);
  display.println(message);
  display.display();
  //display.startscrollleft(0x00, 0x0F);
  delay(1000); 
  }

