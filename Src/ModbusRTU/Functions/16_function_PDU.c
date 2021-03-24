#include "ModbusRTU/Functions/16_function_PDU.h"

#define __MODBUS_RTU_FUNCTION_16_REQUEST_MIN_SIZE \
   (sizeof(ModbusRTU_Function16_RequestData) + sizeof(ModbusRTU_Register))

ModbusRTU_Status ModbusRTU_Process16FunctionRequest(__SDEVICE_HANDLE(ModbusRTU) *handle,
                                                    ModbusRTU_RequestProcessingCommonData *processingData,
                                                    size_t *replySize)
{
   if(unlikely(processingData->FunctionSpecificDataSize < __MODBUS_RTU_FUNCTION_16_REQUEST_MIN_SIZE))
      return MODBUS_RTU_STATUS_NON_MODBUS_ERROR;

   ModbusRTU_Function16_RequestData *request = handle->Dynamic.RequestFunctionSpecificData;

   if(unlikely(request->BytesToFollow / sizeof(ModbusRTU_Register) != request->RegistersToWriteCount ||
               request->BytesToFollow % sizeof(ModbusRTU_Register) != 0                              ||
               request->BytesToFollow !=
                        processingData->FunctionSpecificDataSize - sizeof(ModbusRTU_Function16_RequestData)))
      return MODBUS_RTU_STATUS_ILLEGAL_DATA_ERROR;

   ModbusRTU_MemoryOperationStatus status =
            handle->Constant->WriteRegistersFunction(handle,
                                                     processingData->RequestType,
                                                     &(WriteRegistersParameters)
                                                     {
                                                         .Registers = request->RegistersBuffer,
                                                         .Address = request->DataRegisterAddress,
                                                         .RegistersCount = request->RegistersToWriteCount
                                                     });

   if(unlikely(status != MODBUS_RTU_MEMORY_OPERATION_STATUS_OK))
      return status;

   ModbusRTU_Function16_ReplyData *reply = handle->Dynamic.ReplyFunctionSpecificData;

   reply->DataRegisterAddress = request->DataRegisterAddress;
   reply->WrittenRegistersCount = request->RegistersToWriteCount;

   *replySize = sizeof(ModbusRTU_Function16_ReplyData);

   return MODBUS_RTU_STATUS_OK;
}
