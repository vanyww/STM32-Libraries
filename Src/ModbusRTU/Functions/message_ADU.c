#include "ModbusRTU/Functions/message_ADU.h"
#include "ModbusRTU/Functions/03_function_PDU.h"
#include "ModbusRTU/Functions/16_function_PDU.h"

#define __MODBUS_RTU_EXCEPTION_FLAG 0x80
#define __MODBUS_RTU_EXCEPTION_CODE_SIZE sizeof(uint8_t)
#define __MODBUS_RTU_MIN_REQUEST_ADU_SIZE                                                       \
   sizeof(__MODBUS_RTU_ADU_STRUCT_DECLARATION(__MIN(sizeof(ModbusRTU_Function03_RequestData),   \
                                                    sizeof(ModbusRTU_Function16_RequestData)),))

ModbusRTU_Status ModbusRTU_DecodeADU(__SDEVICE_HANDLE(ModbusRTU) *handle,
                                     size_t bytesReceived,
                                     ModbusRTU_RequestProcessingCommonData *processingData)
{
   if(unlikely(bytesReceived < __MODBUS_RTU_MIN_REQUEST_ADU_SIZE))
      return MODBUS_RTU_STATUS_NON_MODBUS_ERROR;

   typedef __MODBUS_RTU_ADU_STRUCT_DECLARATION(bytesReceived - __MODBUS_RTU_ADU_COMMON_DATA_SIZE,) ModbusRTU_ADU_Data;
   ModbusRTU_ADU_Data *ADU = (ModbusRTU_ADU_Data*)handle->Constant->ReceiveBuffer;

   if(ADU->SlaveAddress != handle->Settings->SlaveAddress)
   {
      if(ADU->SlaveAddress != __MODBUS_RTU_BROADCAST_REQUEST_SLAVE_ADDRESS)
         return MODBUS_RTU_STATUS_NON_MODBUS_ERROR;

      processingData->RequestType = MODBUS_RTU_REQUEST_TYPE_BROADCAST;
   }
   else
   {
      processingData->RequestType = MODBUS_RTU_REQUEST_TYPE_NORMAL;
   }

   if(unlikely(ADU->CRC16 != ModbusRTU_CRC16(ADU, sizeof(ModbusRTU_ADU_Data) - __MODBUS_RTU_CRC_SIZE)))
      return MODBUS_RTU_STATUS_NON_MODBUS_ERROR;

   if(unlikely(__IS_IMPLEMENTED_MODBUS_RTU_FUNCTION_CODE(ADU->FunctionCode) != true))
      return MODBUS_RTU_STATUS_ILLEGAL_FUNCTION_ERROR;

   processingData->FunctionCode = ADU->FunctionCode;
   processingData->FunctionSpecificDataSize = sizeof(ADU->FunctionSpecificData);

   return MODBUS_RTU_STATUS_OK;
}

size_t ModbusRTU_EncodeADU(__SDEVICE_HANDLE(ModbusRTU) *handle,
                           const ModbusRTU_RequestProcessingCommonData *processingData,
                           size_t functionSpecificDataSize)
{
   if(unlikely(processingData->RequestType == MODBUS_RTU_REQUEST_TYPE_BROADCAST))
      return 0;

   typedef __MODBUS_RTU_ADU_STRUCT_DECLARATION(functionSpecificDataSize, ) ModbusRTU_ADU_Data;
   ModbusRTU_ADU_Data *ADU = (ModbusRTU_ADU_Data*)handle->Constant->TransmitBuffer;

   ADU->SlaveAddress = handle->Settings->SlaveAddress;
   ADU->FunctionCode = processingData->FunctionCode;
   ADU->CRC16 = ModbusRTU_CRC16(ADU, sizeof(ModbusRTU_ADU_Data) - __MODBUS_RTU_CRC_SIZE);

   return sizeof(ModbusRTU_ADU_Data);
}

size_t ModbusRTU_EncodeExceptionADU(__SDEVICE_HANDLE(ModbusRTU) *handle,
                                    const ModbusRTU_RequestProcessingCommonData *processingData,
                                    ModbusRTU_Status exceptionCode)
{
   typedef __MODBUS_RTU_ADU_STRUCT_DECLARATION(__MODBUS_RTU_EXCEPTION_CODE_SIZE, ) ModbusRTU_ExceptionADU_Data;
   ModbusRTU_ExceptionADU_Data *ADU = (ModbusRTU_ExceptionADU_Data*)handle->Constant->TransmitBuffer;

   ADU->SlaveAddress = handle->Settings->SlaveAddress;
   ADU->FunctionCode = processingData->FunctionCode | __MODBUS_RTU_EXCEPTION_FLAG;
   ADU->FunctionSpecificData[0] = exceptionCode;
   ADU->CRC16 = ModbusRTU_CRC16(ADU, sizeof(ModbusRTU_ExceptionADU_Data) - __MODBUS_RTU_CRC_SIZE);

   return sizeof(ModbusRTU_ExceptionADU_Data);
}
