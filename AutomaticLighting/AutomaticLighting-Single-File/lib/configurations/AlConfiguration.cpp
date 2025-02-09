#include <AlConfiguration.h>

AlConfiguration::AlConfiguration(char *ssid,
                                 char *pwd,
                                 int LED_OUTPUT,
                                 int INFRARED_INTPUT,
                                 char *serverUrl,
                                 int webServerPort)
{
    this->ssid = ssid;
    this->pwd = pwd;
    this->LED_OUTPUT = LED_OUTPUT;
    this->INFRARED_INTPUT = INFRARED_INTPUT;
    this->serverUrl = serverUrl;
    this->webServerPort = webServerPort;
}

AlConfiguration::~AlConfiguration()
{
}