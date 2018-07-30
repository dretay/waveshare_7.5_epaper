#pragma once

#include "WAVESHARE75.h"
#include "gfx.h"
#include "main.h"
#include "stdbool.h"

#if defined(STM32F407xx)
#include "stm32f4xx_hal.h"
#endif

#include "string.h"
#include "dwt_stm32_delay.h"



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

//pin assignments
#define RST_Pin GPIO_PIN_1
#define RST_GPIO_Port GPIOA
#define DC_Pin GPIO_PIN_2
#define DC_GPIO_Port GPIOA
#define BUSY_Pin GPIO_PIN_3
#define BUSY_GPIO_Port GPIOA
#define SPI_CS_Pin GPIO_PIN_4
#define SPI_CS_GPIO_Port GPIOA

#ifndef GDISP_SCREEN_HEIGHT
#define GDISP_SCREEN_HEIGHT		384
#endif
#ifndef GDISP_SCREEN_WIDTH
#define GDISP_SCREEN_WIDTH		640
#endif

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

static GFXINLINE bool init_board(GDisplay* g) {
	(void) g; 
	
	pins[CS_PIN] = cs_pin;
	pins[RST_PIN] = rst_pin;
	pins[DC_PIN] = dc_pin;
	pins[BUSY_PIN] = busy_pin;	

	return true;

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


static GFXINLINE void acquire_bus(GDisplay* g)
{
	(void) g;
}

static GFXINLINE void release_bus(GDisplay* g)
{
	(void) g;
}