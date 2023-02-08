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

enum InternalResistor
{
    INTERNALRESISTOR_None = 0,
    INTERNALRESISTOR_PullUp = 1,
    INTERNALRESISTOR_PullDown = 2
};

/// @brief An overall class to handle most GPIO tasks
class Pin
{
private:
    GPIO_TypeDef* GPIO;
    int PinID;

    void SetInternalResistor();
public:
    /// @brief Constructor for the Pin class.
    /// @param gpio The GPIO pointer for the register.
    /// @param pin The pin number.
    Pin(GPIO_TypeDef* gpio, int pin);

    /// @brief Sets the type, IE Input, Output etc.
    /// @param type The type to be assigned.
    void SetType(PinType type);

    /// @brief Sets the type, IE Input, Output etc.
    /// @param type The type to be assigned.
    /// @param internalResistor The setting for the internal resistor.
    void SetType(PinType type, InternalResistor internalResistor);

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

    /// @brief Sets the pin to act as an interrupt pin.
    /// @param segment The segement the pin belongs to (A,B,C etc.)
    /// @param irqn_type The IRQn Handler this will need to attach to.
    void SetAsInterrupt(InterruptPinSegment segment, IRQn_Type irqn_type);

    /// @brief Resets the interrupt pin.
    void ResetInterrupt();

};

#endif 
