#include <Arduino.h>
#include "WiFi.h"
#include <WebServer.h>

#define LED_OUTPUT 15      // 出口给 Led 灯输作为正极 VCC
#define INFRARED_INTPUT 14 // 人体红外感应器，感应输出到IO14口
#define PILOT_LAMP 33      // 内置红色指示灯

const char *ssid = "zm";
const char *pwd = "13647190098";

const char *httpUrl = "http://localhost:5295/api/al/log";
// web服务
WebServer server(80);

bool isSendHttp = false;
String digitalReadToString(uint8_t pin);
void handleRootPath(void);
void httpSend(void);
void infraredInterrupt();

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
  digitalWrite(LED_OUTPUT, LOW);

  // 人体红外感应器
  // 测试
  pinMode(INFRARED_INTPUT, INPUT);
  digitalWrite(INFRARED_INTPUT, LOW);

  server.on("/", handleRootPath);
  server.on("", handleRootPath);
  server.onNotFound(handleRootPath);
  server.begin();

  Serial.println("WebServer start");
  Serial.print(WiFi.localIP());
}

void loop()
{
  server.handleClient();
  int state = digitalRead(INFRARED_INTPUT);
  if (state == 1)
  {
    digitalWrite(LED_OUTPUT, HIGH);
  }
  else
  {
    digitalWrite(LED_OUTPUT, LOW);
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
  message += "LED灯 state: " + IO15 + "<br/>";

  server.send(200, "text/html; charset=utf-8", message);
}
