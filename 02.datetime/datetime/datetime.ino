#include <LDateTime.h>

#define LED_GPIO_PIN  13

void setup() {
  // initialize LED pin as an output.
  pinMode(LED_GPIO_PIN, OUTPUT);

  //Setup Serial Port.
  Serial.begin(9600);
  Serial.println("Linkit ONE Board"); //print with line change.

  setDatetime();
  displayDatetime();
}

void ledOn() {
  digitalWrite(LED_GPIO_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void ledOff() {
  digitalWrite(LED_GPIO_PIN, LOW);    // turn the LED off by making the voltage LOW
}

void setDatetime() {
  datetimeInfo t;
  t.year = 2017;
  t.mon = 10;
  t.day = 22;
  t.hour = 15;
  t.min = 29;
  t.sec = 8;

  LDateTime.setTime(&t);
}

void displayDatetime() {
  datetimeInfo t;
  unsigned int rtc;
  LDateTime.getTime(&t);
  LDateTime.getRtc(&rtc);

  Serial.print("Current Time:[");   //print without line change.
  Serial.print(t.year);
  Serial.print("-");
  Serial.print(t.mon);
  Serial.print("-");
  Serial.print(t.day);
  Serial.print(" ");
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.min);
  Serial.print(":");
  Serial.print(t.sec);
  Serial.println("]");
  Serial.print("rtc:");
  Serial.println(rtc);
  Serial.println(" ");
}

void loop() {
  displayDatetime();

  delay(200);
  ledOn();
  delay(800);
  ledOff();
}

