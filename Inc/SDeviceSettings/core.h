#pragma once

#include "SDevice/sdevice_interface.h"

#include <stddef.h>

typedef enum
{
   SDEVICE_SETTING_UPDATE_STATUS_OK,
   SDEVICE_SETTING_UPDATE_STATUS_HANDLED_ERROR,
   SDEVICE_SETTING_UPDATE_STATUS_CRITICAL_ERROR,
} SDeviceSettingUpdateStatus;

typedef struct SDeviceSettingDescription SDeviceSettingDescription;

/* Satty's interface start */

__SDEVICE_HANDLE_FORWARD_DEFINITION(SDeviceSettings);

typedef struct
{
   void *UserData;
   const SDeviceSettingDescription *SettingDescriptions;
   size_t SettingDescriptionsCount;
} __SDEVICE_CONSTANT_DATA(SDeviceSettings);

typedef struct { } __SDEVICE_SETTINGS_DATA(SDeviceSettings);

typedef struct { } __SDEVICE_DYNAMIC_DATA(SDeviceSettings);

__SDEVICE_HANDLE_DEFINITION(SDeviceSettings);

__SDEVICE_INITIALIZE_DYNAMIC_DATA_DECLARATION(SDeviceSettings,);

/* Satty's interface end */

struct SDeviceSettingDescription
{
   SDeviceSettingSetStatus (*WriteValueFunction)(void *, const void *);
   const void *ValueStorage;
   void *Handle;
   size_t Size;
};

SDeviceSettingUpdateStatus SDeviceSettingsTryUpdateValue(__SDEVICE_HANDLE(SDeviceSettings) *,
                                                         size_t,
                                                         const void *,
                                                         size_t,
                                                         size_t);
