//Water Level Detection Project

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

const int pump = 10; //pump connected to pin5
const int TrigPin = 2;  //Trig attach to pin2
const int EchoPin = 3;  //Echo attach to pin3
const int buttonPin1 = 8;
const int buttonPin2 = 9;

int tankHeight = 30;  //set according to tank size
int waterLevel = 0;
unsigned int tankFull = 0;
unsigned int tankLow = 0;
int percent = 0;

int buttonLow = 0;
int buttonFull = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.home();
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT); 
  pinMode(pump, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(TrigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin,LOW);

  //Set low water level (on pump)
  buttonLow = digitalRead(buttonPin1);
  buttonFull = digitalRead(buttonPin2);

  if(buttonLow == LOW){
    if ((tankLow >= 0)&&(tankLow <= 99)){ 
      tankLow = tankLow+10;
    }
    else if (tankLow == 100){
      tankLow = 0;
    }
    Serial.print("TankLow: ");
    Serial.println(tankLow);
    lcd.setCursor(10, 1);
    lcd.print(tankLow);
    delay(500);
  }

  //Set high water level (off pump)
  if(buttonFull == LOW){
    if ((tankFull >= 0)&&(tankFull <= 99)){ 
      tankFull = tankFull+10;
    }
    else if (tankFull == 100){
      tankFull = 0;
    }
    Serial.print("TankFull: ");
    Serial.println(tankFull);
    lcd.setCursor(11, 2);
    lcd.print(tankFull);
    delay(500);
  }
  
  
  waterLevel = pulseIn(EchoPin,HIGH)/58.0;  
  waterLevel = (int(waterLevel * 100.0))/100.0;

  percent = 100*waterLevel/tankHeight;

  Serial.print("Water level: ");
  Serial.print(percent);
  Serial.println(" %");
  
  lcd.setCursor(0, 0);
  lcd.print("Water level: ");
  lcd.setCursor(17, 0);
  lcd.print("%");
  if(percent >= 100){
    lcd.setCursor(13, 0);
    lcd.print("100");
  }
  else{
    lcd.setCursor(13, 0);
    lcd.print(percent);
    lcd.setCursor(15, 0);
    lcd.print("  ");
  }

  lcd.setCursor(0, 1);
  lcd.print("Tank low: ");
  lcd.setCursor(0, 2);
  lcd.print("Tank full: ");
  lcd.setCursor(0, 3);
  lcd.print("Status: ");
  delay(100);
  

  if(percent <= tankLow){
    digitalWrite(pump, HIGH);
    Serial.print("Water level: ");
    Serial.print(percent);
    Serial.print(" % ");
    Serial.println("tank low");
    delay(100);
  }
  else if(percent >= tankFull){
    digitalWrite(pump, LOW);
    Serial.print("Water level: ");
    Serial.print(percent);
    Serial.print(" % ");
    Serial.println("tank full");
    delay(100);
  }

}
