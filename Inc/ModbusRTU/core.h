#pragma once

#include "SDevice/sdevice_interface.h"

#include <stdint.h>
#include <stddef.h>

#define __MODBUS_RTU_MAX_ADU_SIZE 0xFF

typedef enum
{
   MODBUS_RTU_STATUS_OK = 0x00,

   MODBUS_RTU_STATUS_ILLEGAL_FUNCTION_ERROR = 0x01,
   MODBUS_RTU_STATUS_ILLEGAL_ADDRESS_ERROR = 0x02,
   MODBUS_RTU_STATUS_ILLEGAL_DATA_ERROR = 0x03,
   MODBUS_RTU_STATUS_SLAVE_DEVICE_FAILURE_ERROR = 0x04,
   MODBUS_RTU_STATUS_ACKNOWLEDGE = 0x05,
   MODBUS_RTU_STATUS_SLAVE_DEVICE_BUSY_ERROR = 0x06,
   MODBUS_RTU_STATUS_NEGATIVE_ACKNOWLEDGE = 0x07,
   MODBUS_RTU_STATUS_MEMORY_PARITY_ERROR = 0x08,
   MODBUS_RTU_STATUS_GATEWAY_PATH_UNAVAILABLE_ERROR = 0x10,
   MODBUS_RTU_STATUS_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND_ERROR = 0x11,

   MODBUS_RTU_STATUS_NON_MODBUS_ERROR = 0xFF
} ModbusRTU_Status;

typedef enum
{
   MODBUS_RTU_MEMORY_OPERATION_STATUS_OK = MODBUS_RTU_STATUS_OK,
   MODBUS_RTU_MEMORY_OPERATION_STATUS_DATA_ERROR = MODBUS_RTU_STATUS_ILLEGAL_DATA_ERROR,
   MODBUS_RTU_MEMORY_OPERATION_STATUS_DEVICE_ERROR = MODBUS_RTU_STATUS_SLAVE_DEVICE_FAILURE_ERROR
} ModbusRTU_MemoryOperationStatus;

typedef enum
{
   MODBUS_RTU_REQUEST_TYPE_NORMAL,
   MODBUS_RTU_REQUEST_TYPE_BROADCAST
} ModbusRTU_RequestType;

typedef union __attribute__((packed, scalar_storage_order("big-endian")))
{
   uint16_t AsValue;
   uint8_t AsBytes[sizeof(uint16_t)];
} ModbusRTU_Register;

typedef struct
{
   ModbusRTU_Register *Registers;
   uint16_t Address;
   size_t RegistersCount;
} ReadRegistersParameters;

typedef struct
{
   const ModbusRTU_Register *Registers;
   uint16_t Address;
   size_t RegistersCount;
} WriteRegistersParameters;

/* Satty's interface start */

__SDEVICE_HANDLE_FORWARD_DEFINITION(ModbusRTU);

typedef struct
{
   void *UserData;
   ModbusRTU_MemoryOperationStatus (*ReadRegistersFunction)(__SDEVICE_HANDLE(ModbusRTU) *,
                                                            ModbusRTU_RequestType,
                                                            const ReadRegistersParameters *);
   ModbusRTU_MemoryOperationStatus (*WriteRegistersFunction)(__SDEVICE_HANDLE(ModbusRTU) *,
                                                             ModbusRTU_RequestType,
                                                             const WriteRegistersParameters *);
   void *ReceiveBuffer;
   void *TransmitBuffer;
} __SDEVICE_CONSTANT_DATA(ModbusRTU);

typedef struct
{
   uint8_t SlaveAddress;
} __SDEVICE_SETTINGS_DATA(ModbusRTU);

typedef struct
{
   void *RequestFunctionSpecificData;
   void *ReplyFunctionSpecificData;
} __SDEVICE_DYNAMIC_DATA(ModbusRTU);

__SDEVICE_HANDLE_DEFINITION(ModbusRTU);

__SDEVICE_INITIALIZE_DYNAMIC_DATA_DECLARATION(ModbusRTU,);

__SDEVICE_SET_SETTING_DECLARATION(ModbusRTU, SlaveAddress,,);

/* Satty's interface end */

size_t ModbusRTU_ProcessRequest(__SDEVICE_HANDLE(ModbusRTU)*, size_t);
