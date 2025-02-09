
#ifndef AL_CONFIGURATION_H
#define AL_CONFIGURATION_H

class AlConfiguration
{
private:
    /* data */
public:
    /**
     * 服务器地址
     */
    char *serverUrl;
    /* wifi 账号 */
    char *ssid;
    /* wifi 密码 */
    char *pwd;
    /* 自带web服务端口，默认值80 */
    int webServerPort;
    /* 出口给 Led 灯输作为正极 VCC */
    int LED_OUTPUT;
    /* 人体红外感应器，感应输出 */
    int INFRARED_INTPUT;
    /* 灯是否亮 */
    bool IsLighting = false;

    AlConfiguration(char *ssid,
                    char *pwd,
                    int LED_OUTPUT,
                    int INFRARED_INTPUT,
                    char *serverUrl,
                    int webServerPort);
    ~AlConfiguration();
};

/* 自动亮灯全局配置 */
static AlConfiguration AlConfig((char *)"zm", (char *)"13647190098", 15, 14, (char *)"http://localhost:5000/", 80);

#endif