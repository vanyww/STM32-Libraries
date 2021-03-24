#include "BR_PingSonar/core.h"
#include "BR_PingSonar/Internal/messaging.h"

bool BR_PingSonarTryReadData(__SDEVICE_HANDLE(BR_PingSonar) *handle,
                               uint16_t messageID,
                               void *payload,
                               size_t *payloadSize)
{
   if(BR_PingSonarTrySendMessage(handle,
                                 BR_PING_SONAR_COMMON_MESSAGE_ID_GENERAL_REQUEST,
                                 &(BR_PingSonarCommonMessageGeneralRequestPayload){ .RequestedID = messageID },
                                 sizeof(BR_PingSonarCommonMessageGeneralRequestPayload)) != true)
      return false;

   return BR_PingSonarTryReceiveMessage(handle, messageID, payload, payloadSize);
}

bool BR_PingSonarTryWriteData(__SDEVICE_HANDLE(BR_PingSonar) *handle,
                              uint16_t messageID,
                              const void *payload,
                              size_t payloadSize)
{
   return BR_PingSonarTrySendMessage(handle, messageID, payload, payloadSize);
}
