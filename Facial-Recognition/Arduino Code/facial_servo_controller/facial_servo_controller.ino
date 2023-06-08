#include <LiquidCrystal.h>
#include <Servo.h>
#define gas_Pin 7

Servo myservo;
const int led = 11;
int data = 0;
int flag = 0;
// const int max_password = 4;
// char my_password[max_password] = "";
LiquidCrystal lcd(A4, A5, A3, A2, A1, A0);
// const byte rows=4;
// const byte cols=3;

int val= 9;

// byte rowPins[rows]={2,3,4,5};
// byte colPins[cols]={6,7,8};
int currentposition=0; //Position of LCD Cursor

int gas_value;

void setup()
{
  pinMode(led, OUTPUT);
  pinMode(val, OUTPUT);
  
  digitalWrite (led, LOW);
  digitalWrite (val, LOW);
  Serial.begin(9600);
  Serial.println("Connection Established!");
  DisplayScreen();
  lcd.begin(16,2);
}

void loop(){
  DisplayScreen();
  
  if(currentposition == 0){
    DisplayScreen();
  }
  
  if(Serial.available() > 0)
  {
    data = Serial.read();
  }
  
  if (flag == 0 && data == '1')
  {
    OpenDoor();
    flag = 1;
  }
  else if (data == '0')
  {
    digitalWrite (led, LOW);
    flag = 0;
  }


}

void DisplayScreen(){
  lcd.setCursor(0,0);
  lcd.println("SHOW FACE TO  ");
  lcd.setCursor(0,1); 
  lcd.println("START BUS");
}

void OpenDoor(){

  delay(900);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("ENGINE STARTED ");
  
  digitalWrite (led, HIGH);
  digitalWrite (val, HIGH);

  delay(3000);

  digitalWrite(led, LOW);
  
}


void SerialFlush(){
  while(Serial.available() > 0){
    char t = Serial.read();
  }
}

