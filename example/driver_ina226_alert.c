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

#include "driver_ina226_alert.h"

static ina226_handle_t gs_handle;        /**< ina226 handle */

/**
 * @brief  alert irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t ina226_alert_irq_handler(void)
{
    if (ina226_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

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
uint8_t ina226_alert_init(ina226_address_t addr_pin, double r, ina226_mask_t mask, float threshold, void (*callback)(uint8_t type))
{
    uint8_t res;
    uint16_t reg;
    uint16_t calibration;
    
    /* link interface function */
    DRIVER_INA226_LINK_INIT(&gs_handle, ina226_handle_t);
    DRIVER_INA226_LINK_IIC_INIT(&gs_handle, ina226_interface_iic_init);
    DRIVER_INA226_LINK_IIC_DEINIT(&gs_handle, ina226_interface_iic_deinit);
    DRIVER_INA226_LINK_IIC_READ(&gs_handle, ina226_interface_iic_read);
    DRIVER_INA226_LINK_IIC_WRITE(&gs_handle, ina226_interface_iic_write);
    DRIVER_INA226_LINK_DELAY_MS(&gs_handle, ina226_interface_delay_ms);
    DRIVER_INA226_LINK_DEBUG_PRINT(&gs_handle, ina226_interface_debug_print);
    DRIVER_INA226_LINK_RECEIVE_CALLBACK(&gs_handle, callback);
    
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
    res = ina226_set_average_mode(&gs_handle, INA226_ALERT_DEFAULT_AVG_MODE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default bus voltage conversion time */
    res = ina226_set_bus_voltage_conversion_time(&gs_handle, INA226_ALERT_DEFAULT_BUS_VOLTAGE_CONVERSION_TIME);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default shunt voltage conversion time */
    res = ina226_set_shunt_voltage_conversion_time(&gs_handle, INA226_ALERT_DEFAULT_SHUNT_VOLTAGE_CONVERSION_TIME);
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
    
    /* set default conversion ready alert pin */
    res = ina226_set_conversion_ready_alert_pin(&gs_handle, INA226_ALERT_DEFAULT_READY_ALERT_PIN);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set conversion ready alert pin failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default alert polarity pin */
    res = ina226_set_alert_polarity_pin(&gs_handle, INA226_ALERT_DEFAULT_ALERT_POLARITY_NORMAL);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set alert polarity pin failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default alert latch */
    res = ina226_set_alert_latch(&gs_handle, INA226_ALERT_DEFAULT_ALERT_LATCH);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set alert latch failed.\n");
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
    
    /* set mask */
    if (mask == INA226_MASK_SHUNT_VOLTAGE_OVER_VOLTAGE)
    {
        /* shunt voltage convert to register */
        res = ina226_shunt_voltage_convert_to_register(&gs_handle, threshold, &reg);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: shunt voltage convert to register failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set alert limit */
        res = ina226_set_alert_limit(&gs_handle, reg);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set alert limit failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* enable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_OVER_VOLTAGE, INA226_BOOL_TRUE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_UNDER_VOLTAGE, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_OVER_VOLTAGE, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_UNDER_VOLTAGE, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_POWER_OVER_LIMIT, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
    }
    else if (mask == INA226_MASK_SHUNT_VOLTAGE_UNDER_VOLTAGE)
    {
        /* shunt voltage convert to register */
        res = ina226_shunt_voltage_convert_to_register(&gs_handle, threshold, &reg);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: shunt voltage convert to register failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set alert limit */
        res = ina226_set_alert_limit(&gs_handle, reg);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set alert limit failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_OVER_VOLTAGE, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* enable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_UNDER_VOLTAGE, INA226_BOOL_TRUE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_OVER_VOLTAGE, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_UNDER_VOLTAGE, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_POWER_OVER_LIMIT, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
    }
    else if (mask == INA226_MASK_BUS_VOLTAGE_OVER_VOLTAGE)
    {
        /* bus voltage convert to register */
        res = ina226_bus_voltage_convert_to_register(&gs_handle, threshold, &reg);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: shunt voltage convert to register failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set alert limit */
        res = ina226_set_alert_limit(&gs_handle, reg);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set alert limit failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_OVER_VOLTAGE, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_UNDER_VOLTAGE, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* enable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_OVER_VOLTAGE, INA226_BOOL_TRUE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_UNDER_VOLTAGE, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_POWER_OVER_LIMIT, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
    }
    else if (mask == INA226_MASK_BUS_VOLTAGE_UNDER_VOLTAGE)
    {
        /* bus voltage convert to register */
        res = ina226_bus_voltage_convert_to_register(&gs_handle, threshold, &reg);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: shunt voltage convert to register failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set alert limit */
        res = ina226_set_alert_limit(&gs_handle, reg);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set alert limit failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_OVER_VOLTAGE, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_UNDER_VOLTAGE, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_OVER_VOLTAGE, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* enable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_UNDER_VOLTAGE, INA226_BOOL_TRUE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_POWER_OVER_LIMIT, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
    }
    else
    {
        /* power convert to register */
        res = ina226_power_convert_to_register(&gs_handle, threshold, &reg);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: shunt voltage convert to register failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set alert limit */
        res = ina226_set_alert_limit(&gs_handle, reg);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set alert limit failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_OVER_VOLTAGE, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_UNDER_VOLTAGE, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_OVER_VOLTAGE, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* disable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_UNDER_VOLTAGE, INA226_BOOL_FALSE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
        
        /* enable */
        res = ina226_set_mask(&gs_handle, INA226_MASK_POWER_OVER_LIMIT, INA226_BOOL_TRUE);
        if (res != 0)
        {
            ina226_interface_debug_print("ina226: set mask failed.\n");
            (void)ina226_deinit(&gs_handle);
            
            return 1;
        }
    }
    
    return 0;
}

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
uint8_t ina226_alert_read(float *mV, float *mA, float *mW)
{
    uint8_t res;
    int16_t s_raw;
    uint16_t u_raw;
    
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
 * @brief  alert example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ina226_alert_deinit(void)
{
    uint8_t res;
    
    res = ina226_deinit(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}
