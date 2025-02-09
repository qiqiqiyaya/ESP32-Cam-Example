#include <Arduino.h>
#include <sync_time.h>

// NTP配置参数, 使用阿里云的NTP服务器
const char *ntpServer1 = "ntp1.aliyun.com";
const char *ntpServer2 = "ntp2.aliyun.com";
const char *ntpServer3 = "ntp3.aliyun.com";
const long gmtOffset_sec = 8 * 3600; // 时区偏移量，北京是GMT+8
const int daylightOffset_sec = 0;    // 夏令时偏移量，中国无夏令时

/**
 * 时间初始化
 * 需要连接 wifi
 */
void timeInit()
{
    // 配置NTP服务器，开始同步时间
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2, ntpServer3);
    Serial.println("Time config completed.");
}

/**
 * 打印本地时间
 */
void printLocalTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }

    // 格式化并打印时间
    Serial.println(&timeinfo, "%Y-%m-%d %H-%M-%S");
}