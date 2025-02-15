[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver INA226

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ina226/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

INA226은 I2C™ 또는 SMBUS 호환 인터페이스를 갖춘 전류 션트 및 전력 모니터입니다. 이 장치는 션트 전압 강하와 버스 공급 전압을 모두 모니터링합니다. 프로그래밍 가능한 교정 값, 변환 시간 및 평균화는 내부 배수와 결합되어 전류(암페어) 및 전력(와트)을 직접 읽을 수 있게 해줍니다. INA226은 공급 전압에 관계없이 0V~36V의 공통 모드 버스 전압 범위에서 전류를 감지할 수 있습니다. 이 장치는 2.7V~5.5V의 단일 전원 공급 장치에서 작동하고 일반적으로 330μA의 전원 공급 전류를 소모합니다. 이 장치는 -40°C ~ 125°C 온도 범위에서 작동하며 I2C 호환 인터페이스에서 최대 16개의 프로그래밍 가능한 주소를 갖습니다.

LibDriver INA226은 LibDriver가 출시한 INA226의 모든 기능을 갖춘 드라이버입니다. 연속 모드 판독, 단일 모드 판독, 전류 수집, 전압 수집, 전력 수집, 경고 감지 및 기타 기능을 제공합니다. LibDriver는 MISRA 규격을 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example alert](#example-alert)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver INA226의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver INA226용 플랫폼 독립적인 IIC버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver INA226드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver INA226프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver INA226오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 INA226데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인 IIC버스 템플릿을 참조하여 지정된 플랫폼에 대한 IIC버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 문서

온라인 문서: [https://www.libdriver.com/docs/ina226/index.html](https://www.libdriver.com/docs/ina226/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.