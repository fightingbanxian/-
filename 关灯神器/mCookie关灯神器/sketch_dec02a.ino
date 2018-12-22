#include <Servo.h>
Servo myservo;
#define servo_pin SDA

#include <ESP8266.h>

#ifdef ESP32
#error "This code is not recommended to run on the ESP32 platform! Please check your Tools->Board setting."
#endif

/**
**CoreUSB UART Port: [Serial1] [D0,D1]
**Core+ UART Port: [Serial1] [D2,D3]
**/
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define EspSerial Serial1
#define UARTSPEED  115200
#endif

/**
**Core UART Port: [SoftSerial] [D2,D3]
**/
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D2, TX:D3 */

#define EspSerial mySerial
#define UARTSPEED  9600
#endif

#define SSID        F("X")
#define PASSWORD    F("123456789")
#define HOST_NAME   F("api.heclouds.com")
#define HOST_PORT   (80)

static const byte  GETDATA[]  PROGMEM = {
  "GET https://api.heclouds.com/devices/505222423/datapoints?datastream_id=kaiguan&limit=1 HTTP/1.1\r\nHost:api.heclouds.com\r\napi-key:KPGddm68DGn35VA=p=Llg78AJE8=\r\nConnection: close\r\n\r\n"
};

ESP8266 wifi(&EspSerial);

void setup(void)
{

  myservo.attach(servo_pin);
   myservo.write(0);
  Serial.begin(9600);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  Serial.print(F("setup begin\r\n"));
  delay(100);

  WifiInit(EspSerial, UARTSPEED);

  Serial.print(F("FW Version:"));
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print(F("to station + softap ok\r\n"));
  } else {
    Serial.print(F("to station + softap err\r\n"));
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print(F("Join AP success\r\n"));

    Serial.print(F("IP:"));
    Serial.println( wifi.getLocalIP().c_str());
  } else {
    Serial.print(F("Join AP failure\r\n"));
  }

  if (wifi.disableMUX()) {
    Serial.print(F("single ok\r\n"));
  } else {
    Serial.print(F("single err\r\n"));
  }

  Serial.print(F("setup end\r\n"));
}

void loop(void)
{
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
    Serial.print(F("create tcp ok\r\n"));
  } else {
    Serial.print(F("create tcp err\r\n"));
  }

  //char *hello = "GET /testwifi/index.html HTTP/1.0\r\nHost: www.adafruit.com\r\nConnection: close\r\n\r\n";
  //wifi.send((const uint8_t*)hello, strlen(hello));  //直接发送

  wifi.sendFromFlash(GETDATA, sizeof(GETDATA)); //从Flash读取发送内容，节约内存
char a;
  uint8_t buffer[512] = {0};
  uint32_t len = wifi.recv(buffer, sizeof(buffer), 20000);
  if (len > 0) {
    Serial.print(F("Received:["));
    for (uint32_t i = 0; i < len; i++) {
      Serial.print((char)buffer[i]);
    }
    Serial.print(F("]\r\n"));
  
     for (uint32_t i = 0; i < len-12; i++) {
      if((char)buffer[i]=='v'&&(char)buffer[i+1]=='a'&&(char)buffer[i+2]=='l'&&(char)buffer[i+3]=='u'&&(char)buffer[i+4]=='e')
      {
        
        a=buffer[i+8];
      }
      }
    Serial.print(F("]\r\n"));
  }

  if (wifi.releaseTCP()) {
    Serial.print(F("release tcp ok\r\n"));
  } else {
    Serial.print(F("release tcp err\r\n"));
  }
 Serial.println(a);
//控制空调
  if (a== '1')
  {
    Serial.println(a);
myservo.write(0);
  delay(1000);
  myservo.write(160);//舵机有损，所以把数值调大，望注意修改
  delay(1000);
  myservo.write(0);
  delay(1000);
  }
  else
  { 
    myservo.write(0);
  delay(1000);
  
  }

  delay(1000);
}
