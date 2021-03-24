#pragma once

#include "BR_PingSonar/core.h"

#include <stddef.h>

#define __BR_PING_SONAR_MESSAGE_DECLARATION(payload_size, name) \
   struct __attribute__((packed)) name                          \
   {                                                            \
      BR_PingSonarMessagePreamble Preamble;                     \
      uint8_t Payload[(payload_size)];                          \
      uint16_t CRC16;                                           \
   }

bool BR_PingSonarTrySendMessage(__SDEVICE_HANDLE(BR_PingSonar) *, uint16_t, const void *, size_t);
bool BR_PingSonarTryReceiveMessage(__SDEVICE_HANDLE(BR_PingSonar) *, uint16_t, void *, size_t *);
