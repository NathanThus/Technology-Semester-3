#include "timer.hpp"

Timer::Timer(TIM_TypeDef *timer)
{
    this->timer = timer;
}

void Timer::ToggleClock(TimerBit bit)
{
    RCC->APB1ENR ^= (1 << bit);
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
