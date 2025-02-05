/**
 * Copyright (C) 2019 Bosch Sensortec GmbH
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holder nor the names of the
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 *
 * The information provided is believed to be accurate and reliable.
 * The copyright holder assumes no responsibility
 * for the consequences of use
 * of such information nor for any infringement of patents or
 * other rights of third parties which may result from its use.
 * No license is granted by implication or otherwise under any patent or
 * patent rights of the copyright holder.
 *
 * @file	bmi2_ois.h
 * @date	2019-5-28
 * @version	v2.19.0
 *
 */

#ifndef BMI2_OIS_H_
#define BMI2_OIS_H_

/*! CPP guard */
#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************/

/*!             Header files
 ****************************************************************************/
#ifdef __KERNEL__
#include <linux/types.h>
#include <linux/kernel.h>
#else
#include <stdint.h>
#include <stddef.h>
#endif

/******************************************************************************/
/*!  @name          Macros                                        */
/******************************************************************************/
/*! @name  Utility macros */
#define BMI2_OIS_SET_BITS(reg_data, bitname, data) \
    ((reg_data & ~(bitname##_MASK)) | \
     ((data << bitname##_POS) & bitname##_MASK))

#define BMI2_OIS_GET_BITS(reg_data, bitname) \
    ((reg_data & (bitname##_MASK)) >> \
     (bitname##_POS))

/*! @name To define success code */
#define BMI2_OIS_OK                UINT8_C(0)

/*! @name To define error codes */
#define BMI2_OIS_E_NULL_PTR        INT8_C(-1)
#define BMI2_OIS_E_COM_FAIL        INT8_C(-2)
#define BMI2_OIS_E_INVALID_SENSOR  INT8_C(-8)

/*! @name Mask definitions for SPI read/write address for OIS */
#define BMI2_OIS_SPI_RD_MASK       UINT8_C(0x80)
#define BMI2_OIS_SPI_WR_MASK       UINT8_C(0x7F)

/*! @name BMI2 OIS data bytes */
#define BMI2_OIS_ACC_GYR_NUM_BYTES UINT8_C(6)

/*!  @name Macros to select sensor for OIS data read */
#define BMI2_OIS_ACCEL             UINT8_C(0x01)
#define BMI2_OIS_GYRO              UINT8_C(0x02)

/*!  @name Macros to define OIS register addresses */
#define BMI2_OIS_CONFIG_ADDR       UINT8_C(0x40)
#define BMI2_OIS_ACC_X_LSB_ADDR    UINT8_C(0x0C)
#define BMI2_OIS_GYR_X_LSB_ADDR    UINT8_C(0x12)

/*! @name Mask definitions for OIS configurations */
#define BMI2_OIS_GYR_EN_MASK       UINT8_C(0x40)
#define BMI2_OIS_ACC_EN_MASK       UINT8_C(0x80)

/*! @name Bit Positions for OIS configurations */
#define BMI2_OIS_GYR_EN_POS        UINT8_C(0x06)
#define BMI2_OIS_ACC_EN_POS        UINT8_C(0x07)

/******************************************************************************/
/*! @name           Function Pointers                             */
/******************************************************************************/
/*! For interfacing to the I2C or SPI read and write functions */
typedef int8_t (*bmi2_ois_com_fptr_t)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);

/*! For interfacing to the delay function */
typedef void (*bmi2_ois_delay_fptr_t)(uint32_t period);

/******************************************************************************/
/*!  @name         Structure Declarations                             */
/******************************************************************************/
/*! @name Structure to define accelerometer and gyroscope sensor axes for OIS */
struct bmi2_ois_sens_axes_data
{
    /*! Data in x-axis */
    int16_t x;

    /*! Data in y-axis */
    int16_t y;

    /*! Data in z-axis */
    int16_t z;

};

/*!  @name Structure to define bmi2 OIS sensor configurations */
struct bmi2_ois_dev
{
    /*! Device id of BMI2 */
    uint8_t dev_id;

    /*! Read function pointer */
    bmi2_ois_com_fptr_t ois_read;

    /*! Write function pointer */
    bmi2_ois_com_fptr_t ois_write;

    /*!  Delay function pointer */
    bmi2_ois_delay_fptr_t ois_delay_ms;

    /*! Accelerometer enable for OIS */
    uint8_t acc_en;

    /*! Gyroscope enable for OIS */
    uint8_t gyr_en;

    /*! Accelerometer data axes */
    struct bmi2_ois_sens_axes_data acc_data;

    /*! Gyroscope data axes */
    struct bmi2_ois_sens_axes_data gyr_data;

};

/***************************************************************************/

/*!     BMI2 OIS User Interface function prototypes
 ****************************************************************************/

/*!
 * @brief This API reads the data from the given OIS register address of bmi2
 * sensor.
 *
 * @param[in]  ois_reg_addr : OIS register address from which data is read.
 * @param[out] ois_reg_data : Pointer to data buffer where read data is stored.
 * @param[in]  data_len     : No. of bytes of data to be read.
 * @param[in]  ois_dev      : Structure instance of bmi2_ois_dev.
 *
 * @return Result of API execution status
 * @retval zero -> Success / -ve value -> Error
 */
int8_t bmi2_get_ois_regs(uint8_t ois_reg_addr,
                         uint8_t *ois_reg_data,
                         uint16_t data_len,
                         const struct bmi2_ois_dev *ois_dev);

/*!
 * @brief This API writes data to the given OIS register address of bmi2 sensor.
 *
 * @param[in] ois_reg_addr  : OIS register address to which the data is written.
 * @param[in] ois_reg_data  : Pointer to data buffer in which data to be written
 *                is stored.
 * @param[in] data_len      : No. of bytes of data to be written.
 * @param[in] ois_dev       : Structure instance of bmi2_ois_dev.
 *
 * @return Result of API execution status
 * @retval zero -> Success / -ve value -> Error
 */
int8_t bmi2_set_ois_regs(uint8_t ois_reg_addr,
                         uint8_t *ois_reg_data,
                         uint16_t data_len,
                         const struct bmi2_ois_dev *ois_dev);

/*!
 * @brief This API enables/disables accelerometer/gyroscope data read through
 * OIS interface.
 *
 * @param[in] ois_dev   : Structure instance of bmi2_ois_dev.
 *
 * @return Result of API execution status
 * @retval zero -> Success / -ve value -> Error
 */
int8_t bmi2_set_ois_config(const struct bmi2_ois_dev *ois_dev);

/*!
 * @brief This API gets the status of accelerometer/gyroscope enable for data
 * read through OIS interface.
 *
 * @param[in, out] ois_dev : Structure instance of bmi2_ois_dev.
 *
 * @note Enabling and disabling is done during OIS structure initialization.
 *
 * @return Result of API execution status
 * @retval zero -> Success / -ve value -> Error
 */
int8_t bmi2_get_ois_config(struct bmi2_ois_dev *ois_dev);

/*!
 * @brief This API reads accelerometer/gyroscope data through OIS interface.
 *
 * @param[in] sens_sel          : Select sensor whose data is to be read.
 * @param[in] n_sens            : Number of sensors selected.
 * @param[in, out] ois_dev      : Structure instance of bmi2_ois_dev.
 * @param[in] gyr_cross_sens_zx : Store the gyroscope cross sensitivity values taken from the bmi2xy(refer bmi2_ois
 * example).
 *
 *  sens_sel      |   Value
 * ---------------|---------------
 * BMI2_OIS_ACCEL |   0x01
 * BMI2_OIS_GYRO  |   0x02
 *
 * @return Result of API execution status
 * @retval zero -> Success / -ve value -> Error.
 */
int8_t bmi2_read_ois_data(const uint8_t *sens_sel,
                          uint8_t n_sens,
                          struct bmi2_ois_dev *ois_dev,
                          int16_t gyr_cross_sens_zx);

/******************************************************************************/
/*! @name               C++ Guard Macros                              */
/******************************************************************************/
#ifdef __cplusplus
}
#endif /* End of CPP guard */

#endif /* BMI2_OIS_H_ */
