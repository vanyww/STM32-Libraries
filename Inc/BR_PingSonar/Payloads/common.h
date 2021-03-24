#pragma once

#include <stdint.h>

typedef enum
{
   BR_PING_SONAR_COMMON_MESSAGE_ID_ACK                    = 1,
   BR_PING_SONAR_COMMON_MESSAGE_ID_NACK                   = 2,
   BR_PING_SONAR_COMMON_MESSAGE_ID_ASCII_TEXT             = 3,
   BR_PING_SONAR_COMMON_MESSAGE_ID_GENERAL_REQUEST        = 6,

   BR_PING_SONAR_COMMON_MESSAGE_ID_GET_DEVICE_INFORMATION = 4,
   BR_PING_SONAR_COMMON_MESSAGE_ID_GET_PROTOCOL_VERSION   = 5,

   BR_PING_SONAR_COMMON_MESSAGE_ID_SET_DEVICE_ID          = 100
} BR_PingSonarCommonMessageID;

typedef enum
{
   BR_PING_SONAR_COMMON_MESSAGE_DEVICE_TYPE_UNKNOWN          = 0,
   BR_PING_SONAR_COMMON_MESSAGE_DEVICE_TYPE_PING_ECHOSOUNDER = 1,
   BR_PING_SONAR_COMMON_MESSAGE_DEVICE_TYPE_PING360          = 2
} BR_PingSonarCommonMessageDeviceType;

/* general message type payloads */

typedef struct __attribute__((packed))
{
   uint16_t AckedID;
} BR_PingSonarCommonMessageAckPayload;

typedef struct __attribute__((packed))
{
   uint16_t NackedID;
   char NackMessage[];
} BR_PingSonarCommonMessageNAckPayload;

typedef struct __attribute__((packed))
{
   uint16_t RequestedID;
} BR_PingSonarCommonMessageGeneralRequestPayload;

/* get message type payloads */

typedef struct __attribute__((packed))
{
   uint8_t DeviceType;
   uint8_t DeviceRevision;
   uint8_t FirmwareVersionMajor;
   uint8_t FirmwareVersionMinor;
   uint8_t FirmwareVersionPatch;
   uint8_t Reserved;
} BR_PingSonarCommonMessageGetDeviceInformationPayload;

typedef struct __attribute__((packed))
{
   uint8_t VersionMajor;
   uint8_t VersionMinor;
   uint8_t VersionPatch;
   uint8_t Reserved;
} BR_PingSonarCommonMessageGetProtocolVersionPayload;

/* set message type payloads */

typedef struct __attribute__((packed))
{
   uint8_t DeviceID;
} BR_PingSonarCommonMessageSetDeviceID_Payload;
