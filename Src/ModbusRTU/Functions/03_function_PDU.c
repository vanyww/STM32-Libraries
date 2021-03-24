#include "ModbusRTU/Functions/03_function_PDU.h"

#define __MODBUS_RTU_FUNCTION_03_REQUEST_EXACT_SIZE sizeof(ModbusRTU_Function03_RequestData)

ModbusRTU_Status ModbusRTU_Process03FunctionRequest(__SDEVICE_HANDLE(ModbusRTU) *handle,
                                                    ModbusRTU_RequestProcessingCommonData *processingData,
                                                    size_t *replySize)
{
   if(unlikely(processingData->FunctionSpecificDataSize != __MODBUS_RTU_FUNCTION_03_REQUEST_EXACT_SIZE))
      return MODBUS_RTU_STATUS_NON_MODBUS_ERROR;

   ModbusRTU_Function03_RequestData *request = handle->Dynamic.RequestFunctionSpecificData;

   if(unlikely(request->RegistersToReadCount > __MODBUS_RTU_FUNCTION_03_MAX_REGISTERS_COUNT))
      return MODBUS_RTU_STATUS_ILLEGAL_DATA_ERROR;

   ModbusRTU_Function03_ReplyData *reply = handle->Dynamic.ReplyFunctionSpecificData;

   ModbusRTU_MemoryOperationStatus status =
            handle->Constant->ReadRegistersFunction(handle,
                                                    processingData->RequestType,
                                                    &(ReadRegistersParameters)
                                                    {
                                                       .Registers = reply->RegistersBuffer,
                                                       .Address = request->DataRegisterAddress,
                                                       .RegistersCount = request->RegistersToReadCount
                                                    });

   if(unlikely(status != MODBUS_RTU_MEMORY_OPERATION_STATUS_OK))
      return status;

   reply->FollowingDataBytes = request->RegistersToReadCount * sizeof(ModbusRTU_Register);
   *replySize = sizeof(ModbusRTU_Function03_ReplyData) + reply->FollowingDataBytes;

   return MODBUS_RTU_STATUS_OK;
}
