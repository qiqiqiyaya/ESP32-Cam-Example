#include <Arduino.h>
#include <sync_time.h>
#include <HTTPClient.h>
#include "cJSON.h"
#include <AlConfiguration.h>
#include <httpService.h>

String toString(uint8_t pin);
void handleRootPath();

HttpService::HttpService()
{
}

HttpService::~HttpService()
{
}

void handleRootPath()
{
    Serial.printf("handleRootPath invoke");
    String IO14 = toString(AlConfig.INFRARED_INTPUT);
    String IO15 = toString(AlConfig.LED_OUTPUT);

    time_t now;
    time(&now);
    printLocalTime();

    String message = "Server is running! <br/>";
    message += "红外 state: " + IO14 + "<br/>";
    message += "LED灯 state: " + IO15 + "<br/>";
    message += "time: " + String(now);

    // Serial.printf(message.c_str());
    HttpServer.send(200, "text/html; charset=utf-8", message);
}

void HttpService::Init()
{
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    WiFi.begin(AlConfig.ssid, AlConfig.pwd);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("wifi connected");
    Serial.println(WiFi.localIP());

    HttpServer.on("/", handleRootPath);
    HttpServer.onNotFound(handleRootPath);
    HttpServer.begin();
    Serial.println("HttpServer start");
}

String toString(uint8_t pin)
{
    int value = digitalRead(pin);
    if (value == 1)
        return "HIGH";
    if (value == 0)
        return "LOW";
    return "";
}

void HttpService::Send()
{
    Serial.println("Start http send");
    HTTPClient client;
    client.begin(AlConfig.serverUrl);
    // 超时时间10s
    client.setTimeout(10);

    time_t now;
    time(&now);

    cJSON *pRoot = cJSON_CreateObject();
    cJSON_AddNumberToObject(pRoot, "Tsp", now);
    cJSON_AddStringToObject(pRoot, "Pst", "门口");
    cJSON_AddStringToObject(pRoot, "Dn", "Esp32-cam");
    cJSON_AddNumberToObject(pRoot, "Te", AlConfig.IsLighting);
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

    client.end();                 // 结束当前连接
    cJSON_free((void *)sendData); // 释放cJSON_Print ()分配出来的内存空间
    cJSON_Delete(pRoot);          // 释放cJSON_CreateObject ()分配出来的内存空间
    delete[] sendData;
    // delete httpCode; // 释放内存
    // delete client;

    Serial.println("send http post end");
}