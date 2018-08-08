#pragma once

#include <stdint.h>

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

