#include "SDevice/sdevice_interface.h"

#include <stdint.h>

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

   sdevice_assert(commonHandle->IsInitialized != true);

   commonHandle->Constant = constantData;
   commonHandle->Settings = settingsData;

   /* in case of setting set call inside dynamic data initialization function, handle must be marked as initialized */
   commonHandle->IsInitialized = true;

   bool __attribute__((unused)) status = initializeDynamicData(handle);
   sdevice_assert(status == true);

   return true;
}

void SDeviceAssertFailed(char *file, int line)
{
   for(;;) { }
}
