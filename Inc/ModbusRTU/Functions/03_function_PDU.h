#pragma once

#include "ModbusRTU/Functions/message_ADU.h"

#define __MODBUS_RTU_FUNCTION_03_MAX_REGISTERS_COUNT                                                           \
   ((__MODBUS_RTU_MAX_ADU_SIZE - (__MODBUS_RTU_ADU_COMMON_DATA_SIZE + sizeof(ModbusRTU_Function03_ReplyData))) \
            / sizeof(ModbusRTU_Register))

typedef struct __attribute__((scalar_storage_order("big-endian"), packed))
{
   uint16_t DataRegisterAddress;
   uint16_t RegistersToReadCount;
} ModbusRTU_Function03_RequestData;

typedef struct __attribute__((scalar_storage_order("big-endian"), packed))
{
   uint8_t FollowingDataBytes;
   ModbusRTU_Register RegistersBuffer[];
} ModbusRTU_Function03_ReplyData;

ModbusRTU_Status ModbusRTU_Process03FunctionRequest(__SDEVICE_HANDLE(ModbusRTU) *,
                                                    ModbusRTU_RequestProcessingCommonData *,
                                                    size_t *);
