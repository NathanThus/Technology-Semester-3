#ifndef SERVO_HPP
#define SERVO_HPP

#include "Timer.hpp"
#include "Pin.hpp"

class Servo
{
    private:
    Timer OutputTimer = {TIM2};
    Timer InputTimer = {TIM3};

    Pin InputPin;
    Pin OutputPin;

    int currentAngle;
    int desiredAngle;

    int integral;
    int previousError;

    int kPropotional;
    int kIntegral;
    int kDerivative;

    int PositionToAngle(int position);
    public:

    Servo(Pin input, Pin Output);
    ~Servo() = default;

    /// @brief PID Function
    /// @param angle The desired angle of the servo
    void SetAngle(int angle);

    /// @brief Get the current position of the servo
    void GetPosition();

    /// @brief Set the PID values
    /// @param component The component to set
    /// @param value The value to set the component to
    void SetPIDValues(char component, int value);
};

#endif // SERVO_HPP