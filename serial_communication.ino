String data,url2,d1,d2;
#include <SoftwareSerial.h>   
SoftwareSerial mySerial(D2,D3);

/////////////////////// i added

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "GRIET1 2.4G";
const char* password = "abcd1234";
#define ORG "8zldit"
#define DEVICE_TYPE "VRY"
#define DEVICE_ID "280621"
#define TOKEN "yashurammuvyshu123"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/distance/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
float d11,d22;
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);

//////////////////////////////

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);

 ////////////////// i added
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());

 //////////////////////////////
 
  while (!mySerial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() { // run over and over

  if (mySerial.find(","))
{
  url2=mySerial.readStringUntil('!');
  Serial.println(url2);
  d1 = getValue1(url2, ',', 0);
  d2 = getValue1(url2, ',', 1);
  Serial.println(d1);
  Serial.println(d2);
}
d11 = d1.toFloat();
d22 = d2.toFloat();
PublishData(d11,d22);

delay(1000);
 
}

void PublishData(float temp, float humid){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"distance1\":";
  payload += d1;
  payload+="," "\"distance2\":";
  payload += d2;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}

String getValue1(String data, char separator, int index){
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
        
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
