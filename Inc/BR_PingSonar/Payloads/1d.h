#pragma once

#include <stdint.h>

#define __BR_PING_SONAR_MAX_PROFILE_DATA_SIZE 200

#define __BR_PING_SONAR_1D_IS_VALID_MODE_AUTO(value)                                            \
   ({                                                                                           \
       __typeof__ (value) _value = (value);                                                     \
       _value == BR_PING_SONAR_1D_MESSAGE_MODE_MANUAL  ||                                       \
       _value == BR_PING_SONAR_1D_MESSAGE_MODE_AUTO;                                            \
   })

#define __BR_PING_SONAR_1D_IS_VALID_GAIN_SETTING(value)                                         \
   ({                                                                                           \
       __typeof__ (value) _value = (value);                                                     \
       _value == BR_PING_SONAR_1D_MESSAGE_GAIN_SETTING_0P6     ||                               \
       _value == BR_PING_SONAR_1D_MESSAGE_GAIN_SETTING_1P8     ||                               \
       _value == BR_PING_SONAR_1D_MESSAGE_GAIN_SETTING_5P5     ||                               \
       _value == BR_PING_SONAR_1D_MESSAGE_GAIN_SETTING_12P9    ||                               \
       _value == BR_PING_SONAR_1D_MESSAGE_GAIN_SETTING_30P2    ||                               \
       _value == BR_PING_SONAR_1D_MESSAGE_GAIN_SETTING_66P1    ||                               \
       _value == BR_PING_SONAR_1D_MESSAGE_GAIN_SETTING_144P0;                                   \
   })

#define __BR_PING_SONAR_1D_IS_VALID_PING_ENABLED(value)                                         \
   ({                                                                                           \
       __typeof__ (value) _value = (value);                                                     \
       _value == BR_PING_SONAR_1D_MESSAGE_PING_DISABLE ||                                       \
       _value == BR_PING_SONAR_1D_MESSAGE_PING_ENABLE;                                          \
   })

typedef enum
{
   BR_PING_SONAR_1D_MESSAGE_ID_SET_DEVICE_ID             = 1000,
   BR_PING_SONAR_1D_MESSAGE_ID_SET_RANGE                 = 1001,
   BR_PING_SONAR_1D_MESSAGE_ID_SET_SPEED_OF_SOUND        = 1002,
   BR_PING_SONAR_1D_MESSAGE_ID_SET_MODE_AUTO             = 1003,
   BR_PING_SONAR_1D_MESSAGE_ID_SET_PING_INTERVAL         = 1004,
   BR_PING_SONAR_1D_MESSAGE_ID_SET_GAIN_SETTING          = 1005,
   BR_PING_SONAR_1D_MESSAGE_ID_SET_PING_ENABLE           = 1006,

   BR_PING_SONAR_1D_MESSAGE_ID_GET_FIRMWARE_VERSION      = 1200,
   BR_PING_SONAR_1D_MESSAGE_ID_GET_DEVICE_ID             = 1201,
   BR_PING_SONAR_1D_MESSAGE_ID_GET_VOLTAGE_5V            = 1202,
   BR_PING_SONAR_1D_MESSAGE_ID_GET_SPEED_OF_SOUND        = 1203,
   BR_PING_SONAR_1D_MESSAGE_ID_GET_RANGE                 = 1204,
   BR_PING_SONAR_1D_MESSAGE_ID_GET_MODE_AUTO             = 1205,
   BR_PING_SONAR_1D_MESSAGE_ID_GET_PING_INTERVAL         = 1206,
   BR_PING_SONAR_1D_MESSAGE_ID_GET_GAIN_SETTING          = 1207,
   BR_PING_SONAR_1D_MESSAGE_ID_GET_TRANSMIT_DURATION     = 1208,
   BR_PING_SONAR_1D_MESSAGE_ID_GET_GENERAL_INFO          = 1210,
   BR_PING_SONAR_1D_MESSAGE_ID_GET_DISTANCE_SIMPLE       = 1211,
   BR_PING_SONAR_1D_MESSAGE_ID_GET_DISTANCE              = 1212,
   BR_PING_SONAR_1D_MESSAGE_ID_GET_PROCESSOR_TEMPERATURE = 1213,
   BR_PING_SONAR_1D_MESSAGE_ID_GET_PCB_TEMPERATURE       = 1214,
   BR_PING_SONAR_1D_MESSAGE_ID_GET_PING_ENABLE           = 1215,
   BR_PING_SONAR_1D_MESSAGE_ID_GET_PROFILE               = 1300,

   BR_PING_SONAR_1D_MESSAGE_ID_GOTO_BOOTLOADER           = 1100,
   BR_PING_SONAR_1D_MESSAGE_ID_CONTINUOUS_START          = 1400,
   BR_PING_SONAR_1D_MESSAGE_ID_CONTINUOUS_STOP           = 1401
} BR_PingSonar1D_MessageID;

typedef enum
{
   BR_PING_SONAR_1D_MESSAGE_MODE_MANUAL = 0,
   BR_PING_SONAR_1D_MESSAGE_MODE_AUTO   = 1
} BR_PingSonar1D_MessageModeAuto;

typedef enum
{
   BR_PING_SONAR_1D_MESSAGE_GAIN_SETTING_0P6   = 0,
   BR_PING_SONAR_1D_MESSAGE_GAIN_SETTING_1P8   = 1,
   BR_PING_SONAR_1D_MESSAGE_GAIN_SETTING_5P5   = 2,
   BR_PING_SONAR_1D_MESSAGE_GAIN_SETTING_12P9  = 3,
   BR_PING_SONAR_1D_MESSAGE_GAIN_SETTING_30P2  = 4,
   BR_PING_SONAR_1D_MESSAGE_GAIN_SETTING_66P1  = 5,
   BR_PING_SONAR_1D_MESSAGE_GAIN_SETTING_144P0 = 6
} BR_PingSonar1D_MessageGainSetting;

typedef enum
{
   BR_PING_SONAR_1D_MESSAGE_PING_DISABLE = 0,
   BR_PING_SONAR_1D_MESSAGE_PING_ENABLE  = 1
} BR_PingSonar1D_MessagePingEnabled;

typedef enum
{
   BR_PING_SONAR_1D_MESSAGE_DEVICE_TYPE_UNKNOWN     = 0,
   BR_PING_SONAR_1D_MESSAGE_DEVICE_TYPE_ECHOSOUNDER = 1
} BR_PingSonar1D_MessageDeviceType;

typedef enum
{
   BR_PING_SONAR_1D_MESSAGE_DEVICE_MODEL_UNKNOWN = 0,
   BR_PING_SONAR_1D_MESSAGE_DEVICE_MODEL_PING_1D = 1
} BR_PingSonar1D_MessageDeviceModel;

/* set message type payloads */

typedef struct __attribute__((packed))
{
   uint8_t Device_ID;
} BR_PingSonar1D_MessageSetDeviceID_Payload;

typedef struct __attribute__((packed))
{
   uint32_t ScanStart;
   uint32_t ScanLength;
} BR_PingSonar1D_MessageSetRangePayload;

typedef struct __attribute__((packed))
{
   uint32_t SpeedOfSound;
} BR_PingSonar1D_MessageSetSpeedOfSoundPayload;

typedef struct __attribute__((packed))
{
   uint8_t ModeAuto;
} BR_PingSonar1D_MessageSetModeAutoPayload;

typedef struct __attribute__((packed))
{
   uint16_t PingInterval;
} BR_PingSonar1D_MessageSetPingIntervalPayload;

typedef struct __attribute__((packed))
{
   uint8_t GainSetting;
} BR_PingSonar1D_MessageSetGainSettingPayload;

typedef struct __attribute__((packed))
{
   uint8_t PingEnabled;
} BR_PingSonar1D_MessageSetPingEnablePayload;

/* get message type payloads */

typedef struct __attribute__((packed))
{
   uint8_t DeviceType;
   uint8_t DeviceModel;
   uint16_t FirmwareVersionMajor;
   uint16_t FirmwareVersionMinor;
} BR_PingSonar1D_MessageGetFirmwareVersionPayload;

typedef struct __attribute__((packed))
{
   uint8_t Device_ID;
} BR_PingSonar1D_MessageGetDeviceID_Payload;

typedef struct __attribute__((packed))
{
   uint16_t Voltage_5;
} BR_PingSonar1D_MessageGetVoltage_5V_Payload;

typedef struct __attribute__((packed))
{
   uint32_t SpeedOfSound;
} BR_PingSonar1D_MessageGetSpeedOfSoundPayload;

typedef struct __attribute__((packed))
{
   uint32_t ScanStart;
   uint32_t ScanLength;
} BR_PingSonar1D_MessageGetRangePayload;

typedef struct __attribute__((packed))
{
   uint8_t ModeAuto;
} BR_PingSonar1D_MessageGetModeAutoPayload;

typedef struct __attribute__((packed))
{
   uint16_t PingInterval;
} BR_PingSonar1D_MessageGetPingIntervalPayload;

typedef struct __attribute__((packed))
{
   uint32_t GainSetting;
} BR_PingSonar1D_MessageGetGainSettingPayload;

typedef struct __attribute__((packed))
{
   uint16_t TransmitDuration;
} BR_PingSonar1D_MessageGetTransmitDurationPayload;

typedef struct __attribute__((packed))
{
   uint16_t FirmwareVersionMajor;
   uint16_t FirmwareVersionMinor;
   uint16_t Voltage_5;
   uint16_t PingInterval;
   uint8_t GainSetting;
   uint8_t ModeAuto;
} BR_PingSonar1D_MessageGetGeneralInfoPayload;

typedef struct __attribute__((packed))
{
   uint32_t Distance;
   uint8_t Confidence;
} BR_PingSonar1D_MessageGetDistanceSimplePayload;

typedef struct __attribute__((packed))
{
   uint32_t Distance;
   uint8_t Confidence;
   uint16_t TransmitDuration;
   uint32_t PingNumber;
   uint32_t ScanStart;
   uint32_t ScanLength;
   uint32_t GainSetting;
} BR_PingSonar1D_MessageGetDistancePayload;

typedef struct __attribute__((packed))
{
   uint16_t ProcessorTemperature;
} BR_PingSonar1D_MessageGetProcessorTemperaturePayload;

typedef struct __attribute__((packed))
{
   uint16_t PCB_Temperature;
} BR_PingSonar1D_MessageGetPCB_TemperaturePayload;

typedef struct __attribute__((packed))
{
   uint8_t PingEnabled;
} BR_PingSonar1D_MessageGetPingEnablePayload;

typedef struct __attribute__((packed))
{
   uint32_t Distance;
   uint16_t Confidence;
   uint16_t TransmitDuration;
   uint32_t PingNumber;
   uint32_t ScanStart;
   uint32_t ScanLength;
   uint32_t GainSetting;
   uint16_t ProfileDataLength;
   uint8_t Profile[];
} BR_PingSonar1D_MessageGetProfilePayload;

/* control message type payloads */

typedef struct __attribute__((packed))
{
   uint16_t ID;
} BR_PingSonar1D_MessageContinuousStartPayload;

typedef struct __attribute__((packed))
{
   uint16_t ID;
} BR_PingSonar1D_MessageContinuousStopPayload;
