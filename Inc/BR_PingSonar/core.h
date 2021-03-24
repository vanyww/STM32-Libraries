#pragma once

#include "SDevice/sdevice_interface.h"
#include "BR_PingSonar/Payloads/1d.h"
#include "BR_PingSonar/Payloads/common.h"

#include <stddef.h>

#define __BR_PING_SONAR_CRC_SIZE sizeof(uint16_t)
#define __BR_PING_SONAR_MIN_MESSAGE_SIZE (sizeof(BR_PingSonarMessagePreamble) + __BR_PING_SONAR_CRC_SIZE)
#define __BR_PING_SONAR_RECEIVE_MESSAGE_MAX_SIZE      \
   (__BR_PING_SONAR_MIN_MESSAGE_SIZE +                \
    __BR_PING_SONAR_MAX_PROFILE_DATA_SIZE +           \
    sizeof(BR_PingSonar1D_MessageGetProfilePayload))
#define __BR_PING_SONAR_SEND_MESSAGE_MAX_SIZE         \
   (__BR_PING_SONAR_MIN_MESSAGE_SIZE +                \
    sizeof(BR_PingSonar1D_MessageSetRangePayload))

typedef struct __attribute__((packed))
{
   uint8_t Start[2];
   uint16_t PayloadLength;
   uint16_t MessageID;
   uint8_t SrcDeviceID;
   uint8_t DstDeviceID;
} BR_PingSonarMessagePreamble;

/* Satty's interface start */

__SDEVICE_HANDLE_FORWARD_DEFINITION(BR_PingSonar);

typedef struct
{
   void *UserData;
   bool (*TrySendFunction)(__SDEVICE_HANDLE(BR_PingSonar) *, void *, size_t);
   bool (*TryReceiveFunction)(__SDEVICE_HANDLE(BR_PingSonar) *, void *, size_t *);
   void *ReceiveBuffer;
   void *TransmitBuffer;
   size_t ReceiveBufferSize;
   size_t TransmitBufferSize;
}__SDEVICE_CONSTANT_DATA(BR_PingSonar);

typedef struct { }__SDEVICE_SETTINGS_DATA(BR_PingSonar);

typedef struct
{
   void *ResponcePayloadData;
   void *RequestPayloadData;
}__SDEVICE_DYNAMIC_DATA(BR_PingSonar);

__SDEVICE_HANDLE_DEFINITION(BR_PingSonar);

__SDEVICE_INITIALIZE_DYNAMIC_DATA_DECLARATION(BR_PingSonar,);

/* Satty's interface end */

bool BR_PingSonarTryReadData(__SDEVICE_HANDLE(BR_PingSonar)*, uint16_t, void *, size_t *);
bool BR_PingSonarTryWriteData(__SDEVICE_HANDLE(BR_PingSonar)*, uint16_t, const void *, size_t);
