#ifndef TIMER_HPP
#define TIMER_HPP

#include "stm32f303xe.h"

#define PSC_SECOND (7200 - 1)
#define ARR_SECOND 10000

enum TimerBit
{
    TIMER2 = 0,
    TIMER3 = 1,
    TIMER4 = 2,
};

class Timer
{
    public:
    Timer(TIM_TypeDef* timer);
    void ToggleClock(TimerBit bit);
    void DisableClock(TimerBit bit);
    void SetPrescaler(uint16_t prescaler);
    void SetLimit(uint16_t limit);
    void EnableInterrupt(IRQn_Type irq);
    void ResetInterrupt();
    void Enable();

    private:
    TIM_TypeDef* timer;
};

#endif