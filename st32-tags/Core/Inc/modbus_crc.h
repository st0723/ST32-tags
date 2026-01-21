#ifndef __MODBUS_CRC_H
#define __MODBUS_CRC_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"


uint16_t  Modbus_CRC16( uint8_t *puchMsg, uint16_t usDataLen );

#ifdef __cplusplus
}
#endif
#endif

