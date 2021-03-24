#include "EMS22A/core.h"

typedef struct
{
   uint16_t Parity                :1;
   uint16_t MagnitudeDecrease     :1;
   uint16_t MagnitudeIncrease     :1;
   uint16_t LinearityAlarm        :1;
   uint16_t CordicOverflow        :1;
   uint16_t OffsetCompensationEnd :1;
   uint16_t Position              :10;
} EMS22A_DataFrame;

static bool EMS22A_IsFrameParityValid(EMS22A_DataFrame frame)
{
   uint16_t value = *(uint16_t*)&frame >> 1;

   value ^= value >> 8;
   value ^= value >> 4;
   value ^= value >> 2;
   value ^= value >> 1;

   return frame.Parity == (value & 1);
}

EMS22A_Status EMS22A_GetPosition(__SDEVICE_HANDLE(EMS22A) *handle, uint16_t *position)
{
   if(unlikely(handle->IsInitialized != true))
      return EMS22A_STATUS_READ_ERROR;

   EMS22A_DataFrame frame;

   if(unlikely(handle->Constant->TryReadFrameFunction(handle, (uint16_t*)&frame) != true))
      return EMS22A_STATUS_READ_ERROR;

   if(unlikely(EMS22A_IsFrameParityValid(frame) != true))
      return EMS22A_STATUS_PARITY_ERROR;

   if(unlikely(frame.CordicOverflow != 0))
      return EMS22A_STATUS_CORDIC_OVERFLOW_ERROR;

   if(unlikely(frame.LinearityAlarm != 0))
      return EMS22A_STATUS_LINEARITY_ERROR;

   *position = frame.Position;

   return EMS22A_STATUS_OK;
}
