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
 * @file      driver_ina226_alert.c
 * @brief     driver ina226 alert source file
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

#ifndef DRIVER_INA226_ALERT_H
#define DRIVER_INA226_ALERT_H

#include "driver_ina226_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup ina226_example_driver
 * @{
 */

/**
 * @brief ina226 alert example default definition
 */
#define INA226_ALERT_DEFAULT_AVG_MODE                             INA226_AVG_16                        /**< 16 averages */
#define INA226_ALERT_DEFAULT_BUS_VOLTAGE_CONVERSION_TIME          INA226_CONVERSION_TIME_1P1_MS        /**< bus voltage conversion time 1.1 ms */
#define INA226_ALERT_DEFAULT_SHUNT_VOLTAGE_CONVERSION_TIME        INA226_CONVERSION_TIME_1P1_MS        /**< shunt voltage conversion time 1.1 ms */
#define INA226_ALERT_DEFAULT_READY_ALERT_PIN                      INA226_BOOL_FALSE                    /**< disable ready alert pin */
#define INA226_ALERT_DEFAULT_ALERT_POLARITY_NORMAL                INA226_ALERT_POLARITY_NORMAL         /**< alert polarity normal */
#define INA226_ALERT_DEFAULT_ALERT_LATCH                          INA226_BOOL_FALSE                    /**< disable alert latch */

/**
 * @brief  alert irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t ina226_alert_irq_handler(void);

/**
 * @brief     alert example init
 * @param[in] addr_pin iic address pin
 * @param[in] r reference resistor value
 * @param[in] mask set mask
 * @param[in] threshold set threshold
 * @param[in] *callback pointer to a function address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t ina226_alert_init(ina226_address_t addr_pin, double r, ina226_mask_t mask, float threshold, void (*callback)(uint8_t type));

/**
 * @brief  alert example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ina226_alert_deinit(void);

/**
 * @brief      alert example read
 * @param[out] *mV pointer to a mV buffer
 * @param[out] *mA pointer to a mA buffer
 * @param[out] *mW pointer to a mW buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ina226_alert_read(float *mV, float *mA, float *mW);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
