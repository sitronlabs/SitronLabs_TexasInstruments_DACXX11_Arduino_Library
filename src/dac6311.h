#ifndef DAC6311_H
#define DAC6311_H

/* Library headers */
#include "dacxx11.h"

/**
 *
 */
class dac6311 : public dacxx11 {
   public:
    dac6311() : dacxx11(10) {}
};

#endif
