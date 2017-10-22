#include <string.h>

#include <LGPS.h>

#define LED_GPIO_PIN  13

struct gps_info {
  long longitude;
  long latitude;
  int  moveDirection;
  int  speed;
  int  satNum;
};

void setup() {
  // initialize LED pin as an output.
  pinMode(LED_GPIO_PIN, OUTPUT);

  //Setup Serial Port.
  Serial.begin(9600);
  Serial.println("Linkit ONE Board"); //print with line change.

  gpsPowerOn();
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

  parseGPRMC((char*)info.GPRMC);
}

/*RPRMC(Recommended Minimum Specific GPS/TRANSIT Data)
  Format:$GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*hh<CR><LF>
  字段 0：$GPRMC，语句ID，表明该语句为Recommended Minimum Specific GPS/TRANSIT Data（RMC）推荐最小定位信息
  字段 1：UTC时间，hhmmss.sss格式
  字段 2：状态，A=定位，V=未定位
  字段 3：纬度ddmm.mmmm，度分格式（前导位数不足则补0）
  字段 4：纬度N（北纬）或S（南纬）
  字段 5：经度dddmm.mmmm，度分格式（前导位数不足则补0）
  字段 6：经度E（东经）或W（西经）
  字段 7：速度，节，Knots（一节也是1.852千米／小时）
  字段 8：方位角，度（二维方向指向，相当于二维罗盘）
  字段 9：UTC日期，DDMMYY格式
  字段10：磁偏角，（000 - 180）度（前导位数不足则补0）
  字段11：磁偏角方向，E=东，W=西
  字段12：模式，A=自动，D=差分，E=估测，N=数据无效（3.0协议内容）
  字段13：校验值
*/
void parseGPRMC(char* gprmcStr) {
  char* p = NULL;
  char* pData = gprmcStr;
  char* spliter = ",";
  int  spliterPos[12];
  char* headStr = "$GPRMC";

  //
  p = strstr(gprmcStr, headStr);
  if (NULL == p) {
    Serial.println("No $GPRMC");
    return;
  }

  //
  int i = 0;
  for( i = 0; i < 12; i++) {
    p = strstr(pData, spliter);
    spliterPos[i] = p - gprmcStr;
    pData = p + 1;
    //Serial.println(p);
  }

  //01.UTC Time.
  char utcTime[16] = {0};
  memcpy(utcTime, gprmcStr + 1 + spliterPos[0], spliterPos[1] - spliterPos[0] - 1);
  Serial.print("utctime:");
  Serial.println(utcTime);

  //02.Locate Status
  char gpsAvailable = 'V';
  if (0 == strncmp(gprmcStr + 1 + spliterPos[1], "A", 1)) {
    gpsAvailable = 'A';
  }

  Serial.print("gpsAvailable:");
  Serial.println(gpsAvailable);

  //03~04. GPS Latitude
  char gpsLat[16] = {0};
  
  memcpy(gpsLat, gprmcStr + 1 + spliterPos[2], spliterPos[3] - spliterPos[2] - 1);
  if (0 == strncmp(gprmcStr + 1 + spliterPos[3], "N", 1)) {
    Serial.println("N");
  } else {
    Serial.println("S");
  }
  Serial.print("Latitude:");
  Serial.println(gpsLat);

  //05~06. GPS Longitude
  char gpsLon[16] = {0};

  memcpy(gpsLon, gprmcStr + 1 + spliterPos[4], spliterPos[5] - spliterPos[4] - 1);
  if (0 == strncmp(gprmcStr + 1 + spliterPos[5], "E", 1)) {
    Serial.println("E");
  } else {
    Serial.println("W");
  }
  Serial.print("Longitude:");
  Serial.println(gpsLon);

  //07. Speed
  char gpsSpd[16] = {0};
  memcpy(gpsSpd, gprmcStr + 1 + spliterPos[6], spliterPos[7] - spliterPos[6] - 1);
  Serial.print("Speed:");
  Serial.println(gpsSpd);
  

  //08. Direction
  char gpsDir[16] = {0};
  memcpy(gpsDir, gprmcStr + 1 + spliterPos[7], spliterPos[8] - spliterPos[7] - 1);
  Serial.print("Direction:");
  Serial.println(gpsDir);

  //09. UTC date
  char gpsDate[16] = {0};
  memcpy(gpsDate, gprmcStr + 1 + spliterPos[8], spliterPos[9] - spliterPos[8] - 1);
  Serial.print("UTC Date:");
  Serial.println(gpsDate);
  Serial.println(" ");
}

void loop() {
  gpsGetData();

  delay(200);
  ledOn();
  delay(800);
  ledOff();
}

