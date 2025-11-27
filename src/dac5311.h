#ifndef DAC5311_H
#define DAC5311_H

/* Library headers */
#include "dacxx11.h"

/**
 * @brief 8-bit digital-to-analog converter (DAC5311)
 *
 * This class provides an interface to the Texas Instruments DAC5311, a buffered,
 * rail-to-rail voltage output DAC with 8-bit resolution (256 levels).
 *
 * @see dacxx11 for the base class API documentation
 */
class dac5311 : public dacxx11 {
   public:
    /**
     * @brief Constructs a DAC5311 object
     *
     * Initializes the DAC with 8-bit resolution.
     */
    dac5311() : dacxx11(8) {}
};

#endif
