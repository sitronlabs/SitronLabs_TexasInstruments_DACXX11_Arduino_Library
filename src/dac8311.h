#ifndef DAC8311_H
#define DAC8311_H

/* Library headers */
#include "dacxx11.h"

/**
 * @brief 14-bit digital-to-analog converter (DAC8311)
 *
 * This class provides an interface to the Texas Instruments DAC8311, a buffered,
 * rail-to-rail voltage output DAC with 14-bit resolution (16384 levels).
 *
 * @see dacxx11 for the base class API documentation
 */
class dac8311 : public dacxx11 {
   public:
    /**
     * @brief Constructs a DAC8311 object
     *
     * Initializes the DAC with 14-bit resolution.
     */
    dac8311() : dacxx11(14) {}
};

#endif
