#ifndef TIMER_HPP
#define TIMER_HPP

#include "stm32f303xe.h"

#define PSC_SECOND (7200 - 1)
#define ARR_SECOND 10000

enum OCM_Type
{
    OCM_TYPE_Frozen = 0,
    OCM_TYPE_ActiveOnMatch = 1,
    OCM_TYPE_InactiveOnMatch = 2,
    OCM_TYPE_ToggleOnMatch = 3,
    OCM_TYPE_ForceInactive = 4,
    OCM_TYPE_ForceActive = 5,
    OCM_TYPE_PWM1 = 6,
    OCM_TYPE_PWM2 = 7,
};

enum CC_ChannelType
{
    CC_CHANNELTYPE_Input = 0,
    CC_CHANNELTYPE_Output = 1,
    CC_CHANNELTYPE_PWMOutput = 2,
    CC_CHANNELTYPE_PWMInput = 3,
};

/// @brief The timer bit enum, used to toggle the clock for the timer.
enum TimerBit
{
    TIMER2 = 0,
    TIMER3 = 1,
    TIMER4 = 2,
};

/// @brief The timer package struct, used to pass the timer settings to the timer class.
struct BasicTimerPackage
{
    uint16_t prescaler;
    uint16_t limit;
    TimerBit bit;

    BasicTimerPackage(uint16_t prescaler, uint16_t limit, TimerBit bit)
    {
        this->prescaler = prescaler;
        this->limit = limit;
        this->bit = bit;
    }
};

/// @brief The timer base package struct, used to pass the timer base settings to the timer class.
struct TimerBasePackage : public BasicTimerPackage
{
    IRQn_Type irq;

    TimerBasePackage(uint16_t prescaler, uint16_t limit, TimerBit bit, IRQn_Type irq) : BasicTimerPackage(prescaler, limit, bit)
    {
        this->irq = irq;
    }
};

struct PWMOutputPackage : public BasicTimerPackage
{
    uint16_t channel;
    CC_ChannelType type;
    OCM_Type ocm;
    uint16_t dutyCycle;

    PWMOutputPackage(uint16_t prescaler, uint16_t limit, TimerBit bit, uint16_t channel, CC_ChannelType type, OCM_Type ocm, uint16_t dutyCycle) : BasicTimerPackage(prescaler, limit, bit)
    {
        this->channel = channel;
        this->type = type;
        this->ocm = ocm;
        this->dutyCycle = dutyCycle;
    }

    PWMOutputPackage(BasicTimerPackage package, uint16_t channel, CC_ChannelType type, OCM_Type ocm, uint16_t dutyCycle) : BasicTimerPackage(package.prescaler, package.limit, package.bit)
    {
        this->channel = channel;
        this->type = type;
        this->ocm = ocm;
        this->dutyCycle = dutyCycle;
    }
};

class Timer
{
    public:
    Timer(TIM_TypeDef* timer);

    /// @brief Enables the timer as a timer base. 
    /// @param package The package containing the timer base settings.
    void EnableAsTimerBase(TimerBasePackage package);

    /// @brief Enables the timer as a count pulse.
    /// @param package The package containing the count pulse settings.
    void EnableAsCountPulse(BasicTimerPackage package);

    /// @brief Enables the timer as a PWM output.
    /// @param package The package containing the PWM output settings.
    void EnableAsPWMOutput(PWMOutputPackage package);

    /// @brief Enables the timer as a PWM input.
    /// @param package The package containing the PWM input settings.
    void EnableAsPWMInput(BasicTimerPackage package);

    /// @brief Gets the counter value of the timer.
    /// @return The counter value.
    int GetCounter();

    /// @brief Gets the PWM input of the timer. 
    /// @return The PWM input.
    int GetPWMInput();

    /// @brief Resets the interrupt for the timer.
    void ResetInterrupt();

    /// @brief Sends a PWM signal.
    /// @param signal The signal to send.
    void SendPWMSignal(int signal);

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

    /// @brief Sets the timer to external clock mode.
    void SetExternalClockMode();

    /// @brief Sets the timer to internal clock mode.
    /// @param channel The channel to set.
    /// @param type The type of the channel.
    void SetOutputCaptureChannel(uint16_t channel, OCM_Type type);

    /// @brief Sets the timer to internal clock mode.
    /// @param channel The channel to set.
    /// @param type The type of the channel.
    void SetInputCaptureChannel(uint16_t channel, CC_ChannelType type);

    /// @brief Sets the output compare mode for the timer.
    void SetOutputCompareMode();

    /// @brief Sets the capture compare value for the timer.
    /// @param value The value to set.
    void SetCaptureCompareValue(uint16_t value);

    /// @brief Sets the capture compare output for the timer.
    /// @param channel The channel to set.
    /// @param value The value to set.
    void SetCaptureCompareOutput(uint16_t channel, uint16_t value);

    /// @brief Enables the interrupt for the timer.
    /// @param irq The interrupt to enable.
    void EnableInterrupt(IRQn_Type irq);

    /// @brief Enables the timer.
    void Enable();
};

#endif
