#ifndef DAC8411_H
#define DAC8411_H

/* Library headers */
#include "dacxx11.h"

/**
 * @brief 16-bit digital-to-analog converter (DAC8411)
 *
 * This class provides an interface to the Texas Instruments DAC8411, a buffered,
 * rail-to-rail voltage output DAC with 16-bit resolution (65536 levels).
 *
 * @see dacxx11 for the base class API documentation
 */
class dac8411 : public dacxx11 {
   public:
    /**
     * @brief Constructs a DAC8411 object
     *
     * Initializes the DAC with 16-bit resolution.
     */
    dac8411() : dacxx11(16) {}
};

#endif
