#include "PLD330/core.h"

#include <stddef.h>

bool PLD330_MoveSteps(__SDEVICE_HANDLE(PLD330) *handle, uint_fast32_t steps, PLD330_Direction direction)
{
   if(unlikely(handle->IsInitialized != true))
      return false;

   if(unlikely(handle->Dynamic.IsMovingInProgress == true))
      return false;

   if(unlikely(handle->Settings->PowerState == PLD330_POWER_STATE_DISABLED))
      return false;

   if(unlikely(steps == 0))
      return true;

   if(unlikely(steps > handle->Dynamic.MaxOneTimeSteps))
      return false;

   if(handle->Dynamic.Direction != direction)
   {
      if(unlikely(handle->Constant->SetDirectionPinStateFunction(handle, direction) != true))
         return false;

      handle->Dynamic.Direction = direction;
   }

   uint_fast32_t microsteps = steps * handle->Dynamic.MicrostepsConfig->Ratio;

   if(unlikely(handle->Constant->StartGeneratePulsesFunction(handle, microsteps) != true))
      return false;

   handle->Dynamic.IsMovingInProgress = true;

   return true;
}

bool PLD330_MoveAngle(__SDEVICE_HANDLE(PLD330) *handle, float angle, PLD330_Direction direction)
{
   uint_fast32_t steps = angle / handle->Settings->StepAngle;
   return PLD330_MoveSteps(handle, steps, direction);
}

void PLD330_MoveCompleted(__SDEVICE_HANDLE(PLD330) *handle)
{
   if(unlikely(handle->IsInitialized != true))
      return;

   if(unlikely(handle->Dynamic.IsMovingInProgress == false))
      return;

   handle->Dynamic.IsMovingInProgress = false;

   if(handle->Constant->MoveCompletedCallback != NULL)
      handle->Constant->MoveCompletedCallback(handle);
}
