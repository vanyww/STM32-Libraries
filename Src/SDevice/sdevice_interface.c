#include "SDevice/sdevice_interface.h"

typedef struct
{
   bool IsInitialized;
   const void *Constant;
   void *Settings;
   uint8_t Dynamic[];
} SDeviceCommonHandle;

bool SDeviceInitializeHandleCommon(bool (* initializeDynamicData)(void *),
                                   const void *constantData,
                                   void *settingsData,
                                   void *handle)
{
   SDeviceCommonHandle *commonHandle = handle;

   if(unlikely(commonHandle->IsInitialized == true))
      return false;

   commonHandle->Constant = constantData;
   commonHandle->Settings = settingsData;

   /* in case of setting set call inside dynamic data initialization function, handle must be marked as initialized */
   commonHandle->IsInitialized = true;

   if(unlikely(initializeDynamicData(handle) != true))
   {
      commonHandle->IsInitialized = false;
      return false;
   }

   return true;
}
