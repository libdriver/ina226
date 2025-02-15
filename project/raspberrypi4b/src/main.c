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
 * @file      main.c
 * @brief     main source file
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
#include "driver_ina226_basic.h"
#include "driver_ina226_alert.h"
#include "driver_ina226_register_test.h"
#include "driver_ina226_read_test.h"
#include "driver_ina226_alert_test.h"
#include "gpio.h"
#include <getopt.h>
#include <stdlib.h>

uint8_t (*g_gpio_irq)(void) = NULL;    /**< gpio extern callback */
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
 * @brief     ina226 full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t ina226(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"resistance", required_argument, NULL, 2},
        {"times", required_argument, NULL, 3},
        {"type", required_argument, NULL, 4},
        {"threshold", required_argument, NULL, 5},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    double r = 0.1;
    float threshold = 3300.0f;
    ina226_mask_t mask = INA226_MASK_BUS_VOLTAGE_OVER_VOLTAGE;
    ina226_address_t addr = INA226_ADDRESS_0;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* addr */
            case 1 :
            {
                /* set the addr pin */
                if (strcmp("0", optarg) == 0)
                {
                    addr = INA226_ADDRESS_0;
                }
                else if (strcmp("1", optarg) == 0)
                {
                    addr = INA226_ADDRESS_1;
                }
                else if (strcmp("2", optarg) == 0)
                {
                    addr = INA226_ADDRESS_2;
                }
                else if (strcmp("3", optarg) == 0)
                {
                    addr = INA226_ADDRESS_3;
                }
                else if (strcmp("4", optarg) == 0)
                {
                    addr = INA226_ADDRESS_4;
                }
                else if (strcmp("5", optarg) == 0)
                {
                    addr = INA226_ADDRESS_5;
                }
                else if (strcmp("6", optarg) == 0)
                {
                    addr = INA226_ADDRESS_6;
                }
                else if (strcmp("7", optarg) == 0)
                {
                    addr = INA226_ADDRESS_7;
                }
                else if (strcmp("8", optarg) == 0)
                {
                    addr = INA226_ADDRESS_8;
                }
                else if (strcmp("9", optarg) == 0)
                {
                    addr = INA226_ADDRESS_9;
                }
                else if (strcmp("A", optarg) == 0)
                {
                    addr = INA226_ADDRESS_A;
                }
                else if (strcmp("B", optarg) == 0)
                {
                    addr = INA226_ADDRESS_B;
                }
                else if (strcmp("C", optarg) == 0)
                {
                    addr = INA226_ADDRESS_C;
                }
                else if (strcmp("D", optarg) == 0)
                {
                    addr = INA226_ADDRESS_D;
                }
                else if (strcmp("E", optarg) == 0)
                {
                    addr = INA226_ADDRESS_E;
                }
                else if (strcmp("F", optarg) == 0)
                {
                    addr = INA226_ADDRESS_F;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* resistance */
            case 2 :
            {
                /* set the resistance */
                r = atof(optarg);
                
                break;
            }

            /* running times */
            case 3 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* type */
            case 4 :
            {
                uint8_t t;
                
                /* set the times */
                t = (uint8_t)atol(optarg);
                if (t == 0)
                {
                    mask = INA226_MASK_SHUNT_VOLTAGE_OVER_VOLTAGE;
                }
                else if (t == 1)
                {
                    mask = INA226_MASK_SHUNT_VOLTAGE_UNDER_VOLTAGE;
                }
                else if (t == 2)
                {
                    mask = INA226_MASK_BUS_VOLTAGE_OVER_VOLTAGE;
                }
                else if (t == 3)
                {
                    mask = INA226_MASK_BUS_VOLTAGE_UNDER_VOLTAGE;
                }
                else if (t == 4)
                {
                    mask = INA226_MASK_POWER_OVER_LIMIT;
                }
                else
                {
                    return 5;
                }
                
                break;
            } 
            
            /* threshold */
            case 5 :
            {
                /* set the threshold */
                threshold = (float)atof(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        uint8_t res;
        
        /* run the reg test */
        res = ina226_register_test(addr);
        if (res != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_read", type) == 0)
    {
        uint8_t res;
        
        /* run the read test */
        res = ina226_read_test(addr, r, times);
        if (res != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_alert", type) == 0)
    {
        uint8_t res;
        
        /* gpio interrupt init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        g_gpio_irq = ina226_alert_test_irq_handler;
        
        /* run the alert test */
        res = ina226_alert_test(addr, r, mask, threshold, 5000);
        if (res != 0)
        {
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();
            
            return 1;
        }
        else
        {
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();
            
            return 0;
        }
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* basic init */
        res = ina226_basic_init(addr, r);
        if (res != 0)
        {
            return 1;
        }
        
        /* delay 1000ms */
        ina226_interface_delay_ms(1000);
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            float mV;
            float mA;
            float mW;
            
            /* read data */
            res = ina226_basic_read(&mV, &mA, &mW);
            if (res != 0)
            {
                (void)ina226_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ina226_interface_debug_print("ina226: %d/%d.\n", i + 1, times);
            ina226_interface_debug_print("ina226: bus voltage is %0.3fmV.\n", mV);
            ina226_interface_debug_print("ina226: current is %0.3fmA.\n", mA);
            ina226_interface_debug_print("ina226: power is %0.3fmW.\n", mW);
            ina226_interface_delay_ms(1000);
        }
        
        /* deinit */
        (void)ina226_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_shot", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* shot init */
        res = ina226_shot_init(addr, r);
        if (res != 0)
        {
            return 1;
        }
        
        /* delay 1000ms */
        ina226_interface_delay_ms(1000);

        /* loop */
        for (i = 0; i < times; i++)
        {
            float mV;
            float mA;
            float mW;
            
            /* read data */
            res = ina226_shot_read(&mV, &mA, &mW);
            if (res != 0)
            {
                (void)ina226_shot_deinit();
                
                return 1;
            }
            
            /* output */
            ina226_interface_debug_print("ina226: %d/%d.\n", i + 1, times);
            ina226_interface_debug_print("ina226: bus voltage is %0.3fmV.\n", mV);
            ina226_interface_debug_print("ina226: current is %0.3fmA.\n", mA);
            ina226_interface_debug_print("ina226: power is %0.3fmW.\n", mW);
            ina226_interface_delay_ms(1000);
        }
        
        /* shot deinit */
        (void)ina226_shot_deinit();
        
        return 0;
    }
    else if (strcmp("e_alert", type) == 0)
    {
        uint8_t res;
        uint8_t flag;
        uint32_t timeout = 5000;
        
        /* gpio interrupt init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        g_gpio_irq = ina226_alert_irq_handler;
        
        gs_flag = 0;
        if (mask == INA226_MASK_SHUNT_VOLTAGE_OVER_VOLTAGE)
        {
            ina226_interface_debug_print("ina226: set shunt voltage over voltage %0.3fmV.\n", threshold);
            
            flag = 1 << 0;
        }
        else if (mask == INA226_MASK_SHUNT_VOLTAGE_UNDER_VOLTAGE)
        {
            ina226_interface_debug_print("ina226: set shunt voltage under voltage %0.3fmV.\n", threshold);
            
            flag = 1 << 1;
        }
        else if (mask == INA226_MASK_BUS_VOLTAGE_OVER_VOLTAGE)
        {
            ina226_interface_debug_print("ina226: set bus voltage over voltage %0.3fmV.\n", threshold);
            
            flag = 1 << 2;
        }
        else if (mask == INA226_MASK_BUS_VOLTAGE_UNDER_VOLTAGE)
        {
            ina226_interface_debug_print("ina226: set bus voltage under voltage %0.3fmV.\n", threshold);
            
            flag = 1 << 3;
        }
        else
        {
            ina226_interface_debug_print("ina226: set power over limit %0.3fmV.\n", threshold);
            
            flag = 1 << 4;
        }
        
        /* alert init */
        res = ina226_alert_init(addr, r, mask, threshold, a_receive_callback);
        if (res != 0)
        {
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();
            
            return 1;
        }
        
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
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();
            ina226_alert_deinit();
            
            return 1;
        }
        
        /* alert deinit */
        g_gpio_irq = NULL;
        (void)gpio_interrupt_deinit();
        (void)ina226_alert_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        ina226_interface_debug_print("Usage:\n");
        ina226_interface_debug_print("  ina226 (-i | --information)\n");
        ina226_interface_debug_print("  ina226 (-h | --help)\n");
        ina226_interface_debug_print("  ina226 (-p | --port)\n");
        ina226_interface_debug_print("  ina226 (-t reg | --test=reg) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>]\n");
        ina226_interface_debug_print("  ina226 (-t read | --test=read) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>]\n");
        ina226_interface_debug_print("         [--resistance=<r>] [--times=<num>]\n");
        ina226_interface_debug_print("  ina226 (-t alert | --test=alert) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>]\n");
        ina226_interface_debug_print("         [--resistance=<r>] [--type=<type>][--threshold=<th>]\n");
        ina226_interface_debug_print("  ina226 (-e read | --example=read) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>]\n");
        ina226_interface_debug_print("         [--resistance=<r>] [--times=<num>]\n");
        ina226_interface_debug_print("  ina226 (-e shot | --example=shot) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>]\n");
        ina226_interface_debug_print("         [--resistance=<r>] [--times=<num>]\n");
        ina226_interface_debug_print("  ina226 (-e alert | --example=alert) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>]\n");
        ina226_interface_debug_print("         [--resistance=<r>] [--type=<type>][--threshold=<th>]\n");
        ina226_interface_debug_print("\n");
        ina226_interface_debug_print("Options:\n");
        ina226_interface_debug_print("      --addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>\n");
        ina226_interface_debug_print("                                 Set the addr pin.([default: 0])\n");
        ina226_interface_debug_print("  -e <read | shot | alert>, --example=<read | shot | alert>\n");
        ina226_interface_debug_print("                                 Run the driver example.\n");
        ina226_interface_debug_print("  -h, --help                     Show the help.\n");
        ina226_interface_debug_print("  -i, --information              Show the chip information.\n");
        ina226_interface_debug_print("  -p, --port                     Display the pin connections of the current board.\n");
        ina226_interface_debug_print("      --resistance=<r>           Set the sample resistance.([default: 0.1])\n");
        ina226_interface_debug_print("  -t <reg | read | alert>, --test=<reg | read | alert>\n");
        ina226_interface_debug_print("                                 Run the driver test.\n");
        ina226_interface_debug_print("      --times=<num>              Set the running times.([default: 3])\n");
        ina226_interface_debug_print("      --type=<type>              Set alert type,\n");
        ina226_interface_debug_print("                                 0: shunt voltage over voltage;\n");
        ina226_interface_debug_print("                                 1: shunt voltage under voltage;\n");
        ina226_interface_debug_print("                                 2: bus voltage over voltage;\n");
        ina226_interface_debug_print("                                 3: bus voltage under voltage;\n");
        ina226_interface_debug_print("                                 4: power over limit.([default: 2])\n");
        ina226_interface_debug_print("      --threshold=<th>           Set alert threshold.([default: 3300.0f])\n");
          
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        ina226_info_t info;
        
        /* print ina226 info */
        ina226_info(&info);
        ina226_interface_debug_print("ina226: chip is %s.\n", info.chip_name);
        ina226_interface_debug_print("ina226: manufacturer is %s.\n", info.manufacturer_name);
        ina226_interface_debug_print("ina226: interface is %s.\n", info.interface);
        ina226_interface_debug_print("ina226: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ina226_interface_debug_print("ina226: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ina226_interface_debug_print("ina226: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ina226_interface_debug_print("ina226: max current is %0.2fmA.\n", info.max_current_ma);
        ina226_interface_debug_print("ina226: max temperature is %0.1fC.\n", info.temperature_max);
        ina226_interface_debug_print("ina226: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        ina226_interface_debug_print("ina226: SCL connected to GPIO3(BCM).\n");
        ina226_interface_debug_print("ina226: SDA connected to GPIO2(BCM).\n");
        ina226_interface_debug_print("ina226: INT connected to GPIO17(BCM).\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = ina226(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        ina226_interface_debug_print("ina226: run failed.\n");
    }
    else if (res == 5)
    {
        ina226_interface_debug_print("ina226: param is invalid.\n");
    }
    else
    {
        ina226_interface_debug_print("ina226: unknown status code.\n");
    }

    return 0;
}
