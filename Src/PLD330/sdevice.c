#include "PLD330/core.h"

#define __PLD330_IS_VALID_MICROSTEP_MODE(value)    \
   ({                                              \
       __auto_type _value = (value);               \
       _value == PLD330_MICROSTEP_MODE_1_TO_1  ||  \
       _value == PLD330_MICROSTEP_MODE_1_TO_2  ||  \
       _value == PLD330_MICROSTEP_MODE_1_TO_8  ||  \
       _value == PLD330_MICROSTEP_MODE_1_TO_16;    \
   })

#define __PLD330_IS_VALID_POWER_STATE(value)       \
   ({                                              \
       __auto_type _value = (value);               \
       _value == PLD330_POWER_STATE_ENABLED  ||    \
       _value == PLD330_POWER_STATE_DISABLED;      \
   })

static const PLD330_MicrostepConfig _microstepConfigs[] =
   {
       [PLD330_MICROSTEP_MODE_1_TO_1]  = { .Ratio = 1,  .MaxFrequency = 6e3 },
       [PLD330_MICROSTEP_MODE_1_TO_2]  = { .Ratio = 2,  .MaxFrequency = 12e3 },
       [PLD330_MICROSTEP_MODE_1_TO_8]  = { .Ratio = 8,  .MaxFrequency = 50e3 },
       [PLD330_MICROSTEP_MODE_1_TO_16] = { .Ratio = 16, .MaxFrequency = 100e3 }
   };

__SDEVICE_INITIALIZE_DYNAMIC_DATA_DECLARATION(PLD330, handle)
{
   handle->Dynamic.IsMovingInProgress = false;
   handle->Dynamic.Direction = PLD330_DIRECTION_FORWARD;

   if(unlikely(handle->Constant->SetDirectionPinStateFunction(handle, PLD330_DIRECTION_FORWARD) != true))
      return false;

   if(unlikely(__SDEVICE_SET_SETTING(PLD330, PowerState)(handle, &handle->Settings->PowerState) !=
            SDEVICE_SETTING_SET_STATUS_OK))
      return false;

   if(unlikely(__SDEVICE_SET_SETTING(PLD330, MicrostepMode)(handle, &handle->Settings->MicrostepMode) !=
            SDEVICE_SETTING_SET_STATUS_OK))
      return false;

   if(unlikely(__SDEVICE_SET_SETTING(PLD330, Speed)(handle, &handle->Settings->Speed) !=
            SDEVICE_SETTING_SET_STATUS_OK))
      return false;

   return true;
}

__SDEVICE_SET_SETTING_DECLARATION(PLD330, StepAngle, handle, value)
{
   if(unlikely(handle->IsInitialized != true))
      return SDEVICE_SETTING_SET_STATUS_SET_ERROR;

   float angle = *(float*)value;

   if(unlikely(angle <= 0.f))
      return SDEVICE_SETTING_SET_STATUS_VALIDATION_ERROR;

   handle->Settings->StepAngle = angle;

   return SDEVICE_SETTING_SET_STATUS_OK;
}

__SDEVICE_SET_SETTING_DECLARATION(PLD330, PowerState, handle, value)
{
   if(unlikely(handle->IsInitialized != true))
      return SDEVICE_SETTING_SET_STATUS_SET_ERROR;

   PLD330_PowerState powerState = *(PLD330_PowerState*)value;

   if(unlikely(__PLD330_IS_VALID_POWER_STATE(powerState) != true))
      return SDEVICE_SETTING_SET_STATUS_VALIDATION_ERROR;

   if(unlikely(handle->Constant->SetEnablePinStateFunction(handle, powerState) != true))
      return SDEVICE_SETTING_SET_STATUS_SET_ERROR;

   handle->Settings->PowerState = powerState;

   return SDEVICE_SETTING_SET_STATUS_OK;
}

__SDEVICE_SET_SETTING_DECLARATION(PLD330, MicrostepMode, handle, value)
{
   if(unlikely(handle->IsInitialized != true))
      return SDEVICE_SETTING_SET_STATUS_SET_ERROR;

   PLD330_MicrostepMode microstepMode = *(PLD330_MicrostepMode*)value;

   if(unlikely(__PLD330_IS_VALID_MICROSTEP_MODE(microstepMode) != true))
      return SDEVICE_SETTING_SET_STATUS_VALIDATION_ERROR;

   const PLD330_MicrostepConfig *microstepConfig = &_microstepConfigs[microstepMode];
   uint_fast32_t maxOneTimeSteps = handle->Constant->MaxOneTimePulses / microstepConfig->Ratio;

   if(unlikely(maxOneTimeSteps == 0))
      return SDEVICE_SETTING_SET_STATUS_VALIDATION_ERROR;

   handle->Dynamic.MicrostepsConfig = microstepConfig;
   handle->Dynamic.MaxOneTimeSteps = maxOneTimeSteps;

   handle->Settings->MicrostepMode = microstepMode;

   return SDEVICE_SETTING_SET_STATUS_OK;
}

__SDEVICE_SET_SETTING_DECLARATION(PLD330, Speed, handle, value)
{
   if(unlikely(handle->IsInitialized != true))
      return SDEVICE_SETTING_SET_STATUS_SET_ERROR;

   float speed = *(float*)value;

   if(unlikely(speed < 0.f || speed > 1.f))
      return SDEVICE_SETTING_SET_STATUS_VALIDATION_ERROR;

   uint_fast32_t frequency = 1 + (uint_fast32_t)((handle->Dynamic.MicrostepsConfig->MaxFrequency - 1) * speed);

   if(unlikely(handle->Constant->SetPulsesFrequencyFunction(handle, frequency) != true))
      return SDEVICE_SETTING_SET_STATUS_OK;

   handle->Settings->Speed = speed;

   return SDEVICE_SETTING_SET_STATUS_OK;
}
