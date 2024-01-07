#define BLINKER_PRINT Serial
#define BLINKER_MIOT_OUTLET
#define BLINKER_WIFI
#include <Blinker.h>
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>

//输入设备密钥，WIFI名称及密码
char auth[] = "****";
char ssid[] = "****";
char pswd[] = "****";

//输入IP地址，用于判断电脑是否开机，IP段使用,号分割
IPAddress  ip(192,168,2,120);

volatile int press_long;
volatile int press_short;
volatile boolean online;
String state;
BlinkerButton Button1("power-btn");

void Button1_callback(const String & state) {
  Serial.println(String(state));
  if (state == "tap") {
    digitalWrite(2,LOW);
    Button1.color("#009900");
    BUILTIN_SWITCH.print("on");
    Button1.print("on");
    delay(press_short);
    digitalWrite(2,HIGH);
  } else if (state == "pressup") {
    Button1.color("#666666");
    BUILTIN_SWITCH.print("off");
    Button1.print("off");
    digitalWrite(2,LOW);
    delay(press_long);
    digitalWrite(2,HIGH);
  }
}

void miotPowerStatestate(const String & state){
  Serial.println(String(state));
  if (state == BLINKER_CMD_ON) {
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();
    digitalWrite(2,LOW);
    Button1.color("#009900");
    Button1.print("on");
    BUILTIN_SWITCH.print("on");
    delay(press_short);
    digitalWrite(2,HIGH);
  } else if (state == BLINKER_CMD_OFF) {
    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();
    Button1.color("#666666");
    Button1.print("off");
    digitalWrite(2,LOW);
    delay(press_short);
    digitalWrite(2,HIGH);
    BUILTIN_SWITCH.print("off");
  }
}

void heartbeat(){
  online = Ping.ping(ip,1);
  Serial.println(String(online));
  if (online == true) {
    BUILTIN_SWITCH.print("on");
    Button1.print("on");
    Button1.color("#009900");

  } else if (online == false) {
    BUILTIN_SWITCH.print("off");
    Button1.print("off");
    Button1.color("#666666");
  }
}

void switch_callback(const String & state){
  Serial.println(String(state));
  if (state == "on" || state == "off") {
    if (online == true) {
      Button1.color("#666666");
      BUILTIN_SWITCH.print("off");

    } else if (online == false) {
      Button1.color("#009900");
      BUILTIN_SWITCH.print("on");
    }
    digitalWrite(2,LOW);
    delay(press_short);
    digitalWrite(2,HIGH);
  }
}

void setup(){
   BLINKER_DEBUG.stream(Serial);
  press_long = 6000;
  press_short = 800;
  online = false;
  state = "";
  pinMode(2, OUTPUT);
  Blinker.begin(auth,ssid,pswd);
  Serial.begin(9600);
  Button1.attach(Button1_callback);
  BlinkerMIOT.attachPowerState(miotPowerStatestate);
   digitalWrite(2,HIGH);
  online = Ping.ping(ip,1);
  Blinker.attachHeartbeat(heartbeat);
  BUILTIN_SWITCH.attach(switch_callback);
}

void loop(){
  Blinker.run();
}