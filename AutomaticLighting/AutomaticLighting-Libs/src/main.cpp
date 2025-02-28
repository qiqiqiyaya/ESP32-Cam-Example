#include <Arduino.h>
#include "WiFi.h"
#include <WebServer.h>
#include <HTTPClient.h>
#include "cJSON.h"

#define LED_OUTPUT 15      // 出口给 Led 灯输作为正极 VCC
#define INFRARED_INTPUT 14 // 人体红外感应器，感应输出到IO14口
#define PILOT_LAMP 33      // 内置红色指示灯

const char *ssid = "zm";
const char *pwd = "13647190098";

const char *httpUrl = "http://localhost:5295/api/al/log";
// web服务
WebServer server(80);

// 红外感应状态
int InfraredState = 0;
bool isSendHttp = false;
String digitalReadToString(uint8_t pin);
void handleRootPath(void);
void httpSend(void);
void infraredInterrupt();

bool mark = 0;

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("wifi connected");

  // 内置红色指示灯 亮
  pinMode(PILOT_LAMP, OUTPUT);
  digitalWrite(PILOT_LAMP, LOW);

  // Led 灯
  pinMode(LED_OUTPUT, OUTPUT);
  digitalWrite(LED_OUTPUT, HIGH);

  // 人体红外感应器
  // 测试
  // pinMode(INFRARED_INTPUT, INPUT);
  pinMode(INFRARED_INTPUT, OUTPUT);
  digitalWrite(INFRARED_INTPUT, LOW);

  server.on("/", handleRootPath);
  server.on("", handleRootPath);
  server.onNotFound(handleRootPath);
  server.begin();

  Serial.println("WebServer start");
  Serial.print(WiFi.localIP());

  attachInterrupt(digitalPinToInterrupt(INFRARED_INTPUT), infraredInterrupt, CHANGE); // 使能中断
}

void infraredInterrupt()
{
  InfraredState = digitalRead(INFRARED_INTPUT);
  digitalWrite(LED_OUTPUT, InfraredState);

  int st = digitalRead(PILOT_LAMP);
  if (st == 1)
  {
    digitalWrite(PILOT_LAMP, LOW);
  }
  else
  {
    digitalWrite(PILOT_LAMP, HIGH);
  }
}

void loop()
{
  server.handleClient();
  // 红外
  // InfraredState = digitalRead(INFRARED_INTPUT);
  // digitalWrite(LED_OUTPUT, InfraredState);
  // if (InfraredState == 1 && !isSendHttp)
  // {
  //   // httpSend();
  //   isSendHttp = true;
  // }
  // else
  // {
  //   isSendHttp = false;
  // }
  // 每20秒触发一次
  delay(1000 * 20);
  String test = digitalReadToString(INFRARED_INTPUT);
  InfraredState = digitalRead(INFRARED_INTPUT);
  Serial.println("InfraredState " + test);
  if (InfraredState == 1)
  {
    digitalWrite(INFRARED_INTPUT, LOW);
  }
  else
  {
    digitalWrite(INFRARED_INTPUT, HIGH);
  }
}

String digitalReadToString(uint8_t pin)
{
  int value = digitalRead(pin);
  if (value == 1)
  {
    return "HIGH";
  }
  if (value == 0)
  {
    return "LOW";
  }

  return "";
}

void handleRootPath(void)
{
  String IO14 = digitalReadToString(INFRARED_INTPUT);
  String IO15 = digitalReadToString(LED_OUTPUT);

  String message = "Server is running! <br/>";
  message += "红外 state: " + IO14 + "<br/>";
  message += "LED灯 state: " + IO15;

  server.send(200, "text/html; charset=utf-8", message);
}
