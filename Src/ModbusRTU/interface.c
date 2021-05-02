#include "ModbusRTU/core.h"
#include "ModbusRTU/Functions/03_function_PDU.h"
#include "ModbusRTU/Functions/16_function_PDU.h"

static inline ModbusRTU_Status ModbusRTU_ProcessFunction(__SDEVICE_HANDLE(ModbusRTU) *handle,
                                                         ModbusRTU_RequestProcessingCommonData *processingData,
                                                         size_t *replySize)
{
   switch(processingData->FunctionCode)
   {
      case MODBUS_RTU_FUNCTION_READ_HOLDING_REGISTERS:
         return ModbusRTU_Process03FunctionRequest(handle, processingData, replySize);

      case MODBUS_RTU_FUNCTION_PRESET_MULTIPLE_REGISTERS:
         return ModbusRTU_Process16FunctionRequest(handle, processingData, replySize);

      default:
         return MODBUS_RTU_STATUS_ILLEGAL_DATA_ERROR;
   }
}

size_t ModbusRTU_ProcessRequest(__SDEVICE_HANDLE(ModbusRTU) *handle, size_t requestSize)
{
   sdevice_assert(handle->IsInitialized == true);

   ModbusRTU_RequestProcessingCommonData request;
   ModbusRTU_Status operationStatus;
   size_t replySize = 0;

   operationStatus = ModbusRTU_DecodeADU(handle, requestSize, &request);

   if(unlikely(operationStatus != MODBUS_RTU_STATUS_OK))
      goto ExceptionExit;

   operationStatus = ModbusRTU_ProcessFunction(handle, &request, &replySize);

   if(unlikely(operationStatus != MODBUS_RTU_STATUS_OK))
      goto ExceptionExit;

   replySize = ModbusRTU_EncodeADU(handle, &request, replySize);
   return replySize;

ExceptionExit:
   if(operationStatus == MODBUS_RTU_STATUS_NON_MODBUS_ERROR)
      return 0;

   replySize = ModbusRTU_EncodeExceptionADU(handle, &request, operationStatus);
   return replySize;
}
