/* Self header */
#include "dacxx11.h"

/**
 *
 * @param[in] spi_library
 * @return 0 in case of success, or a negative error code otherwise.
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
 *
 * @param[in] voltage
 * @return 0 in case of success, or a negative error code otherwise.
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
 *
 * @param[in] ratio
 * @return 0 in case of success, or a negative error code otherwise.
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
 *
 * @param[in] voltage
 * @return 0 in case of success, or a negative error code otherwise.
 */
int dacxx11::output_voltage_set(const float voltage) {
    if (m_voltage <= 0) {
        return -EINVAL;
    } else {
        return output_ratio_set(voltage / m_voltage);
    }
}

/**
 *
 * @return 0 in case of success, or a negative error code otherwise.
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
