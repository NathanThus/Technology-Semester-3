#include "timer.hpp"

Timer::Timer(TIM_TypeDef *timer)
{
    this->timer = timer;
}

void Timer::EnableAsTimerBase(TimerBasePackage package)
{
    ToggleClock(package.bit);
    SetPrescaler(package.prescaler);
    SetLimit(package.limit);
    EnableInterrupt(package.irq);
    Enable();
}

void Timer::EnableAsCountPulse(BasicTimerPackage package)
{
    ToggleClock(package.bit);
    SetPrescaler(package.prescaler);
    SetExternalClockMode();
    Enable();
}

void Timer::EnableAsPWMOutput(PWMOutputPackage package)
{
    ToggleClock(package.bit);
    SetPrescaler(package.prescaler);
    SetLimit(package.limit);

    SetOutputCaptureChannel(package.channel, package.type);
    SetOutputCompareMode(package.ocm);
    SetCaptureCompareValue(package.dutyCycle);
    SetCaptureCompareOutput(package.channel, 0x1);

    Enable();
}

void Timer::EnableAsPWMInput(BasicTimerPackage package)
{
    ToggleClock(package.bit);
    SetPrescaler(package.prescaler);

    // Configure CC1
    timer->CCMR1 |= 0x1 << TIM_CCMR1_CC1S_Pos;

    // Set Capture Polarity, Rising Edge
    timer->CCER |= 0 << TIM_CCER_CC1NP_Pos;
    timer->CCER |= 0 << TIM_CCER_CC1P_Pos; 

    // Enable Capture
    timer->CCER |= 0x1 << TIM_CCER_CC1E_Pos;

    // Configure CC2
    timer->CCMR1 |= 0x2 << TIM_CCMR1_CC2S_Pos;

    // Set Capture Polarity, Falling Edge
    timer->CCER |= 0x0 << TIM_CCER_CC2NP_Pos;
    timer->CCER |= 0x1 << TIM_CCER_CC2P_Pos;

    // Enable Capture
    timer->CCER |= 0x1 << TIM_CCER_CC2E_Pos;

    // Set Trigger Selection to Filtered Timer Input 1
    timer->SMCR |= 0x5 << TIM_SMCR_TS_Pos;

    // Set Slave Mode to Reset
    timer->SMCR |= 0x4 << TIM_SMCR_SMS_Pos;

    Enable();
}

int Timer::GetCounter()
{
    return timer->CNT;
}

void Timer::ToggleClock(TimerBit bit)
{
    RCC->APB1ENR |= (1 << bit);
}

void Timer::DisableClock(TimerBit bit)
{
    RCC->APB1ENR &= ~(1 << bit);
}

void Timer::SetPrescaler(uint16_t prescaler)
{
    timer->PSC = prescaler;
    timer->EGR |= TIM_EGR_UG;
    ResetInterrupt();
}

void Timer::SetLimit(uint16_t limit)
{
    timer->ARR = limit;
    timer->EGR |= TIM_EGR_UG;
    ResetInterrupt();
}

void Timer::SetExternalClockMode()
{
    timer->SMCR |= TIM_SMCR_ECE;
}

void Timer::SetOutputCaptureChannel(uint16_t channel, CC_ChannelType type)
{
    timer->CCMR1 = (timer->CCMR1 & ~TIM_CCMR1_OC1M_Msk) | (type << channel * 4);
}

void Timer::SetInputCaptureChannel(uint16_t channel, CC_ChannelType type)
{
    timer->CCMR1 = (timer->CCMR1 & ~TIM_CCMR1_CC1S_Msk) | (type << channel * 4);
}

void Timer::SetOutputCompareMode(OCM_Type type)
{
    timer->CCMR1 |= (type << 4);
}

void Timer::SetCaptureCompareValue(uint16_t value)
{
    timer->CCR1 = value;
}

void Timer::SetCaptureCompareOutput(uint16_t channel, uint16_t value)
{
    TIM2->CCER = (TIM2->CCER & ~channel) | (value << (channel - 1));
}

void Timer::EnableInterrupt(IRQn_Type irq)
{
    NVIC_EnableIRQ(irq);
    timer->DIER |= TIM_DIER_UIE;
}

void Timer::ResetInterrupt()
{
    timer->SR &= ~TIM_SR_UIF;
}

void Timer::Enable()
{
    timer->CR1 |= TIM_CR1_CEN;
}
