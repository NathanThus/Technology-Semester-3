#include "BreadBaker.h"
#include "Program.h"
#include "TimeConstants.h"

States currentState;
Program selectedProgram;
int programID = 0;

int bakeTime = 0;

int MotorSwitches = 0;

BreadBaker::BreadBaker(
            IOven& oven, ITimer& timer, IKneadMotor& motor, IYeastTray& yeast,
            IExtraIngredientsTray& extras, IDisplay& display,
            IStartButtonLed& startButton, IEventGenerator& eventGenerator,
            Log& log)
    : oven(oven)
    , timer(timer)
    , motor(motor)
    , yeast(yeast)
    , extras(extras)
    , display(display)
    , startButton(startButton)
    , eventGenerator(eventGenerator)
    , log(log)
{
    currentState = S_STANDBY;
}

bool BreadBaker::Pulse()
{
    auto ev = eventGenerator.GetEvent();
    if (ev != std::nullopt)
    {
        HandleEvent(*ev);
    }
    return ev != std::nullopt;
}

// parameter name in comment to prevent compiler warning as it is unused for now
void BreadBaker::HandleEvent(Events ev)
{

    if(ev == NO_EVENT_OCCURRED)
    {
        return;
    }

    switch(currentState)
    {
        case S_STANDBY:
        {
            if(ev == MENU_BUTTON_PRESSED)
            {
                currentState = S_PROGRAM_SELECTION;
            }
            break;
        }
        case S_BAKING:
        {
            if(ev == TIMER_TIMEOUT)
            {
                currentState = S_DONE;
                timer.Set(5 MIN);
            }

            if(ev == MENU_BUTTON_LONG_PRESSED)
            {
                currentState = S_CANCEL;
                oven.Cancel();
            }
            break;
        }
        case S_KNEADING:
        {
            if(ev == TIMER_TIMEOUT && MotorSwitches < selectedProgram.kneading)
            {
                if(MotorSwitches % 2 == 0)
                {
                    motor.Stop();
                    motor.TurnRight();
                }
                else
                {
                    motor.Stop();
                    motor.TurnLeft();
                }
                MotorSwitches++;
                timer.Set(1 MIN);
            }

            if(ev == TIMER_TIMEOUT && MotorSwitches >= selectedProgram.kneading)
            {
                currentState = S_RISING;
                display.SetCurrentTask(Tasks::RISING);
                oven.StartRise(selectedProgram.rising);
            }

            if(ev == MENU_BUTTON_LONG_PRESSED)
            {
                yeast.Cancel();
                extras.Cancel();
                currentState = S_CANCEL;
            }
            break;
        }
        case S_RISING:
        {
            if(ev == TIMER_TIMEOUT)
            {
                currentState = S_BAKING;
                if(selectedProgram.baking == GetProgram(5).baking)
                {
                    timer.Set(bakeTime);
                }
                else
                {
                    oven.StartBake(selectedProgram.baking);
                }
                display.SetCurrentTask(Tasks::BAKING);
            }

            if(ev == MENU_BUTTON_LONG_PRESSED)
            {
                currentState = S_CANCEL;
                oven.Cancel();
            }
            break;
        }
        case S_RESTING:
        {
            if(ev == TIMER_TIMEOUT)
            {
                currentState = S_KNEADING;
                display.SetCurrentTask(Tasks::KNEADING);
                if(selectedProgram.addYeast)
                {
                    yeast.Drop(selectedProgram.kneading / 2);
                }

                if(selectedProgram.addExtras)
                {
                    extras.Drop(15 MIN);
                }
            }

            if(ev == MENU_BUTTON_LONG_PRESSED)
            {
                currentState = S_CANCEL;
            }
            break;
        }
        case S_DONE:
        {
            if(ev == TIMER_TIMEOUT)
            {
                currentState = S_STANDBY;
            }
            break;
        }
        case S_PROGRAM_SELECTION:
        {
            if(ev == TIMER_TIMEOUT)
            {
                currentState = S_STANDBY;
            }

            if(MENU_BUTTON_PRESSED)
            {
                if(programID > 5)
                {
                    programID = 0;
                }
                else
                {
                    programID++;
                }
            }

            if(ev == START_BUTTON_PRESSED)
            {
                currentState = S_RESTING;
                display.SetCurrentTask(Tasks::WAITING);
            }

            if(ev == TIMER_UP_BUTTON_PRESSED)
            {
                bakeTime += 10;
                if(bakeTime > 90)
                {
                    bakeTime = 90;
                }
            }

            if(ev == TIMER_DOWN_BUTTON_PRESSED)
            {
                bakeTime -= 10;
                if(bakeTime < 30)
                {
                    bakeTime = 30;
                }
            }

            break;
        }
        case S_CANCEL:
        {
            currentState = S_STANDBY;
            break;
        }
        default:
            break;
    }
}
