#include<Servo.h>
#include<LiquidCrystal.h>
#define gas_Pin 7
#define Exit 43         				// EXIT push button sensor.
#define In 44         				// Entry push button sensor.
#define Pwr 41           			// Power for push buttons.
#define Gnd 42                // Ground for push buttons.
#define CAPACITY 30      			// Total Capacity of the parking lot.

const int rs=13, en=12, d4=11, d5=10, d6=9, d7=8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LiquidCrystal lcd1(40, 39, 38, 37, 36, 35);
LiquidCrystal lcd_face(30, 29, 28, 27, 26, 25);

Servo myservo;
int fanpin=50;
int lightpin=52;
int sensorpin= A0;  
int enginepin=6;
int temp;

int trigpin=47;
int echopin=46;

long duration_0;
int distance_0;
int gas_value;
int day_light;
// int temp;

const int dist_trigPin = 34; //trig pin connection 
const int dist_echoPin = 33;  //echopin connection 
long duration;
int distance;

int collisionLedPin=32;
int collisionBuzzerPin=31;

const int faceLedPin = 5;
int data = 0;
int flag = 0;
int stat = 0;

void setup() {
  pinMode(lightpin,OUTPUT);
  pinMode(fanpin,OUTPUT);
  pinMode(sensorpin,INPUT); 
  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);  
  pinMode(enginepin,OUTPUT);  
  pinMode(dist_trigPin, OUTPUT);
  pinMode(dist_echoPin, INPUT);
  myservo.attach(45) ;

  pinMode(Gnd, OUTPUT);
  pinMode(Pwr, OUTPUT);
  pinMode(Exit, INPUT);
  pinMode(In, INPUT);    
  
  pinMode(faceLedPin, OUTPUT);
  digitalWrite (faceLedPin, LOW);
          
  
  digitalWrite(Gnd, LOW);			 // pin 10 is for Ground
  digitalWrite(Pwr, HIGH);		 // pin 6 is for power
    
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd1.begin(16, 2);
  lcd_face.begin(16, 2);
}
int  Available= 3;  

void loop() {
 
  AutoLight();
  
  AutoCooling();
  
  AutoDoor();  

  FacialDetection();

  AlcoholDetection();
  
  AvailableSeatCount();

  CollisionAvoidance();

}


void AutoLight(){
  day_light = analogRead(A1);
  digitalWrite(lightpin, LOW); 
  // Serial.print("distanceCm  ");
  // Serial.println(distance);

  if (day_light >= 25 ) {
    digitalWrite(52, HIGH);
  } else {
    digitalWrite(52, LOW);
  }
}

void AutoCooling(){
  temp = analogRead(sensorpin);
  temp = temp/2.0741;
  temp = temp + 1;

  if(temp>25){
    digitalWrite(fanpin,HIGH);
  } else {
    digitalWrite(fanpin,LOW);
  }
}

void AutoDoor(){
  digitalWrite (trigpin, LOW); 
  delayMicroseconds (2);
  digitalWrite (trigpin, HIGH); 
  delayMicroseconds (10) ;
  digitalWrite (trigpin, LOW);
 
  duration_0 = pulseIn (echopin, HIGH);
  distance_0 = (duration_0 / 2)/ 29.412 ;
  distance_0 = distance_0+1;

  if(distance_0 <= 20){
    if(Available>=1){
      myservo.write(120);
    } else {
      myservo.write(0);
    }    
  } else {
    myservo.write(0);
  } 
}

void FacialDetection(){
  
  if(Serial.available() > 0)
  {
    data = Serial.read();
  }
  
  if (flag == 0 && data == '1')
  {
    VerifyFace();
    stat = 1; 
    flag = 1;
    delay(500);    
  }
  else if (data == '0')
  {
    lcd_face.clear();
    lcd_face.setCursor(0,0);
    lcd_face.println("NOT VERIFIED");
    digitalWrite (faceLedPin, LOW);
    stat = 2;
    flag = 0;
  }  
}

void SerialFlush(){
  while(Serial.available() > 0){
    char t = Serial.read();
  }
}

void AlcoholDetection(){
  gas_value = digitalRead(gas_Pin);
  digitalWrite(enginepin,LOW);
  if(gas_value==1 && stat == 1) {
    lcd.clear();
    digitalWrite(enginepin,HIGH);
    lcd.setCursor(0,0);
    lcd.print("No Alcohol");
    lcd.setCursor(0,1);
    lcd.print("ENGINE STAT:ON");
    lcd.display();
  }
  else {
    lcd.clear();    
    digitalWrite(enginepin,LOW);
    lcd.setCursor(0,1);
    lcd.print("ENGINE STAT:OFF");
    lcd.display();
  }
  delay(500);
  // lcd.clear();
}

// void SerialFlush(){
//   while(Serial.available() > 0){
//     char t = Serial.read();
//   }
// }

void AvailableSeatCount(){
    lcd1.setCursor(0,0);    
    lcd1.print("available seat");
    lcd1.setCursor(1,1);
    lcd1.print(Available);
    lcd1.display();  
    if(digitalRead(In)== 1)
    {       
      if(Available != 0)
      {
        Available = Available - 1;
        lcd1.clear();
      }
    }

    if(digitalRead(Exit)== 1)
    {
      if(Available != CAPACITY)
      {
        Available = Available + 1;
        lcd1.clear();
      }
    }
    // delay(500);
    // lcd1.clear();
}

void CollisionAvoidance(){
    digitalWrite(dist_trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(dist_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(dist_trigPin, LOW);
    duration = pulseIn(dist_echoPin, HIGH);
    distance= duration*0.034/2;
    distance= distance/100; 

    if(distance<4){
      digitalWrite(collisionLedPin, HIGH);  
    } else {
      digitalWrite(collisionLedPin, LOW);  
    }

    if(distance<3){
      tone(collisionBuzzerPin,250);
    } else {
      noTone(collisionBuzzerPin);
    }
}

void VerifyFace(){

  // delay(500);

  lcd_face.clear();
  lcd_face.setCursor(0,0);
  lcd_face.println("FACE VERIFIED ");
  lcd_face.setCursor(0,0);
  lcd_face.println("FACE VERIFIED ");
  digitalWrite (faceLedPin, HIGH);
  // delay(1000);
  // lcd.clear();
    
  // AlcoholDetection();
 
}


// void SerialFlush(){
//   while(Serial.available() > 0){
//     char t = Serial.read();
//   }
// }
// }

