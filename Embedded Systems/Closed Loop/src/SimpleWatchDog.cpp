#include "SimpleWatchDog.h"

#define RESET 0xAAAA
#define START 0xCCCC
#define SETUP 0x5555

SimpleWatchDog::SimpleWatchDog(IWDG_TypeDef *iwdg)
{
    WatchDog = iwdg;
    WatchDog->KR = SETUP;
}

void SimpleWatchDog::Feed()
{
    WatchDog->KR = RESET;
}

void SimpleWatchDog::SetTimeout(unsigned long timeout)
{
    this->timeout = timeout;
    WatchDog->RLR = timeout * IWDG_RLR_SECOND;
}

void SimpleWatchDog::SetPrescaler(WatchDogPrescaler prescaler)
{
    WatchDog->PR = prescaler;
}

void SimpleWatchDog::Start()
{
    WatchDog->KR = START;
}

void SimpleWatchDog::GetTimeout(unsigned long &timeout)
{
    timeout = this->timeout;
}
