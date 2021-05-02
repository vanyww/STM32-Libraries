#pragma once

#include "SDevice/sdevice_interface.h"
#include "VirtualMemory/config.h"

#define __IS_VALID_VIRTUAL_MEMORY_OPERATION_ID(value) ( \
   {                                                    \
      __auto_type _value = (value);                     \
      _value == VIRTUAL_MEMORY_OPERATION_ID_READ   ||   \
      _value == VIRTUAL_MEMORY_OPERATION_ID_WRITE;      \
   })

typedef enum
{
   VIRTUAL_MEMORY_OPERATION_ID_READ,
   VIRTUAL_MEMORY_OPERATION_ID_WRITE,
} VirtualMemoryOperationID;

typedef enum
{
   VIRTUAL_MEMORY_OPERATION_STATUS_OK,
   VIRTUAL_MEMORY_OPERATION_STATUS_DATA_ERROR,
   VIRTUAL_MEMORY_OPERATION_STATUS_DEVICE_ERROR
} VirtualMemoryOperationStatus;

typedef struct
{
   const void *CallArgument;
   VirtualMemoryBaseType Offset;
   VirtualMemoryBaseType BytesCount;
} VirtualMemoryFunctionParameters;

typedef struct VirtualMemoryChunk VirtualMemoryChunk;

/* Satty's interface start */

__SDEVICE_HANDLE_FORWARD_DEFINITION(VirtualMemory);

typedef struct
{
   void *UserData;
   const VirtualMemoryChunk *Chunks;
   VirtualMemoryBaseType ChunksCount;
   VirtualMemoryBaseType AddressingStart;
} __SDEVICE_CONSTANT_DATA(VirtualMemory);

typedef struct { } __SDEVICE_SETTINGS_DATA(VirtualMemory);

typedef struct
{
   VirtualMemoryBaseType AddressingEnd;
} __SDEVICE_DYNAMIC_DATA(VirtualMemory);

__SDEVICE_HANDLE_DEFINITION(VirtualMemory);

__SDEVICE_INITIALIZE_DYNAMIC_DATA_DECLARATION(VirtualMemory,);

/* Satty's interface end */

struct VirtualMemoryChunk
{
   VirtualMemoryOperationStatus (* ReadFunction)(__SDEVICE_HANDLE(VirtualMemory) *,
                                                 const VirtualMemoryFunctionParameters *,
                                                 void *,
                                                 const void *);
   VirtualMemoryOperationStatus (* WriteFunction)(__SDEVICE_HANDLE(VirtualMemory) *,
                                                  const VirtualMemoryFunctionParameters *,
                                                  const void *,
                                                  const void *);
   const void *Argument;
   VirtualMemoryBaseType BytesCount;
};

VirtualMemoryOperationStatus VirtualMemoryTryPerformOperation(__SDEVICE_HANDLE(VirtualMemory) *,
                                                              VirtualMemoryOperationID,
                                                              const void *,
                                                              VirtualMemoryBaseType ,
                                                              void *,
                                                              VirtualMemoryBaseType);
