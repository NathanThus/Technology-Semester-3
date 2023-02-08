#include "Pin.hpp"

Pin::Pin(GPIO_TypeDef *gpio, int pin)
{
    GPIO = gpio;
    PinID = pin;
}

void Pin::SetType(PinType type)
{
    GPIO->MODER = (GPIO->MODER & ~(PinID << PinID * 2)) | ((int)type << PinID * 2);

    switch (type)
    {
        case Input:
            SetPullDown();
            break;
        
        default:
            break;
    }
}

void Pin::SetPullDown()
{
    const int8_t PullDownBits = 0x1;
    GPIO->PUPDR = (GPIO->PUPDR & ~ PinID << PinID * 2) | (PullDownBits << PinID * 2);  
}

void Pin::Toggle()
{
    GPIO->ODR ^= 1 << PinID;
}

void Pin::On()
{
    GPIO->ODR |= 1 << PinID;
}

void Pin::Off()
{
    GPIO->ODR &= ~1 << PinID;
}

bool Pin::DigitalRead()
{
    return GPIO->IDR & (1 << PinID);
}

int Pin::AnalogRead()
{
    return -1; // To be added later. This will require some timing shenanigans.
}

void Pin::SetAsInterrupt(InterruptPinSegment segment, IRQn_Type irqn_type)
{
    const int CrRegister = PinID / 4;
    const int CrPos = (PinID % 4);
    SYSCFG->EXTICR[CrRegister] = (SYSCFG->EXTICR[CrRegister] & ~ (0xF << CrPos)) | ((int)segment << CrPos * 4);

    EXTI->FTSR = 1 << PinID;
    EXTI->IMR = 1 << PinID;
    NVIC_EnableIRQ(irqn_type);
}

void Pin::ResetInterrupt()
{
    EXTI->PR = 1 << PinID; // Verify!
}
