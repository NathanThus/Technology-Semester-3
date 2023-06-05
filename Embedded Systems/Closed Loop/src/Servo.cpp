#include "Servo.hpp"

#define DEFAULT_OUTPUT_PRESCALER 7200
#define DEFAULT_PRESCALER 200
#define DEFAULT_INPUT_PRESCALER 72

#define DEFAULT_CHANNEL 1
#define DUTY_CYCLE 1280

Servo::Servo(Pin input, Pin Output) : InputPin(input), OutputPin(Output)
{
    currentAngle = 0;
    desiredAngle = 0;
    integral = 0;
    previousError = 0;


    // Set Analog Output Pin

    // Set Analog Input Pin

    // Set Output Timer
    BasicTimerPackage outputTimerPackage = {DEFAULT_OUTPUT_PRESCALER, DEFAULT_PRESCALER, TIMER2};
    PWMOutputPackage PWMOutputPackage = {outputTimerPackage, DEFAULT_CHANNEL, CC_CHANNELTYPE_PWMOutput, OCM_Type::OCM_TYPE_PWM1, DUTY_CYCLE}; // Data is from the datasheet
    OutputTimer.EnableAsPWMOutput(PWMOutputPackage);
    // Set Input Timer
    BasicTimerPackage inputTimerPackage = {DEFAULT_INPUT_PRESCALER, DEFAULT_PRESCALER, TIMER3};
    PWMInputPackage PWMInputPackage = {inputTimerPackage, DEFAULT_CHANNEL, CC_ChannelType::CC_CHANNELTYPE_PWMInput}; // Data is from the datasheet
    InputTimer.EnableAsPWMInput(PWMInputPackage);
}

void Servo::SetPIDValues(char component, int value)
{
    switch(component)
    {
        case 'p':
            kPropotional = value;
            break;
        case 'i':
            kIntegral = value;
            break;
        case 'd':
            kDerivative = value;
            break;
        default:
            break;
    }
}

void Servo::SetAngle(int angle)
{
    int error = angle - PositionToAngle(InputTimer.GetCounter());
    integral += error;
    int derivative = error - previousError;
    if(error == 0 || error > desiredAngle)
    {
        integral = 0;
    }

    derivative = error - previousError;
    previousError = error;
    int power = (error * kPropotional) + (integral * kIntegral) + (derivative * kDerivative);
    OutputTimer.SetOutput(power);
}

int Servo::PositionToAngle(int position)
{
    return position * 360 / 1024;
}
