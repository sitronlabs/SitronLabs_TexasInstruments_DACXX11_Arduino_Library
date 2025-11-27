#ifndef DAC7311_H
#define DAC7311_H

/* Library headers */
#include "dacxx11.h"

/**
 * @brief 12-bit digital-to-analog converter (DAC7311)
 *
 * This class provides an interface to the Texas Instruments DAC7311, a buffered,
 * rail-to-rail voltage output DAC with 12-bit resolution (4096 levels).
 *
 * @see dacxx11 for the base class API documentation
 */
class dac7311 : public dacxx11 {
   public:
    /**
     * @brief Constructs a DAC7311 object
     *
     * Initializes the DAC with 12-bit resolution.
     */
    dac7311() : dacxx11(12) {}
};

#endif
