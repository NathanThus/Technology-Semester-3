#if !defined(PIN_H)
#define PIN_H

#include "./stm32f303xe.h"

enum PinType
{
    Input = 0,
    Output = 1,
    Alternate = 2,
    Analog = 3
};

enum InterruptPinSegment
{
    A = 0,
    B = 1,
    C = 2,
    D = 3,
    E = 4,
    F = 5,
    G = 6,
    H = 7
};

class Pin
{
private:
    GPIO_TypeDef* GPIO;
    int PinID;

    void SetPullDown();
public:
    Pin(GPIO_TypeDef* gpio, int pin);
    /// @brief Sets the type, IE Input, Output etc.
    /// @param type The type to be assigned.
    void SetType(PinType type);
    /// @brief Toggles the pin on/off.
    void Toggle();
    /// @brief Turns the pin on.
    void On();
    /// @brief Turns the pin off.
    void Off();
    /// @brief Reads a singular bit.
    /// @return Returns true or false.
    bool DigitalRead();
    /// @brief Not implemented yet.
    /// @return -1, due to lack of implementation.
    int AnalogRead();

    void SetAsInterrupt(InterruptPinSegment segment, IRQn_Type irqn_type);
    void ResetInterrupt();

};

/// @brief Enumeration for easy Pin definition.


#endif 
