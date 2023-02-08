#if !defined(PIN_H)
#define PIN_H

#include "./stm32f303xe.h"

enum PinType
{
    PINTYPE_Input = 0,
    PINTYPE_Output = 1,
    PINTYPE_Alternate = 2,
    PINTYPE_Analog = 3
};

enum InterruptPinSegment
{
    INTERRUPTPINSEGMENT_A = 0,
    INTERRUPTPINSEGMENT_B = 1,
    INTERRUPTPINSEGMENT_C = 2,
    INTERRUPTPINSEGMENT_D = 3,
    INTERRUPTPINSEGMENT_E = 4,
    INTERRUPTPINSEGMENT_F = 5,
    INTERRUPTPINSEGMENT_G = 6,
    INTERRUPTPINSEGMENT_H = 7
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
    int DigitalRead();
    /// @brief Not implemented yet.
    /// @return -1, due to lack of implementation.
    int AnalogRead();

    void SetAsInterrupt(InterruptPinSegment segment, IRQn_Type irqn_type);
    void ResetInterrupt();

};

#endif 
