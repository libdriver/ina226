[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver INA226
[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ina226/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der INA226 ist ein Strom-Shunt und Leistungsmonitor mit einer I2C™- oder SMBUS-kompatiblen Schnittstelle. Das Gerät überwacht sowohl einen Shunt-Spannungsabfall als auch die Busversorgungsspannung. Programmierbare Kalibrierungswerte, Konvertierungszeiten und Mittelwertbildung ermöglichen in Kombination mit einem internen Multiplikator die direkte Anzeige von Strom in Ampere und Leistung in Watt. Der INA226 erfasst Strom auf Gleichtakt-Busspannungen, die unabhängig von der Versorgungsspannung zwischen 0 V und 36 V variieren können. Das Gerät arbeitet mit einer einzelnen 2,7-V- bis 5,5-V-Versorgung und zieht einen typischen Versorgungsstrom von 330 μA. Das Gerät ist für den Betriebstemperaturbereich zwischen –40 °C und 125 °C spezifiziert und verfügt über bis zu 16 programmierbare Adressen auf der I2C-kompatiblen Schnittstelle.

LibDriver INA226 ist der voll funktionsfähige Treiber von INA226, der von LibDriver herausgebracht wurde. Er bietet kontinuierliches Lesen, Einzelmodus-Lesen, Stromerfassung, Spannungserfassung, Leistungserfassung, Alarmerkennung und andere Funktionen. LibDriver ist MISRA-kompatibel.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example alert](#example-alert)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver INA226-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver INA226IIC.

/test enthält den Testcode des LibDriver INA226-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver INA226-Beispielcode.

/doc enthält das LibDriver INA226-Offlinedokument.

/Datenblatt enthält INA226-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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


### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/ina226/index.html](https://www.libdriver.com/docs/ina226/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.