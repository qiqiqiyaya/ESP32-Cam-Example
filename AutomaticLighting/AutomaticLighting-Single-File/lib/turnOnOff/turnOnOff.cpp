#include <Arduino.h>
#include <AlConfiguration.h>
#include <turnOnOff.h>

// void callback();
/**
 * 红外感应触发回调
 */
voidAction _interruptAction;

void turnOnOffInit(voidAction interruptAction)
{
    _interruptAction = interruptAction;
    // LED 针脚初始设置
    pinMode(AlConfig.LED_OUTPUT, OUTPUT);
    digitalWrite(AlConfig.LED_OUTPUT, HIGH);

    // 人体红外感应器 针脚初始设置
    pinMode(AlConfig.INFRARED_INTPUT, INPUT);
    digitalWrite(AlConfig.INFRARED_INTPUT, LOW);

    // 注册，中断功能
    // attachInterrupt(AlConfig.INFRARED_INTPUT, callback, CHANGE);
    Serial.println("LED init completed.");
}

void turnOnOffLoop()
{
    // 读取红外感应状态
    int value = digitalRead(AlConfig.INFRARED_INTPUT);
    // Serial.println("红外感应触发: " + String(value));
    if (value == 1)
    {
        AlConfig.IsLighting = true;

        // 写入LED
        digitalWrite(AlConfig.LED_OUTPUT, value);
        _interruptAction();
    }
    else
    {
        AlConfig.IsLighting = false;
    }
}
