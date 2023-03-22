#ifndef SIMPLEWATCHDOG_H
#define SIMPLEWATCHDOG_H

#include "stm32f4xx_hal.h"

class SimpleWatchDog
{
    public:
        SimpleWatchDog(IWDG_TypeDef *iwdg);
        virtual ~SimpleWatchDog();
        void Feed();
        void SetTimeout(unsigned long timeout);
        void Start();

    private:
        IWDG_TypeDef *WatchDog;
        unsigned long timeout;
};

#endif // SIMPLEWATCHDOG_H