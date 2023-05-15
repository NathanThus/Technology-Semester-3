#ifndef BREADBAKER_H
#define BREADBAKER_H

#include "Events.h"
#include "IDisplay.h"
#include "IEventGenerator.h"
#include "IExtraIngredientsTray.h"
#include "IKneadMotor.h"
#include "IOven.h"
#include "IStartButtonLed.h"
#include "ITimer.h"
#include "IYeastTray.h"
#include "Log.h"
#include "States.h"

class BreadBaker
{
public:
    BreadBaker(IOven& oven, ITimer& timer, IKneadMotor& motor,
               IYeastTray& yeast, IExtraIngredientsTray& extras,
               IDisplay& display, IStartButtonLed& startButton,
               IEventGenerator& eventGenerator, Log& log);

    BreadBaker(const BreadBaker& other) = delete;
    BreadBaker& operator=(const BreadBaker&) = delete;

    bool Pulse();

    // For testing purposes
    // You'll have to decide yourself if you'd rather:
    // - have these methods private (better encapsulation)
    // - be able to test these methods
    // You cannot have both at the same time (without dirty tricks)
    void HandleEvent(Events ev);

private:
    IOven& oven;
    ITimer& timer;
    IKneadMotor& motor;
    IYeastTray& yeast;
    IExtraIngredientsTray& extras;
    IDisplay& display;
    IStartButtonLed& startButton;
    IEventGenerator& eventGenerator;
    Log& log;

    void HandleEvent_Processing(Events ev);

    void HandleState_S_ProgramSelection(Events ev);
    void HandleState_S_Baking(Events ev);
    void HandleState_S_Kneading(Events ev);
    void HandleState_S_Rising(Events ev);
    void HandleState_S_Resting(Events ev);
    void HandleState_S_Done(Events ev);
    void HandleState_S_Standby(Events ev);

    void HandleState_S_Cancel();

};

#endif
