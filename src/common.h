#ifndef SENSOR_BOARD_COMMON_H
#define SENSOR_BOARD_COMMON_H

#ifndef SOURCE_FILE
#define SOURCE_FILE "no Source file defined!"
#endif

#include "stdio.h"

#define PRINT(str, ...)                                                                                      \
    {                                                                                                        \
        printf("[%s: %s] ", SOURCE_FILE, __FUNCTION__);                                                       \
        printf(str, ##__VA_ARGS__);                                                                          \
        printf("\n");                                                                                        \
    }

#define ASSERT(test)                                                                                    \
    if (!(test))                                                                                        \
        return 0;

#endif //SENSOR_BOARD_COMMON_H
