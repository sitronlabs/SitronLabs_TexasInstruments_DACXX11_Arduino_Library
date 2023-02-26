#ifndef DAC8311_H
#define DAC8311_H

/* Library headers */
#include "dacxx11.h"

/**
 *
 */
class dac8311 : public dacxx11 {
   public:
    dac8311() : dacxx11(14) {}
};

#endif
