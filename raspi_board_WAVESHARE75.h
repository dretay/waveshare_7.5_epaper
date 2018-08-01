#pragma once	

#include "WAVESHARE75.h"
#include "gfx.h"
#include "stdbool.h"

#include "string.h"

#include <wiringPi.h>
//#include <wiringPiSPI.h>

#include <pigpio.h>


//structures
enum PixelMode {
	PixelClear = 0,    // clear the pixel(s)
	PixelSet   = 1,      // set the pixel(s)
	PixelFlip  = 2      // invert the pixel(s)
};
//typedef struct {
//	GPIO_TypeDef* port;
//	int pin;
//} Board_Pin;

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

#define	SPI_CHAN		0
static int h;

//Board_Pin cs_pin = {
//	SPI_CS_GPIO_Port,
//	SPI_CS_Pin,
//};
//Board_Pin rst_pin = {
//	RST_GPIO_Port,
//	RST_Pin,
//};
//Board_Pin dc_pin = {
//	DC_GPIO_Port,
//	DC_Pin,
//};
//Board_Pin busy_pin = {
//	BUSY_GPIO_Port,
//	BUSY_Pin,
//};
//Board_Pin pins[4];

//spi handle
//extern SPI_HandleTypeDef hspi1;

//constants
const uint8_t CS_PIN = 8;
const uint8_t RST_PIN = 17;
const uint8_t DC_PIN = 25;
const uint8_t BUSY_PIN = 24;

static void enable(void) {
	gpioWrite(CS_PIN, PI_ON);
}
static void disable(void) {
	gpioWrite(CS_PIN, PI_OFF);
}

static void reset(void) {
	gpioWrite(RST_PIN, PI_OFF);
	delay(200);
	gpioWrite(RST_PIN, PI_ON);
	delay(200);
}

void spiTransfer(unsigned char data) {
	gpioWrite(CS_PIN, PI_OFF);
    spiXfer(h, &data, NULL, 1);
	gpioWrite(CS_PIN, PI_ON);
}

void sendCommand(uint8_t command) {
	gpioWrite(DC_PIN, PI_OFF);
	spiTransfer(command);
}
void sendData(uint8_t command) {
	gpioWrite(DC_PIN, PI_ON);
	spiTransfer(command);
}

void waitUntilIdle(void) {
	//0: busy, 1: idle
	while(gpioRead(BUSY_PIN) == PI_OFF)
	{
	  delay(100);
	}	
}

static GFXINLINE bool init_board(GDisplay* g) {
	(void) g; 
	
	if(gpioInitialise() < 0) return 1;
	gpioSetMode(RST_PIN, PI_OUTPUT);
	gpioSetMode(DC_PIN, PI_OUTPUT);
	gpioSetMode(CS_PIN, PI_OUTPUT);
	gpioSetMode(BUSY_PIN, PI_INPUT);

	h = spiOpen(SPI_CHAN, 30 * 1000000, 0);

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