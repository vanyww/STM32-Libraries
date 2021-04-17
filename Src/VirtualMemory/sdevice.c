#include "VirtualMemory/core.h"

__SDEVICE_INITIALIZE_DYNAMIC_DATA_DECLARATION(VirtualMemory, handle)
{
   VirtualMemoryBaseType lastAddress = -1;

   for(VirtualMemoryBaseType i = 0; i < handle->Constant->ChunksCount; i++)
      lastAddress += handle->Constant->Chunks[i].BytesCount;

   handle->Dynamic.AddressingEnd = lastAddress;

   return true;
}
