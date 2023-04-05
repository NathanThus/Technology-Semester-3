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

struct BasicTimerPackage
{
    uint16_t prescaler;
    uint16_t limit;
    IRQn_Type irq;
    TimerBit bit;
};

struct PWMTimerPackage : public BasicTimerPackage
{
    uint16_t pulse;
};

class Timer
{
    public:
    Timer(TIM_TypeDef* timer);

    /// @brief Enables the timer as a timer base. 
    /// @param package The package containing the timer base settings.
    void EnableAsTimerBase(BasicTimerPackage package);
    /// @brief Resets the interrupt for the timer.
    void ResetInterrupt();

    private:
    TIM_TypeDef* timer;

    /// @brief Toggles the clock for the timer.
    /// @param bit The bit to toggle.
    void ToggleClock(TimerBit bit);
    /// @brief Disables the clock for the timer.
    /// @param bit The bit to disable.
    void DisableClock(TimerBit bit);
    /// @brief Sets the prescaler for the timer.
    /// @param prescaler The prescaler value.
    void SetPrescaler(uint16_t prescaler);
    /// @brief Sets the limit for the timer.
    /// @param limit The limit value.
    void SetLimit(uint16_t limit);
    /// @brief Enables the interrupt for the timer.
    /// @param irq The interrupt to enable.
    void EnableInterrupt(IRQn_Type irq);

    /// @brief Enables the timer.
    void Enable();
};

#endif