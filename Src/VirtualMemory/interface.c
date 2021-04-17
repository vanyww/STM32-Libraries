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

typedef struct {
   const VirtualMemoryChunk *Chunk;
   VirtualMemoryBaseType Offset;
} VirtualMemoryPointer;

static inline bool VirtualMemoryTryFindChunk(__SDEVICE_HANDLE(VirtualMemory) *handle,
                                             VirtualMemoryBaseType address,
                                             VirtualMemoryPointer *pointer)
{
   if(unlikely(address < handle->Constant->AddressingStart || address > handle->Dynamic.AddressingEnd))
      return false;

   VirtualMemoryBaseType chunkLastAddress = -1;

   for(VirtualMemoryBaseType i = 0; i < handle->Constant->ChunksCount; i++)
   {
      chunkLastAddress += handle->Constant->Chunks[i].BytesCount;

      if(unlikely(address <= chunkLastAddress))
      {
         *pointer = (VirtualMemoryPointer)
                  {
                     .Chunk = &handle->Constant->Chunks[i],
                     .Offset = address - (chunkLastAddress - handle->Constant->Chunks[i].BytesCount + 1)
                  };
         return true;
      }
   }

   __builtin_unreachable();
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

   VirtualMemoryPointer memory;

   if(unlikely(VirtualMemoryTryFindChunk(handle, address, &memory) != true))
      return VIRTUAL_MEMORY_OPERATION_STATUS_DATA_ERROR;

   VirtualMemoryOperation operationFunction = VirtualMemoryOperationsMap[operation];
   VirtualMemoryFunctionParameters operationParameters =
            {
               .Offset = memory.Offset,
               .CallArgument = argument,
               .BytesCount = __MIN(memory.Chunk->BytesCount - memory.Offset, count)
            };

   while(count > 0)
   {
      VirtualMemoryOperationStatus operationStatus =
               operationFunction(handle, data, memory.Chunk, &operationParameters);

      if(unlikely(operationStatus != VIRTUAL_MEMORY_OPERATION_STATUS_OK))
         return operationStatus;

      memory.Chunk++;
      count -= operationParameters.BytesCount;
      data += operationParameters.BytesCount;
      operationParameters.BytesCount = __MIN(memory.Chunk->BytesCount, count);

      if(operationParameters.Offset != 0)
         operationParameters.Offset = 0;
   }

   return VIRTUAL_MEMORY_OPERATION_STATUS_OK;
}
