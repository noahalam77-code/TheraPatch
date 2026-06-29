
//TheraPatch project COOODDEEEEEEEEEEE final
//
//
//
#include <U8g2lib.h>   
#include <Wire.h>     
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
//buttons
#define BTN_LEFT A2   
#define BTN_RIGHT 4    
#define BTN_BACK 2   
#define BTN_START 9    
#define PIN_TEMP A0  
#define PIN_BATT A7  
#define PIN_FAN 3    
#define PIN_INA 6  
#define PIN_INB 7    
#define PIN_PWM 5   
// themisotr calabration
#define BETA_VLU 3950.0
#define T_R 10000.0    
#define KELVIN 298.15     
#define TEN_R 10000.0  

//Battery Dispplay
#define VOLT_SCL 5.7       
#define BATT_FULL 12.6       
#define BATT_EMPTY 9.0     
// safety cuttof
#define WIGGLE 1    
#define COLD_CUTOFF 30  
#define HOT_CUTOFF 113  
#define PWM_FULL 255  
//ranges
#define COLD_MIN 35
#define COLD_MAX 60
#define HOT_MIN 93
#define HOT_MAX 108
#define TIME_MIN 5
#define TIME_MAX 30
//screens
#define MODE 0
#define TEMP 1
#define TIME 2
#define RUN 3
#define DONE 4
#define ERROR 5

//Memory
int screen = MODE;  
int mode = 0;    
int target = 60;   
int minutes = 15;   

unsigned long lastTick = 0;  
unsigned long lastCtrl = 0;  
int secondsLeft = 0;         
int curTemp = 0;             
int batt = 0;           

//PELITER AND FAN CONTROL
//
//
void peltierOff() {
  analogWrite(PIN_PWM, 0);    
  digitalWrite(PIN_INA, LOW);
  digitalWrite(PIN_INB, LOW);
  digitalWrite(PIN_FAN, LOW);
}

//skin side cold
void peltierCool() {
  digitalWrite(PIN_INA, HIGH);
  digitalWrite(PIN_INB, LOW);
  analogWrite(PIN_PWM, PWM_FULL);
  digitalWrite(PIN_FAN, HIGH); 
}

// skin side hot
void peltierHeat() {
  digitalWrite(PIN_INA, LOW);
  digitalWrite(PIN_INB, HIGH);
  analogWrite(PIN_PWM, PWM_FULL);
  digitalWrite(PIN_FAN, HIGH);
}

//themsitor reading
int readTempF() {
  int raw = analogRead(PIN_TEMP);
  if (raw <= 0 || raw >= 1023) {
    return -100;  
  }
  
  float ratio = 1023.0 / raw;       
  float r = TEN_R  * (ratio - 1.0); 
  float a = 1.0 / KELVIN;                   
  float b = (1.0 / BETA_VLU) * log(r / T_R);
  float tK = 1.0 / (a + b);               
  float tC = tK - 273.15;
  float tF = tC * 9.0 / 5.0 + 32.0;
  return (int)tF;
}

// battery reading
int readBattPct() {
  int raw = analogRead(PIN_BATT);
  float v = (raw / 1023.0) * 5.0 * VOLT_SCL;   
  int pct = (int)(((v - BATT_EMPTY) / (BATT_FULL - BATT_EMPTY)) * 100.0);
  if (pct < 0)   pct = 0;     
  if (pct > 100) pct = 100;     
  return pct;
}

//BUTTONS
//
//
int lLeft = HIGH, lRight = HIGH, lBack = HIGH, lStart = HIGH;
bool press(int pin, int &last) {
  int now = digitalRead(pin);
  bool justPressed = (now == LOW && last == HIGH);
  last = now;
  return justPressed;
}

void startRun() {
  secondsLeft = minutes * 60;
  lastTick = millis();
  lastCtrl = 0;
  screen = RUN;
}
void stopRun() {
  peltierOff();
  screen = MODE;
}

void onLeft() {
  if (screen == MODE) 
    mode = 0;
  else if (screen == TEMP) target = (mode == 0) ? max(COLD_MIN, target - 1) : max(HOT_MIN, target - 1);
  else if (screen == TIME) minutes = max(TIME_MIN, minutes - 1);
}
void onRight() {
  if (screen == MODE) 
    mode = 1;
  else if (screen == TEMP) target = (mode == 0) ? min(COLD_MAX, target + 1) : min(HOT_MAX, target + 1);
  else if (screen == TIME) minutes = min(TIME_MAX, minutes + 1);
}
void onStart() {
  if (screen == MODE) 
    { target = (mode == 0) ? 60 : 104; screen = TEMP; }
  else if (screen == TEMP) screen = TIME;
  else if (screen == TIME) startRun();
  else if (screen == RUN)  stopRun();
  else                     screen = MODE;
}
void onBack() {
  if (screen == TEMP) 
    screen = MODE;
  else if (screen == TIME) screen = TEMP;
  else if (screen == RUN)  stopRun();
  else if (screen == DONE || screen == ERROR) screen = MODE;
}


//LCD SCREEN
//
//
void centerStr(int y, const char* s) {
  int w = u8g2.getStrWidth(s);
  u8g2.drawStr((128 - w) / 2, y, s);
}
void drawSetup(const char* title, const char* big, const char* point) {
  u8g2.setFont(u8g2_font_6x10_tr);
  centerStr(12, title);
  u8g2.setFont(u8g2_font_ncenB14_tr);
  centerStr(40, big);
  u8g2.setFont(u8g2_font_5x7_tr);
  centerStr(60, point);
}                        

void drawRun() {
  char buf[16];

  u8g2.setFont(u8g2_font_5x7_tr);

  // battery bar
  snprintf(buf, sizeof(buf), "%d%%", batt);
  u8g2.drawStr(0, 8, buf);

  // current temp 
  snprintf(buf, sizeof(buf), "%dF", curTemp);
  int w = u8g2.getStrWidth(buf);
  u8g2.drawStr((128 - w) / 2, 8, buf);
  // mode on the right
  const char* m = (mode == 0) ? "COOL" : "HEAT";
  w = u8g2.getStrWidth(m);
  u8g2.drawStr(128 - w, 8, m);
  // staus bar speration
  u8g2.drawHLine(0, 12, 128);
//main aereia
  // target temp
  u8g2.setFont(u8g2_font_ncenB10_tr);
  snprintf(buf, sizeof(buf), "Target %dF", target);
  centerStr(34, buf);
  //  timer
  u8g2.setFont(u8g2_font_ncenB14_tr);
  snprintf(buf, sizeof(buf), "%02d:%02d", secondsLeft / 60, secondsLeft % 60);
  centerStr(58, buf);
}
// scrren dispaly
void draw() {
  char buf[16];
  u8g2.clearBuffer();

  if (screen == MODE) {
    drawSetup("SET MODE", (mode == 0) ? "COLD" : "HOT", "< COLD    HOT >");
  }
  else if (screen == TEMP) {
    snprintf(buf, sizeof(buf), "%d F", target);
    drawSetup("SET TEMP", buf, "<  -        +  >");
  }
  else if (screen == TIME) {
    snprintf(buf, sizeof(buf), "%d min", minutes);
    drawSetup("SET TIME", buf, "<  -        +  >");
  }
  else if (screen == RUN) {
    drawRun();
  }
  else if (screen == DONE) {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    centerStr(30, "DONE");
    u8g2.setFont(u8g2_font_6x10_tr);
    centerStr(50, "press any key");
  }
  else if (screen == ERROR) {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    centerStr(30, "ERROR");
    u8g2.setFont(u8g2_font_6x10_tr);
    centerStr(50, "check sensor");
  }
  u8g2.sendBuffer();
}
//SAFTEY AND BANG BANG
//
//
void controlTick() {
  int t = readTempF();

  // Saftery
  if (t == -100) { peltierOff(); screen = ERROR; return; }
  curTemp = t;
  if (mode == 0 && t <= COLD_CUTOFF) { peltierOff(); screen = ERROR; return; }
  if (mode == 1 && t >= HOT_CUTOFF)  { peltierOff(); screen = ERROR; return; }

 // bang bang
  if (mode == 0) {                       
    int tooWarm   = target + WIGGLE;     
    int coldEnough = target - WIGGLE;   

    if (t > tooWarm) {
      peltierCool();                   
    }
    else if (t < coldEnough) {
      peltierOff();                 
    }
  }
  else {                                 
    int tooCool   = target - WIGGLE;     
    int hotEnough = target + WIGGLE;   

    if (t < tooCool) {
      peltierHeat();                 
    }
    else if (t > hotEnough) {
      peltierOff();                     
    }
  }
}
//SETUP BOARD WHEN PLUGED
//
//
void setup() {
  pinMode(PIN_INA, OUTPUT);
  pinMode(PIN_INB, OUTPUT);
  pinMode(PIN_PWM, OUTPUT);
  pinMode(PIN_FAN, OUTPUT);
  peltierOff();


  pinMode(BTN_LEFT,  INPUT);
  pinMode(BTN_RIGHT, INPUT);
  pinMode(BTN_BACK,  INPUT);
  pinMode(BTN_START, INPUT);

  u8g2.begin();  
}

//Big loop
//
//
void loop() {
  if (press(BTN_LEFT,  lLeft))  onLeft();
  if (press(BTN_RIGHT, lRight)) onRight();
  if (press(BTN_START, lStart)) onStart();
  if (press(BTN_BACK,  lBack))  onBack();

  batt = readBattPct();  
  if (screen == RUN) {
    if (millis() - lastCtrl >= 500) { lastCtrl = millis(); controlTick(); }
    if (millis() - lastTick >= 1000) {
      lastTick += 1000;
      if (secondsLeft > 0) secondsLeft--;
      if (secondsLeft == 0) { peltierOff(); screen = DONE; } 
    }
  }

  draw();        
  delay(20);   
}
