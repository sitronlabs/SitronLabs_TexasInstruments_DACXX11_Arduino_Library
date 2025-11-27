[![Designed by Sitron Labs](https://img.shields.io/badge/Designed_by-Sitron_Labs-FCE477.svg)](https://www.sitronlabs.com/)
[![Join the Discord community](https://img.shields.io/discord/552242187665145866.svg?logo=discord&logoColor=white&label=Discord&color=%237289da)](https://discord.gg/btnVDeWhfW)
[![PayPal Donate](https://img.shields.io/badge/PayPal-Donate-00457C.svg?logo=paypal&logoColor=white)](https://www.paypal.com/donate/?hosted_button_id=QLX8VU9Q3PFFL)
![License](https://img.shields.io/github/license/sitronlabs/SitronLabs_TexasInstruments_DACXX11_Arduino_Library.svg)
![Latest Release](https://img.shields.io/github/release/sitronlabs/SitronLabs_TexasInstruments_DACXX11_Arduino_Library.svg)
[![Arduino Library Manager](https://www.ardu-badge.com/badge/Sitron%20Labs%20DACXX11%20Arduino%20Library.svg)](https://www.ardu-badge.com/Sitron%20Labs%20DACXX11%20Arduino%20Library)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/sitronlabs/library/Sitron_Labs_DACXX11_Arduino_Library.svg)](https://registry.platformio.org/libraries/sitronlabs/Sitron_Labs_DACXX11_Arduino_Library)

# Sitron Labs Texas Instruments DACXX11 Arduino Library

Arduino library for interfacing with the Texas Instruments family of DACxx11 digital-to-analog converters (DACs).

## Description

The DACxx11 family consists of buffered, rail-to-rail voltage output DACs with different resolutions:
- **DAC5311**: 8-bit resolution
- **DAC6311**: 10-bit resolution
- **DAC7311**: 12-bit resolution
- **DAC8311**: 14-bit resolution
- **DAC8411**: 16-bit resolution

All devices feature SPI communication, low power consumption, and power-down modes. This library provides a unified interface to control any device in the family, with automatic handling of resolution-specific differences.

## Installation

### Arduino IDE

Install via the Arduino Library Manager by searching for "Sitron Labs DACXX11".

Alternatively, install manually:
1. Download or clone this repository
2. Place it in your Arduino `libraries` folder
3. Restart the Arduino IDE

### PlatformIO

Install via the PlatformIO Library Manager by searching for "Sitron Labs DACXX11".

Alternatively, add the library manually to your `platformio.ini` file:

```ini
lib_deps = 
    https://github.com/sitronlabs/SitronLabs_TexasInstruments_DACXX11_Arduino_Library.git
```

## Hardware Connections

Connect the DAC to your Arduino using SPI:

- AVDD/VREF → 2.7V to 5.5V (check your board's specifications)
- GND → GND
- DIN → MOSI (SPI Data)
- SCLK → SCK (SPI Clock)
- SYNC/CS → Any digital pin (Chip Select)
- VOUT → Your analog output

**Note:** The SYNC/CS pin can be connected to any available digital pin on your Arduino. Make sure to specify this pin number when calling `setup()`.

## Usage

### Basic Voltage Output

```cpp
#include <SPI.h>
#include <dac7311.h>  // For 12-bit DAC7311

// Create DAC object
dac7311 dac;

// SPI settings
const int SPI_SPEED = 10000000;  // 10 MHz
const int PIN_CS = 10;           // Chip select pin
const float REFERENCE_VOLTAGE = 3.3;  // Reference voltage in volts

void setup() {
  Serial.begin(9600);
  
  // Initialize SPI
  SPI.begin();
  
  // Setup the DAC (SPI library, SPI speed, CS pin, reference voltage)
  if (dac.setup(SPI, SPI_SPEED, PIN_CS, REFERENCE_VOLTAGE) != 0) {
    Serial.println("Failed to setup DAC");
    return;
  }
  
  Serial.println("DAC initialized");
}

void loop() {
  // Set output to 1.65V (half of 3.3V reference)
  if (dac.output_voltage_set(1.65) != 0) {
    Serial.println("Failed to set output voltage");
  }
  delay(1000);
  
  // Set output to 0V
  if (dac.output_voltage_set(0.0) != 0) {
    Serial.println("Failed to set output voltage");
  }
  delay(1000);
}
```

### Using Output Ratio

```cpp
#include <SPI.h>
#include <dac8411.h>  // For 16-bit DAC8411

dac8411 dac;
const int SPI_SPEED = 10000000;
const int PIN_CS = 10;
const float REFERENCE_VOLTAGE = 5.0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  
  dac.setup(SPI, SPI_SPEED, PIN_CS, REFERENCE_VOLTAGE);
}

void loop() {
  // Set output to 50% of reference voltage (2.5V with 5V reference)
  dac.output_ratio_set(0.5);
  delay(1000);
  
  // Set output to 25% of reference voltage (1.25V with 5V reference)
  dac.output_ratio_set(0.25);
  delay(1000);
  
  // Set output to 75% of reference voltage (3.75V with 5V reference)
  dac.output_ratio_set(0.75);
  delay(1000);
}
```

### Power Down Modes

```cpp
#include <SPI.h>
#include <dac5311.h>  // For 8-bit DAC5311

dac5311 dac;
const int SPI_SPEED = 10000000;
const int PIN_CS = 10;
const float REFERENCE_VOLTAGE = 3.3;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  
  dac.setup(SPI, SPI_SPEED, PIN_CS, REFERENCE_VOLTAGE);
  
  // Set output voltage
  dac.output_voltage_set(1.65);
  delay(1000);
  
  // Power down with 1 kΩ to GND
  dac.power_down(dac5311::POWER_DOWN_PD1K);
  delay(1000);
  
  // Power down with 100 kΩ to GND
  dac.power_down(dac5311::POWER_DOWN_PD100K);
  delay(1000);
  
  // Power down with high impedance (output floating)
  dac.power_down(dac5311::POWER_DOWN_HIGHZ);
  delay(1000);
  
  // Resume normal operation by setting a voltage
  dac.output_voltage_set(1.65);
}
```

### Changing Reference Voltage

```cpp
#include <SPI.h>
#include <dac8311.h>  // For 14-bit DAC8311

dac8311 dac;
const int SPI_SPEED = 10000000;
const int PIN_CS = 10;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  
  // Initialize with 3.3V reference
  dac.setup(SPI, SPI_SPEED, PIN_CS, 3.3);
  
  // Set output to 1.65V (50% of 3.3V)
  dac.output_voltage_set(1.65);
  delay(1000);
  
  // Change reference voltage to 5.0V
  dac.reference_voltage_set(5.0);
  
  // Now 1.65V is only 33% of the new reference
  // To get 50% again, set to 2.5V
  dac.output_voltage_set(2.5);
}
```

## API Reference

### setup(SPIClass& spi_library, int spi_speed, int pin_cs, float voltage)

Initializes the DAC device with SPI communication.

- `spi_library`: SPI library instance to use (typically `SPI`)
- `spi_speed`: SPI clock speed in Hz (maximum 50 MHz)
- `pin_cs`: GPIO pin number connected to the chip select (CS) pin
- `voltage`: Reference voltage in volts (maximum 5.5V)

Returns 0 on success, or a negative error code otherwise.

### reference_voltage_set(float voltage)

Updates the reference voltage value used for voltage calculations.

- `voltage`: Reference voltage in volts (maximum 5.5V)

Returns 0 on success, or a negative error code otherwise.

**Note:** This only updates the library's internal reference voltage value. It does not change the actual hardware reference voltage.

### output_ratio_set(float ratio)

Sets the DAC output as a ratio of the reference voltage.

- `ratio`: Output ratio from 0.0 (0% of reference) to 1.0 (100% of reference)

Returns 0 on success, or a negative error code otherwise.

### output_voltage_set(float voltage)

Sets the DAC output to a specific voltage.

- `voltage`: Desired output voltage in volts (must be between 0 and the reference voltage)

Returns 0 on success, or a negative error code otherwise.

### power_down(power_down_mode mode)

Puts the DAC into a power-down mode to reduce power consumption.

- `mode`: Power-down mode to use:
  - `POWER_DOWN_PD1K`: Output connected to GND through 1 kΩ resistor
  - `POWER_DOWN_PD100K`: Output connected to GND through 100 kΩ resistor
  - `POWER_DOWN_HIGHZ`: Output in high-impedance state (floating)

Returns 0 on success, or a negative error code otherwise.

**Note:** To resume normal operation, call `output_voltage_set()` or `output_ratio_set()`.

## Device-Specific Classes

The library provides specific classes for each DAC variant:

- `dac5311`: 8-bit DAC (256 levels)
- `dac6311`: 10-bit DAC (1024 levels)
- `dac7311`: 12-bit DAC (4096 levels)
- `dac8311`: 14-bit DAC (16384 levels)
- `dac8411`: 16-bit DAC (65536 levels)

All classes inherit from the base `dacxx11` class and provide the same API. The resolution is automatically handled by the library.

## Specifications

- **Communication interface**: SPI (Mode 0, MSB first)
- **SPI speed**: Up to 50 MHz
- **Reference voltage**: 0V to 5.5V
- **Output**: Rail-to-rail voltage output
- **Power-down modes**: 1 kΩ to GND, 100 kΩ to GND, or high-impedance
- **Resolution**: 8, 10, 12, 14, or 16 bits (device dependent)
- **Supply voltage**: 2.7V to 5.5V

