#ifndef DATA_T_H
#define DATA_T_H

//! Enum value to differentiate between possible data types.
typedef enum {
NONE=-1
,U8 /*!< Unsigned 8-bit. */
,S8 /*!< Signed 8-bit. */
,U16 /*!< Unsigned 16-bit. */
,S16 /*!< Signed 16-bit. */
,U32 /*!< Unsigned 32-bit. */
,S32 /*!< Signed 32-bit. */
,FLOAT32 /*!< 32-bit floating point.*/
,FLOAT64 /*!< 64-bit floating point.*/
} DataType;


#endif