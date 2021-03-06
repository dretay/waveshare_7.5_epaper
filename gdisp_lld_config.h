/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

#ifndef _GDISP_LLD_CONFIG_H
#define _GDISP_LLD_CONFIG_H

/*===========================================================================*/
/* Driver hardware support.                                                  */
/*===========================================================================*/

#define GDISP_HARDWARE_PIXELREAD		FALSE
#define GDISP_HARDWARE_DRAWPIXEL        TRUE
//#define GDISP_LLD_PIXELFORMAT			GDISP_PIXELFORMAT_GRAY4
#define GDISP_LLD_PIXELFORMAT			GDISP_PIXELFORMAT_MONO
#define GDISP_HARDWARE_FLUSH		TRUE
#define GDISP_HARDWARE_CONTROL TRUE
#endif	/* _GDISP_LLD_CONFIG_H */
