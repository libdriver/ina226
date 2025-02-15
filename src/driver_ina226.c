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
 * @file      driver_ina226.c
 * @brief     driver ina226 source file
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

#include "driver_ina226.h" 
#include <math.h>

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Texas Instruments INA226"        /**< chip name */
#define MANUFACTURER_NAME         "Texas Instruments"               /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        2.7f                              /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                              /**< chip max supply voltage */
#define MAX_CURRENT               0.33f                             /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                            /**< chip min operating temperature */
#define TEMPERATURE_MAX           125.0f                            /**< chip max operating temperature */
#define DRIVER_VERSION            1000                              /**< driver version */

/**
 * @brief chip register definition
 */
#define INA226_REG_CONF                 0x00        /**< configuration register */
#define INA226_REG_SHUNT_VOLTAGE        0x01        /**< shunt voltage register */
#define INA226_REG_BUS_VOLTAGE          0x02        /**< bus voltage register */
#define INA226_REG_POWER                0x03        /**< power register */
#define INA226_REG_CURRENT              0x04        /**< current register */
#define INA226_REG_CALIBRATION          0x05        /**< calibration register */
#define INA226_REG_MASK                 0x06        /**< mask register */
#define INA226_REG_ALERT_LIMIT          0x07        /**< alert limit register */
#define INA226_REG_MANUFACTURER         0xFE        /**< manufacturer id register */
#define INA226_REG_DIE                  0xFF        /**< die id register */

/**
 * @brief      iic interface read bytes
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[in]  reg iic register address
 * @param[out] *data pointer to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_ina226_iic_read(ina226_handle_t *handle, uint8_t reg, uint16_t *data)
{
    uint8_t buf[2];
    
    memset(buf, 0, sizeof(uint8_t) * 2);                                        /* clear the buffer */
    if (handle->iic_read(handle->iic_addr, reg, (uint8_t *)buf, 2) != 0)        /* read data */
    {
        return 1;                                                               /* return error */
    }
    else
    {
        *data = (uint16_t)buf[0] << 8 | buf[1];                                 /* get data */
        
        return 0;                                                               /* success return 0 */
    }
}

/**
 * @brief     iic interface write bytes
 * @param[in] *handle pointer to an ina226 handle structure
 * @param[in] reg iic register address
 * @param[in] data written data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_ina226_iic_write(ina226_handle_t *handle, uint8_t reg, uint16_t data)
{
    uint8_t buf[2];
    
    buf[0] = (uint8_t)((data >> 8) & 0xFF);                                      /* get MSB */
    buf[1] = (uint8_t)((data >> 0) & 0xFF);                                      /* get LSB */
    if (handle->iic_write(handle->iic_addr, reg, (uint8_t *)buf, 2) != 0)        /* write data */
    {
        return 1;                                                                /* return error */
    }
    else
    {
        return 0;                                                                /* success return 0 */
    }
}

/**
 * @brief     set the resistance
 * @param[in] *handle pointer to an ina226 handle structure
 * @param[in] resistance current sampling resistance value
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ina226_set_resistance(ina226_handle_t *handle, double resistance)
{
    if (handle == NULL)            /* check handle */
    {
        return 2;                  /* return error */
    }
    
    handle->r = resistance;        /* set resistance */
    
    return 0;                      /* success return 0 */
}

/**
 * @brief      get the resistance
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *resistance pointer to a current sampling resistance value buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ina226_get_resistance(ina226_handle_t *handle, double *resistance)
{
    if (handle == NULL)            /* check handle */
    {
        return 2;                  /* return error */
    }
    
    *resistance = handle->r;       /* get resistance */
    
    return 0;                      /* success return 0 */
}

/**
 * @brief     set the iic address pin
 * @param[in] *handle pointer to an ina226 handle structure
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ina226_set_addr_pin(ina226_handle_t *handle, ina226_address_t addr_pin)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    
    handle->iic_addr = (uint8_t)addr_pin;        /* set pin */
    
    return 0;                                    /* success return 0 */
}

/**
 * @brief      get the iic address pin
 * @param[in]  *handle pointer to an ina226 handle structure
 * @param[out] *addr_pin pointer to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ina226_get_addr_pin(ina226_handle_t *handle, ina226_address_t *addr_pin)
{
    if (handle == NULL)                                      /* check handle */
    {
        return 2;                                            /* return error */
    }
    
    *addr_pin = (ina226_address_t)(handle->iic_addr);        /* get pin */
    
    return 0;                                                /* success return 0 */
}

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
uint8_t ina226_soft_reset(ina226_handle_t *handle)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_CONF, (uint16_t *)&prev);       /* read config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ina226: read conf register failed.\n");           /* read conf register failed */
       
        return 1;                                                              /* return error */
    }
    prev &= ~(1 << 15);                                                        /* clear soft reset */
    prev |= 1 << 15;                                                           /* set soft reset */

    return a_ina226_iic_write(handle, INA226_REG_CONF, (uint16_t )prev);       /* write config */
}

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
uint8_t ina226_set_average_mode(ina226_handle_t *handle, ina226_avg_t mode)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_CONF, (uint16_t *)&prev);       /* read config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ina226: read conf register failed.\n");           /* read conf register failed */
       
        return 1;                                                              /* return error */
    }
    prev &= ~(0x7 << 9);                                                       /* clear mode bits */
    prev |= mode << 9;                                                         /* set mode bits */

    return a_ina226_iic_write(handle, INA226_REG_CONF, (uint16_t )prev);       /* write config */
}

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
uint8_t ina226_get_average_mode(ina226_handle_t *handle, ina226_avg_t *mode)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_CONF, (uint16_t *)&prev);       /* read config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ina226: read conf register failed.\n");           /* read conf register failed */
       
        return 1;                                                              /* return error */
    }
    *mode = (ina226_avg_t)((prev >> 9) & 0x07);                                /* get mode */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t ina226_set_bus_voltage_conversion_time(ina226_handle_t *handle, ina226_conversion_time_t t)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_CONF, (uint16_t *)&prev);       /* read config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ina226: read conf register failed.\n");           /* read conf register failed */
       
        return 1;                                                              /* return error */
    }
    prev &= ~(7 << 6);                                                         /* clear time bits */
    prev |= t << 6;                                                            /* set time bits */

    return a_ina226_iic_write(handle, INA226_REG_CONF, (uint16_t )prev);       /* write config */
}

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
uint8_t ina226_get_bus_voltage_conversion_time(ina226_handle_t *handle, ina226_conversion_time_t *t)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_CONF, (uint16_t *)&prev);       /* read config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ina226: read conf register failed.\n");           /* read conf register failed */
       
        return 1;                                                              /* return error */
    }
    *t = (ina226_conversion_time_t)((prev >> 6) & 0x07);                       /* get time */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t ina226_set_shunt_voltage_conversion_time(ina226_handle_t *handle, ina226_conversion_time_t t)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_CONF, (uint16_t *)&prev);       /* read config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ina226: read conf register failed.\n");           /* read conf register failed */
       
        return 1;                                                              /* return error */
    }
    prev &= ~(7 << 3);                                                         /* clear time bits */
    prev |= t << 3;                                                            /* set time bits */

    return a_ina226_iic_write(handle, INA226_REG_CONF, (uint16_t )prev);       /* write config */
}

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
uint8_t ina226_get_shunt_voltage_conversion_time(ina226_handle_t *handle, ina226_conversion_time_t *t)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_CONF, (uint16_t *)&prev);       /* read config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ina226: read conf register failed.\n");           /* read conf register failed */
       
        return 1;                                                              /* return error */
    }
    *t = (ina226_conversion_time_t)((prev >> 3) & 0x07);                       /* get time */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t ina226_set_mode(ina226_handle_t *handle, ina226_mode_t mode)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_CONF, (uint16_t *)&prev);       /* read config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ina226: read conf register failed.\n");           /* read conf register failed */
       
        return 1;                                                              /* return error */
    }
    prev &= ~(0x7 << 0);                                                       /* clear mode bit */
    prev |= mode << 0;                                                         /* set mode bit */
    res = a_ina226_iic_write(handle, INA226_REG_CONF, (uint16_t )prev);        /* write config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ina226: write conf register failed.\n");          /* write conf register failed */
       
        return 1;                                                              /* return error */
    }
    if ((mode == INA226_MODE_SHUNT_VOLTAGE_TRIGGERED) ||
        (mode == INA226_MODE_BUS_VOLTAGE_TRIGGERED) ||
        (mode == INA226_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED))                     /* check triggered mode */
    {
        handle->trigger = 1;                                                   /* set 1 */
    }
    else
    {
        handle->trigger = 0;                                                   /* set 0 */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t ina226_get_mode(ina226_handle_t *handle, ina226_mode_t *mode)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_CONF, (uint16_t *)&prev);       /* read config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ina226: read conf register failed.\n");           /* read conf register failed */
       
        return 1;                                                              /* return error */
    }
    *mode = (ina226_mode_t)((prev >> 0) & 0x7);                                /* get mode */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t ina226_get_die_id(ina226_handle_t *handle, uint16_t *device_id, uint8_t *die_revision_id)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_DIE, (uint16_t *)&prev);       /* read config */
    if (res != 0)                                                             /* check result */
    {
        handle->debug_print("ina226: read die register failed.\n");           /* read die register failed */
       
        return 1;                                                             /* return error */
    }
    *device_id = (prev >> 4) & 0xFFF;                                         /* get device id */
    *die_revision_id = prev & 0xF;                                            /* get die revision id */
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t ina226_read_shunt_voltage(ina226_handle_t *handle, int16_t *raw, float *mV)
{
    uint8_t res;
    union
    {
        uint16_t u;
        int16_t s;
    } u;
    uint16_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_MASK, (uint16_t *)&prev);                /* read mask */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("ina226: read mask register failed.\n");                    /* read mask register failed */
       
        return 1;                                                                       /* return error */
    }
    if ((prev & (1 << 2)) != 0)                                                         /* check math overflow */
    {
        handle->debug_print("ina226: math overflow.\n");                                /* math overflow */
       
        return 4;                                                                       /* return error */
    }
    if (handle->trigger != 0)                                                           /* trigger mode */
    {
        uint16_t i;
        uint16_t timeout;
        
        timeout = INA226_READ_TIMEOUT;                                                  /* set timeout */
        for (i = 0; i< timeout; i++)                                                    /* loop all */
        {
            res = a_ina226_iic_read(handle, INA226_REG_MASK, (uint16_t *)&prev);        /* read mask */
            if (res != 0)                                                               /* check result */
            {
                handle->debug_print("ina226: read mask register failed.\n");            /* read mask register failed */
               
                return 1;                                                               /* return error */
            }
            if ((prev & (1 << 3)) != 0)                                                 /* check conversion ready flag */
            {
                break;                                                                  /* break */
            }
            handle->delay_ms(1);                                                        /* delay 1ms */
            timeout--;                                                                  /* timeout-- */
        }
        if (timeout == 0)                                                               /* check timeout */
        {
            handle->debug_print("ina226: read timeout.\n");                             /* timeout */
           
            return 5;                                                                   /* return error */
        }
        handle->trigger = 0;                                                            /* set 0 */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_SHUNT_VOLTAGE, (uint16_t *)&u.u);        /* read shunt voltage */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("ina226: read shunt voltage register failed.\n");           /* read shunt voltage register failed */
       
        return 1;                                                                       /* return error */
    }
    *raw = u.s;                                                                         /* set the raw */
    *mV = (float)(*raw) / 400.0f;                                                       /* set the converted data */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t ina226_read_bus_voltage(ina226_handle_t *handle, uint16_t *raw, float *mV)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_MASK, (uint16_t *)&prev);                /* read mask */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("ina226: read mask register failed.\n");                    /* read mask register failed */
       
        return 1;                                                                       /* return error */
    }
    if ((prev & (1 << 2)) != 0)                                                         /* check math overflow */
    {
        handle->debug_print("ina226: math overflow.\n");                                /* math overflow */
       
        return 4;                                                                       /* return error */
    }
    if (handle->trigger != 0)                                                           /* trigger mode */
    {
        uint16_t i;
        uint16_t timeout;
        
        timeout = INA226_READ_TIMEOUT;                                                  /* set timeout */
        for (i = 0; i< timeout; i++)                                                    /* loop all */
        {
            res = a_ina226_iic_read(handle, INA226_REG_MASK, (uint16_t *)&prev);        /* read mask */
            if (res != 0)                                                               /* check result */
            {
                handle->debug_print("ina226: read mask register failed.\n");            /* read mask register failed */
               
                return 1;                                                               /* return error */
            }
            if ((prev & (1 << 3)) != 0)                                                 /* check conversion ready flag */
            {
                break;                                                                  /* break */
            }
            handle->delay_ms(1);                                                        /* delay 1ms */
            timeout--;                                                                  /* timeout-- */
        }
        if (timeout == 0)                                                               /* check timeout */
        {
            handle->debug_print("ina226: read timeout.\n");                             /* timeout */
           
            return 5;                                                                   /* return error */
        }
        handle->trigger = 0;                                                            /* set 0 */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_BUS_VOLTAGE, (uint16_t *)raw);           /* read bus voltage */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("ina226: read bus voltage register failed.\n");             /* read bus voltage register failed */
       
        return 1;                                                                       /* return error */
    }
    *mV = (float)(*raw) * 1.25f;                                                        /* set the converted data */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t ina226_read_current(ina226_handle_t *handle, int16_t *raw, float *mA)
{
    uint8_t res;
    union
    {
        uint16_t u;
        int16_t s;
    } u;
    uint16_t prev;
   
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_MASK, (uint16_t *)&prev);                /* read mask */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("ina226: read mask register failed.\n");                    /* read mask register failed */
       
        return 1;                                                                       /* return error */
    }
    if ((prev & (1 << 2)) != 0)                                                         /* check math overflow */
    {
        handle->debug_print("ina226: math overflow.\n");                                /* math overflow */
       
        return 4;                                                                       /* return error */
    }
    if (handle->trigger != 0)                                                           /* trigger mode */
    {
        uint16_t i;
        uint16_t timeout;
        
        timeout = INA226_READ_TIMEOUT;                                                  /* set timeout */
        for (i = 0; i< timeout; i++)                                                    /* loop all */
        {
            res = a_ina226_iic_read(handle, INA226_REG_MASK, (uint16_t *)&prev);        /* read mask */
            if (res != 0)                                                               /* check result */
            {
                handle->debug_print("ina226: read mask register failed.\n");            /* read mask register failed */
               
                return 1;                                                               /* return error */
            }
            if ((prev & (1 << 3)) != 0)                                                 /* check conversion ready flag */
            {
                break;                                                                  /* break */
            }
            handle->delay_ms(1);                                                        /* delay 1ms */
            timeout--;                                                                  /* timeout-- */
        }
        if (timeout == 0)                                                               /* check timeout */
        {
            handle->debug_print("ina226: read timeout.\n");                             /* timeout */
           
            return 5;                                                                   /* return error */
        }
        handle->trigger = 0;                                                            /* set 0 */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_CURRENT, (uint16_t *)&u.u);              /* read current */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("ina226: read current register failed.\n");                 /* read current register failed */
       
        return 1;                                                                       /* return error */
    }
    *raw = u.s;                                                                         /* set the raw */
    *mA = (float)((double)(*raw) * handle->current_lsb * 1000);                         /* set the converted data */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t ina226_read_power(ina226_handle_t *handle, uint16_t *raw, float *mW)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_MASK, (uint16_t *)&prev);                /* read mask */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("ina226: read mask register failed.\n");                    /* read mask register failed */
       
        return 1;                                                                       /* return error */
    }
    if ((prev & (1 << 2)) != 0)                                                         /* check math overflow */
    {
        handle->debug_print("ina226: math overflow.\n");                                /* math overflow */
       
        return 4;                                                                       /* return error */
    }
    if (handle->trigger != 0)                                                           /* trigger mode */
    {
        uint16_t i;
        uint16_t timeout;
        
        timeout = INA226_READ_TIMEOUT;                                                  /* set timeout */
        for (i = 0; i< timeout; i++)                                                    /* loop all */
        {
            res = a_ina226_iic_read(handle, INA226_REG_MASK, (uint16_t *)&prev);        /* read mask */
            if (res != 0)                                                               /* check result */
            {
                handle->debug_print("ina226: read mask register failed.\n");            /* read mask register failed */
               
                return 1;                                                               /* return error */
            }
            if ((prev & (1 << 3)) != 0)                                                 /* check conversion ready flag */
            {
                break;                                                                  /* break */
            }
            handle->delay_ms(1);                                                        /* delay 1ms */
            timeout--;                                                                  /* timeout-- */
        }
        if (timeout == 0)                                                               /* check timeout */
        {
            handle->debug_print("ina226: read timeout.\n");                             /* timeout */
           
            return 5;                                                                   /* return error */
        }
        handle->trigger = 0;                                                            /* set 0 */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_POWER, (uint16_t *)raw);                 /* read power */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("ina226: read power register failed.\n");                   /* read power register failed */
       
        return 1;                                                                       /* return error */
    }
    *mW = (float)((double)(*raw) * handle->current_lsb * 25.0 * 1000.0);                /* set the converted data */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t ina226_get_calibration(ina226_handle_t *handle, uint16_t *data)
{
    uint8_t res;
   
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_CALIBRATION, (uint16_t *)data);       /* read calibration */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("ina226: read calibration register failed.\n");          /* read calibration register failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t ina226_calculate_calibration(ina226_handle_t *handle, uint16_t *calibration)
{
    double v;
   
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    if ((handle->r >= -0.000001f)  && (handle->r <= 0.000001f))        /* check the r */
    {
        handle->debug_print("ina226: r can't be zero.\n");             /* r can't be zero */
       
        return 4;                                                      /* return error */
    }
    
    v = 0.08192;                                                       /* set max range */
    handle->current_lsb = v / handle->r / pow(2.0, 15.0);              /* current lsb */
    *calibration = (uint16_t)(0.00512 / (v / pow(2.0, 15.0)));         /* set calibration */
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t ina226_set_calibration(ina226_handle_t *handle, uint16_t data)
{
    uint8_t res;
   
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_ina226_iic_write(handle, INA226_REG_CALIBRATION, data);                 /* write calibration */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("ina226: write calibration register failed.\n");        /* write calibration register failed */
       
        return 1;                                                                   /* return error */
    }
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t ina226_set_mask(ina226_handle_t *handle, ina226_mask_t mask, ina226_bool_t enable)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_MASK, (uint16_t *)&prev);        /* read mask */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ina226: read mask register failed.\n");            /* read mask register failed */
       
        return 1;                                                               /* return error */
    }
    prev &= ~(1 << mask);                                                       /* clear settings */
    prev |= (enable << mask);                                                   /* set mask */
    res = a_ina226_iic_write(handle, INA226_REG_MASK, prev);                    /* write mask */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ina226: write mask register failed.\n");           /* write mask register failed */
       
        return 1;                                                               /* return error */
    }
    
    return 0;                                                                   /* success return 0 */
}

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
uint8_t ina226_get_mask(ina226_handle_t *handle, ina226_mask_t mask, ina226_bool_t *enable)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_MASK, (uint16_t *)&prev);        /* read mask */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ina226: read mask register failed.\n");            /* read mask register failed */
       
        return 1;                                                               /* return error */
    }
    *enable = (ina226_bool_t)((prev >> mask) & 0x01);                           /* get bool */
    
    return 0;                                                                   /* success return 0 */
}

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
uint8_t ina226_set_conversion_ready_alert_pin(ina226_handle_t *handle, ina226_bool_t enable)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_MASK, (uint16_t *)&prev);        /* read mask */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ina226: read mask register failed.\n");            /* read mask register failed */
       
        return 1;                                                               /* return error */
    }
    prev &= ~(1 << 10);                                                         /* clear settings */
    prev |= (enable << 10);                                                     /* set mask */
    res = a_ina226_iic_write(handle, INA226_REG_MASK, prev);                    /* write mask */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ina226: write mask register failed.\n");           /* write mask register failed */
       
        return 1;                                                               /* return error */
    }
    
    return 0;                                                                   /* success return 0 */
}

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
uint8_t ina226_get_conversion_ready_alert_pin(ina226_handle_t *handle, ina226_bool_t *enable)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_MASK, (uint16_t *)&prev);        /* read mask */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ina226: read mask register failed.\n");            /* read mask register failed */
       
        return 1;                                                               /* return error */
    }
    *enable = (ina226_bool_t)((prev >> 10) & 0x01);                             /* get bool */
    
    return 0;                                                                   /* success return 0 */
}

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
uint8_t ina226_set_alert_polarity_pin(ina226_handle_t *handle, ina226_alert_polarity_t pin)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_MASK, (uint16_t *)&prev);        /* read mask */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ina226: read mask register failed.\n");            /* read mask register failed */
       
        return 1;                                                               /* return error */
    }
    prev &= ~(1 << 1);                                                          /* clear settings */
    prev |= (pin << 1);                                                         /* set mask */
    res = a_ina226_iic_write(handle, INA226_REG_MASK, prev);                    /* write mask */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ina226: write mask register failed.\n");           /* write mask register failed */
       
        return 1;                                                               /* return error */
    }
    
    return 0;                                                                   /* success return 0 */
}

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
uint8_t ina226_get_alert_polarity_pin(ina226_handle_t *handle, ina226_alert_polarity_t *pin)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_MASK, (uint16_t *)&prev);        /* read mask */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ina226: read mask register failed.\n");            /* read mask register failed */
       
        return 1;                                                               /* return error */
    }
    *pin = (ina226_alert_polarity_t)((prev >> 1) & 0x01);                       /* get alert polarity pin */
    
    return 0;                                                                   /* success return 0 */
}

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
uint8_t ina226_set_alert_latch(ina226_handle_t *handle, ina226_bool_t enable)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_MASK, (uint16_t *)&prev);        /* read mask */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ina226: read mask register failed.\n");            /* read mask register failed */
       
        return 1;                                                               /* return error */
    }
    prev &= ~(1 << 0);                                                          /* clear settings */
    prev |= (enable << 0);                                                      /* set mask */
    res = a_ina226_iic_write(handle, INA226_REG_MASK, prev);                    /* write mask */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ina226: write mask register failed.\n");           /* write mask register failed */
       
        return 1;                                                               /* return error */
    }
    
    return 0;                                                                   /* success return 0 */
}

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
uint8_t ina226_get_alert_latch(ina226_handle_t *handle, ina226_bool_t *enable)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_MASK, (uint16_t *)&prev);        /* read mask */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ina226: read mask register failed.\n");            /* read mask register failed */
       
        return 1;                                                               /* return error */
    }
    *enable = (ina226_bool_t)((prev >> 0) & 0x01);                              /* get bool */
    
    return 0;                                                                   /* success return 0 */
}

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
uint8_t ina226_set_alert_limit(ina226_handle_t *handle, uint16_t reg)
{
    uint8_t res;
   
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_ina226_iic_write(handle, INA226_REG_ALERT_LIMIT, reg);                  /* write config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("ina226: write alert limit register failed.\n");        /* write alert limit register failed */
       
        return 1;                                                                   /* return error */
    }
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t ina226_get_alert_limit(ina226_handle_t *handle, uint16_t *reg)
{
    uint8_t res;
   
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_ALERT_LIMIT, reg);                  /* read config */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("ina226: read alert limit register failed.\n");        /* read alert limit register failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t ina226_shunt_voltage_convert_to_register(ina226_handle_t *handle, float mV, uint16_t *reg)
{
    if (handle == NULL)                  /* check handle */
    {
        return 2;                        /* return error */
    }
    if (handle->inited != 1)             /* check handle initialization */
    {
        return 3;                        /* return error */
    }
    
    *reg = (uint16_t)(mV * 400.0f);      /* convert real data to register data */
    
    return 0;                            /* success return 0 */
}

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
uint8_t ina226_shunt_voltage_convert_to_data(ina226_handle_t *handle, uint16_t reg, float *mV)
{
    if (handle == NULL)               /* check handle */
    {
        return 2;                     /* return error */
    }
    if (handle->inited != 1)          /* check handle initialization */
    {
        return 3;                     /* return error */
    }
    
    *mV = (float)(reg) / 400.0f;      /* convert real data to register data */
    
    return 0;                         /* success return 0 */
}

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
uint8_t ina226_bus_voltage_convert_to_register(ina226_handle_t *handle, float mV, uint16_t *reg)
{
    if (handle == NULL)                   /* check handle */
    {
        return 2;                         /* return error */
    }
    if (handle->inited != 1)              /* check handle initialization */
    {
        return 3;                         /* return error */
    }
    
    *reg = (uint16_t)(mV / 1.25f);        /* convert real data to register data */
    
    return 0;                             /* success return 0 */
}

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
uint8_t ina226_bus_voltage_convert_to_data(ina226_handle_t *handle, uint16_t reg, float *mV)
{
    if (handle == NULL)                /* check handle */
    {
        return 2;                      /* return error */
    }
    if (handle->inited != 1)           /* check handle initialization */
    {
        return 3;                      /* return error */
    }
    
    *mV = (float)(reg) * 1.25f;        /* convert real data to register data */
    
    return 0;                          /* success return 0 */
}

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
uint8_t ina226_power_convert_to_register(ina226_handle_t *handle, float mW, uint16_t *reg)
{
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    *reg = (uint16_t)((double)(mW) / (handle->current_lsb * 25.0 * 1000.0));        /* set the converted data */
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t ina226_power_convert_to_data(ina226_handle_t *handle, uint16_t reg, float *mW)
{
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    *mW = (float)((double)(reg) * handle->current_lsb * 25.0 * 1000.0);        /* set the converted data */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t ina226_irq_handler(ina226_handle_t *handle)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_MASK, (uint16_t *)&prev);                    /* read mask */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("ina226: read mask register failed.\n");                        /* read mask register failed */
       
        return 1;                                                                           /* return error */
    }
    if ((prev & (1 << 4)) != 0)                                                             /* check alert flag */
    {
        if ((prev & (1 << 15)) != 0)                                                        /* shunt voltage over voltage */
        {
            if (handle->receive_callback != NULL)                                           /* check not null */
            {
                handle->receive_callback(INA226_STATUS_SHUNT_VOLTAGE_OVER_VOLTAGE);         /* run callback */
            }
        }
        else if ((prev & (1 << 14)) != 0)                                                   /* shunt voltage under voltage */
        {
            if (handle->receive_callback != NULL)                                           /* check not null */
            {
                handle->receive_callback(INA226_STATUS_SHUNT_VOLTAGE_UNDER_VOLTAGE);        /* run callback */
            }
        }
        else if ((prev & (1 << 13)) != 0)                                                   /* bus voltage over voltage */
        {
            if (handle->receive_callback != NULL)                                           /* check not null */
            {
                handle->receive_callback(INA226_STATUS_BUS_VOLTAGE_OVER_VOLTAGE);           /* run callback */
            }
        }
        else if ((prev & (1 << 12)) != 0)                                                   /* bus voltage under voltage */
        {
            if (handle->receive_callback != NULL)                                           /* check not null */
            {
                handle->receive_callback(INA226_STATUS_BUS_VOLTAGE_UNDER_VOLTAGE);          /* run callback */
            }
        }
        else if ((prev & (1 << 11)) != 0)                                                   /* power over limit */
        {
            if (handle->receive_callback != NULL)                                           /* check not null */
            {
                handle->receive_callback(INA226_STATUS_POWER_OVER_LIMIT);                   /* run callback */
            }
        }
        else
        {
                                                                                            /* do nothing */
        }
    }
    
    return 0;                                                                               /* success return 0 */
}

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
uint8_t ina226_init(ina226_handle_t *handle)
{
    uint8_t res;
    uint16_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->debug_print == NULL)                                                   /* check debug_print */
    {
        return 3;                                                                      /* return error */
    }
    if (handle->iic_init == NULL)                                                      /* check iic_init */
    {
        handle->debug_print("ina226: iic_init is null.\n");                            /* iic_init is null */
        
        return 3;                                                                      /* return error */
    }
    if (handle->iic_deinit == NULL)                                                    /* check iic_deinit */
    {
        handle->debug_print("ina226: iic_deinit is null.\n");                          /* iic_deinit is null */
        
        return 3;                                                                      /* return error */
    }
    if (handle->iic_read == NULL)                                                      /* check iic_read */
    {
        handle->debug_print("ina226: iic_read is null.\n");                            /* iic_read is null */
        
        return 3;                                                                      /* return error */
    }
    if (handle->iic_write == NULL)                                                     /* check iic_write */
    {
        handle->debug_print("ina226: iic_write is null.\n");                           /* iic_write is null */
        
        return 3;                                                                      /* return error */
    }
    if (handle->delay_ms == NULL)                                                      /* check delay_ms */
    {
        handle->debug_print("ina226: delay_ms is null.\n");                            /* delay_ms is null */
        
        return 3;                                                                      /* return error */
    }
    if (handle->receive_callback == NULL)                                              /* check receive_callback */
    {
        handle->debug_print("ina226: receive_callback is null.\n");                    /* receive_callback is null */
        
        return 3;                                                                      /* return error */
    }
    
    if (handle->iic_init() != 0)                                                       /* iic init */
    {
        handle->debug_print("ina226: iic init failed.\n");                             /* iic init failed */
        
        return 1;                                                                      /* return error */
    }
    res = a_ina226_iic_read(handle, INA226_REG_MANUFACTURER, (uint16_t *)&prev);       /* read manufacturer */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("ina226: read manufacturer failed.\n");                    /* read manufacturer failed */
        (void)handle->iic_deinit();                                                    /* iic deinit */
       
        return 4;                                                                      /* return error */
    }
    if (prev != 0x5449)                                                                /* check id */
    {
        handle->debug_print("ina226: id is invalid.\n");                               /* id is invalid */
        (void)handle->iic_deinit();                                                    /* iic deinit */
       
        return 4;                                                                      /* return error */
    }
    res = a_ina226_iic_read(handle, INA226_REG_CONF, (uint16_t *)&prev);               /* read conf */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("ina226: read conf register failed.\n");                   /* read conf register failed */
        (void)handle->iic_deinit();                                                    /* iic deinit */
       
        return 5;                                                                      /* return error */
    }
    prev |= 1 << 15;                                                                   /* set bit */
    res = a_ina226_iic_write(handle, INA226_REG_CONF, prev);                           /* write conf */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("ina226: write conf register failed.\n");                  /* write conf register failed */
        (void)handle->iic_deinit();                                                    /* iic deinit */
        
        return 5;                                                                      /* return error */
    }
    handle->delay_ms(10);                                                              /* delay 10 ms */
    res = a_ina226_iic_read(handle, INA226_REG_CONF, (uint16_t *)&prev);               /* read conf */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("ina226: read conf register failed.\n");                   /* read conf register failed */
        (void)handle->iic_deinit();                                                    /* iic deinit */
        
        return 5;                                                                      /* return error */
    }
    if ((prev & (1 << 15)) != 0)                                                       /* check the result */
    {
        handle->debug_print("ina226: soft reset failed.\n");                           /* soft reset failed */
        (void)handle->iic_deinit();                                                    /* iic deinit */
        
        return 5;                                                                      /* return error */
    }
    handle->trigger = 0;                                                               /* none */
    handle->inited = 1;                                                                /* flag inited */
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t ina226_deinit(ina226_handle_t *handle)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_ina226_iic_read(handle, INA226_REG_CONF, (uint16_t *)&prev);       /* read config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ina226: read conf register failed.\n");           /* read conf register failed */
       
        return 4;                                                              /* return error */
    }
    prev &= ~(0x07);                                                           /* clear mode */
    res = a_ina226_iic_write(handle, INA226_REG_CONF, (uint16_t )prev);        /* write config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ina226: write conf register failed.\n");          /* write conf register failed */
       
        return 4;                                                              /* return error */
    }
    res = handle->iic_deinit();                                                /* iic deinit */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ina226: iic deinit failed.\n");                   /* iic deinit failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t ina226_set_reg(ina226_handle_t *handle, uint8_t reg, uint16_t data)
{
    if (handle == NULL)                                 /* check handle */
    {
        return 2;                                       /* return error */
    }
    if (handle->inited != 1)                            /* check handle initialization */
    {
        return 3;                                       /* return error */
    }
    
    return a_ina226_iic_write(handle, reg, data);       /* write data */
}

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
uint8_t ina226_get_reg(ina226_handle_t *handle, uint8_t reg, uint16_t *data)
{
    if (handle == NULL)                                /* check handle */
    {
        return 2;                                      /* return error */
    }
    if (handle->inited != 1)                           /* check handle initialization */
    {
        return 3;                                      /* return error */
    }
    
    return a_ina226_iic_read(handle, reg, data);       /* read data */
}

/**
 * @brief      get chip's information
 * @param[out] *info pointer to an ina226 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ina226_info(ina226_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(ina226_info_t));                         /* initialize ina226 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
