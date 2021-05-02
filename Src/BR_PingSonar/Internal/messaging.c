#include "BR_PingSonar/Internal/messaging.h"

#include <memory.h>

#define __BR_PING_SONAR_MESSAGE_START_0 'B'
#define __BR_PING_SONAR_MESSAGE_START_1 'R'
#define __BR_PING_SONAR_SRC_DEVICE_ID 0
#define __BR_PING_SONAR_DST_DEVICE_ID 0

static uint16_t BR_PingSonarCRC16(void *data, size_t length)
{
   uint8_t *bytes = data;
   uint16_t CRC = 0;

   while(length-- > 0)
      CRC += bytes[length];

   return CRC;
}

bool BR_PingSonarTrySendMessage(__SDEVICE_HANDLE(BR_PingSonar) *handle,
                                uint16_t messageID,
                                const void *payload,
                                size_t payloadSize)
{
   typedef __BR_PING_SONAR_MESSAGE_DECLARATION(payloadSize, ) RequestMessage;
   RequestMessage *request = (RequestMessage*)handle->Constant->TransmitBuffer;

   request->Preamble.Start[0] = __BR_PING_SONAR_MESSAGE_START_0;
   request->Preamble.Start[1] = __BR_PING_SONAR_MESSAGE_START_1;
   request->Preamble.PayloadLength = payloadSize;
   request->Preamble.MessageID = messageID;
   request->Preamble.SrcDeviceID = __BR_PING_SONAR_SRC_DEVICE_ID;
   request->Preamble.DstDeviceID = __BR_PING_SONAR_DST_DEVICE_ID;

   if(payload != NULL)
      memcpy(request->Payload, payload, payloadSize);

   request->CRC16 = BR_PingSonarCRC16(request, sizeof(RequestMessage) - __BR_PING_SONAR_CRC_SIZE);

   return handle->Constant->TrySendFunction(handle, request, sizeof(RequestMessage));
}

bool BR_PingSonarTryReceiveMessage(__SDEVICE_HANDLE(BR_PingSonar) *handle, uint16_t *messageID, size_t *payloadSize)
{
   size_t receiveSize;
   if(unlikely(handle->Constant->TryReceiveFunction(handle, handle->Constant->ReceiveBuffer, &receiveSize) != true))
      return false;

   if(unlikely(receiveSize < __BR_PING_SONAR_MIN_MESSAGE_SIZE))
      return false;

   size_t receivedPayloadSize = ((BR_PingSonarMessagePreamble *)handle->Constant->ReceiveBuffer)->PayloadLength;

   if(unlikely(receivedPayloadSize > handle->Constant->ReceiveBufferSize - __BR_PING_SONAR_MIN_MESSAGE_SIZE))
      return false;

   typedef __BR_PING_SONAR_MESSAGE_DECLARATION(receivedPayloadSize, ) ResponseMessage;
   ResponseMessage *response = (ResponseMessage*)handle->Constant->ReceiveBuffer;

   if(unlikely(response->Preamble.Start[0] != __BR_PING_SONAR_MESSAGE_START_0))
      return false;

   if(unlikely(response->Preamble.Start[1] != __BR_PING_SONAR_MESSAGE_START_1))
      return false;

   if(unlikely(response->CRC16 != BR_PingSonarCRC16(response, sizeof(ResponseMessage) - __BR_PING_SONAR_CRC_SIZE)))
      return false;

   *messageID = response->Preamble.MessageID;
   *payloadSize = receivedPayloadSize;

   return true;
}
