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
    SetLimit(package.limit);
    SetExternalClockMode();
    Enable();

}

void Timer::EnableAsPWMOutput(PWMOutputPackage package)
{
    ToggleClock(package.bit);
    SetPrescaler(package.prescaler);
    SetLimit(package.limit);

    SetCaptureChannel(package.channel, package.type);
    SetOutputCompareMode(package.ocm);
    SetCaptureCompareValue(package.dutyCycle);
    SetCaptureCompareOutput(package.channel, 0x1);

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

void Timer::SetCaptureChannel(uint16_t channel, CC_ChannelType type)
{
    timer->CCMR1 = (timer->CCMR1 & ~TIM_CCMR1_OC1M_Msk) | (type << channel * 4);
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
