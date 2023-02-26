#ifndef DAC7311_H
#define DAC7311_H

/* Library headers */
#include "dacxx11.h"

/**
 *
 */
class dac7311 : public dacxx11 {
   public:
    dac7311() : dacxx11(12) {}
};

#endif
