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
 * @file      driver_ina226_shot.c
 * @brief     driver ina226 shot source file
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

#include "driver_ina226_shot.h"

static ina226_handle_t gs_handle;        /**< ina226 handle */

/**
 * @brief     shot example init
 * @param[in] addr_pin iic address pin
 * @param[in] r reference resistor value
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t ina226_shot_init(ina226_address_t addr_pin, double r)
{
    uint8_t res;
    uint16_t calibration;
    
    /* link interface function */
    DRIVER_INA226_LINK_INIT(&gs_handle, ina226_handle_t);
    DRIVER_INA226_LINK_IIC_INIT(&gs_handle, ina226_interface_iic_init);
    DRIVER_INA226_LINK_IIC_DEINIT(&gs_handle, ina226_interface_iic_deinit);
    DRIVER_INA226_LINK_IIC_READ(&gs_handle, ina226_interface_iic_read);
    DRIVER_INA226_LINK_IIC_WRITE(&gs_handle, ina226_interface_iic_write);
    DRIVER_INA226_LINK_DELAY_MS(&gs_handle, ina226_interface_delay_ms);
    DRIVER_INA226_LINK_DEBUG_PRINT(&gs_handle, ina226_interface_debug_print);
    DRIVER_INA226_LINK_RECEIVE_CALLBACK(&gs_handle, ina226_interface_receive_callback);
    
    /* set addr pin */
    res = ina226_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }

    /* set the r */
    res = ina226_set_resistance(&gs_handle, r);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set resistance failed.\n");
       
        return 1;
    }
    
    /* init */
    res = ina226_init(&gs_handle);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: init failed.\n");
       
        return 1;
    }
    
    /* set default average mode */
    res = ina226_set_average_mode(&gs_handle, INA226_SHOT_DEFAULT_AVG_MODE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default bus voltage conversion time */
    res = ina226_set_bus_voltage_conversion_time(&gs_handle, INA226_SHOT_DEFAULT_BUS_VOLTAGE_CONVERSION_TIME);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default shunt voltage conversion time */
    res = ina226_set_shunt_voltage_conversion_time(&gs_handle, INA226_SHOT_DEFAULT_SHUNT_VOLTAGE_CONVERSION_TIME);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set shunt voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    /* calculate calibration */
    res = ina226_calculate_calibration(&gs_handle, (uint16_t *)&calibration);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: calculate calibration failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    res = ina226_set_calibration(&gs_handle, calibration);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set calibration failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set power down */
    res = ina226_set_mode(&gs_handle, INA226_MODE_POWER_DOWN);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief      shot example read
 * @param[out] *mV pointer to a mV buffer
 * @param[out] *mA pointer to a mA buffer
 * @param[out] *mW pointer to a mW buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ina226_shot_read(float *mV, float *mA, float *mW)
{
    uint8_t res;
    int16_t s_raw;
    uint16_t u_raw;
    
    /* set shunt bus voltage triggered */
    res = ina226_set_mode(&gs_handle, INA226_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED);
    if (res != 0)
    {
        return 1;
    }
    
    /* read bus voltage */
    res = ina226_read_bus_voltage(&gs_handle, (uint16_t *)&u_raw, mV);
    if (res != 0)
    {
        return 1;
    }
    
    /* read current */
    res = ina226_read_current(&gs_handle, (int16_t *)&s_raw, mA);
    if (res != 0)
    {
        return 1;
    }
    
    /* read power */
    res = ina226_read_power(&gs_handle, (uint16_t *)&u_raw, mW);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  shot example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ina226_shot_deinit(void)
{
    uint8_t res;
    
    res = ina226_deinit(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}
