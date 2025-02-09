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
  pinMode(INFRARED_INTPUT, INPUT);
  digitalWrite(INFRARED_INTPUT, LOW);

  server.on("/", handleRootPath);
  server.onNotFound(handleRootPath);
  server.begin();

  Serial.println("WebServer start");
  Serial.print(WiFi.localIP());
}

void loop()
{
  server.handleClient();
  // 红外
  InfraredState = digitalRead(INFRARED_INTPUT);
  digitalWrite(LED_OUTPUT, InfraredState);
  if (InfraredState == 1 && !isSendHttp)
  {
    // httpSend();
    isSendHttp = true;
  }
  else
  {
    isSendHttp = false;
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

void httpSend()
{
  Serial.println("Start http send");
  HTTPClient client;
  client.begin(httpUrl);
  // 超时时间10s
  client.setTimeout(10);

  time_t now;
  time(&now);

  cJSON *pRoot = cJSON_CreateObject();
  cJSON_AddNumberToObject(pRoot, "Tsp", now);
  cJSON_AddStringToObject(pRoot, "Pst", "门口");
  cJSON_AddStringToObject(pRoot, "Dn", "Esp32-cam");
  cJSON_AddNumberToObject(pRoot, "Te", InfraredState);
  char *sendData = cJSON_Print(pRoot);
  int httpCode = client.POST(sendData);

  while (client.connected())
  {
  }
  if (httpCode > 0)
  {
    Serial.println("send http post httpCode: " + String(httpCode));
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = client.getString(); // 读取服务器返回的响应正文数据
      // 如果正文数据很多该方法会占用很大的内存
      Serial.println(payload);
    }
  }
  else
  {
    Serial.printf("[HTTP] POST... failed, error: %s\n", client.errorToString(httpCode).c_str());
  }

  client.end(); // 结束当前连接
  // cJSON_free((void *)sendData); // 释放cJSON_Print ()分配出来的内存空间
  cJSON_Delete(pRoot); // 释放cJSON_CreateObject ()分配出来的内存空间
  // delete[] sendData;
  // delete httpCode; // 释放内存
  // delete client;

  Serial.println("send http post end");
}
