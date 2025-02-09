#include <Arduino.h>
#include "WiFi.h"
#include <WebServer.h>
#include <sync_time.h>
#include <turnOnOff.h>
#include <AlConfiguration.h>
#include <httpService.h>

/* http 服务 */
HttpService http;
void turnOnOffCallBack()
{
  http.Send();
}

void setup()
{
  Serial.begin(115200);

  http.Init();
  timeInit();
  // printLocalTime();
  turnOnOffInit(turnOnOffCallBack);
}

void loop()
{
  HttpServer.handleClient();
  // turnOnOffLoop();
}
