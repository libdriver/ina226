[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver INA226

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ina226/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

INA226 是具有 I2C™ 或 SMBUS 兼容接口的电流分流器和功率监测器。该器件同时监控分流压降和总线电源电压。可编程校准值、转换时间、和均值计算，与一个内部乘法器相组合，实现电流值（安培）和功率值（瓦）的直接读取。INA226 可在 0V 至 36V 的共模总线电压范围内感测电流，与电源电压无关。该器件由一个 2.7V 至 5.5V 的单电源供电，汲取 330μA 的典型电源电流。该器件的额定工作温度范围为 –40°C 至 125°C，I2C 兼容接口上具有多达 16 个可编程地址。

LibDriver INA226是LibDriver推出的INA226的全功能驱动，该驱动提供连续模式读取、单次模式读取，电流采集、电压采集、功率采集、报警检测等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example alert](#example-alert)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver INA226的源文件。

/interface目录包含了LibDriver INA226与平台无关的IIC总线模板。

/test目录包含了LibDriver INA226驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver INA226编程范例。

/doc目录包含了LibDriver INA226离线文档。

/datasheet目录包含了INA226数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC总线模板，完成指定平台的IIC总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

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

### 文档

在线文档: [https://www.libdriver.com/docs/ina226/index.html](https://www.libdriver.com/docs/ina226/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。