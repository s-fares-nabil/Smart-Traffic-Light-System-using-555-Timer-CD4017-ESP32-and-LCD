#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 

const int RED_PIN = 32;
const int YELLOW_PIN = 33;
const int GREEN_PIN = 25;

bool lastRed = LOW;
bool lastYellow = LOW;
bool lastGreen = LOW;

unsigned long timeRedRise = 0;
unsigned long timeYellowRise = 0;
unsigned long timeGreenRise = 0;

// المتغيرات الديناميكية اللي بتسجل الوقت الفعلي لكل لون
unsigned long dynamicRedDuration = 0; 
unsigned long dynamicYellowDuration = 0;
unsigned long dynamicGreenDuration = 0; 

int currentState = 0; // 0=Syncing, 1=RED, 2=YELLOW, 3=GREEN

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  
  pinMode(RED_PIN, INPUT_PULLDOWN);
  pinMode(YELLOW_PIN, INPUT_PULLDOWN);
  pinMode(GREEN_PIN, INPUT_PULLDOWN);
  
  lcd.setCursor(0, 0);
  lcd.print("System Syncing..");
}

void loop() {
  bool currentRed = digitalRead(RED_PIN);
  bool currentYellow = digitalRead(YELLOW_PIN);
  bool currentGreen = digitalRead(GREEN_PIN);
  unsigned long now = millis();

  // ==========================================
  // 1. بداية الأحمر
  // ==========================================
  if (currentRed == HIGH && lastRed == LOW) {
    if (timeGreenRise > 0) {
      // هنا بنحسب وقت الأخضر (شامل الـ Overlap اللي تجاهلناه)
      dynamicGreenDuration = now - timeGreenRise; 
    }
    timeRedRise = now;
    currentState = 1; 
  }

  // ==========================================
  // 2. بداية الأصفر (يُقبل فقط لو إحنا في حالة الأحمر)
  // ==========================================
  if (currentYellow == HIGH && lastYellow == LOW) {
    if (currentState == 1) { // الشرط ده بيمنع الأصفر يظهر بعد الأخضر
      if (timeRedRise > 0) {
        // بنقيس وقت الأحمر الصافي بدون الأصفر
        dynamicRedDuration = now - timeRedRise; 
      }
      timeYellowRise = now;
      currentState = 2;
    }
  }

  // ==========================================
  // 3. بداية الأخضر
  // ==========================================
  if (currentGreen == HIGH && lastGreen == LOW) {
    if (currentState == 2 || currentState == 1) { 
      if (timeYellowRise > 0) {
        // بنقيس وقت الأصفر الصافي
        dynamicYellowDuration = now - timeYellowRise; 
      }
      timeGreenRise = now;
      currentState = 3;
    }
  }

  lastRed = currentRed;
  lastYellow = currentYellow;
  lastGreen = currentGreen;

  // ==========================================
  // تحديث شاشة الـ LCD
  // ==========================================
  
  if (dynamicRedDuration > 0 && dynamicYellowDuration > 0 && dynamicGreenDuration > 0 && currentState != 0) {
    unsigned long elapsed = 0;
    unsigned long total_time = 0;
    String label = "";

    if (currentState == 1) { 
      label = "RED   ";
      elapsed = now - timeRedRise; 
      total_time = dynamicRedDuration; 
    } 
    else if (currentState == 2) { 
      label = "YELLOW";
      elapsed = now - timeYellowRise; 
      total_time = dynamicYellowDuration; 
    }
    else if (currentState == 3) { 
      label = "GREEN ";
      elapsed = now - timeGreenRise; 
      total_time = dynamicGreenDuration; 
    }

    lcd.setCursor(0, 0);
    lcd.print("State: " + label);
    
    lcd.setCursor(0, 1);
    if (total_time >= elapsed) {
      int remain_sec = ((total_time - elapsed) + 999) / 1000; 
      lcd.print("Time : ");
      if (remain_sec < 10) lcd.print(" "); 
      lcd.print(remain_sec);
      lcd.print(" s    ");
    } else {
      lcd.print("Time : 0 s      "); 
    }
  } else {
    // شاشة الانتظار في أول دورة بس لحد ما يجمع أوقات الـ 3 ألوان
    lcd.setCursor(0, 0);
    lcd.print("Syncing Cycles..");
    lcd.setCursor(0, 1);
    lcd.print("Reading Pot...  ");
  }
}