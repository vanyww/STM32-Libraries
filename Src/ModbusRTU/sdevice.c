#include "ModbusRTU/core.h"
#include "ModbusRTU/Functions/message_ADU.h"

#define __MODBUS_RTU_MAX_VALID_SLAVE_ADDRESS 247

__SDEVICE_INITIALIZE_DYNAMIC_DATA_DECLARATION(ModbusRTU, handle)
{
   handle->Dynamic.RequestFunctionSpecificData = handle->Constant->ReceiveBuffer +
            __MODBUS_RTU_ADU_FUNCTION_SPECIFIC_DATA_OFFSET;
   handle->Dynamic.ReplyFunctionSpecificData = handle->Constant->TransmitBuffer +
            __MODBUS_RTU_ADU_FUNCTION_SPECIFIC_DATA_OFFSET;

   return true;
}

__SDEVICE_SET_SETTING_DECLARATION(ModbusRTU, SlaveAddress, handle, value)
{
   __SDEVICE_HANDLE(ModbusRTU) *device_handle = handle;

   sdevice_assert(device_handle->IsInitialized == true);

   uint8_t address = *(uint8_t*)value;

   if(unlikely(address == __MODBUS_RTU_BROADCAST_REQUEST_SLAVE_ADDRESS ||
               address > __MODBUS_RTU_MAX_VALID_SLAVE_ADDRESS))
      return SDEVICE_SETTING_SET_STATUS_VALIDATION_ERROR;

   device_handle->Settings->SlaveAddress = address;

   return SDEVICE_SETTING_SET_STATUS_OK;
}
