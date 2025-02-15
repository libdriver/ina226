[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver INA226

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ina226/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

INA226是具有I2C ™  或SMBUS相容介面的電流分流器和功率監測器。 該器件同時監控分流壓降和匯流排電源電壓。 可程式設計校準值、轉換時間、和平均值計算，與一個內部乘法器相組合，實現電流值（安培）和功率值（瓦）的直接讀取。 INA226可在0V至36V的共模匯流排電壓範圍內感測電流，與電源電壓無關。 該器件由一個2.7V至5.5V的單電源供電，汲取330μA的典型電源電流。 該器件的額定工作溫度範圍為–40°C至125°C，I2C相容介面上具有多達16個可程式設計地址。

LibDriver INA226是LibDriver推出的INA226的全功能驅動，該驅動提供連續模式讀取、單次模式讀取，電流採集、電壓採集、功率採集、報警檢測等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example alert](#example-alert)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver INA226的源文件。

/interface目錄包含了LibDriver INA226與平台無關的IIC總線模板。

/test目錄包含了LibDriver INA226驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver INA226編程範例。

/doc目錄包含了LibDriver INA226離線文檔。

/datasheet目錄包含了INA226數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC總線模板，完成指定平台的IIC總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/ina226/index.html](https://www.libdriver.com/docs/ina226/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。

