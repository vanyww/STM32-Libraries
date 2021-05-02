#include "SDeviceSettings/core.h"

#include <memory.h>
#include <alloca.h>

SDeviceSettingUpdateStatus SDeviceSettingsTryUpdateValue(__SDEVICE_HANDLE(SDeviceSettings) *handle,
                                                         size_t settingID,
                                                         const void *value,
                                                         size_t offset,
                                                         size_t length)
{
   sdevice_assert(handle->IsInitialized == true);

   const SDeviceSettingDescription *settingDescription = &handle->Constant->SettingDescriptions[settingID];

   if(unlikely(offset + length > settingDescription->Size))
      return SDEVICE_SETTING_UPDATE_STATUS_HANDLED_ERROR;

   /* check if value to write differ from already written one */
   if(unlikely(memcmp(settingDescription->ValueStorage + offset, value, length) == 0))
      return SDEVICE_SETTING_UPDATE_STATUS_OK;

   void *renewedSettingValue = alloca(settingDescription->Size);

   /* prepare full renewed value */
   memcpy(renewedSettingValue, settingDescription->ValueStorage, offset);
   memcpy(renewedSettingValue + offset, value, length);
   memcpy(renewedSettingValue + (offset + length),
          settingDescription->ValueStorage + (offset + length),
          settingDescription->Size - (offset + length));

   SDeviceSettingSetStatus setStatus;
   setStatus = settingDescription->WriteValueFunction(settingDescription->Handle, renewedSettingValue);

   if(unlikely(setStatus == SDEVICE_SETTING_SET_STATUS_VALIDATION_ERROR))
      return SDEVICE_SETTING_UPDATE_STATUS_HANDLED_ERROR;

   /* check if error occurred in write function and revert changes if so */
   if(unlikely(setStatus == SDEVICE_SETTING_SET_STATUS_SET_ERROR))
   {
      memcpy(renewedSettingValue + offset, settingDescription->ValueStorage + offset, length);

      setStatus = settingDescription->WriteValueFunction(settingDescription->Handle, renewedSettingValue);

      /* if reverted setting value still causes error, return critical error */
      if(unlikely(setStatus != SDEVICE_SETTING_SET_STATUS_OK))
         return SDEVICE_SETTING_UPDATE_STATUS_CRITICAL_ERROR;

      return SDEVICE_SETTING_UPDATE_STATUS_HANDLED_ERROR;
   }

   return SDEVICE_SETTING_UPDATE_STATUS_OK;
}
