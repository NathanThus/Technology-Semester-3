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

void SetPullDown()
{

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
    return -1;
}

void Pin::SetAsInterrupt(InterruptPinSegment segment)
{
    SYSCFG->EXTICR[PinID] = (SYSCFG->EXTICR[PinID] & ~SYSCFG_EXTICR1_EXTI0) | ((int)segment << SYSCFG_EXTI);
    SYSCFG->EXTICR[PinID] = (SYSCFG->EXTICR[PinID] & ~) | ((int)segment << SYSCFG_EXTI);

    EXTI->FTSR = 1 << PinID;
    EXTI->IMR = 1 << PinID;
    NVIC_EnableIRQ(EXTI0_IRQn);
}
