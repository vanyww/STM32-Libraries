#include "VirtualMemory/core.h"

#include <memory.h>

typedef VirtualMemoryOperationStatus (* VirtualMemoryOperation)(__SDEVICE_HANDLE(VirtualMemory) *,
                                                                void *,
                                                                const VirtualMemoryChunk *,
                                                                VirtualMemoryFunctionParameters *);

static VirtualMemoryOperationStatus VirtualMemoryTryReadOperation(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                                                  void *data,
                                                                  const VirtualMemoryChunk *chunk,
                                                                  VirtualMemoryFunctionParameters *operationParameters)
{
   if(chunk->ReadFunction != NULL)
      return chunk->ReadFunction(handle, operationParameters, data, chunk->Argument);
   else
      memset(data, __VIRTUAL_MEMORY_MOCK_VALUE, operationParameters->BytesCount);

   return VIRTUAL_MEMORY_OPERATION_STATUS_OK;
}

static VirtualMemoryOperationStatus VirtualMemoryTryWriteOperation(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                                                   void *data,
                                                                   const VirtualMemoryChunk *chunk,
                                                                   VirtualMemoryFunctionParameters *operationParameters)
{
   if(chunk->WriteFunction != NULL)
      return chunk->WriteFunction(handle, operationParameters, data, chunk->Argument);

   return VIRTUAL_MEMORY_OPERATION_STATUS_OK;
}

static const VirtualMemoryOperation VirtualMemoryOperationsMap[] =
         {
            [VIRTUAL_MEMORY_OPERATION_ID_READ] = VirtualMemoryTryReadOperation,
            [VIRTUAL_MEMORY_OPERATION_ID_WRITE] = VirtualMemoryTryWriteOperation
         };

static inline bool VirtualMemoryTryFindChunk(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                             VirtualMemoryBaseType address,
                                             const VirtualMemoryChunk **chunk)
{
   if(unlikely(address < handle->Constant->AddressingStart))
      return false;

   VirtualMemoryBaseType chunkLastAddress = -1;

   for(VirtualMemoryBaseType i = 0; i < handle->Constant->ChunksCount; i++)
   {
      chunkLastAddress += handle->Constant->Chunks[i].BytesCount;

      if(unlikely(address <= chunkLastAddress))
      {
         *chunk = &handle->Constant->Chunks[i];
         return true;
      }
   }

   return false;
}

VirtualMemoryOperationStatus VirtualMemoryTryPerformOperation(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                                              VirtualMemoryOperationID operation,
                                                              const void *argument,
                                                              VirtualMemoryBaseType address,
                                                              void *data,
                                                              VirtualMemoryBaseType count)
{
   if(unlikely(handle->IsInitialized != true))
      return VIRTUAL_MEMORY_OPERATION_STATUS_DEVICE_ERROR;

   if(unlikely(__IS_VALID_VIRTUAL_MEMORY_OPERATION_ID(operation) != true))
      return VIRTUAL_MEMORY_OPERATION_STATUS_DEVICE_ERROR;

   const VirtualMemoryChunk *chunk;

   if(unlikely(VirtualMemoryTryFindChunk(handle, address + count - 1, &chunk) != true))
      return VIRTUAL_MEMORY_OPERATION_STATUS_DATA_ERROR;

   VirtualMemoryOperation operationFunction = VirtualMemoryOperationsMap[operation];
   VirtualMemoryFunctionParameters operationParameters = { .Offset = 0, .CallArgument = argument };
   data += count;

   while(count > 0)
   {
      /* offset can only appear if there is only one chunk left to process (so no need to zero it again) */
      if(count < chunk->BytesCount)
         operationParameters.Offset = chunk->BytesCount - count;

      operationParameters.BytesCount = __MIN(chunk->BytesCount - operationParameters.Offset, count);
      data -= operationParameters.BytesCount;

      VirtualMemoryOperationStatus operationStatus = operationFunction(handle, data, chunk, &operationParameters);

      if(unlikely(operationStatus != VIRTUAL_MEMORY_OPERATION_STATUS_OK))
         return operationStatus;

      chunk--;
      count -= operationParameters.BytesCount;
   }

   return VIRTUAL_MEMORY_OPERATION_STATUS_OK;
}
