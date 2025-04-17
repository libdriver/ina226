[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver INA226

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ina226/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The INA226 is a current shunt and power monitor with an I2C™- or SMBUS-compatible interface. The device monitors both a shunt voltage drop and bus supply voltage. Programmable calibration value, conversion times, and averaging, combined with an internal multiplier, enable direct readouts of current in amperes and power in watts.The INA226 senses current on common-mode bus voltages that can vary from 0V to 36V, independent of the supply voltage. The device operates from a single 2.7V to 5.5V supply, drawing a typical of 330μA of supply current. The device is specified over the operating temperature range between –40°C and 125°C and features up to 16 programmable addresses on the I2C-compatible interface.

LibDriver INA226 is a full-featured driver of INA226 launched by LibDriver.It provides continuous mode reading, single mode reading, current acquisition, voltage acquisition, power acquisition, alert detect and other functions.LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example alert](#example-alert)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver INA226 source files.

/interface includes LibDriver INA226 IIC platform independent template.

/test includes LibDriver INA226 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver INA226 sample code.

/doc includes LibDriver INA226 offline document.

/datasheet includes INA226 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface IIC platform independent template and finish your platform IIC driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_ina226_basic.h"

uint8_t res;

res = ina226_basic_init(INA226_ADDRESS_0, 0.1);
if (res != 0)
{
    return 1;
}

...

for (i = 0; i < times; i++)
{
    float mV;
    float mA;
    float mW;

    res = ina226_basic_read(&mV, &mA, &mW);
    if (res != 0)
    {
        (void)ina226_basic_deinit();

        return 1;
    }

    ina226_interface_debug_print("ina226: %d/%d.\n", i + 1, times);
    ina226_interface_debug_print("ina226: bus voltage is %0.3fmV.\n", mV);
    ina226_interface_debug_print("ina226: current is %0.3fmA.\n", mA);
    ina226_interface_debug_print("ina226: power is %0.3fmW.\n", mW);
    ina226_interface_delay_ms(1000);
    
    ...
}

...

(void)ina226_basic_deinit();

return 0;
```

#### example shot

```C
#include "driver_ina226_shot.h"

uint8_t res;

res = ina226_shot_init(INA226_ADDRESS_0, 0.1);
if (res != 0)
{
    return 1;
}

...

for (i = 0; i < times; i++)
{
    float mV;
    float mA;
    float mW;

    res = ina226_shot_read(&mV, &mA, &mW);
    if (res != 0)
    {
        (void)ina226_shot_deinit();

        return 1;
    }

    ina226_interface_debug_print("ina226: %d/%d.\n", i + 1, times);
    ina226_interface_debug_print("ina226: bus voltage is %0.3fmV.\n", mV);
    ina226_interface_debug_print("ina226: current is %0.3fmA.\n", mA);
    ina226_interface_debug_print("ina226: power is %0.3fmW.\n", mW);
    ina226_interface_delay_ms(1000);
    
    ...
}

...

(void)ina226_shot_deinit();

return 0;
```

#### example alert

```C
#include "driver_ina226_alert.h"

uint8_t res;
uint8_t (*g_gpio_irq)(void) = NULL;

static void a_receive_callback(uint8_t type)
{
    switch (type)
    {
        case INA226_STATUS_SHUNT_VOLTAGE_OVER_VOLTAGE :
        {
            ina226_interface_debug_print("ina226: irq shunt voltage over voltage.\n");
            
            break;
        }
        case INA226_STATUS_SHUNT_VOLTAGE_UNDER_VOLTAGE :
        {
            ina226_interface_debug_print("ina226: irq shunt voltage under voltage.\n");
            
            break;
        }
        case INA226_STATUS_BUS_VOLTAGE_OVER_VOLTAGE :
        {
            ina226_interface_debug_print("ina226: irq bus voltage over voltage.\n");
            
            break;
        }
        case INA226_STATUS_BUS_VOLTAGE_UNDER_VOLTAGE :
        {
            ina226_interface_debug_print("ina226: irq bus voltage under voltage.\n");
            
            break;
        }
        case INA226_STATUS_POWER_OVER_LIMIT :
        {
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

/* gpio interrupt init */
res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}
g_gpio_irq = ina226_alert_irq_handler;

/* alert init */
res = ina226_alert_init(INA226_ADDRESS_0, 0.1, INA226_MASK_BUS_VOLTAGE_OVER_VOLTAGE, 3300.0f, a_receive_callback);
if (res != 0)
{
    g_gpio_irq = NULL;
    (void)gpio_interrupt_deinit();

    return 1;
}

...
    
/* alert deinit */
g_gpio_irq = NULL;
(void)gpio_interrupt_deinit();
(void)ina226_alert_deinit();

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/ina226/index.html](https://www.libdriver.com/docs/ina226/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.