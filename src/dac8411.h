#ifndef DAC8411_H
#define DAC8411_H

/* Library headers */
#include "dacxx11.h"

/**
 *
 */
class dac8411 : public dacxx11 {
   public:
    dac8411() : dacxx11(16) {}
};

#endif
