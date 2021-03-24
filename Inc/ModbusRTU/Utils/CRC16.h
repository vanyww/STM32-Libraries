#pragma once

#include <stdint.h>
#include <stddef.h>

#define __MODBUS_RTU_CRC_SIZE sizeof(uint16_t)

uint16_t ModbusRTU_CRC16(void *, size_t);
