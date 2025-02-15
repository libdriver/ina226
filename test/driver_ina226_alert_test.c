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
 * @file      driver_ina226_alert_test.c
 * @brief     driver ina226 alert test source file
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

#include "driver_ina226_alert_test.h"

static ina226_handle_t gs_handle;      /**< ina226 handle */
static volatile uint16_t gs_flag;      /**< flag */

/**
 * @brief     interface receive callback
 * @param[in] type irq type
 * @note      none
 */
static void a_receive_callback(uint8_t type)
{
    switch (type)
    {
        case INA226_STATUS_SHUNT_VOLTAGE_OVER_VOLTAGE :
        {
            gs_flag |= 1 << 0;
            ina226_interface_debug_print("ina226: irq shunt voltage over voltage.\n");
            
            break;
        }
        case INA226_STATUS_SHUNT_VOLTAGE_UNDER_VOLTAGE :
        {
            gs_flag |= 1 << 1;
            ina226_interface_debug_print("ina226: irq shunt voltage under voltage.\n");
            
            break;
        }
        case INA226_STATUS_BUS_VOLTAGE_OVER_VOLTAGE :
        {
            gs_flag |= 1 << 2;
            ina226_interface_debug_print("ina226: irq bus voltage over voltage.\n");
            
            break;
        }
        case INA226_STATUS_BUS_VOLTAGE_UNDER_VOLTAGE :
        {
            gs_flag |= 1 << 3;
            ina226_interface_debug_print("ina226: irq bus voltage under voltage.\n");
            
            break;
        }
        case INA226_STATUS_POWER_OVER_LIMIT :
        {
            gs_flag |= 1 << 4;
            ina226_interface_debug_print("ina226: irq power over limit.\n");
            
            break;
        }
        default :
        {
            ina226_interface_debug_print("ina226: unknown code.\n");
            
            break;
        }
    }
}

/**
 * @brief  alert test irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t ina226_alert_test_irq_handler(void)
{
    if (ina226_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     alert test
 * @param[in] addr_pin iic device address
 * @param[in] r extern resistance
 * @param[in] mask set mask
 * @param[in] threshold set threshold
 * @param[in] timeout_ms timeout in ms
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t ina226_alert_test(ina226_address_t addr_pin, double r, ina226_mask_t mask, float threshold, uint32_t timeout_ms)
{
    uint8_t res;
    uint16_t reg;
    uint16_t flag;
    uint32_t timeout;
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
    DRIVER_INA226_LINK_RECEIVE_CALLBACK(&gs_handle, a_receive_callback);
    
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
    
    /* start alert test */
    ina226_interface_debug_print("ina226: start alert test.\n");
    
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
    
    /* disable conversion ready alert pin */
    res = ina226_set_conversion_ready_alert_pin(&gs_handle, INA226_BOOL_FALSE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set conversion ready alert pin failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alert polarity pin normal */
    res = ina226_set_alert_polarity_pin(&gs_handle, INA226_ALERT_POLARITY_NORMAL);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set alert polarity pin failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable alert latch */
    res = ina226_set_alert_latch(&gs_handle, INA226_BOOL_FALSE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set alert latch failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    gs_flag = 0;
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
        flag = 1 << 0;
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
        flag = 1 << 1;
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
        flag = 1 << 2;
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
        flag = 1 << 3;
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
        flag = 1 << 4;
    }
    
    timeout = timeout_ms;
    while (timeout != 0)
    {
        if ((gs_flag & flag) != 0)
        {
            break;
        }
        timeout--;
        ina226_interface_delay_ms(1);
    }
    
    /* check timeout */
    if (timeout == 0)
    {
        ina226_interface_debug_print("ina226: alert timeout.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish alert test */
    (void)ina226_deinit(&gs_handle);
    ina226_interface_debug_print("ina226: finish alert test.\n");
    
    return 0;
}
