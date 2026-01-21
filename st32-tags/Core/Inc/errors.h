#ifndef __ERRORS_H__
#define __ERRORS_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

enum
{
    ALERT_NO_ERROR,                   		// 无错误
	ALERT_LACK_TAG,                			// 缺标警告
	ALERT_SPILL_TAG,                		// 漏标警告
	ALERT_OPERATION_TIMEOUT,				// 运转超时警告	
	ALERT_OVER_TEMPERATUER,					// 过温警告	
	ALERT_LOCKED_ROTOR,						// 堵转警告	
	ALERT_STORAG_FAULT,						// 存储故障	
	ALERT_TAPEBREAK,						// 断带警告
	ALERT_LABEL_COUNT,						// 标签数预警
    ENUM_ALERT_ID_COUNT               		// 告警ID计数
};
typedef BYTE ENUM_ALERT_ID;

#ifdef __cplusplus
}
#endif

#endif 


