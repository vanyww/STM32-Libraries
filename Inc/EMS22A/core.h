#pragma once

#include "SDevice/sdevice_interface.h"

#define __EMS22A_FULL_TURN_VALUE 1024

typedef enum
{
   EMS22A_STATUS_OK,
   EMS22A_STATUS_READ_ERROR,
   EMS22A_STATUS_CORDIC_OVERFLOW_ERROR,
   EMS22A_STATUS_LINEARITY_ERROR,
   EMS22A_STATUS_PARITY_ERROR
} EMS22A_Status;

/* Satty's interface start */

__SDEVICE_HANDLE_FORWARD_DEFINITION(EMS22A);

typedef struct
{
   bool (* TryReadFrameFunction)(__SDEVICE_HANDLE(EMS22A) *, uint16_t *);
   void *UserData;
}__SDEVICE_CONSTANT_DATA(EMS22A);

typedef struct { }__SDEVICE_SETTINGS_DATA(EMS22A);

typedef struct { }__SDEVICE_DYNAMIC_DATA(EMS22A);

__SDEVICE_HANDLE_DEFINITION(EMS22A);

__SDEVICE_INITIALIZE_DYNAMIC_DATA_DECLARATION(EMS22A,);

/* Satty's interface end */

EMS22A_Status EMS22A_GetPosition(__SDEVICE_HANDLE(EMS22A) *, uint16_t *);
