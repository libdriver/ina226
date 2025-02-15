/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_ina226.h
 * @brief     driver ina226 header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2025-01-29
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2025/01/29  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_INA226_H
#define DRIVER_INA226_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ina226_driver ina226 driver function
 * @brief    ina226 driver modules
 * @{
 */

/**
 * @addtogroup ina226_basic_driver
 * @{
 */

/**
 * @brief ina226 read timeout definition
 */
#ifndef INA226_READ_TIMEOUT
    #define INA226_READ_TIMEOUT    1000        /**< 1000ms */
#endif

/**
 * @brief ina226 address enumeration definition
 */
typedef enum
{
    INA226_ADDRESS_0 = (0x40 << 1),        /**< A0 = GND, A1 = GND */
    INA226_ADDRESS_1 = (0x41 << 1),        /**< A0 = VS+, A1 = GND */
    INA226_ADDRESS_2 = (0x42 << 1),        /**< A0 = SDA, A1 = GND */
    INA226_ADDRESS_3 = (0x43 << 1),        /**< A0 = SCL, A1 = GND */
    INA226_ADDRESS_4 = (0x44 << 1),        /**< A0 = GND, A1 = VS+ */
    INA226_ADDRESS_5 = (0x45 << 1),        /**< A0 = VS+, A1 = VS+ */
    INA226_ADDRESS_6 = (0x46 << 1),        /**< A0 = SDA, A1 = VS+ */
    INA226_ADDRESS_7 = (0x47 << 1),        /**< A0 = SCL, A1 = VS+ */
    INA226_ADDRESS_8 = (0x48 << 1),        /**< A0 = GND, A1 = SDA */
    INA226_ADDRESS_9 = (0x49 << 1),        /**< A0 = VS+, A1 = SDA */
    INA226_ADDRESS_A = (0x4A << 1),        /**< A0 = SDA, A1 = SDA */
    INA226_ADDRESS_B = (0x4B << 1),        /**< A0 = SCL, A1 = SDA */
    INA226_ADDRESS_C = (0x4C << 1),        /**< A0 = GND, A1 = SCL */
    INA226_ADDRESS_D = (0x4D << 1),        /**< A0 = VS+, A1 = SCL */
    INA226_ADDRESS_E = (0x4E << 1),        /**< A0 = SDA, A1 = SCL */
    INA226_ADDRESS_F = (0x4F << 1)         /**< A0 = SCL, A1 = SCL */
} ina226_address_t;

/**
 * @brief ina226 bool enumeration definition
 */
typedef enum
{
    INA226_BOOL_FALSE = 0x00,        /**< false */
    INA226_BOOL_TRUE  = 0x01,        /**< true */
} ina226_bool_t;

/**
 * @brief ina226 average enumeration definition
 */
typedef enum
{
    INA226_AVG_1    = 0,        /**< 1 average */
    INA226_AVG_4    = 1,        /**< 4 averages */
    INA226_AVG_16   = 2,        /**< 16 averages */
    INA226_AVG_64   = 3,        /**< 64 averages */
    INA226_AVG_128  = 4,        /**< 128 averages */
    INA226_AVG_256  = 5,        /**< 256 averages */
    INA226_AVG_512  = 6,        /**< 512 averages */
    INA226_AVG_1024 = 7,        /**< 1024 averages */
} ina226_avg_t;

typedef enum
{
    INA226_CONVERSION_TIME_140_US   = 0,        /**< 140 us */
    INA226_CONVERSION_TIME_204_US   = 1,        /**< 204 us */
    INA226_CONVERSION_TIME_332_US   = 2,        /**< 332 us */
    INA226_CONVERSION_TIME_588_US   = 3,        /**< 588 us */
    INA226_CONVERSION_TIME_1P1_MS   = 4,        /**< 1.1 ms */
    INA226_CONVERSION_TIME_2P116_MS = 5,        /**< 2.116 ms */
    INA226_CONVERSION_TIME_4P156_MS = 6,        /**< 4.156 ms */
    INA226_CONVERSION_TIME_8P244_MS = 7,        /**< 8.244 ms */
} ina226_conversion_time_t;

/**
 * @brief ina226 mode enumeration definition
 */
typedef enum
{
    INA226_MODE_POWER_DOWN                   = 0x0,        /**< power down */
    INA226_MODE_SHUNT_VOLTAGE_TRIGGERED      = 0x1,        /**< shunt voltage triggered */
    INA226_MODE_BUS_VOLTAGE_TRIGGERED        = 0x2,        /**< bus voltage triggered */
    INA226_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED  = 0x3,        /**< shunt and bus triggered */
    INA226_MODE_SHUTDOWN                     = 0x4,        /**< shutdown */
    INA226_MODE_SHUNT_VOLTAGE_CONTINUOUS     = 0x5,        /**< shunt voltage continuous */
    INA226_MODE_BUS_VOLTAGE_CONTINUOUS       = 0x6,        /**< bus voltage continuous */
    INA226_MODE_SHUNT_BUS_VOLTAGE_CONTINUOUS = 0x7,        /**< shunt and bus voltage continuous */
} ina226_mode_t;

/**
 * @brief ina226 status enumeration definition
 */
typedef enum
{
    INA226_STATUS_SHUNT_VOLTAGE_OVER_VOLTAGE  = 15,        /**< shunt voltage over voltage */
    INA226_STATUS_SHUNT_VOLTAGE_UNDER_VOLTAGE = 14,        /**< shunt voltage under voltage */
    INA226_STATUS_BUS_VOLTAGE_OVER_VOLTAGE    = 13,        /**< bus voltage over voltage */
    INA226_STATUS_BUS_VOLTAGE_UNDER_VOLTAGE   = 12,        /**< bus voltage under voltage */
    INA226_STATUS_POWER_OVER_LIMIT            = 11,        /**< power over limit */
} ina226_status_t;

/**
 * @brief ina226 mask enumeration definition
 */
typedef enum
{
    INA226_MASK_SHUNT_VOLTAGE_OVER_VOLTAGE  = 15,        /**< shunt voltage over voltage */
    INA226_MASK_SHUNT_VOLTAGE_UNDER_VOLTAGE = 14,        /**< shunt voltage under voltage */
    INA226_MASK_BUS_VOLTAGE_OVER_VOLTAGE    = 13,        /**< bus voltage over voltage */
    INA226_MASK_BUS_VOLTAGE_UNDER_VOLTAGE   = 12,        /**< bus voltage under voltage */
    INA226_MASK_POWER_OVER_LIMIT            = 11,        /**< power over limit */
} ina226_mask_t;

/**
 * @brief ina226 alert polarity enumeration definition
 */
typedef enum
{
    INA226_ALERT_POLARITY_NORMAL   = 0,        /**< active low open collector */
    INA226_ALERT_POLARITY_INVERTED = 1,        /**< active high open collector */
} ina226_alert_polarity_t;

/**
 * @brief ina226 handle structure definition
 */
typedef struct ina226_handle_s
{
    uint8_t iic_addr;                                                                   /**< iic device address */
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    void (*receive_callback)(uint8_t type);                                             /**< point to a receive_callback function address */
    double r;                                                                           /**< resistance */
    double current_lsb;                                                                 /**< current lsb */
    uint8_t inited;                                                                     /**< inited flag */
    uint8_t trigger;                                                                    /**< trigger flag */
} ina226_handle_t;

/**
 * @brief ina226 information structure definition
 */
typedef struct ina226_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} ina226_info_t;

/**
 * @}
 */

/**
 * @defgroup ina226_link_driver ina226 link driver function
 * @brief    ina226 link driver modules
 * @ingroup  ina226_driver
 * @{
 */

/**
 * @brief     initialize ina226_handle_t structure
 * @param[in] HANDLE pointer to an ina226 handle structure
 * @param[in] STRUCTURE ina226_handle_t
 * @note      none
 */
#define DRIVER_INA226_LINK_INIT(HANDLE, STRUCTURE)         memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE pointer to an ina226 handle structure
 * @param[in] FUC pointer to an iic_init function address
 * @note      none
 */
#define DRIVER_INA226_LINK_IIC_INIT(HANDLE, FUC)          (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE pointer to an ina226 handle structure
 * @param[in] FUC pointer to an iic_deinit function address
 * @note      none
 */
#define DRIVER_INA226_LINK_IIC_DEINIT(HANDLE, FUC)        (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE pointer to an ina226 handle structure
 * @param[in] FUC pointer to an iic_read function address
 * @note      none
 */
#define DRIVER_INA226_LINK_IIC_READ(HANDLE, FUC)          (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE pointer to an ina226 handle structure
 * @param[in] FUC pointer to an iic_write function address
 * @note      none
 */
#define DRIVER_INA226_LINK_IIC_WRITE(HANDLE, FUC)         (HANDLE)->iic_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to an ina226 handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_INA226_LINK_DELAY_MS(HANDLE, FUC)          (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to an ina226 handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_INA226_LINK_DEBUG_PRINT(HANDLE, FUC)       (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE pointer to an ina226 handle structure
 * @param[in] FUC pointer to a receive_callback function address
 * @note      none
 */
#define DRIVER_INA226_LINK_RECEIVE_CALLBACK(HANDLE, FUC)  (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup ina226_basic_driver ina226 basic driver function
 * @brief    ina226 basic driver modules
 * @ingroup  ina226_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info pointer to an ina226 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ina226_info(ina226_info_t *info);

/**
 * @brief     set the iic address pin
 * @param[in] *handle pointer to an ina226 handle structure
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ina226_set_addr_pin(ina226_handle_t *handle, ina226_address_t addr_pin);

/**
 * @brief      get the iic address pin
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *addr_pin pointer to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ina226_get_addr_pin(ina226_handle_t *handle, ina226_address_t *addr_pin);

/**
 * @brief     set the resistance
 * @param[in] *handle pointer to an ina226 handle structure
 * @param[in] resistance current sampling resistance value
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ina226_set_resistance(ina226_handle_t *handle, double resistance);

/**
 * @brief      get the resistance
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *resistance pointer to a current sampling resistance value buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ina226_get_resistance(ina226_handle_t *handle, double *resistance);

/**
 * @brief     irq handler
 * @param[in] *handle pointer to an ina226 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina226_irq_handler(ina226_handle_t *handle);

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to an ina226 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 id is invalid
 *            - 5 soft reset failed
 * @note      none
 */
uint8_t ina226_init(ina226_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to an ina226 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 * @note      none
 */
uint8_t ina226_deinit(ina226_handle_t *handle);

/**
 * @brief     soft reset the chip
 * @param[in] *handle pointer to an ina226 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 soft reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina226_soft_reset(ina226_handle_t *handle);

/**
 * @brief     set average mode
 * @param[in] *handle pointer to an ina226 handle structure
 * @param[in] mode average mode
 * @return    status code
 *            - 0 success
 *            - 1 set average mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina226_set_average_mode(ina226_handle_t *handle, ina226_avg_t mode);

/**
 * @brief      get average mode
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *mode pointer to an average mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get average mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_get_average_mode(ina226_handle_t *handle, ina226_avg_t *mode);

/**
 * @brief     set bus voltage conversion time
 * @param[in] *handle pointer to an ina226 handle structure
 * @param[in] t conversion time
 * @return    status code
 *            - 0 success
 *            - 1 set bus voltage conversion time failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina226_set_bus_voltage_conversion_time(ina226_handle_t *handle, ina226_conversion_time_t t);

/**
 * @brief      get bus voltage conversion time
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *t pointer to a conversion time buffer
 * @return     status code
 *             - 0 success
 *             - 1 get bus voltage conversion time failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_get_bus_voltage_conversion_time(ina226_handle_t *handle, ina226_conversion_time_t *t);

/**
 * @brief     set shunt voltage conversion time
 * @param[in] *handle pointer to an ina226 handle structure
 * @param[in] t conversion time
 * @return    status code
 *            - 0 success
 *            - 1 set shunt voltage conversion time failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina226_set_shunt_voltage_conversion_time(ina226_handle_t *handle, ina226_conversion_time_t t);

/**
 * @brief      get shunt voltage conversion time
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *t pointer to a conversion time buffer
 * @return     status code
 *             - 0 success
 *             - 1 get shunt voltage conversion time failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_get_shunt_voltage_conversion_time(ina226_handle_t *handle, ina226_conversion_time_t *t);

/**
 * @brief     set the mode
 * @param[in] *handle pointer to an ina226 handle structure
 * @param[in] mode chip mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina226_set_mode(ina226_handle_t *handle, ina226_mode_t mode);

/**
 * @brief      get the mode
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *mode pointer to a chip mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_get_mode(ina226_handle_t *handle, ina226_mode_t *mode);

/**
 * @brief      read the shunt voltage
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *raw pointer to raw data buffer
 * @param[out] *mV pointer to converted data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read shunt voltage failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 math overflow
 *             - 5 read timeout
 * @note       none
 */
uint8_t ina226_read_shunt_voltage(ina226_handle_t *handle, int16_t *raw, float *mV);

/**
 * @brief      read the bus voltage
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *raw pointer to raw data buffer
 * @param[out] *mV pointer to converted data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read bus voltage failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 math overflow
 *             - 5 read timeout
 * @note       none
 */
uint8_t ina226_read_bus_voltage(ina226_handle_t *handle, uint16_t *raw, float *mV);

/**
 * @brief      read the power
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *raw pointer to raw data buffer
 * @param[out] *mW pointer to converted data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read power failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 math overflow
 *             - 5 read timeout
 * @note       none
 */
uint8_t ina226_read_power(ina226_handle_t *handle, uint16_t *raw, float *mW);

/**
 * @brief      read the current
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *raw pointer to raw data buffer
 * @param[out] *mA pointer to converted data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read current failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 math overflow
 *             - 5 read timeout
 * @note       none
 */
uint8_t ina226_read_current(ina226_handle_t *handle, int16_t *raw, float *mA);

/**
 * @brief     set the calibration
 * @param[in] *handle pointer to an ina226 handle structure
 * @param[in] data calibration data
 * @return    status code
 *            - 0 success
 *            - 1 set calibration failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina226_set_calibration(ina226_handle_t *handle, uint16_t data);

/**
 * @brief      get the calibration
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *data pointer to a calibration data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get calibration failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_get_calibration(ina226_handle_t *handle, uint16_t *data);

/**
 * @brief      calculate the calibration
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *calibration pointer to a calibration data buffer
 * @return     status code
 *             - 0 success
 *             - 1 calculate calibration failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 r can't be zero
 * @note       none
 */
uint8_t ina226_calculate_calibration(ina226_handle_t *handle, uint16_t *calibration);

/**
 * @brief     enable or disable mask
 * @param[in] *handle pointer to an ina226 handle structure
 * @param[in] mask set mask
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set mask failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina226_set_mask(ina226_handle_t *handle, ina226_mask_t mask, ina226_bool_t enable);

/**
 * @brief      get mask
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[in]  mask set mask
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mask failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_get_mask(ina226_handle_t *handle, ina226_mask_t mask, ina226_bool_t *enable);

/**
 * @brief     enable or disable conversion ready alert pin
 * @param[in] *handle pointer to an ina226 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set conversion ready alert pin failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina226_set_conversion_ready_alert_pin(ina226_handle_t *handle, ina226_bool_t enable);

/**
 * @brief      get conversion ready alert pin status
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get conversion ready alert pin failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_get_conversion_ready_alert_pin(ina226_handle_t *handle, ina226_bool_t *enable);

/**
 * @brief     set alert polarity pin
 * @param[in] *handle pointer to an ina226 handle structure
 * @param[in] pin alert polarity pin
 * @return    status code
 *            - 0 success
 *            - 1 set alert polarity pin failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina226_set_alert_polarity_pin(ina226_handle_t *handle, ina226_alert_polarity_t pin);

/**
 * @brief      get alert polarity pin
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *pin pointer to an alert polarity pin buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alert polarity pin failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_get_alert_polarity_pin(ina226_handle_t *handle, ina226_alert_polarity_t *pin);

/**
 * @brief     enable or disable alert latch
 * @param[in] *handle pointer to an ina226 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set alert latch failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina226_set_alert_latch(ina226_handle_t *handle, ina226_bool_t enable);

/**
 * @brief      get alert latch status
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alert latch failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_get_alert_latch(ina226_handle_t *handle, ina226_bool_t *enable);

/**
 * @brief     set alert limit
 * @param[in] *handle pointer to an ina226 handle structure
 * @param[in] reg set register
 * @return    status code
 *            - 0 success
 *            - 1 set alert limit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina226_set_alert_limit(ina226_handle_t *handle, uint16_t reg);

/**
 * @brief      get alert limit
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *reg pointer to a register buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alert limit failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_get_alert_limit(ina226_handle_t *handle, uint16_t *reg);

/**
 * @brief      convert the shunt voltage to the register raw data
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[in]  mV millivolt
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_shunt_voltage_convert_to_register(ina226_handle_t *handle, float mV, uint16_t *reg);

/**
 * @brief      convert the register raw data to the shunt voltage
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[in]  reg register raw data
 * @param[out] *mV pointer to a millivolt buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_shunt_voltage_convert_to_data(ina226_handle_t *handle, uint16_t reg, float *mV);

/**
 * @brief      convert the bus voltage to the register raw data
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[in]  mV millivolt
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_bus_voltage_convert_to_register(ina226_handle_t *handle, float mV, uint16_t *reg);

/**
 * @brief      convert the register raw data to the bus voltage
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[in]  reg register raw data
 * @param[out] *mV pointer to a millivolt buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_bus_voltage_convert_to_data(ina226_handle_t *handle, uint16_t reg, float *mV);

/**
 * @brief      convert the power to the register raw data
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[in]  mW milliwatt
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_power_convert_to_register(ina226_handle_t *handle, float mW, uint16_t *reg);

/**
 * @brief      convert the register raw data to the power
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[in]  reg register raw data
 * @param[out] *mW pointer to a milliwatt buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_power_convert_to_data(ina226_handle_t *handle, uint16_t reg, float *mW);

/**
 * @brief      get the die id
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *device_id pointer to a device id buffer
 * @param[out] *die_revision_id pointer to a die revision id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get die id failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_get_die_id(ina226_handle_t *handle, uint16_t *device_id, uint8_t *die_revision_id);

/**
 * @}
 */

/**
 * @defgroup ina226_extern_driver ina226 extern driver function
 * @brief    ina226 extern driver modules
 * @ingroup  ina226_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle pointer to an ina226 handle structure
 * @param[in] reg register address
 * @param[in] data written data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina226_set_reg(ina226_handle_t *handle, uint8_t reg, uint16_t data);

/**
 * @brief      get the chip register
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[in]  reg register address
 * @param[out] *data pointer to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina226_get_reg(ina226_handle_t *handle, uint8_t reg, uint16_t *data);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
