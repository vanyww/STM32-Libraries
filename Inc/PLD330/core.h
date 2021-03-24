#pragma once

#include "SDevice/sdevice_interface.h"

#define __PLD330_GET_ANOTHER_DIRECTION(direction)                                                               \
        (((direction) == PLD330_DIRECTION_FORWARD) ? PLD330_DIRECTION_BACKWARD : PLD330_DIRECTION_FORWARD)

typedef struct
{
   uint_fast32_t Ratio;
   uint_fast32_t MaxFrequency;
} PLD330_MicrostepConfig;

typedef enum
{
   PLD330_PIN_STATE_LOW,
   PLD330_PIN_STATE_HIGH
} PLD330_PinState;

typedef enum
{
   PLD330_MICROSTEP_MODE_1_TO_1,
   PLD330_MICROSTEP_MODE_1_TO_2,
   PLD330_MICROSTEP_MODE_1_TO_8,
   PLD330_MICROSTEP_MODE_1_TO_16
} PLD330_MicrostepMode;

typedef enum
{
   PLD330_DIRECTION_FORWARD = PLD330_PIN_STATE_LOW,
   PLD330_DIRECTION_BACKWARD = PLD330_PIN_STATE_HIGH
} PLD330_Direction;

typedef enum
{
   PLD330_POWER_STATE_ENABLED = PLD330_PIN_STATE_HIGH,
   PLD330_POWER_STATE_DISABLED = PLD330_PIN_STATE_LOW
} PLD330_PowerState;

/* Satty's interface start */

__SDEVICE_HANDLE_FORWARD_DEFINITION(PLD330);

typedef struct
{
   void *UserData;
   bool (* SetDirectionPinStateFunction)(__SDEVICE_HANDLE(PLD330) *, PLD330_PinState);
   bool (* SetEnablePinStateFunction)(__SDEVICE_HANDLE(PLD330) *, PLD330_PinState);
   bool (* StartGeneratePulsesFunction)(__SDEVICE_HANDLE(PLD330) *, uint_fast32_t);
   bool (* SetPulsesFrequencyFunction)(__SDEVICE_HANDLE(PLD330) *, uint_fast32_t);
   void (* MoveCompletedCallback)(__SDEVICE_HANDLE(PLD330) *);
   uint_fast32_t MaxOneTimePulses;
}__SDEVICE_CONSTANT_DATA(PLD330);

typedef struct
{
   float StepAngle;
   float Speed;
   PLD330_MicrostepMode MicrostepMode;
   PLD330_PowerState PowerState;
}__SDEVICE_SETTINGS_DATA(PLD330);

typedef struct
{
   uint_fast32_t MaxOneTimeSteps;
   const PLD330_MicrostepConfig *MicrostepsConfig;
   PLD330_Direction Direction;
   bool IsMovingInProgress;
}__SDEVICE_DYNAMIC_DATA(PLD330);

__SDEVICE_HANDLE_DEFINITION(PLD330);

__SDEVICE_INITIALIZE_DYNAMIC_DATA_DECLARATION(PLD330,);

__SDEVICE_SET_SETTING_DECLARATION(PLD330, StepAngle,,);
__SDEVICE_SET_SETTING_DECLARATION(PLD330, Speed,,);
__SDEVICE_SET_SETTING_DECLARATION(PLD330, MicrostepMode,,);
__SDEVICE_SET_SETTING_DECLARATION(PLD330, PowerState,,);
__SDEVICE_SET_SETTING_DECLARATION(PLD330, MaxOneTimePulses,,);

/* Satty's interface end */

bool PLD330_MoveSteps(__SDEVICE_HANDLE(PLD330) *, uint_fast32_t, PLD330_Direction);
bool PLD330_MoveAngle(__SDEVICE_HANDLE(PLD330) *, float, PLD330_Direction);
void PLD330_MoveCompleted(__SDEVICE_HANDLE(PLD330) *);
