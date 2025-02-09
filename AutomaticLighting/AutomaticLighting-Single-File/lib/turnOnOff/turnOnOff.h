
#ifndef AUTOLIGHT_TURNONOFF_H
#define AUTOLIGHT_TURNONOFF_H

typedef void (*voidAction)(void);

/**
 * 红外感应控制灯熄亮模块初始化
 */
void turnOnOffInit(voidAction interruptAction);

void turnOnOffLoop();
#endif