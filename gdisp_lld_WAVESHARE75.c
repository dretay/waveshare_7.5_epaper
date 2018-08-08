#include "gfx.h"

#if GFX_USE_GDISP



#define GDISP_DRIVER_VMT			GDISPVMT_WAVESHARE75
#include "gdisp_lld_config.h"
#include "../ugfx/src/gdisp/gdisp_driver.h"
#if defined(STM32F407xx)
#include "stm32_board_WAVESHARE75.h"
#else
#include "raspi_board_WAVESHARE75.h"
#endif

/*===========================================================================
Framebuffer setup 
===========================================================================*/

#define FB_WIDTH					GDISP_SCREEN_WIDTH
#define FB_HEIGHT					GDISP_SCREEN_HEIGHT
#define FB_PAGES					1
#define FB_PIXELORDER_MSB			TRUE

/*------------------ Include Generic FB Code ------------------*/
// This FB is for 1,2 or 4 bits per pixel packed along the x-axis
#define FB_TYPE_PIXELS				(LLDCOLOR_TYPE_BITS/LLDCOLOR_BITS)
#define FB_AXIS_MASK				(FB_TYPE_PIXELS-1)
#define FB_LINE_TYPES				((FB_WIDTH+FB_TYPE_PIXELS-1)/FB_TYPE_PIXELS)
#define FB_PAGE_TYPES				(FB_LINE_TYPES*FB_HEIGHT)
#define FB_ADDR(fbp, pg, px, py)	((fbp)->fb + ((px)/FB_TYPE_PIXELS + (py)*FB_LINE_TYPES + (pg)*FB_PAGE_TYPES))
#if FB_PIXELORDER_MSB
#define FB_COLOR(px, py, c)		((c) << ((LLDCOLOR_TYPE_BITS-LLDCOLOR_BITS)-(((px) & FB_AXIS_MASK)<<(LLDCOLOR_BITS-1))))
#else
#define FB_COLOR(px, py, c)		((c) << (((px) & FB_AXIS_MASK)<<(LLDCOLOR_BITS-1)))
#endif
#define FB_FLUSH_REQUIRED(fbp)		((fbp)->fa0.x < (fbp)->fa1.x)
#define FB_FLUSH_WIDTH(fbp)			((fbp)->fa1.x - (fbp)->fa0.x + 1)
#define FB_FLUSH_HEIGHT(fbp)		((fbp)->fa1.y - (fbp)->fa0.y + 1)
#define FB_FLUSH_ALL(fbp)			{ (fbp)->fa0.x = (fbp)->fa0.y = 0; (fbp)->fa1.x = FB_WIDTH-1; (fbp)->fa1.y = FB_HEIGHT-1; }
#define FB_FLUSH_NONE(fbp)			{ (fbp)->fa0.x = FB_WIDTH; (fbp)->fa0.y = FB_HEIGHT; (fbp)->fa1.x = (fbp)->fa1.y = -1; }
#define FB_FLUSH_POINT(fbp, px, py)	{											\
			if ((px) < (fbp)->fa0.x) (fbp)->fa0.x = (px) & ~FB_AXIS_MASK;		\
			if ((px) > (fbp)->fa1.x) (fbp)->fa1.x = (px) | FB_AXIS_MASK;		\
			if ((py) < (fbp)->fa0.y) (fbp)->fa0.y = (py);						\
			if ((py) > (fbp)->fa1.y) (fbp)->fa1.y = (py);						\
		}
#define FB_SETPIXEL(fbp, pg, px, py, c)	{										\
			LLDCOLOR_TYPE	*p, oc;												\
			p = FB_ADDR((fbp), (pg), (px), (py));								\
			oc = (*p & ~FB_COLOR((px), (py), LLDCOLOR_MASK())) | FB_COLOR((px), (py), (c));	\
			if (oc != *p) { *p = oc; FB_FLUSH_POINT((fbp), (px), (py)); }		\
		}

typedef struct FBpriv {
	point_t			fa0, fa1;
	LLDCOLOR_TYPE	fb[FB_PAGE_TYPES * FB_PAGES];
} FBpriv;

// UC8173 Inernal Macros & types
typedef struct UC8173_Private {
	FBpriv		fb;
} WAVESHARE75_Private;

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

LLDSPEC bool_t gdisp_lld_init(GDisplay *g) {

	WAVESHARE75_Private	*priv;
	
	// Allocate the private area
	g->priv = gfxAlloc(sizeof(WAVESHARE75_Private));
	if (!g->priv)
		return FALSE;
	priv = (WAVESHARE75_Private *)g->priv;

	// Initialise the board interface
	if(!init_board(g))
	{
		return FALSE;
	}



	reset();
	
	acquire_bus(g);

	sendCommand(POWER_SETTING); 
	sendData(0x37);
	sendData(0x00);
  
	sendCommand(PANEL_SETTING);	
	sendData(0xCF);	//600x448, lut from flash, scan up, shift right
	sendData(0x08);		
    
	sendCommand(BOOSTER_SOFT_START);
	sendData(0xc7);     
	sendData(0xcc);
	sendData(0x28);
  
	sendCommand(POWER_ON);
	waitUntilIdle();
  
	sendCommand(PLL_CONTROL);
	sendData(0x3c);        
  
	sendCommand(TEMPERATURE_CALIBRATION);
	sendData(0x00);
  
	sendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
	sendData(0x77);
  
	sendCommand(TCON_SETTING);
	sendData(0x22);
  
	sendCommand(TCON_RESOLUTION);
	sendData(0x02);       //source 640
	sendData(0x80);
	sendData(0x01);       //gate 384
	sendData(0x80);
  
	sendCommand(VCM_DC_SETTING);
	sendData(0x1E);        //decide by LUT file
  
	sendCommand(0xe5);             //FLASH MODE            
	sendData(0x03);  	

	release_bus(g);

	// Finish Init
	post_init_board(g);

	// Initialise the GDISP structure
	g->g.Width = GDISP_SCREEN_WIDTH;
	g->g.Height = GDISP_SCREEN_HEIGHT;
	g->g.Orientation = GDISP_ROTATE_0;
	g->g.Powermode = powerOn;
	

	return TRUE;
}

#if GDISP_HARDWARE_FLUSH
LLDSPEC void gdisp_lld_flush(GDisplay* g)
{
	coord_t 		cy, cx, dx, dy;
	LLDCOLOR_TYPE	*fb;
	WAVESHARE75_Private	*priv;

	priv = (WAVESHARE75_Private *)g->priv;

	if (!FB_FLUSH_REQUIRED(&priv->fb))
		return;
		
	// Acquire the bus to communicate with the display controller
	acquire_bus(g);

	// Calculate the width, height
	FB_FLUSH_ALL(&priv->fb);
	cx = FB_FLUSH_WIDTH(&priv->fb);
	cy = FB_FLUSH_HEIGHT(&priv->fb);
		
	// Transfer the buffer
	#if GDISP_LLD_PIXELFORMAT != GDISP_PIXELFORMAT_MONO && GDISP_LLD_PIXELFORMAT != GDISP_PIXELFORMAT_GRAY4		
		#error "UC8173: Unsupported driver color format"
	#endif
	sendCommand(DATA_START_TRANSMISSION_1);
	int i = 0;
	int j = 0;
	unsigned char temp1, temp2;
	dx = (cx+FB_TYPE_PIXELS-1)/FB_TYPE_PIXELS * (LLDCOLOR_TYPE_BITS/8);
	for (fb = FB_ADDR(&priv->fb, 0, priv->fb.fa0.x, priv->fb.fa0.y), dy = cy; dy; dy--, fb += FB_LINE_TYPES)
	{
		for (i = 0; i < dx; i++) {	
			temp1 = fb[i];
			if (GDISP_LLD_PIXELFORMAT == GDISP_PIXELFORMAT_MONO) 
			{
				for (j = 0; j < 4; j++)
				{ 
					if (temp1 & 0x80)
						temp2 = 0x03;
					else
						temp2 = 0x00;
					temp2 <<= 4;
					temp1 <<= 1;
						
					if (temp1 & 0x80)
						temp2 |= 0x03;
					else
						temp2 |= 0x00;
					temp1 <<= 1;
					sendData(temp2); 
				}
			}
			else
			{
				for (j = 0; j < 2; j++) { 
					temp2 = (temp1 & 0xC0)>>2;
					temp1 <<= 2;
					temp2 |= (temp1 & 0xC0)>>6;
					temp1 <<= 2;
					sendData(temp2); 
				}					
			}		
		}			
	}
	sendCommand(DISPLAY_REFRESH);
	waitUntilIdle();
		
	// Release the bus
	release_bus(g);

	// Mark as flushed
	FB_FLUSH_NONE(&priv->fb);
}
#endif
#if GDISP_HARDWARE_PIXELREAD || GDISP_HARDWARE_DRAWPIXEL

static void translate_coordinates(GDisplay* g, coord_t *_x, coord_t *_y)
{	
	coord_t			x, y;	
	// Handle the different possible orientations
	#if GDISP_NEED_CONTROL && GDISP_HARDWARE_CONTROL
		switch(g->g.Orientation) {
			default:
			case GDISP_ROTATE_0:
				#if FB_REVERSEAXIS_X
					x = FB_WIDTH - 1 - g->p.x;
				#else
					x = g->p.x;
				#endif
				#if FB_REVERSEAXIS_Y
					y = FB_HEIGHT - 1 - g->p.y;
				#else
					y = g->p.y;
				#endif
				break;
			case GDISP_ROTATE_90:
				#if FB_REVERSEAXIS_X
						x = FB_WIDTH - 1 - g->p.y;
				#else
						x = g->p.y;
				#endif
				#if FB_REVERSEAXIS_Y
						y = g->p.x;
				#else
						y = FB_HEIGHT - 1 - g->p.x;
				#endif
				break;
			case GDISP_ROTATE_180:
				#if FB_REVERSEAXIS_X
						x = g->p.x;
				#else
						x = FB_WIDTH - 1 - g->p.x;
				#endif
				#if FB_REVERSEAXIS_Y
						y = g->p.y;
				#else
						y = FB_HEIGHT - 1 - g->p.y;
				#endif
				break;
			case GDISP_ROTATE_270:
				#if FB_REVERSEAXIS_X
						x = g->p.y;
				#else
						x = FB_WIDTH - 1 - g->p.y;
				#endif
				#if FB_REVERSEAXIS_Y
						y = FB_HEIGHT - 1 - g->p.x;
				#else
						y = g->p.x;
				#endif
				break;
		}
	#else
		#if FB_REVERSEAXIS_X
			x = FB_WIDTH - 1 - g->p.x;
		#else
			x = g->p.x;
		#endif
		#if FB_REVERSEAXIS_Y
			y = FB_HEIGHT - 1 - g->p.y;
		#else
			y = g->p.y;
		#endif
	#endif

	*_x = x;
	*_y = y;
}
#endif

#if GDISP_HARDWARE_DRAWPIXEL
LLDSPEC void gdisp_lld_draw_pixel(GDisplay* g)
{
	coord_t			x, y;
	WAVESHARE75_Private	*priv;
	priv = (WAVESHARE75_Private *)g->priv;
	translate_coordinates(g, &x, &y);	
		
	// Modify the framebuffer content
	int addr = (x) / FB_TYPE_PIXELS + (y)*FB_LINE_TYPES;
	unsigned char color = gdispColor2Native(g->p.color);
	FB_SETPIXEL(&priv->fb, 0, x, y, color);
}
#endif

#if GDISP_NEED_CONTROL
LLDSPEC void gdisp_lld_control(GDisplay *g) {
	switch (g->p.x) {
	case GDISP_CONTROL_POWER:
		if (g->g.Powermode == (powermode_t)g->p.ptr)
			return;
		switch ((powermode_t)g->p.ptr) {
		case powerOff:			
			break;
		case powerSleep:
			break;
		case powerDeepSleep:			
			break;
		case powerOn:			
			break;
		default:
			return;
		}
		g->g.Powermode = (powermode_t)g->p.ptr;
		return;

	case GDISP_CONTROL_ORIENTATION:
		if (g->g.Orientation == (orientation_t)g->p.ptr)
			return;
		switch ((orientation_t)g->p.ptr) {
		case GDISP_ROTATE_0:
		case GDISP_ROTATE_180:
			if (g->g.Orientation == GDISP_ROTATE_90 || g->g.Orientation == GDISP_ROTATE_270) {
				coord_t		tmp;

				tmp = g->g.Width;
				g->g.Width = g->g.Height;
				g->g.Height = tmp;
			}
			break;
		case GDISP_ROTATE_90:
		case GDISP_ROTATE_270:
			if (g->g.Orientation == GDISP_ROTATE_0 || g->g.Orientation == GDISP_ROTATE_180) {
				coord_t		tmp;

				tmp = g->g.Width;
				g->g.Width = g->g.Height;
				g->g.Height = tmp;
			}
			break;
		default:
			return;
		}
		g->g.Orientation = (orientation_t)g->p.ptr;
		return;

	case GDISP_CONTROL_BACKLIGHT:
		return;

	case GDISP_CONTROL_CONTRAST:		
		return;
	}
}
#endif /* GDISP_NEED_CONTROL */

#endif /* GFX_USE_GDISP */
