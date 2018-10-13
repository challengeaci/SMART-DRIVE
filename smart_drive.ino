#define trigPin1 4  
#define echoPin1 5
#define trigPin2 8 
#define echoPin2 10
// the setup function runs once when you press reset or power the board
#include <SoftwareSerial.h>
SoftwareSerial myserial(2,3);// rx,tx
void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
   Serial.begin(115200);
   myserial.begin(115200);
  Serial.print("Ultrasonic Sensor Starting!!!");
  Serial.println("");
}
// the loop function runs over and over again forever
void loop() 
{
  int duration_1,duration2,distance_1,distance2;
  digitalWrite(trigPin1, HIGH);
  delay(1000);
  digitalWrite(trigPin1, LOW);
  duration_1 = pulseIn(echoPin1, HIGH);
  distance_1 =(duration_1/2) * 0.0343;// D = time * speed of sound(0.0343cm/microsec)
  Serial.print("Front distance:");
  Serial.print(distance_1);
  Serial.println(" cm");
  if(distance_1>10 && distance_1<=15)
  {
   analogWrite(11,60);//led
   analogWrite(6,60);//buzzer
  }
  else if(distance_1>5 && distance_1<=10)
  {
    analogWrite(11,128);
    analogWrite(6,128);
  }
   else if(distance_1>=1 && distance_1<=5)
  {
    analogWrite(11,255);
    analogWrite(6,255);
  }
  else if(distance_1<0)
  {
    analogWrite(11,255);
    analogWrite(6,255);
    analogWrite(11,0);
    analogWrite(6,255);
    analogWrite(11,255);
    analogWrite(6,255);
    analogWrite(11,0);
    analogWrite(6,255);
    analogWrite(11,255);
    analogWrite(6,255);
  }
  else
  {
    analogWrite(11,0);
    analogWrite(6,0);
  }// wait for a second
  digitalWrite(trigPin2, HIGH);
  delay(1000);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 =(duration2/2) * 0.0343;// D = time * speed of sound(0.0343cm/microsec)
  Serial.print("Rear distance:");
  Serial.print(distance2);
  Serial.println(" cm");
  if(distance2>8 && distance2<=10)
  {
    analogWrite(9,60);//seperate LEDs
  }
  else if(distance2>5 && distance2<=8)
  {
    analogWrite(9,128);
  }
   else if(distance2>=1 && distance2<=5)
  {
    analogWrite(9,255);
  }
  else if(distance2<0)
  {
    analogWrite(11,255);
    analogWrite(11,0);
    analogWrite(11,255);
    analogWrite(11,0);
    analogWrite(11,255);
  }
  else
  {
    analogWrite(9,0);
  }// wait for a second
  
  String data= ","+String(distance_1)+","+String(distance2)+"!";
  myserial.println(data);
  Serial.println(data);
  
}
