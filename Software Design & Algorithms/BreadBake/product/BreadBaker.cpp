#include "BreadBaker.h"
#include "Program.h"
#include "TimeConstants.h"

#include <iostream>

States currentState;
Program selectedProgram;

int programID = 0;
int bakeTime = 0;
int MotorSwitches = 0;
int delayTimer = 0;

bool isDelayTimer = false;

BreadBaker::BreadBaker(
    IOven &oven, ITimer &timer, IKneadMotor &motor, IYeastTray &yeast,
    IExtraIngredientsTray &extras, IDisplay &display,
    IStartButtonLed &startButton, IEventGenerator &eventGenerator,
    Log &log)
    : oven(oven), timer(timer), motor(motor), yeast(yeast), extras(extras), display(display), startButton(startButton), eventGenerator(eventGenerator), log(log)
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

    if (ev == NO_EVENT_OCCURRED)
    {
        return;
    }

    // These all exist in a substate.
    if (currentState >= S_CANCEL && currentState <= S_DONE)
    {
        HandleEvent_Processing(ev);
        return;
    }

    switch (currentState)
    {
    case S_STANDBY:
    {
        HandleState_S_Standby(ev);
        break;
    }
    case S_PROGRAM_SELECTION:
    {
        HandleState_S_ProgramSelection(ev);
        break;
    }

    default:
        break;
    }
}

void BreadBaker::HandleEvent_Processing(Events ev)
{
    switch (currentState)
    {
    case S_BAKING:
    {
        HandleState_S_Baking(ev);
        break;
    }
    case S_KNEADING:
    {
        HandleState_S_Kneading(ev);
        break;
    }
    case S_RISING:
    {
        HandleState_S_Rising(ev);
        break;
    }
    case S_RESTING:
    {
        HandleState_S_Resting(ev);
        break;
    }
    case S_DONE:
    {
        HandleState_S_Done(ev);
        break;
    }
    case S_CANCEL:
    {
        HandleState_S_Cancel();
        break;
    }
    default:
        break;
    }
}

void BreadBaker::HandleState_S_ProgramSelection(Events ev)
{
    if (ev == TIMER_TIMEOUT)
    {
        currentState = S_STANDBY;
    }

    if (ev == MENU_BUTTON_PRESSED)
    {
        if (programID < 5)
        {
            programID++;
        }
        else
        {
            programID = 1;
        }
    }

    if (ev == START_BUTTON_PRESSED)
    {
        if (oven.GetTemperature() > 50)
        {
            return;
        }

        currentState = S_RESTING;
        display.SetCurrentTask(Tasks::WAITING);
        int minutes = selectedProgram.resting + selectedProgram.kneading + selectedProgram.rising + selectedProgram.baking;
        // Convert minutes to hours and minutes
        int hours = minutes / 60;
        minutes = minutes % 60;
        display.SetTime(hours, minutes);
        startButton.LedOff();
        timer.Set(delayTimer);
    }

    if (programID == 5)
    {
        if (ev == TIMER_UP_BUTTON_PRESSED)
        {
            bakeTime += 10;
            if (bakeTime > 90)
            {
                bakeTime = 90;
            }
        }

        if (ev == TIMER_DOWN_BUTTON_PRESSED)
        {
            bakeTime -= 10;
            if (bakeTime < 30)
            {
                bakeTime = 30;
            }
        }
    }

    selectedProgram = GetProgram(programID);
    display.SetMenu(programID);
}

void BreadBaker::HandleState_S_Baking(Events ev)
{
    if (ev == OVEN_DONE)
    {
        currentState = S_DONE;
        timer.Set(5 MIN);
        display.SetCurrentTask(Tasks::DONE);
    }

    if (ev == MENU_BUTTON_LONG_PRESSED)
    {
        currentState = S_CANCEL;
    }
}

void BreadBaker::HandleState_S_Rising(Events ev)
{
    if (ev == OVEN_DONE)
    {
        currentState = S_BAKING;
        if (selectedProgram.baking == GetProgram(5).baking)
        {
            oven.StartBake(bakeTime);
        }
        else
        {
            oven.StartBake(selectedProgram.baking);
        }
        display.SetCurrentTask(Tasks::BAKING);
    }

    if (ev == MENU_BUTTON_LONG_PRESSED)
    {
        currentState = S_CANCEL;
        oven.Cancel();
    }
}

void BreadBaker::HandleState_S_Resting(Events ev)
{
    if (ev == TIMER_TIMEOUT && isDelayTimer)
    {
        timer.Set(selectedProgram.resting);
        isDelayTimer = false;
    }

    if (ev == TIMER_TIMEOUT)
    {
        currentState = S_KNEADING;
        display.SetCurrentTask(Tasks::KNEADING);
    }

    if (ev == MENU_BUTTON_LONG_PRESSED)
    {
        currentState = S_CANCEL;
    }
}

void BreadBaker::HandleState_S_Kneading(Events ev)
{
    if (MotorSwitches == 0)
    {
        if (selectedProgram.addExtras)
        {
            extras.Drop(15 MIN);
        }
        if (selectedProgram.addYeast)
        {
            yeast.Drop(selectedProgram.kneading / 2);
        }
    }

    if (ev == TIMER_TIMEOUT && MotorSwitches < selectedProgram.kneading)
    {
        if (MotorSwitches % 2 == 0)
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

    if (ev == TIMER_TIMEOUT && MotorSwitches == selectedProgram.kneading)
    {
        currentState = S_RISING;

        display.SetCurrentTask(Tasks::RISING);
        oven.StartRise(selectedProgram.rising);
    }

    if (ev == MENU_BUTTON_LONG_PRESSED)
    {
        currentState = S_CANCEL;
    }
}

void BreadBaker::HandleState_S_Cancel()
{
    currentState = S_STANDBY;
    display.SetCurrentTask(Tasks::DONE);

    if (oven.IsOn())
    {
        oven.Cancel();
    }

    yeast.Cancel();
    extras.Cancel();

    motor.Stop();
    startButton.LedOn();
    ResetValues();
}
void BreadBaker::HandleState_S_Done(Events ev)
{
    if (ev == TIMER_TIMEOUT)
    {
        currentState = S_STANDBY;
    }
    display.SetCurrentTask(Tasks::DONE);
    startButton.LedOn();
    ResetValues();
}

void BreadBaker::HandleState_S_Standby(Events ev)
{
    if (ev == MENU_BUTTON_PRESSED)
    {
        currentState = S_PROGRAM_SELECTION;
    }
}

void BreadBaker::ResetValues()
{
    programID = 1;
    MotorSwitches = 0;
    bakeTime = 0;
    isDelayTimer = false;
    delayTimer = 0;
}  
