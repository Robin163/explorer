/**
 ***********************************************************************************
 * @file  fonts.h
 * @brief Definitions for current setting.
 * @author Robin Li
 * @version V1.0.0
 * @date 09/05/2018
 ************************************************************************************
 * <h2><center>&copy; COPYRIGHT 2018 DEFA TECHNOLOGY (WUXI) CO.,LTD.</center></h2>
 ************************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  __FONTS_H
#define  __FONTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint-gcc.h>


typedef struct
{
    const uint8_t *tableIndex;
    uint8_t height;
    uint8_t width;
} Font;

extern Font font16x8;
extern Font font32x16;
/*=============================================================
**Constant vector define
**===========================================================*/
/* Exported types ------------------------------------------------------------*/
//extern const uint8_t g_Ascii16x8Table[];
//extern const uint8_t g_Ascii32x16Table[];

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}/*extern C*/
#endif

#endif  /* __FONTS_H */
/* =============== (C) COPYRIGHT 2018 DEFA TECHNOLOGY ===== End Of File ===== */

