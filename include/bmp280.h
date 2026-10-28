#ifndef __BMP280_H__
#define __BMP280_H__

#include "stm32f0xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/*
    For I2C communication:
        0x77 if SDO high
        0x76 if SDO low
*/

#define BMP280_I2C_ADDRESS_0 0x76
#define BMP280_I2C_ADDRESS_1 0x77

#define BMP280_CHIP_ID 0x58 // chip ID

/*
    Operation Modes:
        Forced - measurement done at user command
        Normal - Continues measurement
*/

typedef enum {
    BMP280_MODE_SLEEP = 0,
    BMP280_MODE_FORCED = 1,
    BMP280_MODE_NORMAL = 3
} BMP280_Mode;

typedef enum {
    BMP280_FILTER_OFF = 0,
    BMP280_FILTER_2 = 1,
    BMP280_FILTER_4 = 2,
    BMP280_FILTER_8 = 3,
    BMP280_FILTER_16 = 4
} BMP280_Filter;

/*
    Pressure oversampling
    Adjust the oversampling settings
*/

typedef enum {
    BMP280_SKIPPED = 0,         // no oversampling
    BMP280_ULTRA_LOW_POWER = 1, // x1 oversampling
    BMP280_LOW_POWER = 2,       // x2 oversampling
    BMP280_STANDART = 3,        // x4 oversampling
    BMP280_HIGH_RES = 4,        // x8 oversampling
    BMP280_ULTRA_HIGH_RES = 5   // x16 oversampling
} BMP280_Oversampling;

/*
    Stand by time 
*/

typedef enum {
    BMP280_STANDBY_05 = 0,      // 0.5 [ms]
    BMP280_STANDBY_62 = 1,      // 62.5 [ms]
    BMP280_STANDBY_125 = 2,     // 125 [ms]
    BMP280_STANDBY_250 = 3,     // 250 [ms]
    BMP280_STANDBY_500 = 4,     // 500 [ms]
    BMP280_STANDBY_1000 = 5,    // 1 [s]
    BMP280_STANDBY_2000 = 6,    // 2 [s]
    BMP280_STANDBY_4000 = 7     // 4 [s]
} BMP280_StandbyTime;

/*
    Configuration parameters
*/

typedef struct {
    BMP280_Mode mode;
    BMP280_Filter filter;
    BMP280_Oversampling oversampling_pressure;
    BMP280_Oversampling oversampling_temperature;
    BMP280_StandbyTime standby;
} bmp280_params_t;

typedef struct {

    /*
        Temperature variables
    */

    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;

    /*
        Pressure variables
    */

    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t dig_P3;
    int16_t  dig_P4;
    int16_t dig_P5;
    int16_t  dig_P6;
    int16_t dig_P7;
    int16_t  dig_P8;
    int16_t dig_P9;
    

    uint16_t addr;

    I2C_HandleTypeDef* i2c;

    bmp280_params_t params;

    uint8_t id;

}   BMP280_HandleTypedef;


/*
    Default parameters

        mode: NORMAL
        filter: OFF
        oversampling: x4(STANDART)
        standby time: 250 [ms]
*/

void bmp280_init_default_params(bmp280_params_t *params);

/*
    BMP280 Initialisation
        - probes for device
        - soft resets the device
        - read calibration constants
        - configure the device
*/


bool bmp280_init(BMP280_HandleTypedef *dev, bmp280_params_t *params);

/**
 *  Start measurement in forced mode
 *          - means we start the measurements
 *  
 *  The module remains in forced mode after this function call
 *  Don`t call the funtion in normal mode
*/

bool bmp280_force_measurement(BMP280_HandleTypedef *dev);

/**
 *  Check if BMP280 is busy with measuring temperature/pressure
 *  True if BMP280 is busy 
*/

bool bmp280_is_measuring(BMP280_HandleTypedef *dev);


/**
 * Read temperature and pressure data
 *  
 *      Temperature - degrees Celsius * 100
 * 
 *      Pressure in Pascal
 *              - fixed point
 *              - 24 bit integer, 8 bit fraction format 
 * 
*/



bool bmp280_read_fixed(BMP280_HandleTypedef *dev, 
                        int32_t *temperature, uint32_t *pressure);


/**
 *  Read temperature and pressude data:
 *      
 *      Temperature - degrees Celsius
 * 
 *      Pressure - Pascals
 * 
 * In floating point
*/

bool bmp280_read_float(BMP280_HandleTypedef *dev,
                        float *temperature, float *pressure);


#endif // __BMP280_H__