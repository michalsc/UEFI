#ifndef INCLUDE_EFI_TIME_H_
#define INCLUDE_EFI_TIME_H_

#include <efi/types.h>

//*******************************************************
// EFI_TIME
//*******************************************************
// This represents the current time information
typedef struct
{
    UINT16 Year;
    UINT8 Month;
    UINT8 Day;
    UINT8 Hour;
    UINT8 Minute;
    UINT8 Second; // 1900 – 9999
    // 1 – 12
    // 1 – 31
    // 0 – 23
    // 0 – 59
    // 0 – 59
    UINT8 Pad1;
    UINT32 Nanosecond; // 0 – 999,999,999
    INT16 TimeZone;    // -1440 to 1440 or 2047
    UINT8 Daylight;
    UINT8 Pad2;
} EFI_TIME;

//*******************************************************
// Bit Definitions for EFI_TIME.Daylight. See below.
//*******************************************************
#define EFI_TIME_ADJUST_DAYLIGHT 0x01
#define EFI_TIME_IN_DAYLIGHT 0x02
//*******************************************************
// Value Definition for EFI_TIME.TimeZone. See below.
//*******************************************************
#define EFI_UNSPECIFIED_TIMEZONE 0x07FF

//*******************************************************
// EFI_TIME_CAPABILITIES
//*******************************************************
// This provides the capabilities of the
// real time clock device as exposed through the EFI interfaces.
typedef struct
{
    UINT32 Resolution;
    UINT32 Accuracy;
    BOOLEAN SetsToZero;
} EFI_TIME_CAPABILITIES;

//*******************************************************
// EFI_TIMER_DELAY
//*******************************************************
typedef enum
{
    TimerCancel,
    TimerPeriodic,
    TimerRelative
} EFI_TIMER_DELAY;

#endif // INCLUDE_EFI_TIME_H_
