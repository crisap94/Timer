#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AnalogButtons.h>

uint8_t analogButtonsPin = A3;
uint8_t pinRelay = 4;

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

AnalogButtons analogButtons = AnalogButtons(analogButtonsPin,20);

void start_stop_ButtonClick();
void M_ButtonClick();
void m_ButtonClick();
void S_ButtonClick();
void s_ButtonClick();
void displayInformation();
void run();
void noneButtonPressed();
void turnOffSwitch();
void turnOnSwitch();

Button start_stop_Button = Button(72, &start_stop_ButtonClick);
Button M_Button = Button(215, &M_ButtonClick);
Button m_Button = Button(400, &m_ButtonClick);
Button S_Button = Button(730, &S_ButtonClick);
Button s_Button = Button(1023, &s_ButtonClick);

//tiempo ingresado por el usuario
int8_t minutes 			= 0;
int8_t seconds 	  	= 0;

//Tiempo restante

int8_t remain_min    = 0;
int8_t remain_seg    = 0;
//Banderas para la toma unica de datos
bool start 			    	= false;
bool stateStart 		  = true;

//variable que define el comienzo del conteo del tiempo
unsigned long tiempoOn= 0;
bool tiempoOn_f 	   	= false;

//bandera de presionado
bool statePressed     = false;

void setup(){

  analogButtons.add(start_stop_Button);
  analogButtons.add(M_Button);
  analogButtons.add(m_Button);
  analogButtons.add(S_Button);
  analogButtons.add(s_Button);

  // Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setCursor(2,0);
  lcd.print("TEMPORIZADOR");
  lcd.setCursor(5,1);
  lcd.print("V.2.0");
  delay(1000);
  lcd.clear();
}

void loop(){
  analogButtons.check();
  displayInformation();
  run();

  //Code....!!
  statePressed = false;
}

void displayInformation() {
  if(!start){

    lcd.setCursor(1,0);
    lcd.print("AJUSTE TIEMPO");
    lcd.setCursor(5,1);
    if(minutes<10){
      lcd.print("0");
    }
    lcd.print(minutes);
    lcd.print(":");
    if(seconds < 10){
      lcd.print("0");
    }
    lcd.print(seconds);

  }else{

    lcd.setCursor(0, 0);
    lcd.print("TIEMPO RESTANTE");
    lcd.setCursor(5,1);
    if(remain_min<10){
      lcd.print("0");
    }
    lcd.print(remain_min);
    lcd.print(":");
    if(remain_seg < 10){
      lcd.print("0");
    }
    lcd.print(remain_seg);
  }
}

void run(){
  if(start){
    turnOnSwitch();
    if(tiempoOn_f){
      tiempoOn = millis();
      remain_min = minutes;
      remain_seg = seconds;
      tiempoOn_f = false;
    }
    if (millis()-tiempoOn > 1000){
      remain_seg--;
      if (remain_seg < 0) {
        remain_min--;
        remain_seg = 59;
      }
      tiempoOn = millis();
    }

    if(remain_min == 0 && remain_seg == 0){
      start=false;
      lcd.clear();
      minutes = 0;
      seconds = 0;
    }
  }else{
    turnOffSwitch();
  }
  noneButtonPressed();
}

void noneButtonPressed(){
  if (!statePressed) {
    stateStart = true;
  }
}

void turnOnSwitch() {
  digitalWrite(pinRelay, HIGH);
}

void turnOffSwitch() {
  digitalWrite(pinRelay, LOW);
}

void start_stop_ButtonClick(){
  if(stateStart){
    if(start){
      start = false;
      tiempoOn_f = false;
      minutes = 0;
      seconds = 0;
      lcd.clear();
    }else{
      start = true;
      tiempoOn_f = true;
      lcd.clear();
    }
    stateStart = false;
  }
  statePressed = true;
}

void M_ButtonClick(){

  minutes++;
  if(minutes > 59){
    minutes = 0;
  }
  if (minutes < 0) {
    minutes = 0;
  }
  statePressed = true;
}

void m_ButtonClick(){
  minutes--;
  if(minutes<0){
    minutes = 59;
  }
  statePressed = true;
}

void S_ButtonClick(){
  seconds+=5;
  if(seconds>59){
    minutes-=5;
    if(minutes < 0){
      minutes = 0;
    }
    seconds=0;
  }
  statePressed = true;
}

void s_ButtonClick(){
  seconds-=5;
  if(seconds<0){
    minutes--;
    if(minutes < 0){
      minutes = 0;
    }
    seconds=55;
  }
  statePressed = true;
}
