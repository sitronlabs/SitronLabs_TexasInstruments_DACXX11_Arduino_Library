#ifndef DAC5311_H
#define DAC5311_H

/* Library headers */
#include "dacxx11.h"

/**
 *
 */
class dac5311 : public dacxx11 {
   public:
    dac5311() : dacxx11(8) {}
};

#endif
