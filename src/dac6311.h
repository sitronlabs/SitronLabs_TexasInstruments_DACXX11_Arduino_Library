#ifndef DAC6311_H
#define DAC6311_H

/* Library headers */
#include "dacxx11.h"

/**
 * @brief 10-bit digital-to-analog converter (DAC6311)
 *
 * This class provides an interface to the Texas Instruments DAC6311, a buffered,
 * rail-to-rail voltage output DAC with 10-bit resolution (1024 levels).
 *
 * @see dacxx11 for the base class API documentation
 */
class dac6311 : public dacxx11 {
   public:
    /**
     * @brief Constructs a DAC6311 object
     *
     * Initializes the DAC with 10-bit resolution.
     */
    dac6311() : dacxx11(10) {}
};

#endif
