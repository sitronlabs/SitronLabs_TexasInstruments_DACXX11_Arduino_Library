/* Self header */
#include "dacxx11.h"

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
int dacxx11::setup(SPIClass& spi_library, const int spi_speed, const int pin_cs, const float voltage) {

    /* Ensure spi speed is within supported range */
    if (spi_speed > 50000000) {
        return -EINVAL;
    }

    /* Ensure voltage is valid */
    if (voltage > 6) {
        return -EINVAL;
    }

    /* Save parameters */
    m_spi_library = &spi_library;
    m_spi_settings = SPISettings(spi_speed, MSBFIRST, SPI_MODE0);
    m_pin_cs = pin_cs;
    m_voltage = voltage;

    /* Configure gpio */
    pinMode(m_pin_cs, OUTPUT);
    digitalWrite(m_pin_cs, HIGH);

    /* Return success */
    return 0;
}

/**
 * @brief Updates the reference voltage value used for voltage calculations
 *
 * This function updates the library's internal reference voltage value.
 * It does not change the actual hardware reference voltage.
 *
 * @param[in] voltage Reference voltage in volts (maximum 5.5V)
 * @return 0 on success, or a negative error code otherwise
 */
int dacxx11::reference_voltage_set(const float voltage) {

    /* Ensure voltage is valid */
    if (voltage > 6) {
        return -EINVAL;
    }

    /* Save parameter */
    m_voltage = voltage;

    /* Return success */
    return 0;
}

/**
 * @brief Sets the DAC output as a ratio of the reference voltage
 *
 * Sets the output voltage as a percentage of the reference voltage.
 * A ratio of 0.0 corresponds to 0V, and 1.0 corresponds to the full reference voltage.
 *
 * @param[in] ratio Output ratio from 0.0 (0% of reference) to 1.0 (100% of reference)
 * @return 0 on success, or a negative error code otherwise
 */
int dacxx11::output_ratio_set(const float ratio) {

    /* Ensure ratio is valid */
    if (ratio < 0 || ratio > 1) {
        return -EINVAL;
    }

    /* Ensure interface has been configured */
    if (m_spi_library == NULL) {
        return -ENODEV;
    }

    /* Send code to dac */
    switch (m_bits) {
        case 8:
        case 10:
        case 12:
        case 14: {
            uint16_t code = 1;
            code <<= m_bits;
            code -= 1;
            code *= ratio;
            code <<= (14 - m_bits);
            m_spi_library->beginTransaction(m_spi_settings);
            digitalWrite(m_pin_cs, LOW);
            m_spi_library->transfer(code >> 8);
            m_spi_library->transfer(code >> 0);
            digitalWrite(m_pin_cs, HIGH);
            m_spi_library->endTransaction();
            break;
        }

        case 16: {
            uint32_t code = 1;
            code <<= m_bits;
            code -= 1;
            code *= ratio;
            code <<= 6;
            m_spi_library->beginTransaction(m_spi_settings);
            digitalWrite(m_pin_cs, LOW);
            m_spi_library->transfer(code >> 16);
            m_spi_library->transfer(code >> 8);
            m_spi_library->transfer(code >> 0);
            digitalWrite(m_pin_cs, HIGH);
            m_spi_library->endTransaction();
            break;
        }

        default: {
            return -EINVAL;
        }
    }

    /* Return success */
    return 0;
}

/**
 * @brief Sets the DAC output to a specific voltage
 *
 * Calculates the required DAC code to achieve the specified output voltage
 * based on the current reference voltage setting.
 *
 * @param[in] voltage Desired output voltage in volts (must be between 0 and the reference voltage)
 * @return 0 on success, or a negative error code otherwise
 */
int dacxx11::output_voltage_set(const float voltage) {
    if (m_voltage <= 0) {
        return -EINVAL;
    } else {
        return output_ratio_set(voltage / m_voltage);
    }
}

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
int dacxx11::power_down(const enum power_down_mode mode) {

    /* Ensure interface has been configured */
    if (m_spi_library == NULL) {
        return -ENODEV;
    }

    /* Send power down to dac */
    switch (m_bits) {
        case 8:
        case 10:
        case 12:
        case 14: {
            uint16_t code = mode;
            code <<= 14;
            m_spi_library->beginTransaction(m_spi_settings);
            digitalWrite(m_pin_cs, LOW);
            m_spi_library->transfer(code >> 8);
            m_spi_library->transfer(code >> 0);
            digitalWrite(m_pin_cs, HIGH);
            m_spi_library->endTransaction();
            break;
        }

        case 16: {
            uint32_t code = mode;
            code <<= 22;
            m_spi_library->beginTransaction(m_spi_settings);
            digitalWrite(m_pin_cs, LOW);
            m_spi_library->transfer(code >> 16);
            m_spi_library->transfer(code >> 8);
            m_spi_library->transfer(code >> 0);
            digitalWrite(m_pin_cs, HIGH);
            m_spi_library->endTransaction();
            break;
        }

        default: {
            return -EINVAL;
        }
    }

    /* Return success */
    return 0;
}
