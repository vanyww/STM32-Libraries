#include "BR_PingSonar/core.h"
#include "BR_PingSonar/Internal/messaging.h"

#include <memory.h>

BR_PingSonarStatus BR_PingSonarTryReadData(__SDEVICE_HANDLE(BR_PingSonar) *handle,
                                           uint16_t messageID,
                                           void *payload,
                                           size_t *payloadSize)
{
   if(BR_PingSonarTrySendMessage(handle,
                                 BR_PING_SONAR_COMMON_MESSAGE_ID_GENERAL_REQUEST,
                                 &(BR_PingSonarCommonMessageGeneralRequestPayload){ .RequestedID = messageID },
                                 sizeof(BR_PingSonarCommonMessageGeneralRequestPayload)) != true)
      return BR_PING_SONAR_STATUS_DEVICE_ERROR;

   uint16_t receivedMessageId;
   size_t receivedPayloadSize;

   if(BR_PingSonarTryReceiveMessage(handle, &receivedMessageId, &receivedPayloadSize) != true)
      return BR_PING_SONAR_STATUS_DEVICE_ERROR;

   if(receivedMessageId != messageID)
      return BR_PING_SONAR_STATUS_DEVICE_ERROR;

   if(payload != NULL)
      memcpy(payload, handle->Dynamic.ResponcePayloadData, receivedPayloadSize);

   *payloadSize = receivedPayloadSize;

   return BR_PING_SONAR_STATUS_OK;
}

BR_PingSonarStatus BR_PingSonarTryWriteData(__SDEVICE_HANDLE(BR_PingSonar) *handle,
                                            uint16_t messageID,
                                            const void *payload,
                                            size_t payloadSize)
{
   if(BR_PingSonarTrySendMessage(handle, messageID, payload, payloadSize) != true)
      return BR_PING_SONAR_STATUS_DEVICE_ERROR;

   uint16_t receivedMessageId;
   size_t receivedPayloadSize;

   if(BR_PingSonarTryReceiveMessage(handle, &receivedMessageId, &receivedPayloadSize) != true)
      return BR_PING_SONAR_STATUS_DEVICE_ERROR;

   switch(receivedMessageId)
   {
      case BR_PING_SONAR_COMMON_MESSAGE_ID_ACK:
         return (((BR_PingSonarCommonMessageAckPayload *)handle->Dynamic.ResponcePayloadData)->AckedID == messageID) ?
                  BR_PING_SONAR_STATUS_OK :
                  BR_PING_SONAR_STATUS_DEVICE_ERROR;

      case BR_PING_SONAR_COMMON_MESSAGE_ID_NACK:
         if(((BR_PingSonarCommonMessageNAckPayload *)handle->Dynamic.ResponcePayloadData)->NAckedID == messageID)
            return BR_PING_SONAR_STATUS_DATA_ERROR;

      default:
         return BR_PING_SONAR_STATUS_DEVICE_ERROR;
   }
}
