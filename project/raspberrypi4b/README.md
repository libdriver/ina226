### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

GPIO Pin: INT GPIO17.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(ina226 REQUIRED)
```

### 3. INA226

#### 3.1 Command Instruction

1. Show ina226 chip and driver information.

   ```shell
   ina226 (-i | --information)
   ```

2. Show ina226 help.

   ```shell
   ina226 (-h | --help)
   ```

3. Show ina226 pin connections of the current board.

   ```shell
   ina226 (-p | --port)
   ```

4. Run ina226 register test.

   ```shell
   ina226 (-t reg | --test=reg) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>]
   ```

5. Run ina226 read test, num is the test times, r is the sample resistance.

   ```shell
   ina226 (-t read | --test=read) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>] [--resistance=<r>] [--times=<num>]
   ```

6. Run ina226 alert test, r is the sample resistance.type is the alert type, 0: shunt voltage over voltage;1: shunt voltage under voltage;2: bus voltage over voltage;3: bus voltage under voltage;4: power over limit.th is the alert threshold.

   ```shell
   ina226 (-t alert | --test=alert) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>] [--resistance=<r>] [--type=<type>][--threshold=<th>]
   ```

7. Run ina226 read function, num is the test times, r is the sample resistance.

   ```shell
   ina226 (-e read | --example=read) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>] [--resistance=<r>] [--times=<num>]
   ```

8. Run ina226 shot function, num is test times, r is the sample resistance.

   ```shell
   ina226 (-e shot | --example=shot) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>] [--resistance=<r>] [--times=<num>]
   ```
   
9. Run ina226 alert function, r is the sample resistance.type is the alert type, 0: shunt voltage over voltage;1: shunt voltage under voltage;2: bus voltage over voltage;3: bus voltage under voltage;4: power over limit. th is the alert threshold.

   ```shell
   ina226 (-e alert | --example=alert) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>] [--resistance=<r>] [--type=<type>][--threshold=<th>]
   ```

#### 3.2 Command Example

```shell
./ina226 -i

ina226: chip is Texas Instruments INA226.
ina226: manufacturer is Texas Instruments.
ina226: interface is IIC.
ina226: driver version is 1.0.
ina226: min supply voltage is 2.7V.
ina226: max supply voltage is 5.5V.
ina226: max current is 0.33mA.
ina226: max temperature is 125.0C.
ina226: min temperature is -40.0C.
```

```shell
./ina226 -p

ina226: SCL connected to GPIO3(BCM).
ina226: SDA connected to GPIO2(BCM).
ina226: INT connected to GPIO17(BCM).
```

```shell
./ina226 -t reg --addr=0

ina226: chip is Texas Instruments INA226.
ina226: manufacturer is Texas Instruments.
ina226: interface is IIC.
ina226: driver version is 1.0.
ina226: min supply voltage is 2.7V.
ina226: max supply voltage is 5.5V.
ina226: max current is 0.33mA.
ina226: max temperature is 125.0C.
ina226: min temperature is -40.0C.
ina226: start register test.
ina226: ina226_set_addr_pin/ina226_get_addr_pin test.
ina226: set addr pin 0.
ina226: check addr pin ok.
ina226: set addr pin 1.
ina226: check addr pin ok.
ina226: set addr pin 2.
ina226: check addr pin ok.
ina226: set addr pin 3.
ina226: check addr pin ok.
ina226: set addr pin 4.
ina226: check addr pin ok.
ina226: set addr pin 5.
ina226: check addr pin ok.
ina226: set addr pin 6.
ina226: check addr pin ok.
ina226: set addr pin 7.
ina226: check addr pin ok.
ina226: set addr pin 8.
ina226: check addr pin ok.
ina226: set addr pin 9.
ina226: check addr pin ok.
ina226: set addr pin 10.
ina226: check addr pin ok.
ina226: set addr pin 11.
ina226: check addr pin ok.
ina226: set addr pin 12.
ina226: check addr pin ok.
ina226: set addr pin 13.
ina226: check addr pin ok.
ina226: set addr pin 14.
ina226: check addr pin ok.
ina226: set addr pin 15.
ina226: check addr pin ok.
ina226: ina226_set_resistance/ina226_get_resistance test.
ina226: set resistance 0.083000.
ina226: check resistance 0.083000.
ina226: ina226_set_average_mode/ina226_get_average_mode test.
ina226: set average 1.
ina226: check average mode ok.
ina226: set average 4.
ina226: check average mode ok.
ina226: set average 16.
ina226: check average mode ok.
ina226: set average 64.
ina226: check average mode ok.
ina226: set average 128.
ina226: check average mode ok.
ina226: set average 256.
ina226: check average mode ok.
ina226: set average 512.
ina226: check average mode ok.
ina226: set average 1024.
ina226: check average mode ok.
ina226: ina226_set_bus_voltage_conversion_time/ina226_get_bus_voltage_conversion_time test.
ina226: set bus voltage conversion time 140us.
ina226: check bus voltage conversion time ok.
ina226: set bus voltage conversion time 204us.
ina226: check bus voltage conversion time ok.
ina226: set bus voltage conversion time 332us.
ina226: check bus voltage conversion time ok.
ina226: set bus voltage conversion time 588us.
ina226: check bus voltage conversion time ok.
ina226: set bus voltage conversion time 1.1ms.
ina226: check bus voltage conversion time ok.
ina226: set bus voltage conversion time 2.116ms.
ina226: check bus voltage conversion time ok.
ina226: set bus voltage conversion time 4.156ms.
ina226: check bus voltage conversion time ok.
ina226: set bus voltage conversion time 8.244ms.
ina226: check bus voltage conversion time ok.
ina226: ina226_set_shunt_voltage_conversion_time/ina226_get_shunt_voltage_conversion_time test.
ina226: set shunt voltage conversion time 140us.
ina226: check shunt voltage conversion time ok.
ina226: set shunt voltage conversion time 204us.
ina226: check shunt voltage conversion time ok.
ina226: set shunt voltage conversion time 332us.
ina226: check shunt voltage conversion time ok.
ina226: set shunt voltage conversion time 588us.
ina226: check shunt voltage conversion time ok.
ina226: set shunt voltage conversion time 1.1ms.
ina226: check shunt voltage conversion time ok.
ina226: set shunt voltage conversion time 2.116ms.
ina226: check shunt voltage conversion time ok.
ina226: set shunt voltage conversion time 4.156ms.
ina226: check shunt voltage conversion time ok.
ina226: set shunt voltage conversion time 8.244ms.
ina226: check shunt voltage conversion time ok.
ina226: ina226_set_mode/ina226_get_mode test.
ina226: set mode power down.
ina226: check mode ok.
ina226: set mode shutdown.
ina226: check mode ok.
ina226: set mode shunt voltage continuous.
ina226: check mode ok.
ina226: set mode bus voltage continuous.
ina226: check mode ok.
ina226: set mode shunt and bus voltage continuous.
ina226: check mode ok.
ina226: ina226_set_calibration/ina226_get_calibration test.
ina226: set calibration 0x08BD.
ina226: check calibration ok.
ina226: ina226_calculate_calibration test.
ina226: calculate calibration is 0x0800.
ina226: ina226_set_mask/ina226_get_mask test.
ina226: enable shunt voltage over voltage.
ina226: check mask ok.
ina226: disable shunt voltage over voltage.
ina226: check mask ok.
ina226: enable shunt voltage under voltage.
ina226: check mask ok.
ina226: disable shunt voltage under voltage.
ina226: check mask ok.
ina226: enable bus voltage over voltage.
ina226: check mask ok.
ina226: disable bus voltage over voltage.
ina226: check mask ok.
ina226: enable bus voltage under voltage.
ina226: check mask ok.
ina226: disable bus voltage under voltage.
ina226: check mask ok.
ina226: enable power over limit.
ina226: check mask ok.
ina226: disable power over limit.
ina226: check mask ok.
ina226: ina226_set_conversion_ready_alert_pin/ina226_get_conversion_ready_alert_pin test.
ina226: enable conversion ready alert pin.
ina226: check conversion ready alert pin ok.
ina226: disable conversion ready alert pin.
ina226: check conversion ready alert pin ok.
ina226: ina226_set_alert_polarity_pin/ina226_get_alert_polarity_pin test.
ina226: set alert polarity pin normal.
ina226: check alert polarity pin ok.
ina226: set alert polarity pin inverted.
ina226: check alert polarity pin ok.
ina226: ina226_set_alert_latch/ina226_get_alert_latch test.
ina226: enable alert latch.
ina226: check alert latch ok.
ina226: disable alert latch.
ina226: check alert latch ok.
ina226: ina226_set_alert_limit/ina226_get_alert_limit test.
ina226: set alert limit 0xFCA5.
ina226: check alert limit ok.
ina226: ina226_shunt_voltage_convert_to_register/ina226_shunt_voltage_convert_to_data test.
ina226: set shunt voltage 9.15mV.
ina226: check shunt voltage 9.15mV.
ina226: ina226_bus_voltage_convert_to_register/ina226_bus_voltage_convert_to_data test.
ina226: set bus voltage 7.93mV.
ina226: check bus voltage 7.50mV.
ina226: ina226_power_convert_to_register/ina226_power_convert_to_data test.
ina226: set power 3.35mW.
ina226: check power 3.01mW.
ina226: ina226_get_die_id test.
ina226: device id is 0x0226.
ina226: die revision id is 0x00.
ina226: ina226_soft_reset test.
ina226: finish register test.
```

```shell
./ina226 -t read --addr=0 --resistance=0.1 --times=3

ina226: chip is Texas Instruments INA226.
ina226: manufacturer is Texas Instruments.
ina226: interface is IIC.
ina226: driver version is 1.0.
ina226: min supply voltage is 2.7V.
ina226: max supply voltage is 5.5V.
ina226: max current is 0.33mA.
ina226: max temperature is 125.0C.
ina226: min temperature is -40.0C.
ina226: start read test.
ina226: shunt bus voltage continuous test.
ina226: shunt voltage is 1.087mV.
ina226: bus voltage is 4893.750mV.
ina226: current is 10.875mA.
ina226: power is 53.125mW.
ina226: shunt voltage is 1.087mV.
ina226: bus voltage is 4891.250mV.
ina226: current is 10.875mA.
ina226: power is 53.125mW.
ina226: shunt voltage is 1.090mV.
ina226: bus voltage is 4893.750mV.
ina226: current is 10.900mA.
ina226: power is 53.125mW.
ina226: shunt and bus triggered test.
ina226: shunt voltage is 1.090mV.
ina226: bus voltage is 4893.750mV.
ina226: current is 10.900mA.
ina226: power is 53.125mW.
ina226: shunt voltage is 1.090mV.
ina226: bus voltage is 4896.250mV.
ina226: current is 10.900mA.
ina226: power is 53.125mW.
ina226: shunt voltage is 1.090mV.
ina226: bus voltage is 4893.750mV.
ina226: current is 10.900mA.
ina226: power is 53.125mW.
ina226: finish read test.
```

```shell
./ina226 -t alert --addr=0 --resistance=0.1 --type=0 --threshold=0.8

ina226: chip is Texas Instruments INA226.
ina226: manufacturer is Texas Instruments.
ina226: interface is IIC.
ina226: driver version is 1.0.
ina226: min supply voltage is 2.7V.
ina226: max supply voltage is 5.5V.
ina226: max current is 0.33mA.
ina226: max temperature is 125.0C.
ina226: min temperature is -40.0C.
ina226: start alert test.
ina226: irq shunt voltage over voltage.
ina226: finish alert test.
```
```shell
./ina226 -t alert --addr=0 --resistance=0.1 --type=1 --threshold=0.8

ina226: chip is Texas Instruments INA226.
ina226: manufacturer is Texas Instruments.
ina226: interface is IIC.
ina226: driver version is 1.0.
ina226: min supply voltage is 2.7V.
ina226: max supply voltage is 5.5V.
ina226: max current is 0.33mA.
ina226: max temperature is 125.0C.
ina226: min temperature is -40.0C.
ina226: start alert test.
ina226: irq shunt voltage under voltage.
ina226: finish alert test.
```
```shell
./ina226 -t alert --addr=0 --resistance=0.1 --type=2 --threshold=3000.0

ina226: chip is Texas Instruments INA226.
ina226: manufacturer is Texas Instruments.
ina226: interface is IIC.
ina226: driver version is 1.0.
ina226: min supply voltage is 2.7V.
ina226: max supply voltage is 5.5V.
ina226: max current is 0.33mA.
ina226: max temperature is 125.0C.
ina226: min temperature is -40.0C.
ina226: start alert test.
ina226: irq bus voltage over voltage.
ina226: finish alert test.
```
```shell
./ina226 -t alert --addr=0 --resistance=0.1 --type=3 --threshold=3000.0

ina226: chip is Texas Instruments INA226.
ina226: manufacturer is Texas Instruments.
ina226: interface is IIC.
ina226: driver version is 1.0.
ina226: min supply voltage is 2.7V.
ina226: max supply voltage is 5.5V.
ina226: max current is 0.33mA.
ina226: max temperature is 125.0C.
ina226: min temperature is -40.0C.
ina226: start alert test.
ina226: irq bus voltage under voltage.
ina226: finish alert test.
```
```shell
./ina226 -t alert --addr=0 --resistance=0.1 --type=4 --threshold=50.0

ina226: chip is Texas Instruments INA226.
ina226: manufacturer is Texas Instruments.
ina226: interface is IIC.
ina226: driver version is 1.0.
ina226: min supply voltage is 2.7V.
ina226: max supply voltage is 5.5V.
ina226: max current is 0.33mA.
ina226: max temperature is 125.0C.
ina226: min temperature is -40.0C.
ina226: start alert test.
ina226: irq power over limit.
ina226: finish alert test.
```
```shell
./ina226 -e read --addr=0 --resistance=0.1 --times=3

ina226: 1/3.
ina226: bus voltage is 4857.500mV.
ina226: current is 10.725mA.
ina226: power is 52.500mW.
ina226: 2/3.
ina226: bus voltage is 4863.750mV.
ina226: current is 10.750mA.
ina226: power is 52.500mW.
ina226: 3/3.
ina226: bus voltage is 4861.250mV.
ina226: current is 10.750mA.
ina226: power is 52.500mW.
```

```shell
./ina226 -e shot --addr=0 --resistance=0.1 --times=3

ina226: 1/3.
ina226: bus voltage is 4861.250mV.
ina226: current is 10.775mA.
ina226: power is 52.500mW.
ina226: 2/3.
ina226: bus voltage is 4861.250mV.
ina226: current is 10.775mA.
ina226: power is 52.500mW.
ina226: 3/3.
ina226: bus voltage is 4861.250mV.
ina226: current is 10.775mA.
ina226: power is 52.500mW.
```

```shell
./ina226 -e alert --addr=0 --resistance=0.1 --type=0 --threshold=0.8

ina226: set shunt voltage over voltage 0.800mV.
ina226: irq shunt voltage over voltage.
```
```shell
./ina226 -e alert --addr=0 --resistance=0.1 --type=1 --threshold=0.8

ina226: set shunt voltage under voltage 0.800mV.
ina226: irq shunt voltage under voltage.
```
```shell
./ina226 -e alert --addr=0 --resistance=0.1 --type=2 --threshold=3000.0

ina226: set bus voltage over voltage 3000.000mV.
ina226: irq bus voltage over voltage.
```
```shell
./ina226 -e alert --addr=0 --resistance=0.1 --type=3 --threshold=3000.0

ina226: set bus voltage under voltage 3000.000mV.
ina226: irq bus voltage under voltage.
```
```shell
./ina226 -e alert --addr=0 --resistance=0.1 --type=4 --threshold=50.0

ina226: set power over limit 50.000mV.
ina226: irq power over limit.
```

```shell
./ina226 -h

Usage:
  ina226 (-i | --information)
  ina226 (-h | --help)
  ina226 (-p | --port)
  ina226 (-t reg | --test=reg) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>]
  ina226 (-t read | --test=read) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>]
         [--resistance=<r>] [--times=<num>]
  ina226 (-t alert | --test=alert) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>]
         [--resistance=<r>] [--type=<type>][--threshold=<th>]
  ina226 (-e read | --example=read) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>]
         [--resistance=<r>] [--times=<num>]
  ina226 (-e shot | --example=shot) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>]
         [--resistance=<r>] [--times=<num>]
  ina226 (-e alert | --example=alert) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>]
         [--resistance=<r>] [--type=<type>][--threshold=<th>]

Options:
      --addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F>
                                 Set the addr pin.([default: 0])
  -e <read | shot | alert>, --example=<read | shot | alert>
                                 Run the driver example.
  -h, --help                     Show the help.
  -i, --information              Show the chip information.
  -p, --port                     Display the pin connections of the current board.
      --resistance=<r>           Set the sample resistance.([default: 0.1])
  -t <reg | read | alert>, --test=<reg | read | alert>
                                 Run the driver test.
      --times=<num>              Set the running times.([default: 3])
      --type=<type>              Set alert type,
                                 0: shunt voltage over voltage;
                                 1: shunt voltage under voltage;
                                 2: bus voltage over voltage;
                                 3: bus voltage under voltage;
                                 4: power over limit.([default: 2])
      --threshold=<th>           Set alert threshold.([default: 3300.0f])
```

