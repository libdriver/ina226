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
 * @file      driver_ina226_register_test.c
 * @brief     driver ina226 register test source file
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

#include "driver_ina226_register_test.h"
#include <stdlib.h>

static ina226_handle_t gs_handle;        /**< ina226 handle */

/**
 * @brief     register test
 * @param[in] addr_pin iic device address
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t ina226_register_test(ina226_address_t addr_pin)
{
    uint8_t res;
    uint8_t die_revision_id;
    float f;
    float f_check;
    double r;
    double r_check;
    uint16_t data;
    uint16_t data_check;
    uint16_t device_id;
    ina226_info_t info;
    ina226_address_t addr;
    ina226_avg_t mode;
    ina226_conversion_time_t t;
    ina226_mode_t chip_mode;
    ina226_bool_t enable;
    ina226_alert_polarity_t pin;
    
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
    
    /* start register test */
    ina226_interface_debug_print("ina226: start register test.\n");
    
    /* ina226_set_addr_pin/ina226_get_addr_pin test */
    ina226_interface_debug_print("ina226: ina226_set_addr_pin/ina226_get_addr_pin test.\n");
    
    /* set address 0 */
    res = ina226_set_addr_pin(&gs_handle, INA226_ADDRESS_0);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: set addr pin 0.\n");
    res = ina226_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: check addr pin %s.\n", addr == INA226_ADDRESS_0 ? "ok" : "error");
    
    /* set address 1 */
    res = ina226_set_addr_pin(&gs_handle, INA226_ADDRESS_1);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: set addr pin 1.\n");
    res = ina226_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: check addr pin %s.\n", addr == INA226_ADDRESS_1 ? "ok" : "error");
    
    /* set address 2 */
    res = ina226_set_addr_pin(&gs_handle, INA226_ADDRESS_2);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: set addr pin 2.\n");
    res = ina226_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: check addr pin %s.\n", addr == INA226_ADDRESS_2 ? "ok" : "error");
    
    /* set address 3 */
    res = ina226_set_addr_pin(&gs_handle, INA226_ADDRESS_3);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: set addr pin 3.\n");
    res = ina226_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: check addr pin %s.\n", addr == INA226_ADDRESS_3 ? "ok" : "error");
    
    /* set address 4 */
    res = ina226_set_addr_pin(&gs_handle, INA226_ADDRESS_4);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: set addr pin 4.\n");
    res = ina226_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: check addr pin %s.\n", addr == INA226_ADDRESS_4 ? "ok" : "error");
    
    /* set address 5 */
    res = ina226_set_addr_pin(&gs_handle, INA226_ADDRESS_5);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: set addr pin 5.\n");
    res = ina226_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: check addr pin %s.\n", addr == INA226_ADDRESS_5 ? "ok" : "error");
    
    /* set address 6 */
    res = ina226_set_addr_pin(&gs_handle, INA226_ADDRESS_6);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: set addr pin 6.\n");
    res = ina226_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: check addr pin %s.\n", addr == INA226_ADDRESS_6 ? "ok" : "error");
    
    /* set address 7 */
    res = ina226_set_addr_pin(&gs_handle, INA226_ADDRESS_7);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: set addr pin 7.\n");
    res = ina226_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: check addr pin %s.\n", addr == INA226_ADDRESS_7 ? "ok" : "error");
    
    /* set address 8 */
    res = ina226_set_addr_pin(&gs_handle, INA226_ADDRESS_8);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: set addr pin 8.\n");
    res = ina226_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: check addr pin %s.\n", addr == INA226_ADDRESS_8 ? "ok" : "error");
    
    /* set address 9 */
    res = ina226_set_addr_pin(&gs_handle, INA226_ADDRESS_9);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: set addr pin 9.\n");
    res = ina226_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: check addr pin %s.\n", addr == INA226_ADDRESS_9 ? "ok" : "error");
    
    /* set address 10 */
    res = ina226_set_addr_pin(&gs_handle, INA226_ADDRESS_A);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: set addr pin 10.\n");
    res = ina226_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: check addr pin %s.\n", addr == INA226_ADDRESS_A ? "ok" : "error");
    
    /* set address 11 */
    res = ina226_set_addr_pin(&gs_handle, INA226_ADDRESS_B);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: set addr pin 11.\n");
    res = ina226_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: check addr pin %s.\n", addr == INA226_ADDRESS_B ? "ok" : "error");
    
    /* set address 12 */
    res = ina226_set_addr_pin(&gs_handle, INA226_ADDRESS_C);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: set addr pin 12.\n");
    res = ina226_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: check addr pin %s.\n", addr == INA226_ADDRESS_C ? "ok" : "error");
    
    /* set address 13 */
    res = ina226_set_addr_pin(&gs_handle, INA226_ADDRESS_D);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: set addr pin 13.\n");
    res = ina226_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: check addr pin %s.\n", addr == INA226_ADDRESS_D ? "ok" : "error");
    
    /* set address 14 */
    res = ina226_set_addr_pin(&gs_handle, INA226_ADDRESS_E);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: set addr pin 14.\n");
    res = ina226_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: check addr pin %s.\n", addr == INA226_ADDRESS_E ? "ok" : "error");
    
    /* set address 15 */
    res = ina226_set_addr_pin(&gs_handle, INA226_ADDRESS_F);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: set addr pin 15.\n");
    res = ina226_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get addr pin failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: check addr pin %s.\n", addr == INA226_ADDRESS_F ? "ok" : "error");
    
    /* ina226_set_resistance/ina226_get_resistance test */
    ina226_interface_debug_print("ina226: ina226_set_resistance/ina226_get_resistance test.\n");
    
    /* generate the r */
    r = (double)(rand() % 100) / 1000.0;
    res = ina226_set_resistance(&gs_handle, r);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set resistance failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: set resistance %f.\n", r);
    res = ina226_get_resistance(&gs_handle, (double *)&r_check);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get resistance failed.\n");
       
        return 1;
    }
    ina226_interface_debug_print("ina226: check resistance %f.\n", r_check);
    
    /* set addr pin */
    res = ina226_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set addr pin failed.\n");
       
        return 1;
    }
    
    /* init */
    res = ina226_init(&gs_handle);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: init failed.\n");
       
        return 1;
    }
    
    /* ina226_set_average_mode/ina226_get_average_mode test */
    ina226_interface_debug_print("ina226: ina226_set_average_mode/ina226_get_average_mode test.\n");
    
    /* set average 1 */
    res = ina226_set_average_mode(&gs_handle, INA226_AVG_1);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set average 1.\n");
    res = ina226_get_average_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check average mode %s.\n", mode == INA226_AVG_1 ? "ok" : "error");
    
    /* set average 4 */
    res = ina226_set_average_mode(&gs_handle, INA226_AVG_4);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set average 4.\n");
    res = ina226_get_average_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check average mode %s.\n", mode == INA226_AVG_4 ? "ok" : "error");
    
    /* set average 16 */
    res = ina226_set_average_mode(&gs_handle, INA226_AVG_16);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set average 16.\n");
    res = ina226_get_average_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check average mode %s.\n", mode == INA226_AVG_16 ? "ok" : "error");
    
    /* set average 64 */
    res = ina226_set_average_mode(&gs_handle, INA226_AVG_64);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set average 64.\n");
    res = ina226_get_average_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check average mode %s.\n", mode == INA226_AVG_64 ? "ok" : "error");
    
    /* set average 128 */
    res = ina226_set_average_mode(&gs_handle, INA226_AVG_128);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set average 128.\n");
    res = ina226_get_average_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check average mode %s.\n", mode == INA226_AVG_128 ? "ok" : "error");
    
    /* set average 256 */
    res = ina226_set_average_mode(&gs_handle, INA226_AVG_256);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set average 256.\n");
    res = ina226_get_average_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check average mode %s.\n", mode == INA226_AVG_256 ? "ok" : "error");
    
    /* set average 512 */
    res = ina226_set_average_mode(&gs_handle, INA226_AVG_512);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set average 512.\n");
    res = ina226_get_average_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check average mode %s.\n", mode == INA226_AVG_512 ? "ok" : "error");
    
    /* set average 1024 */
    res = ina226_set_average_mode(&gs_handle, INA226_AVG_1024);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set average 1024.\n");
    res = ina226_get_average_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get average mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check average mode %s.\n", mode == INA226_AVG_1024 ? "ok" : "error");
    
    /* ina226_set_bus_voltage_conversion_time/ina226_get_bus_voltage_conversion_time test */
    ina226_interface_debug_print("ina226: ina226_set_bus_voltage_conversion_time/ina226_get_bus_voltage_conversion_time test.\n");
    
    /* set bus voltage conversion time 140us */
    res = ina226_set_bus_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_140_US);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set bus voltage conversion time 140us.\n");
    res = ina226_get_bus_voltage_conversion_time(&gs_handle, &t);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check bus voltage conversion time %s.\n", t == INA226_CONVERSION_TIME_140_US ? "ok" : "error");
    
    /* set bus voltage conversion time 204us */
    res = ina226_set_bus_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_204_US);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set bus voltage conversion time 204us.\n");
    res = ina226_get_bus_voltage_conversion_time(&gs_handle, &t);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check bus voltage conversion time %s.\n", t == INA226_CONVERSION_TIME_204_US ? "ok" : "error");
    
    /* set bus voltage conversion time 332us */
    res = ina226_set_bus_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_332_US);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set bus voltage conversion time 332us.\n");
    res = ina226_get_bus_voltage_conversion_time(&gs_handle, &t);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check bus voltage conversion time %s.\n", t == INA226_CONVERSION_TIME_332_US ? "ok" : "error");
    
    /* set bus voltage conversion time 588us */
    res = ina226_set_bus_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_588_US);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set bus voltage conversion time 588us.\n");
    res = ina226_get_bus_voltage_conversion_time(&gs_handle, &t);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check bus voltage conversion time %s.\n", t == INA226_CONVERSION_TIME_588_US ? "ok" : "error");
    
    /* set bus voltage conversion time 1.1ms */
    res = ina226_set_bus_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_1P1_MS);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set bus voltage conversion time 1.1ms.\n");
    res = ina226_get_bus_voltage_conversion_time(&gs_handle, &t);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check bus voltage conversion time %s.\n", t == INA226_CONVERSION_TIME_1P1_MS ? "ok" : "error");
    
    /* set bus voltage conversion time 2.116ms */
    res = ina226_set_bus_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_2P116_MS);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set bus voltage conversion time 2.116ms.\n");
    res = ina226_get_bus_voltage_conversion_time(&gs_handle, &t);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check bus voltage conversion time %s.\n", t == INA226_CONVERSION_TIME_2P116_MS ? "ok" : "error");
    
    /* set bus voltage conversion time 4.156ms */
    res = ina226_set_bus_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_4P156_MS);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set bus voltage conversion time 4.156ms.\n");
    res = ina226_get_bus_voltage_conversion_time(&gs_handle, &t);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check bus voltage conversion time %s.\n", t == INA226_CONVERSION_TIME_4P156_MS ? "ok" : "error");
    
    /* set bus voltage conversion time 8.244ms */
    res = ina226_set_bus_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_8P244_MS);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set bus voltage conversion time 8.244ms.\n");
    res = ina226_get_bus_voltage_conversion_time(&gs_handle, &t);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get bus voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check bus voltage conversion time %s.\n", t == INA226_CONVERSION_TIME_8P244_MS ? "ok" : "error");
    
    /* ina226_set_shunt_voltage_conversion_time/ina226_get_shunt_voltage_conversion_time test */
    ina226_interface_debug_print("ina226: ina226_set_shunt_voltage_conversion_time/ina226_get_shunt_voltage_conversion_time test.\n");
    
    /* set shunt voltage conversion time 140us */
    res = ina226_set_shunt_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_140_US);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set shunt voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set shunt voltage conversion time 140us.\n");
    res = ina226_get_shunt_voltage_conversion_time(&gs_handle, &t);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get shunt voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check shunt voltage conversion time %s.\n", t == INA226_CONVERSION_TIME_140_US ? "ok" : "error");
    
    /* set shunt voltage conversion time 204us */
    res = ina226_set_shunt_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_204_US);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set shunt voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set shunt voltage conversion time 204us.\n");
    res = ina226_get_shunt_voltage_conversion_time(&gs_handle, &t);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get shunt voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check shunt voltage conversion time %s.\n", t == INA226_CONVERSION_TIME_204_US ? "ok" : "error");
    
    /* set shunt voltage conversion time 332us */
    res = ina226_set_shunt_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_332_US);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set shunt voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set shunt voltage conversion time 332us.\n");
    res = ina226_get_shunt_voltage_conversion_time(&gs_handle, &t);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get shunt voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check shunt voltage conversion time %s.\n", t == INA226_CONVERSION_TIME_332_US ? "ok" : "error");
    
    /* set shunt voltage conversion time 588us */
    res = ina226_set_shunt_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_588_US);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set shunt voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set shunt voltage conversion time 588us.\n");
    res = ina226_get_shunt_voltage_conversion_time(&gs_handle, &t);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get shunt voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check shunt voltage conversion time %s.\n", t == INA226_CONVERSION_TIME_588_US ? "ok" : "error");
    
    /* set shunt voltage conversion time 1.1ms */
    res = ina226_set_shunt_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_1P1_MS);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set shunt voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set shunt voltage conversion time 1.1ms.\n");
    res = ina226_get_shunt_voltage_conversion_time(&gs_handle, &t);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get shunt voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check shunt voltage conversion time %s.\n", t == INA226_CONVERSION_TIME_1P1_MS ? "ok" : "error");
    
    /* set shunt voltage conversion time 2.116ms */
    res = ina226_set_shunt_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_2P116_MS);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set shunt voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set shunt voltage conversion time 2.116ms.\n");
    res = ina226_get_shunt_voltage_conversion_time(&gs_handle, &t);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get shunt voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check shunt voltage conversion time %s.\n", t == INA226_CONVERSION_TIME_2P116_MS ? "ok" : "error");
    
    /* set shunt voltage conversion time 4.156ms */
    res = ina226_set_shunt_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_4P156_MS);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set shunt voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set shunt voltage conversion time 4.156ms.\n");
    res = ina226_get_shunt_voltage_conversion_time(&gs_handle, &t);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get shunt voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check shunt voltage conversion time %s.\n", t == INA226_CONVERSION_TIME_4P156_MS ? "ok" : "error");
    
    /* set shunt voltage conversion time 8.244ms */
    res = ina226_set_shunt_voltage_conversion_time(&gs_handle, INA226_CONVERSION_TIME_8P244_MS);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set shunt voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set shunt voltage conversion time 8.244ms.\n");
    res = ina226_get_shunt_voltage_conversion_time(&gs_handle, &t);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get shunt voltage conversion time failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check shunt voltage conversion time %s.\n", t == INA226_CONVERSION_TIME_8P244_MS ? "ok" : "error");
    
    /* ina226_set_mode/ina226_get_mode test */
    ina226_interface_debug_print("ina226: ina226_set_mode/ina226_get_mode test.\n");
    
    /* set power down */
    res = ina226_set_mode(&gs_handle, INA226_MODE_POWER_DOWN);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set mode power down.\n");
    res = ina226_get_mode(&gs_handle, &chip_mode);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check mode %s.\n", chip_mode == INA226_MODE_POWER_DOWN ? "ok" : "error");
    
    /* set shutdown */
    res = ina226_set_mode(&gs_handle, INA226_MODE_SHUTDOWN);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set mode shutdown.\n");
    res = ina226_get_mode(&gs_handle, &chip_mode);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check mode %s.\n", chip_mode == INA226_MODE_SHUTDOWN ? "ok" : "error");
    
    /* set shunt voltage continuous */
    res = ina226_set_mode(&gs_handle, INA226_MODE_SHUNT_VOLTAGE_CONTINUOUS);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set mode shunt voltage continuous.\n");
    res = ina226_get_mode(&gs_handle, &chip_mode);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check mode %s.\n", chip_mode == INA226_MODE_SHUNT_VOLTAGE_CONTINUOUS ? "ok" : "error");
    
    /* set bus voltage continuous */
    res = ina226_set_mode(&gs_handle, INA226_MODE_BUS_VOLTAGE_CONTINUOUS);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set mode bus voltage continuous.\n");
    res = ina226_get_mode(&gs_handle, &chip_mode);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check mode %s.\n", chip_mode == INA226_MODE_BUS_VOLTAGE_CONTINUOUS ? "ok" : "error");
    
    /* set shunt and bus voltage continuous */
    res = ina226_set_mode(&gs_handle, INA226_MODE_SHUNT_BUS_VOLTAGE_CONTINUOUS);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set mode shunt and bus voltage continuous.\n");
    res = ina226_get_mode(&gs_handle, &chip_mode);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get mode failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check mode %s.\n", chip_mode == INA226_MODE_SHUNT_BUS_VOLTAGE_CONTINUOUS ? "ok" : "error");
    
    /* ina226_set_calibration/ina226_get_calibration test */
    ina226_interface_debug_print("ina226: ina226_set_calibration/ina226_get_calibration test.\n");
    
    data = rand() % 0x7FFFU;
    res = ina226_set_calibration(&gs_handle, data);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set calibration failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set calibration 0x%04X.\n", data);
    res = ina226_get_calibration(&gs_handle, &data_check);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get calibration failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check calibration %s.\n", data == data_check ? "ok" : "error");
    
    /* ina226_calculate_calibration test */
    ina226_interface_debug_print("ina226: ina226_calculate_calibration test.\n");
    
    /* calculate calibration */
    res = ina226_calculate_calibration(&gs_handle, &data_check);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: calculate calibration failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: calculate calibration is 0x%04X.\n", data_check);
    
    /* ina226_set_mask/ina226_get_mask test */
    ina226_interface_debug_print("ina226: ina226_set_mask/ina226_get_mask test.\n");
    
    /* enable shunt voltage over voltage */
    res = ina226_set_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_OVER_VOLTAGE, INA226_BOOL_TRUE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: enable shunt voltage over voltage.\n");
    res = ina226_get_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_OVER_VOLTAGE, &enable);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check mask %s.\n", enable == INA226_BOOL_TRUE ? "ok" : "error");
    
    /* disable shunt voltage over voltage */
    res = ina226_set_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_OVER_VOLTAGE, INA226_BOOL_FALSE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: disable shunt voltage over voltage.\n");
    res = ina226_get_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_OVER_VOLTAGE, &enable);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check mask %s.\n", enable == INA226_BOOL_FALSE ? "ok" : "error");
    
    /* enable shunt voltage under voltage */
    res = ina226_set_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_UNDER_VOLTAGE, INA226_BOOL_TRUE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: enable shunt voltage under voltage.\n");
    res = ina226_get_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_UNDER_VOLTAGE, &enable);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check mask %s.\n", enable == INA226_BOOL_TRUE ? "ok" : "error");
    
    /* disable shunt voltage under voltage */
    res = ina226_set_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_UNDER_VOLTAGE, INA226_BOOL_FALSE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: disable shunt voltage under voltage.\n");
    res = ina226_get_mask(&gs_handle, INA226_MASK_SHUNT_VOLTAGE_UNDER_VOLTAGE, &enable);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check mask %s.\n", enable == INA226_BOOL_FALSE ? "ok" : "error");
    
    /* enable bus voltage over voltage */
    res = ina226_set_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_OVER_VOLTAGE, INA226_BOOL_TRUE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: enable bus voltage over voltage.\n");
    res = ina226_get_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_OVER_VOLTAGE, &enable);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check mask %s.\n", enable == INA226_BOOL_TRUE ? "ok" : "error");
    
    /* disable bus voltage over voltage */
    res = ina226_set_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_OVER_VOLTAGE, INA226_BOOL_FALSE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: disable bus voltage over voltage.\n");
    res = ina226_get_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_OVER_VOLTAGE, &enable);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check mask %s.\n", enable == INA226_BOOL_FALSE ? "ok" : "error");
    
    /* enable bus voltage under voltage */
    res = ina226_set_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_UNDER_VOLTAGE, INA226_BOOL_TRUE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: enable bus voltage under voltage.\n");
    res = ina226_get_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_UNDER_VOLTAGE, &enable);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check mask %s.\n", enable == INA226_BOOL_TRUE ? "ok" : "error");
    
    /* disable bus voltage under voltage */
    res = ina226_set_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_UNDER_VOLTAGE, INA226_BOOL_FALSE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: disable bus voltage under voltage.\n");
    res = ina226_get_mask(&gs_handle, INA226_MASK_BUS_VOLTAGE_UNDER_VOLTAGE, &enable);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check mask %s.\n", enable == INA226_BOOL_FALSE ? "ok" : "error");
    
    /* enable power over limit */
    res = ina226_set_mask(&gs_handle, INA226_MASK_POWER_OVER_LIMIT, INA226_BOOL_TRUE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: enable power over limit.\n");
    res = ina226_get_mask(&gs_handle, INA226_MASK_POWER_OVER_LIMIT, &enable);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check mask %s.\n", enable == INA226_BOOL_TRUE ? "ok" : "error");
    
    /* disable power over limit */
    res = ina226_set_mask(&gs_handle, INA226_MASK_POWER_OVER_LIMIT, INA226_BOOL_FALSE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: disable power over limit.\n");
    res = ina226_get_mask(&gs_handle, INA226_MASK_POWER_OVER_LIMIT, &enable);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get mask failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check mask %s.\n", enable == INA226_BOOL_FALSE ? "ok" : "error");
    
    /* ina226_set_conversion_ready_alert_pin/ina226_get_conversion_ready_alert_pin test */
    ina226_interface_debug_print("ina226: ina226_set_conversion_ready_alert_pin/ina226_get_conversion_ready_alert_pin test.\n");
    
    /* enable conversion ready alert pin */
    res = ina226_set_conversion_ready_alert_pin(&gs_handle, INA226_BOOL_TRUE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set conversion ready alert pin failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: enable conversion ready alert pin.\n");
    res = ina226_get_conversion_ready_alert_pin(&gs_handle, &enable);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get conversion ready alert pin failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check conversion ready alert pin %s.\n", enable == INA226_BOOL_TRUE ? "ok" : "error");
    
    /* disable conversion ready alert pin */
    res = ina226_set_conversion_ready_alert_pin(&gs_handle, INA226_BOOL_FALSE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set conversion ready alert pin failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: disable conversion ready alert pin.\n");
    res = ina226_get_conversion_ready_alert_pin(&gs_handle, &enable);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get conversion ready alert pin failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check conversion ready alert pin %s.\n", enable == INA226_BOOL_FALSE ? "ok" : "error");
    
    /* ina226_set_alert_polarity_pin/ina226_get_alert_polarity_pin test */
    ina226_interface_debug_print("ina226: ina226_set_alert_polarity_pin/ina226_get_alert_polarity_pin test.\n");
    
    /* set alert polarity pin normal */
    res = ina226_set_alert_polarity_pin(&gs_handle, INA226_ALERT_POLARITY_NORMAL);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set alert polarity pin failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set alert polarity pin normal.\n");
    res = ina226_get_alert_polarity_pin(&gs_handle, &pin);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get alert polarity pin failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check alert polarity pin %s.\n", pin == INA226_ALERT_POLARITY_NORMAL ? "ok" : "error");
    
    /* set alert polarity pin inverted */
    res = ina226_set_alert_polarity_pin(&gs_handle, INA226_ALERT_POLARITY_INVERTED);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set alert polarity pin failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set alert polarity pin inverted.\n");
    res = ina226_get_alert_polarity_pin(&gs_handle, &pin);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get alert polarity pin failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check alert polarity pin %s.\n", pin == INA226_ALERT_POLARITY_INVERTED ? "ok" : "error");
    
    /* ina226_set_alert_latch/ina226_get_alert_latch test */
    ina226_interface_debug_print("ina226: ina226_set_alert_latch/ina226_get_alert_latch test.\n");
    
    /* enable alert latch */
    res = ina226_set_alert_latch(&gs_handle, INA226_BOOL_TRUE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set alert latch failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: enable alert latch.\n");
    res = ina226_get_alert_latch(&gs_handle, &enable);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get alert latch failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check alert latch %s.\n", enable == INA226_BOOL_TRUE ? "ok" : "error");
    
    /* disable alert latch */
    res = ina226_set_alert_latch(&gs_handle, INA226_BOOL_FALSE);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set alert latch failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: disable alert latch.\n");
    res = ina226_get_alert_latch(&gs_handle, &enable);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get alert latch failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check alert latch %s.\n", enable == INA226_BOOL_FALSE ? "ok" : "error");
    
    /* ina226_set_alert_limit/ina226_get_alert_limit test */
    ina226_interface_debug_print("ina226: ina226_set_alert_limit/ina226_get_alert_limit test.\n");
    
    data = rand() % 0xFFFFU;
    res = ina226_set_alert_limit(&gs_handle, data);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: set alert limit failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set alert limit 0x%04X.\n", data);
    res = ina226_get_alert_limit(&gs_handle, &data_check);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get alert limit failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check alert limit %s.\n", data == data_check ? "ok" : "error");
    
    /* ina226_shunt_voltage_convert_to_register/ina226_shunt_voltage_convert_to_data test */
    ina226_interface_debug_print("ina226: ina226_shunt_voltage_convert_to_register/ina226_shunt_voltage_convert_to_data test.\n");
    
    f = (float)(rand() % 1000) / 100.0f;
    res = ina226_shunt_voltage_convert_to_register(&gs_handle, f, &data);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: shunt voltage convert to register failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set shunt voltage %0.2fmV.\n", f);
    res = ina226_shunt_voltage_convert_to_data(&gs_handle, data, &f_check);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: shunt voltage convert to data failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check shunt voltage %0.2fmV.\n", f_check);
    
    /* ina226_bus_voltage_convert_to_register/ina226_bus_voltage_convert_to_data test */
    ina226_interface_debug_print("ina226: ina226_bus_voltage_convert_to_register/ina226_bus_voltage_convert_to_data test.\n");
    
    f = (float)(rand() % 1000) / 100.0f;
    res = ina226_bus_voltage_convert_to_register(&gs_handle, f, &data);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: bus voltage convert to register failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set bus voltage %0.2fmV.\n", f);
    res = ina226_bus_voltage_convert_to_data(&gs_handle, data, &f_check);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: bus voltage convert to data failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check bus voltage %0.2fmV.\n", f_check);
    
    /* ina226_power_convert_to_register/ina226_power_convert_to_data test */
    ina226_interface_debug_print("ina226: ina226_power_convert_to_register/ina226_power_convert_to_data test.\n");
    
    f = (float)(rand() % 1000) / 100.0f;
    res = ina226_power_convert_to_register(&gs_handle, f, &data);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: power convert to register failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: set power %0.2fmW.\n", f);
    res = ina226_power_convert_to_data(&gs_handle, data, &f_check);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: power convert to data failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    ina226_interface_debug_print("ina226: check power %0.2fmW.\n", f_check);
    
    /* ina226_get_die_id test */
    ina226_interface_debug_print("ina226: ina226_get_die_id test.\n");
    
    /* get die id */
    res = ina226_get_die_id(&gs_handle, &device_id, &die_revision_id);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: get die id failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    ina226_interface_debug_print("ina226: device id is 0x%04X.\n", device_id);
    
    /* output */
    ina226_interface_debug_print("ina226: die revision id is 0x%02X.\n", die_revision_id);
    
    /* ina226_soft_reset test */
    ina226_interface_debug_print("ina226: ina226_soft_reset test.\n");
    
    /* soft reset */
    res = ina226_soft_reset(&gs_handle);
    if (res != 0)
    {
        ina226_interface_debug_print("ina226: soft reset failed.\n");
        (void)ina226_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish register test */
    (void)ina226_deinit(&gs_handle);
    ina226_interface_debug_print("ina226: finish register test.\n");
    
    return 0;
}
