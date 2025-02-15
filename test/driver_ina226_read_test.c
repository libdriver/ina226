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
 * @file      driver_ina226_read_test.c
 * @brief     driver ina226 read test source file
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

#include "driver_ina226_read_test.h"

static ina226_handle_t gs_handle;        /**< ina226 handle */

/**
 * @brief     read test
 * @param[in] addr_pin iic device address
 * @param[in] r extern resistance
 * @param[in] times test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t ina226_read_test(ina226_address_t addr_pin, double r, uint32_t times)
{
    uint8_t res;
    uint32_t i;
    uint16_t calibration;
    ina226_info_t info;
    
    /* link interface function */
    DRIVER_INA226_LINK_INIT(&gs_handle, ina226_handle_t);
    DRIVER_INA226_LINK_IIC_INIT(&gs_handle, ina226_interface_iic_init);
    DRIVER_INA226_LINK_IIC_DEINIT(&gs_handle, ina226_interface_iic_deinit);
    DRIVER_INA226_LINK_IIC_READ(&gs_handle, ina226_interface_iic_read);
    DRIVER_INA226_LINK_IIC_WRITE(&gs_handle, ina226_interface_iic_write);
    DRIVER_INA226_LINK_DELAY_MS(&gs_handle, ina226_interface_delay_ms);
    DRIVER_INA226_LINK_DEBUG_PRINT(&gs_handle, ina226_interface_debug_print);
    DRIVER_INA226_LINK_RECEIVE_CALLBACK(&gs_handle, ina226_interface_receive_callback);
    
    /* get information */
    res = ina226_info(&info);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        ina226_interface_debug_print("ina226: chip is %s.\n", info.chip_name);
        ina226_interface_debug_print("ina226: manufacturer is %s.\n", info.manufacturer_name);
        ina226_interface_debug_print("ina226: interface is %s.\n", info.interface);
        ina226_interface_debug_print("ina226: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ina226_interface_debug_print("ina226: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ina226_interface_debug_print("ina226: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ina226_interface_debug_print("ina226: max current is %0.2fmA.\n", info.max_current_ma);
        ina226_interface_debug_print("ina226: max temperature is %0.1fC.\n", info.temperature_max);
        ina226_interface_debug_print("ina226: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start read test */
    ina226_interface_debug_print("ina226: start read test.\n");
    
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
    
    /* set average mode 16 */
    res = ina226_set_average_mode(&gs_handle, INA226_AVG_16);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set bus voltage conversion time 1.1ms */
    res = ina226_set_bus_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_1P1_MS);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set shunt voltage conversion time 1.1ms */
    res = ina226_set_shunt_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_1P1_MS);
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
    
    /* set shunt bus voltage continuous */
    res = ina226_set_mode(&gs_handle, INA226_MODE_SHUNT_BUS_VOLTAGE_CONTINUOUS);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    /* shunt bus voltage continuous test */
    ina226_interface_debug_print("ina226: shunt bus voltage continuous test.\n");
    
    /* delay 1000ms */
    ina226_interface_delay_ms(1000);
    
    for (i = 0; i < times; i++)
    {
        int16_t s_raw;
        uint16_t u_raw;
        float m;
        
        /* read shunt voltage */
        res = ina226_read_shunt_voltage(&gs_handle, (int16_t *)&s_raw, (float *)&m);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: read shunt voltage failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        ina226_interface_debug_print("ina226: shunt voltage is %0.3fmV.\n", m);
        
        /* read bus voltage */
        res = ina226_read_bus_voltage(&gs_handle, (uint16_t *)&u_raw, (float *)&m);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: read bus voltage failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        ina226_interface_debug_print("ina226: bus voltage is %0.3fmV.\n", m);
        
        /* read current */
        res = ina226_read_current(&gs_handle, (int16_t *)&s_raw, (float *)&m);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: read current failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        ina226_interface_debug_print("ina226: current is %0.3fmA.\n", m);
        
        /* read power */
        res = ina226_read_power(&gs_handle, (uint16_t *)&u_raw, (float *)&m);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: read power failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        ina226_interface_debug_print("ina226: power is %0.3fmW.\n", m);
        
        /* delay 1000ms */
        ina226_interface_delay_ms(1000);
    }
    
    /* set power down */
    res = ina226_set_mode(&gs_handle, INA226_MODE_POWER_DOWN);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    /* shunt and bus triggered test */
    ina226_interface_debug_print("ina226: shunt and bus triggered test.\n");
    
    /* delay 1000ms */
    ina226_interface_delay_ms(1000);
    
    for (i = 0; i < times; i++)
    {
        int16_t s_raw;
        uint16_t u_raw;
        float m;
        
        /* set shunt and bus triggered */
        res = ina226_set_mode(&gs_handle, INA226_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mode failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* read shunt voltage */
        res = ina226_read_shunt_voltage(&gs_handle, (int16_t *)&s_raw, (float *)&m);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: read shunt voltage failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        ina226_interface_debug_print("ina226: shunt voltage is %0.3fmV.\n", m);
        
        /* read bus voltage */
        res = ina226_read_bus_voltage(&gs_handle, (uint16_t *)&u_raw, (float *)&m);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: read bus voltage failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        ina226_interface_debug_print("ina226: bus voltage is %0.3fmV.\n", m);
        
        /* read current */
        res = ina226_read_current(&gs_handle, (int16_t *)&s_raw, (float *)&m);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: read current failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        ina226_interface_debug_print("ina226: current is %0.3fmA.\n", m);
        
        /* read power */
        res = ina226_read_power(&gs_handle, (uint16_t *)&u_raw, (float *)&m);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: read power failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        ina226_interface_debug_print("ina226: power is %0.3fmW.\n", m);
        
        /* delay 1000ms */
        ina226_interface_delay_ms(1000);
    }
    
    /* finish read test */
    (void)ina226_deinit(&gs_handle);
    ina226_interface_debug_print("ina226: finish read test.\n");
    
    return 0;
}
