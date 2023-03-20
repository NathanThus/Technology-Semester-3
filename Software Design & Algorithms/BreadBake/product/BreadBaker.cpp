#include "BreadBaker.h"
#include "Program.h"
#include "TimeConstants.h"

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
void BreadBaker::HandleEvent(Events /* ev */)
{
}
