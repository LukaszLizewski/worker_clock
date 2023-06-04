/*
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int powerSupply = 7, input = 8, outputGreen = 9, outputRed = 10, outputTurnOff = 13;
long freeHoursTime = 0;
long freeMinutesTime = 0;
long freeSecondsTime = 0;
long workHoursTime = 0;
long workMinutesTime = 0;
long workSecondsTime = 0;

long CONSTANT = 60;
long secondOff = 60;
// long secondOff = 1*CONSTANT*CONSTANT;

String work = String("Work=>");
String freedom = String("Free=>");
char dots = ':';
char dash = '-';
long latestTimestamp = 0;
long savedTimestamp = 0;
boolean isWorkTimer = true;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  pinMode(input, INPUT);
  pinMode(outputGreen, OUTPUT);
  pinMode(outputRed, OUTPUT);
  pinMode(outputTurnOff, OUTPUT);
  pinMode(powerSupply, OUTPUT);
  digitalWrite(outputTurnOff, LOW);
  digitalWrite(powerSupply, HIGH);
  lcd.begin(16, 2);
  sayHello();
  printSolids();
  lightProperLed(isWorkTimer);
}

void loop() {
  if (digitalRead(input) == HIGH) {
    delay(20);
    isWorkTimer = !isWorkTimer;
    while (digitalRead(input) == HIGH);
    delay(20);
    lightProperLed(isWorkTimer);
  }
  latestTimestamp = millis() / 1000;
  if (latestTimestamp > savedTimestamp) {
    savedTimestamp = latestTimestamp;
    if (isWorkTimer) {
      setWorkTime();
    } else {
      setFreeTime();
    }
  }
  checkHasWorkDone();
}

void lightProperLed(boolean workingMode) {
  if (workingMode) {
    digitalWrite(outputRed, HIGH);
    digitalWrite(outputGreen, LOW);
  } else {
    digitalWrite(outputRed, LOW);
    digitalWrite(outputGreen, HIGH);
  }
}

void checkHasWorkDone() {
  if (savedTimestamp > secondOff) {
    sayGoodBye();
    digitalWrite(powerSupply, LOW);
    delay(500);
    digitalWrite(outputTurnOff, HIGH);
    delay(500);
  }
}

void setWorkTime() {
  printTime(workSecondsTime, workMinutesTime, workHoursTime, 0);
  workSecondsTime++;
  if (workSecondsTime % CONSTANT == 0) {
    workMinutesTime++;
    if (workMinutesTime % CONSTANT == 0) {
      workHoursTime++;
    }
  }
}

void setFreeTime() {
  printTime(freeSecondsTime, freeMinutesTime, freeHoursTime, 1);
  freeSecondsTime++;
  if (freeSecondsTime % CONSTANT == 0) {
    freeMinutesTime++;
    if (freeMinutesTime % CONSTANT == 0) {
      freeHoursTime++;
    }
  }
}

void printTime(long sec, long minutes, long hours, int row) {
  lcd.setCursor(7, row);
  printDigit(hours, 7, row);
  lcd.setCursor(10, row);
  printDigit(minutes - hours * CONSTANT, 10, row);
  lcd.setCursor(13, row);
  printDigit(sec - minutes * CONSTANT, 13, row);
}

void printDigit(long digit, int intCursor, int row) {
  if (digit < 10) {
    lcd.print(0);
    lcd.setCursor(intCursor + 1, row);
    lcd.print(digit);
  } else {
    lcd.print(digit);
  }
}

void sayHello() {
  lcd.print("Hello, Luke!");
  delay(1500);
  lcd.clear();
  lcd.print("Beautiful day!");
  lcd.setCursor(0, 1);
  lcd.print("Is't it?");
  delay(1500);
  lcd.clear();
}

void sayGoodBye() {
  lcd.clear();
  lcd.print("Good Bye Luke!");
  delay(1500);
  lcd.clear();
}

void printSolids() {
  lcd.setCursor(0, 0);
  lcd.print(work);
  lcd.setCursor(0, 1);
  lcd.print(freedom);
  lcd.setCursor(9, 0);
  lcd.print(dots);
  lcd.setCursor(12, 0);
  lcd.print(dots);
  lcd.setCursor(9, 1);
  lcd.print(dots);
  lcd.setCursor(12, 1);
  lcd.print(dots);
}