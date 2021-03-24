#pragma once

#include "ModbusRTU/core.h"
#include "ModbusRTU/Utils/CRC16.h"

#define __MODBUS_RTU_BROADCAST_REQUEST_SLAVE_ADDRESS 0x00
#define __MODBUS_RTU_ADU_COMMON_DATA_SIZE sizeof(__MODBUS_RTU_ADU_STRUCT_DECLARATION(0,))
#define __MODBUS_RTU_ADU_FUNCTION_SPECIFIC_DATA_OFFSET (__MODBUS_RTU_ADU_COMMON_DATA_SIZE - __MODBUS_RTU_CRC_SIZE)

#define __MODBUS_RTU_ADU_STRUCT_DECLARATION(function_specific_size, name) \
    struct __attribute__((packed)) name                                   \
    {                                                                     \
        uint8_t SlaveAddress;                                             \
        uint8_t FunctionCode;                                             \
        uint8_t FunctionSpecificData[(function_specific_size)];           \
        uint16_t CRC16;                                                   \
    }

#define __IS_IMPLEMENTED_MODBUS_RTU_FUNCTION_CODE(code) ({                \
    __auto_type _code = (code);                                           \
    _code == MODBUS_RTU_FUNCTION_READ_HOLDING_REGISTERS     ||            \
    _code == MODBUS_RTU_FUNCTION_PRESET_MULTIPLE_REGISTERS;               \
})

/* function codes that are implemented */
typedef enum
{
   MODBUS_RTU_FUNCTION_READ_HOLDING_REGISTERS = 0x03,
   MODBUS_RTU_FUNCTION_PRESET_MULTIPLE_REGISTERS = 0x10
} ModbusRTU_FunctionCode;

typedef struct
{
   size_t FunctionSpecificDataSize;
   ModbusRTU_FunctionCode FunctionCode;
   ModbusRTU_RequestType RequestType;
} ModbusRTU_RequestProcessingCommonData;

ModbusRTU_Status ModbusRTU_DecodeADU(__SDEVICE_HANDLE(ModbusRTU)*,
                                     size_t,
                                     ModbusRTU_RequestProcessingCommonData*);

size_t ModbusRTU_EncodeADU(__SDEVICE_HANDLE(ModbusRTU)*,
                           const ModbusRTU_RequestProcessingCommonData*,
                           size_t);

size_t ModbusRTU_EncodeExceptionADU(__SDEVICE_HANDLE(ModbusRTU)*,
                                    const ModbusRTU_RequestProcessingCommonData*,
                                    ModbusRTU_Status);
