#pragma once

#include <math.h>

#define __MODBUS_RTU_INTERCHAR_DELAY_CHARS 1.5f
#define __MODBUS_RTU_INTERFRAME_DELAY_CHARS 3.5f

#define __MODBUS_RTU_PREDEFINED_DELAYS_USAGE_THERESHOLD_BAUD 19200
#define __MODBUS_RTU_INTERCHAR_DELAY_PREDEFINED 0.750e-3f
#define __MODBUS_RTU_INTERFRAME_DELAY_PREDEFINED 1.750e-3f

#define __MODBUS_RTU_CHARS_TO_SECONDS_DELAY(baud, chars, char_size) (((char_size) * (chars)) / (baud))

#define __MODBUS_RTU_GET_INTERCHAR_DELAY(baud, char_size)                                                         \
    (((baud) > __MODBUS_RTU_PREDEFINED_DELAYS_USAGE_THERESHOLD_BAUD) ?                                            \
            __MODBUS_RTU_INTERCHAR_DELAY_PREDEFINED :                                                             \
            __MODBUS_RTU_CHARS_TO_SECONDS_DELAY(baud, __MODBUS_RTU_INTERCHAR_DELAY_CHARS, char_size))
#define __MODBUS_RTU_GET_INTERFRAME_DELAY(baud, char_size)                                                        \
    (((baud) > __MODBUS_RTU_PREDEFINED_DELAYS_USAGE_THERESHOLD_BAUD) ?                                            \
            __MODBUS_RTU_INTERFRAME_DELAY_PREDEFINED :                                                            \
            __MODBUS_RTU_CHARS_TO_SECONDS_DELAY(baud, __MODBUS_RTU_INTERFRAME_DELAY_CHARS, char_size))

#define __MODBUS_RTU_TIMER_PERIOD 1e-6f

/* gcc's ceilf is compiler intrinsic, so we can use it as constant value */
#define __MODBUS_RTU_DELAY_TO_TIMER_PERIOD(delay) (__builtin_ceilf((delay) / __MODBUS_RTU_TIMER_PERIOD) - 1.f)
#define __MODBUS_RTU_GET_TIMER_PRESCALER(clock) (__builtin_ceilf((clock) * __MODBUS_RTU_TIMER_PERIOD) - 1.f)
