#pragma once

#include "gfx.h"
#include "main.h"
#include "stdbool.h"

#if defined(STM32F407xx)
#include "stm32f4xx_hal.h"
#endif

#include "string.h"
#include "dwt_stm32_delay.h"

// EPD basic instructions
const uint8_t PANEL_SETTING = 0x00;
const uint8_t POWER_SETTING = 0x01;
const uint8_t POWER_OFF = 0x02;
const uint8_t POWER_OFF_SEQUENCE_SETTING = 0x03;
const uint8_t POWER_ON = 0x04;
const uint8_t POWER_ON_MEASURE = 0x05;
const uint8_t BOOSTER_SOFT_START = 0x06;
const uint8_t DEEP_SLEEP = 0x07;
const uint8_t DATA_START_TRANSMISSION_1 = 0x10;
const uint8_t DATA_STOP = 0x11;
const uint8_t DISPLAY_REFRESH = 0x12;
const uint8_t IMAGE_PROCESS = 0x13;
const uint8_t LUT_FOR_VCOM = 0x20;
const uint8_t LUT_BLUE = 0x21;
const uint8_t LUT_WHITE = 0x22;
const uint8_t LUT_GRAY_1 = 0x23;
const uint8_t LUT_GRAY_2 = 0x24;
const uint8_t LUT_RED_0 = 0x25;
const uint8_t LUT_RED_1 = 0x26;
const uint8_t LUT_RED_2 = 0x27;
const uint8_t LUT_RED_3 = 0x28;
const uint8_t LUT_XON = 0x29;
const uint8_t PLL_CONTROL = 0x30;
const uint8_t TEMPERATURE_SENSOR_COMMAND = 0x40;
const uint8_t TEMPERATURE_CALIBRATION = 0x41;
const uint8_t TEMPERATURE_SENSOR_WRITE = 0x42;
const uint8_t TEMPERATURE_SENSOR_READ = 0x43;
const uint8_t VCOM_AND_DATA_INTERVAL_SETTING = 0x50;
const uint8_t LOW_POWER_DETECTION = 0x51;
const uint8_t TCON_SETTING = 0x60;
const uint8_t TCON_RESOLUTION = 0x61;
const uint8_t SPI_FLASH_CONTROL = 0x65;
const uint8_t REVISION = 0x70;
const uint8_t GET_STATUS = 0x71;
const uint8_t AUTO_MEASUREMENT_VCOM = 0x80;
const uint8_t READ_VCOM_VALUE = 0x81;
const uint8_t VCM_DC_SETTING = 0x82;
const uint8_t LUT_WHITE_TO_WHITE = 0x21;
const uint8_t LUT_BLACK_TO_WHITE = 0x22;
const uint8_t LUT_WHITE_TO_BLACK = 0x23;
const uint8_t LUT_BLACK_TO_BLACK = 0x24;
const uint8_t PARTIAL_WINDOW = 0x90;
const uint8_t PARTIAL_IN = 0x91;
const uint8_t PARTIAL_OUT = 0x92;
const uint8_t DATA_START_TRANSMISSION_2 = 0x13;


//structures
enum PixelMode {
	PixelClear = 0,    // clear the pixel(s)
	PixelSet   = 1,      // set the pixel(s)
	PixelFlip  = 2      // invert the pixel(s)
};
typedef struct {
	GPIO_TypeDef* port;
	int pin;
} Board_Pin;


//framebuffer setup
#ifndef GDISP_SCREEN_HEIGHT
#define GDISP_SCREEN_HEIGHT		384
#endif
#ifndef GDISP_SCREEN_WIDTH
#define GDISP_SCREEN_WIDTH		640
#endif
uint8_t frame_buffer[(GDISP_SCREEN_HEIGHT * GDISP_SCREEN_WIDTH) / 8];   

//pin assignments
#define RST_Pin GPIO_PIN_1
#define RST_GPIO_Port GPIOA
#define DC_Pin GPIO_PIN_2
#define DC_GPIO_Port GPIOA
#define BUSY_Pin GPIO_PIN_3
#define BUSY_GPIO_Port GPIOA
#define SPI_CS_Pin GPIO_PIN_4
#define SPI_CS_GPIO_Port GPIOA

Board_Pin cs_pin = {
	SPI_CS_GPIO_Port,
	SPI_CS_Pin,
};
Board_Pin rst_pin = {
	RST_GPIO_Port,
	RST_Pin,
};
Board_Pin dc_pin = {
	DC_GPIO_Port,
	DC_Pin,
};
Board_Pin busy_pin = {
	BUSY_GPIO_Port,
	BUSY_Pin,
};
Board_Pin pins[4];

//spi handle
extern SPI_HandleTypeDef hspi1;

//constants
const uint8_t CS_PIN = 0;
const uint8_t RST_PIN = 1;
const uint8_t DC_PIN = 2;
const uint8_t BUSY_PIN = 3;


const unsigned char lut_vcom0[] =
{
0x40, 0x17, 0x00, 0x00, 0x00, 0x02,        
0x00, 0x17, 0x17, 0x00, 0x00, 0x02,        
0x00, 0x0A, 0x01, 0x00, 0x00, 0x01,        
0x00, 0x0E, 0x0E, 0x00, 0x00, 0x02,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char lut_vcom0_quick[] =
{
0x00, 0x0E, 0x00, 0x00, 0x00, 0x01,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};



const unsigned char lut_ww[] ={
0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char lut_ww_quick[] ={
0xA0, 0x0E, 0x00, 0x00, 0x00, 0x01,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


const unsigned char lut_bw[] ={
0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     
};


const unsigned char lut_bw_quick[] ={
0xA0, 0x0E, 0x00, 0x00, 0x00, 0x01,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     
};

const unsigned char lut_bb[] ={
0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
};

const unsigned char lut_bb_quick[] ={
0x50, 0x0E, 0x00, 0x00, 0x00, 0x01,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     
};


const unsigned char lut_wb[] ={
0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,         
};

const unsigned char lut_wb_quick[] ={
0x50, 0x0E, 0x00, 0x00, 0x00, 0x01,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,         
};

static void enable(void) {
	HAL_GPIO_WritePin(pins[CS_PIN].port, pins[CS_PIN].pin, GPIO_PIN_SET); 
}
static void disable(void) {
	HAL_GPIO_WritePin(pins[CS_PIN].port, pins[CS_PIN].pin, GPIO_PIN_RESET); 
}

static void reset(void) {
	HAL_GPIO_WritePin(pins[RST_PIN].port, pins[RST_PIN].pin, GPIO_PIN_RESET); 
	HAL_Delay(200);	
	HAL_GPIO_WritePin(pins[RST_PIN].port, pins[RST_PIN].pin, GPIO_PIN_SET); 	
	HAL_Delay(200);	
}

void spiTransfer(unsigned char data) {
	HAL_GPIO_WritePin((GPIO_TypeDef*)pins[CS_PIN].port, pins[CS_PIN].pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &data, 1, 1000);
	HAL_GPIO_WritePin((GPIO_TypeDef*)pins[CS_PIN].port, pins[CS_PIN].pin, GPIO_PIN_SET);	
}

void sendCommand(uint8_t command) {
	HAL_GPIO_WritePin((GPIO_TypeDef*)pins[DC_PIN].port, pins[DC_PIN].pin, GPIO_PIN_RESET);
	spiTransfer(command);
}
void sendData(uint8_t command) {
	HAL_GPIO_WritePin((GPIO_TypeDef*)pins[DC_PIN].port, pins[DC_PIN].pin, GPIO_PIN_SET);
	spiTransfer(command);
}

void waitUntilIdle(void) {
	//0: busy, 1: idle
	while(HAL_GPIO_ReadPin(pins[BUSY_PIN].port, pins[BUSY_PIN].pin) == GPIO_PIN_RESET)
	{
	  HAL_Delay(100);
	}	
}
static void setLut(void) {
	unsigned int count;     
	sendCommand(LUT_FOR_VCOM);                             //vcom
	for(count = 0 ; count < 44 ; count++) {
		sendData(lut_vcom0[count]);
	}
    
	sendCommand(LUT_WHITE_TO_WHITE);                       //ww --
	for(count = 0 ; count < 42 ; count++) {
		sendData(lut_ww[count]);
	}   
    
	sendCommand(LUT_BLACK_TO_WHITE);                       //bw r
	for(count = 0 ; count < 42 ; count++) {
		sendData(lut_bw[count]);
	} 

	sendCommand(LUT_WHITE_TO_BLACK);                       //wb w
	for(count = 0 ; count < 42 ; count++) {
		sendData(lut_wb[count]);
	} 

	sendCommand(LUT_BLACK_TO_BLACK);                       //bb b
	for(count = 0 ; count < 42 ; count++) {
		sendData(lut_bb[count]);
	} 
}
static void setLutQuick(void) {
	unsigned int count;     
	sendCommand(LUT_FOR_VCOM);                                //vcom
	for(count = 0 ; count < 44 ; count++) {
		sendData(lut_vcom0_quick[count]);
	}
    
	sendCommand(LUT_WHITE_TO_WHITE);                          //ww --
	for(count = 0 ; count < 42 ; count++) {
		sendData(lut_ww_quick[count]);
	}   
    
	sendCommand(LUT_BLACK_TO_WHITE);                          //bw r
	for(count = 0 ; count < 42 ; count++) {
		sendData(lut_bw_quick[count]);
	} 

	sendCommand(LUT_WHITE_TO_BLACK);                          //wb w
	for(count = 0 ; count < 42 ; count++) {
		sendData(lut_wb_quick[count]);
	} 

	sendCommand(LUT_BLACK_TO_BLACK);                          //bb b
	for(count = 0 ; count < 42 ; count++) {
		sendData(lut_bb_quick[count]);
	} 
}
static void setPartialWindow(const unsigned char* buffer_black, int x, int y, int w, int l, int dtm) {
	sendCommand(PARTIAL_IN);
	sendCommand(PARTIAL_WINDOW);
	sendData(x >> 8);
	sendData(x & 0xf8);       // x should be the multiple of 8, the last 3 bit will always be ignored
	sendData(((x & 0xf8) + w  - 1) >> 8);
	sendData(((x & 0xf8) + w  - 1) | 0x07);
	sendData(y >> 8);        
	sendData(y & 0xff);
	sendData((y + l - 1) >> 8);        
	sendData((y + l - 1) & 0xff);
	sendData(0x01);           // Gates scan both inside and outside of the partial window. (default) 
  //  DelayMs(2);
    sendCommand((dtm == 1) ? DATA_START_TRANSMISSION_1 : DATA_START_TRANSMISSION_2);
	if (buffer_black != NULL) {
		for (int i = 0; i < w  / 8 * l; i++) {
			sendData(buffer_black[i]);  
		}  
	}
	else {
		for (int i = 0; i < w  / 8 * l; i++) {
			sendData(0x00);  
		}  
	}
	//   DelayMs(2);
	   sendCommand(PARTIAL_OUT);  
}
void _flush()
{
	setPartialWindow(frame_buffer, 0, 0, GDISP_SCREEN_HEIGHT, GDISP_SCREEN_WIDTH, 1);
	setLutQuick();
	sendCommand(DISPLAY_REFRESH);
}
void flush() {
	unsigned char temp1, temp2;
	sendCommand(DATA_START_TRANSMISSION_1);
	for (int i = 0; i < 30720; i++) {   
		temp1 = frame_buffer[i];
		for (unsigned char j = 0; j < 8; j++) {
			__disable_irq();
			if (temp1 & 0x80)
				temp2 = 0x03;
			else
				temp2 = 0x00;
			temp2 <<= 4;
			temp1 <<= 1;
			j++;
			if (temp1 & 0x80)
				temp2 |= 0x03;
			else
				temp2 |= 0x00;
			temp1 <<= 1;
			sendData(temp2); 
			__enable_irq();

		}
	}
//	setLut();
	sendCommand(DISPLAY_REFRESH);
	HAL_Delay(100);	
	waitUntilIdle();
}
void setPixel(unsigned int x, unsigned int y, enum PixelMode mode) {
	if (x < GDISP_SCREEN_WIDTH && y < GDISP_SCREEN_HEIGHT)
	{
		unsigned char *p = frame_buffer + ((y * (GDISP_SCREEN_WIDTH / 8)) + (x / 8));
		unsigned char mask = 0x80u >> (x % 8);
		switch (mode)
		{
		case PixelClear:
			*p &= ~mask;
			break;
		case PixelSet:
			*p |= mask;
			break;
		case PixelFlip:
			*p ^= mask;
			break;
		}
	}
}

static GFXINLINE void init_board(GDisplay* g) {
	(void) g; 
	
	pins[CS_PIN] = cs_pin;
	pins[RST_PIN] = rst_pin;
	pins[DC_PIN] = dc_pin;
	pins[BUSY_PIN] = busy_pin;

	reset();
  
	sendCommand(POWER_SETTING); 
	sendData(0x37);
	sendData(0x00);
  
	sendCommand(PANEL_SETTING);
	sendData(0xCF);
//	sendData(0x28); //this will setup expectation of LUT in the register
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
	sendData(0x02);      //source 640
	sendData(0x80);
	sendData(0x01);      //gate 384
	sendData(0x80);
  
	sendCommand(VCM_DC_SETTING);
	sendData(0x1E);       //decide by LUT file
  
	sendCommand(0xe5);            //FLASH MODE            
	sendData(0x03);  
	

}
static GFXINLINE void post_init_board(GDisplay *g) {
	(void) g;
}

#if GDISP_NEED_CONTROL
static void board_backlight(GDisplay *g, uint8_t percent) {
	// TODO: Can be an empty function if your hardware doesn't support this
	(void) g;
	(void) percent;
}

static void board_contrast(GDisplay *g, uint8_t percent) {
	// TODO: Can be an empty function if your hardware doesn't support this
	(void) g;
	(void) percent;
}

static void board_power(GDisplay *g, powermode_t pwr) {
	// TODO: Can be an empty function if your hardware doesn't support this
	(void) g;
	(void) pwr;
}
#endif /* GDISP_NEED_CONTROL */

void gdisp_lld_flush(GDisplay *g){
	(void) g;	
	flush();	
}