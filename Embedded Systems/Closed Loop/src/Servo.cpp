#include "Servo.hpp"

#define DEFAULT_OUTPUT_PRESCALER 7200
#define DEFAULT_PRESCALER 200
#define DEFAULT_INPUT_PRESCALER 72

#define OUTPUT_CHANNEL 3
#define INPUT_CHANNEL 1 
#define DUTY_CYCLE 1280

#define ALTERNATE_FUNCTION_1 0x2

Servo::Servo()
{
    currentAngle = 0;
    desiredAngle = 0;
    integral = 0;
    previousError = 0;

    GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODER4) | (ALTERNATE_FUNCTION_1 << GPIO_MODER_MODER4_Pos);
    GPIOB->AFR[0] = (GPIOB->AFR[0] & ~GPIO_AFRL_AFRL4) | (ALTERNATE_FUNCTION_1 << GPIO_AFRL_AFRL4_Pos);

    BasicTimerPackage timerPackage = {72, 200, TIMER3};
    PWMInputPackage PWMPackage = {timerPackage, 1, CC_ChannelType::CC_CHANNELTYPE_PWMInput};
    positionTimer.EnableAsPWMInput(PWMPackage);

    timerPackage = {7200, 200, TIMER2};
    PWMOutputPackage outputPackage = {timerPackage, 1, CC_CHANNELTYPE_PWMOutput, OCM_Type::OCM_TYPE_PWM1, 1280};
    speedTimer.EnableAsPWMOutput(outputPackage);

    // GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODER4) | (0b10 << GPIO_MODER_MODER4_Pos);
    // GPIOB->AFR[0] = (GPIOB->AFR[0] & ~GPIO_AFRL_AFRL4) | (0B0010 << GPIO_AFRL_AFRL4_Pos);

    // // Set Analog Input Pin
    // GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODER4) | (0b10 << GPIO_MODER_MODER4_Pos);
    // GPIOB->AFR[0] = (GPIOB->AFR[0] & ~GPIO_AFRL_AFRL4) | (0B0010 << GPIO_AFRL_AFRL4_Pos); // PIN D5
    // // Set Input Timer
    // BasicTimerPackage inputTimerPackage = {DEFAULT_INPUT_PRESCALER, DEFAULT_PRESCALER, TIMER3};
    // PWMInputPackage PWMInputPackage = {inputTimerPackage, INPUT_CHANNEL, CC_ChannelType::CC_CHANNELTYPE_PWMInput}; // Data is from the datasheet
    // ServoPositionTimer.EnableAsPWMInput(PWMInputPackage);

    // // Set Analog Output Pin
    // GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODER10) | (0b10 << GPIO_MODER_MODER10_Pos);
    // GPIOB->AFR[1] = (GPIOB->AFR[1] & ~GPIO_AFRH_AFRH2) | (0B0010 << GPIO_AFRH_AFRH2_Pos); // PIN D6
    // // Set Output Timer
    // BasicTimerPackage outputTimerPackage = {DEFAULT_OUTPUT_PRESCALER, DEFAULT_PRESCALER, TIMER2};
    // PWMOutputPackage PWMOutputPackage = {outputTimerPackage, OUTPUT_CHANNEL, CC_CHANNELTYPE_PWMOutput, OCM_Type::OCM_TYPE_PWM1, DUTY_CYCLE}; // Data is from the datasheet
    // ServoSpeedTimer.EnableAsPWMOutput(PWMOutputPackage);
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

void SetSpeed(int speed)
{
    TIM2->CCR3 = speed; // Working > Pretty
}

int Servo::GetPosition()
{
    return (int)TIM3->CCR2; // Working > Pretty
}

void Servo::SetAngle(int angle)
{
    int error = angle - PositionToAngle(GetPosition());
    integral += error;
    int derivative = error - previousError;

    if(error == 0 || error > desiredAngle)
    {
        integral = 0;
    }

    derivative = error - previousError;
    previousError = error;
    int power = (error * kPropotional) + (integral * kIntegral) + (derivative * kDerivative);
    SetSpeed(power);
}

int Servo::PositionToAngle(int position)
{
    return position * 360 / 1024; // Basic Clamp
}
