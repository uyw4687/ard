#include <Servo.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <DHT_U.h>

#include <SoftwareSerial.h>

const int piezo_ = 2;
DHT_Unified dht(3, DHT11);
SoftwareSerial BTSerial(4, 5);
const int irdist_ = 5;
const int servo_ = 6;
Servo servo;
LiquidCrystal_I2C lc(0x27, 16, 2);
const int cds_ = A0;

const int CS = 1109;
const int A = 1760;
const int B = 1976;

const int o = 128;
const int h = o / 2;
const int o2 = o * 2;

const int frqs[] = {B/2, B/2, A/2, CS
                   };
const int lens[] = {o2, o, h, h+o2,
                   };

byte temp[] = {
  B00100,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};

byte humi[] = {
  B00100,
  B00100,
  B01010,
  B01010,
  B10001,
  B11111,
  B11111,
  B01110
};

byte C[] = {
  B10000,
  B00110,
  B01001,
  B01000,
  B01000,
  B01001,
  B00110,
  B00000
};

int servo__ = 1;
int irdist__ = 1;
bool lc__ = false;
bool dht__ = false;
int ledair_ = 7;
bool ledair__ = false;
int led1r_ = 8;
bool led1r__ = false;
int led1g_ = 9;
bool led1g__ = false;
int led1b_ = 10;
bool led1b__ = false;
int led2r_ = 11;
bool led2r__ = false;
int led2g_ = 12;
bool led2g__ = false;
int led2b_ = 13;
bool led2b__ = false;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  
  pinMode(piezo_, OUTPUT);
  pinMode(ledair_, OUTPUT);
  pinMode(led1r_, OUTPUT);
  pinMode(led1g_, OUTPUT);
  pinMode(led1b_, OUTPUT);
  pinMode(led2r_, OUTPUT);
  pinMode(led2g_, OUTPUT);
  pinMode(led2b_, OUTPUT);

  servo.attach(servo_);
  servo.write(0);
  
  pinMode(irdist_, INPUT);

  lc.init();
  lc.backlight();
  lc.setCursor(0,0);
  lc.print("TV is playing...");
  lc.createChar(0, temp);
  lc.createChar(1, humi);
  lc.createChar(2, C);

  dht.begin();
}

void loop() {
  int cdsVal = analogRead(cds_);
//  Serial.println(cdsVal);
  if (cdsVal < 700) {
    if (servo__ == 1) {
      for(int i=0;i<4;i++) {
        if (frqs[i]!=-1) {
          tone(piezo_, frqs[i],lens[i]);
        }
        delay(lens[i]+10);
      }
      servo.write(90);
      servo__ = 0;
    }
  }
  else {
    if (servo__ == 0) {
      servo.write(0);
      servo__ = 1;
    }
  }

  int irdistVal = digitalRead(irdist_);
//  Serial.println(irdistVal);
  if (irdistVal==1) {
    if (irdist__==0) {
      lc.backlight();
      irdist__ = 1;
    }
  }
  else {
    if (irdist__==1) {
      lc.noBacklight();
      irdist__ = 0;
    }
  }

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
//    Serial.println(event.temperature);
  }
  
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
//    Serial.println(event.relative_humidity);
  }

  if (dht__) {
    lc.setCursor(0, 0);
    lc.print("Weather Today");
    lc.setCursor(0, 1);
    lc.write((byte)0);
    lc.print(" ");
    lc.print(event.temperature);
    lc.write(2);
  }

  if (BTSerial.available()) {
    char c = BTSerial.read();
    Serial.write(c);
    
    switch(c) {
      case 'a':
      revLED(led1r_,led1r__);
      break;
      case 'b':
      revLED(led1g_,led1g__);
      break;
      case 'c':
      revLED(led1b_,led1b__);
      break;

      case 'd':
      revLED(ledair_, ledair__);
      break;
      
      case 'e':
      if(!lc__) {
        lc.backlight();
      }
      else {
        lc.noBacklight();
        lc.clear();
      }
      lc__ = !lc__;
      break;
      
      case 'f':
      dht__ = !dht__;
      break;
      
      case 'g':
      revLED(led2r_,led2r__);
      break;
      case 'h':
      revLED(led2g_,led2g__);
      break;
      case 'i':
      revLED(led2b_,led2b__);
      break;
    }
  }
}

void revLED(int led_, boolean &led__) {
  if(led__) {
    digitalWrite(led_, LOW);
  }
  else {
    digitalWrite(led_, HIGH);
  }
  led__ = !led__;
}
