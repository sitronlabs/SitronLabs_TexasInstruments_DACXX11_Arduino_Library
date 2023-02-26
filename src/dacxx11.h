#ifndef DACXX11_H
#define DACXX11_H

/* Arduino libraries */
#include <Arduino.h>
#include <SPI.h>

/* C/C++ libraries */
#include <errno.h>
#include <stdint.h>

/**
 *
 */
class dacxx11 {
   public:
    /* Setup */
    dacxx11(const uint8_t bits) : m_bits(bits) {}
    int setup(SPIClass& spi_library, const int spi_speed, const int pin_cs, const float voltage);
    int reference_voltage_set(const float voltage);

    /* Output */
    int output_ratio_set(const float ratio);
    int output_voltage_set(const float voltage);

    /* Power down */
    enum power_down_mode {
        POWER_DOWN_PD1K = 1,    // Output 1 kΩ to GND
        POWER_DOWN_PD100K = 2,  // Output 100 kΩ to GND
        POWER_DOWN_HIGHZ = 3,   // High-Z
    };
    int power_down(const enum power_down_mode mode);

   protected:
    uint8_t m_bits;
    float m_voltage = 0;
    SPIClass* m_spi_library = NULL;
    SPISettings m_spi_settings;
    int m_pin_cs = 0;
};

#endif
