#include <LDateTime.h>
#include <LStorage.h>
#include <LSD.h>


#define LED_GPIO_PIN  13

void setup() {
  // initialize LED pin as an output.
  pinMode(LED_GPIO_PIN, OUTPUT);

  //Setup Serial Port.
  Serial.begin(9600);
  Serial.println("Linkit ONE Board"); //print with line change.

  setDatetime();
  initSd();
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
  t.day = 24;
  t.hour = 03;
  t.min = 14;
  t.sec = 8;

  LDateTime.setTime(&t);
}

char* makeDatetimeStr() {
  datetimeInfo t;
  unsigned int rtc;
  LDateTime.getTime(&t);
  LDateTime.getRtc(&rtc);

  //20171022-03:14:08
  static char dtStr[18] = {0};
  sprintf(dtStr, "%04d%02d%02d-%02d:%02d:%02d",
    t.year, t.mon, t.day, t.hour, t.min, t.sec);
  Serial.print("Current Time:[");   //print without line change.
  Serial.print(dtStr);
  Serial.println("]");

  return dtStr;
}

void initSd() {
  LSD.begin();
}

boolean setupDir(char* pathStr) {
  if (false == LSD.exists(pathStr)) {
    if (false == LSD.mkdir(pathStr)) {
      Serial.print("Create directory ");
      Serial.print(pathStr);
      Serial.println("Failed");

      return false;
    }

    return true;
  }

  return true;
}

int writeBuf(char* filenameStr, uint8_t* buf, size_t len) {
  LFile f = LSD.open(filenameStr, FILE_WRITE);
  if (false == f) {
    Serial.print("Open file ");
    Serial.print(filenameStr);
    Serial.println("Failed");

    return -1;
  }

  if (true == f.isDirectory()) {
    Serial.print("File ");
    Serial.print(f.name());
    Serial.println(" is Directory");

    return -1;
  }

  if (0 < f.size()) {
    f.seek(f.size());
  }

  int i = 0;
  for (i = 0; i < len; i++) {
    f.write((uint8_t)buf[i]);
  }

  Serial.print("File size:");
  Serial.println(f.size());
  
  f.flush();
  f.close();
  return len;  
}

void loop() {
  char* timeStr = makeDatetimeStr();
  writeBuf("number.txt", (uint8_t*)timeStr, strlen(timeStr));
  writeBuf("number.txt", (uint8_t*)":0123456789\n", 12);

  delay(200);
  ledOn();
  delay(800);
  ledOff();
}
