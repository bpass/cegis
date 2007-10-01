#ifndef MAPIMGDATATYPES_H
#define MAPIMGDATATYPES_H

#define Q_UINT8_MAX      0xFF
#define Q_UINT8_MIN      0

#define Q_INT8_MAX         127
#define Q_INT8_MIN       (-Q_INT8_MAX -1)

#define Q_UINT16_MAX      0xFFFF
#define Q_UINT16_MIN      0

#define Q_INT16_MAX         32767
#define Q_INT16_MIN       (-Q_INT16_MAX -1)

#define Q_UINT32_MAX      0xFFFFFFFF
#define Q_UINT32_MIN      0

#define Q_INT32_MAX         2147483647
#define Q_INT32_MIN       (-Q_INT32_MAX - 1)

#if (defined (Q_OS_WIN32) || defined (Q_OS_WIN64)) && defined (Q_CC_MSVC)
       #define Q_UINT64_MAX      0xFFFFFFFFFFFFFFFFi64
       #define Q_UINT64_MIN      0
       
       #define Q_INT64_MAX         9223372036854775807i64
       #define Q_INT64_MIN       (-Q_INT64_MAX - 1i64)
#else
       #define Q_UINT64_MAX      0xFFFFFFFFFFFFFFFFLL
       #define Q_UINT64_MIN      0
       
       #define Q_INT64_MAX         9223372036854775807LL
       #define Q_INT64_MIN       (-Q_INT64_MAX - 1LL)
#endif

#define FLOAT_MAX         3.402823466e+38F
#define FLOAT_MIN       (-FLOAT_MAX - 1)


#define DOUBLE_MAX         1.7976931348623158e+308
#define DOUBLE_MIN        (-DOUBLE_MAX - 1)


#endif //MAPIMGDATATYPES_H
