#ifndef SIMPLEWATCHDOG_H
#define SIMPLEWATCHDOG_H

#include "stm32f3xx.h"

/// @brief Prescaler value, which when combined with RLR_SECOND gives a 1 second timeout
#define SWD_PSC_SECOND 6
/// @brief RLR value for 1 second. Only works with PSC_SECOND
#define RLR_SECOND 2500

/// @brief The prescaler values for the watchdog timer
enum WatchDogPrescaler
{
    PSC_4 = 0x0,
    PSC_8 = 0x1,
    PSC_16 = 0x2,
    PSC_32 = 0x3,
    PSC_64 = 0x4,
    PSC_128 = 0x5,
    PSC_256 = 0x6,
};

/// @brief Class used to implement a simplified watchdog timer
class SimpleWatchDog
{
    public:
        SimpleWatchDog(IWDG_TypeDef *iwdg);
        /// @brief Resets the watchdog timer
        void Feed();
        /// @brief Sets the timeout value
        /// @param timeout The timeout value in Seconds
        void SetTimeout(unsigned long timeout);
        /// @brief Sets the prescaler value
        void SetPrescaler(WatchDogPrescaler prescaler);
        /// @brief Starts the watchdog timer
        void Start();
        /// @brief Gets the time out value
        void GetTimeout(unsigned long &timeout);

    private:
        IWDG_TypeDef *WatchDog;
        unsigned long timeout;
};


#endif // SIMPLEWATCHDOG_H