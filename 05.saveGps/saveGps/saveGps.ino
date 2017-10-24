#include <LGPS.h>
#include <LStorage.h>
#include <LSD.h>


#define LED_GPIO_PIN  13

void setup() {
  // initialize LED pin as an output.
  pinMode(LED_GPIO_PIN, OUTPUT);

  //Setup Serial Port.
  Serial.begin(9600);
  Serial.println("Linkit ONE Board"); //print with line change.

  gpsPowerOn();
  initSd();
}

void ledOn() {
  digitalWrite(LED_GPIO_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void ledOff() {
  digitalWrite(LED_GPIO_PIN, LOW);    // turn the LED off by making the voltage LOW
}

void gpsPowerOn() {
  LGPS.powerOn();
}

void gpsPowerOff() {
  LGPS.powerOff();
}

void gpsGetData() {
  gpsSentenceInfoStruct info;
  LGPS.getData(&info);
  Serial.print((char*)info.GPGGA);
  Serial.print((char*)info.GPGSA);
  Serial.print((char*)info.GPRMC);
  Serial.print((char*)info.GPVTG);
  Serial.print((char*)info.GPGSV);
  Serial.print((char*)info.GLGSV);
  Serial.print((char*)info.BDGSV);
  Serial.print((char*)info.BDGSA);
  Serial.println(" ");

  writeBuf("gpsdata.txt", (uint8_t*)((char*)info.GPGGA), strlen((char*)info.GPGGA));
  writeBuf("gpsdata.txt", (uint8_t*)((char*)info.GPGSA), strlen((char*)info.GPGSA));
  writeBuf("gpsdata.txt", (uint8_t*)((char*)info.GPRMC), strlen((char*)info.GPRMC));
  writeBuf("gpsdata.txt", (uint8_t*)((char*)info.GPVTG), strlen((char*)info.GPVTG));
  writeBuf("gpsdata.txt", (uint8_t*)((char*)info.GPGSV), strlen((char*)info.GPGSV));
  writeBuf("gpsdata.txt", (uint8_t*)((char*)info.GLGSV), strlen((char*)info.GLGSV));
  writeBuf("gpsdata.txt", (uint8_t*)((char*)info.BDGSV), strlen((char*)info.BDGSV));
  writeBuf("gpsdata.txt", (uint8_t*)((char*)info.BDGSA), strlen((char*)info.BDGSA));
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
  gpsGetData();

  delay(200);
  ledOn();
  delay(800);
  ledOff();
}
