#ifndef DACXX11_H
#define DACXX11_H

/* Arduino libraries */
#include <Arduino.h>
#include <SPI.h>

/* C/C++ libraries */
#include <errno.h>
#include <stdint.h>

/**
 * @brief Base class for Texas Instruments DACxx11 family of digital-to-analog converters
 *
 * This class provides a unified interface to control DACxx11 devices with different
 * resolutions (8, 10, 12, 14, or 16 bits). The class handles SPI communication and
 * automatically manages resolution-specific differences in command formatting.
 *
 * Supported devices:
 * - DAC5311 (8-bit)
 * - DAC6311 (10-bit)
 * - DAC7311 (12-bit)
 * - DAC8311 (14-bit)
 * - DAC8411 (16-bit)
 */
class dacxx11 {
   public:
    /**
     * @brief Constructs a DACxx11 object with the specified resolution
     *
     * @param[in] bits Resolution in bits (8, 10, 12, 14, or 16)
     */
    dacxx11(const uint8_t bits) : m_bits(bits) {}

    //!@{
    //! Initialization and setup
    /**
     * @brief Initializes the DAC device with SPI communication
     *
     * Configures the SPI interface and chip select pin. The reference voltage
     * parameter is used for voltage calculations but does not set the actual
     * hardware reference voltage.
     *
     * @note Call this from the Arduino setup function.
     * @note Make sure the SPI library has been initialized (with a call to SPI.begin() for example).
     * @param[in] spi_library Reference to the SPI library instance to use
     * @param[in] spi_speed SPI clock speed in Hz (maximum 50 MHz)
     * @param[in] pin_cs GPIO pin number connected to the chip select (CS) pin
     * @param[in] voltage Reference voltage in volts (maximum 5.5V)
     * @return 0 on success, or a negative error code otherwise
     */
    int setup(SPIClass& spi_library, const int spi_speed, const int pin_cs, const float voltage);

    /**
     * @brief Updates the reference voltage value used for voltage calculations
     *
     * This function updates the library's internal reference voltage value.
     * It does not change the actual hardware reference voltage.
     *
     * @param[in] voltage Reference voltage in volts (maximum 5.5V)
     * @return 0 on success, or a negative error code otherwise
     */
    int reference_voltage_set(const float voltage);
    //!@}

    //!@{
    //! Output control
    /**
     * @brief Sets the DAC output as a ratio of the reference voltage
     *
     * Sets the output voltage as a percentage of the reference voltage.
     * A ratio of 0.0 corresponds to 0V, and 1.0 corresponds to the full reference voltage.
     *
     * @param[in] ratio Output ratio from 0.0 (0% of reference) to 1.0 (100% of reference)
     * @return 0 on success, or a negative error code otherwise
     */
    int output_ratio_set(const float ratio);

    /**
     * @brief Sets the DAC output to a specific voltage
     *
     * Calculates the required DAC code to achieve the specified output voltage
     * based on the current reference voltage setting.
     *
     * @param[in] voltage Desired output voltage in volts (must be between 0 and the reference voltage)
     * @return 0 on success, or a negative error code otherwise
     */
    int output_voltage_set(const float voltage);
    //!@}

    //!@{
    //! Power management
    /**
     * @brief Power-down mode options
     *
     * Available power-down modes to reduce power consumption:
     * - PD1K: Output connected to GND through 1 kΩ resistor
     * - PD100K: Output connected to GND through 100 kΩ resistor
     * - HIGHZ: Output in high-impedance state (floating)
     */
    enum power_down_mode {
        POWER_DOWN_PD1K = 1,    //!< Output 1 kΩ to GND
        POWER_DOWN_PD100K = 2,  //!< Output 100 kΩ to GND
        POWER_DOWN_HIGHZ = 3,   //!< High-impedance (floating)
    };

    /**
     * @brief Puts the DAC into a power-down mode
     *
     * Reduces power consumption by putting the DAC into a low-power state.
     * The output behavior depends on the selected power-down mode.
     *
     * @note To resume normal operation, call output_voltage_set() or output_ratio_set().
     * @param[in] mode Power-down mode to use
     * @return 0 on success, or a negative error code otherwise
     */
    int power_down(const enum power_down_mode mode);
    //!@}

   protected:
    uint8_t m_bits;                  //!< DAC resolution in bits
    float m_voltage = 0;             //!< Reference voltage in volts
    SPIClass* m_spi_library = NULL;  //!< Pointer to SPI library instance
    SPISettings m_spi_settings;      //!< SPI communication settings
    int m_pin_cs = 0;                //!< Chip select pin number
};

#endif
