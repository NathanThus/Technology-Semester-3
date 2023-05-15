#include "BreadBaker.h"
#include "BrokenLog.h"
#include "MDisplay.h"
#include "MEventGenerator.h"
#include "MExtraIngredientsTray.h"
#include "MKneadMotor.h"
#include "MOven.h"
#include "MStartButtonLed.h"
#include "MTimer.h"
#include "MYeastTray.h"
#include "TimeConstants.h"

using ::testing::Return;
using ::testing::_;

class StateTest: public ::testing::Test
{
protected:
    StateTest()
    {
        baker = new BreadBaker(
                    oven, timer, motor, yeast, extras,
                    display, startButton, event, log);
    }

    virtual ~StateTest()
    {
        delete baker;
        baker = nullptr;
    }

    MDisplay display;
    MEventGenerator event;
    MExtraIngredientsTray extras;
    MKneadMotor motor;
    MOven oven;
    MStartButtonLed startButton;
    MTimer timer;
    MYeastTray yeast;

    BrokenLog log;
    BreadBaker* baker;
};

TEST_F(StateTest, test_BrokenEmptyTest)
{
    EXPECT_CALL(display, SetCurrentTask(_)).Times(0); // please note: _ is a wildcard, it means: "any value for that parameter"
    EXPECT_CALL(display, SetMenu(_)).Times(0);
    EXPECT_CALL(display, SetTime(_, _)).Times(0);
    EXPECT_CALL(display, DisplayOff()).Times(0);

    EXPECT_CALL(event, GetEvent()).Times(0);

    EXPECT_CALL(extras, Drop(_)).Times(0);
    EXPECT_CALL(extras, Cancel()).Times(0);

    EXPECT_CALL(motor, TurnLeft()).Times(0);
    EXPECT_CALL(motor, TurnRight()).Times(0);
    EXPECT_CALL(motor, Stop()).Times(0);

    EXPECT_CALL(oven, StartRise(_)).Times(0);
    EXPECT_CALL(oven, StartBake(_)).Times(0);
    EXPECT_CALL(oven, IsOn()).Times(0);
    EXPECT_CALL(oven, GetTemperature()).Times(0);
    EXPECT_CALL(oven, Cancel()).Times(0);

    EXPECT_CALL(startButton, LedOn()).Times(0);
    EXPECT_CALL(startButton, LedOff()).Times(0);

    EXPECT_CALL(timer, Set(_)).Times(0);
    EXPECT_CALL(timer, Cancel()).Times(0);

    EXPECT_CALL(yeast, Drop(_)).Times(0);
    EXPECT_CALL(yeast, Cancel()).Times(0);

    EXPECT_EQ(1, 1); // a Google test project must have at least one EXPECT_... or ASSERT_..., else it won't compile
}

TEST_F(StateTest, test_Program_PlainBread)
{
    // Program: Plain Bread
    const int numberOfMotorTurns = 20;

    EXPECT_CALL(display, SetCurrentTask(_)).Times(6);
    EXPECT_CALL(display, SetMenu(_)).Times(1);
    EXPECT_CALL(display, SetTime(_, _)).Times(1);
    EXPECT_CALL(display, DisplayOff()).Times(0);

    EXPECT_CALL(extras, Drop(_)).Times(0);
    EXPECT_CALL(extras, Cancel()).Times(0);
    EXPECT_CALL(yeast, Drop(_)).Times(1);
    EXPECT_CALL(yeast, Cancel()).Times(0);

    EXPECT_CALL(motor, TurnLeft()).Times(10);
    EXPECT_CALL(motor, TurnRight()).Times(10);
    EXPECT_CALL(motor, Stop()).Times(20);

    EXPECT_CALL(oven, StartRise(_)).Times(1);
    EXPECT_CALL(oven, StartBake(_)).Times(1);
    EXPECT_CALL(oven, IsOn()).Times(0);
    EXPECT_CALL(oven, GetTemperature()).Times(1);
    EXPECT_CALL(oven, Cancel()).Times(0); // Is used to switch off manually.

    EXPECT_CALL(startButton, LedOn()).Times(1);
    EXPECT_CALL(startButton, LedOff()).Times(1);

    EXPECT_CALL(timer, Set(_)).Times(2 + 20); 
    EXPECT_CALL(timer, Cancel()).Times(0);

    baker->HandleEvent(Events::MENU_BUTTON_PRESSED); // Select Program
    baker->HandleEvent(Events::START_BUTTON_PRESSED); // Start Program
    baker->HandleEvent(Events::TIMER_TIMEOUT); // Waiting Delay
    baker->HandleEvent(Events::TIMER_TIMEOUT); // Resting Delay

    // Add 1 for the yeast drop
    for (int i = 0; i < numberOfMotorTurns + 1; ++i) // Kneading Delay
    {
        baker->HandleEvent(Events::TIMER_TIMEOUT);
    }

    baker->HandleEvent(Events::OVEN_DONE); // Rising
    baker->HandleEvent(Events::OVEN_DONE); // Baking
    baker->HandleEvent(Events::TIMER_TIMEOUT); // Done
    baker->HandleEvent(Events::TIMER_TIMEOUT); // Done
    
    EXPECT_EQ(1, 1); // a Google test project must have at least one EXPECT_... or ASSERT_..., else it won't compile
}