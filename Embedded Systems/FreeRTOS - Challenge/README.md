# FreeRTOS, Timers & Watchdog Challenge

## The Challenge

The aim of this challenge is to create a system of PWN signals, which can be controlled by either a Manual Control Panel or the Serial Bus.

### Requirements

- Make use of an MCP (Manual Control Panel)
  - LEDs & Buttons
- Make use of Serial Communication
  - UART
- PWM Input & Output
- At least one timer
- Responsive

## Chosen Hardware

| Hardware | Description |
| --- | --- |
| STM32F404XE | Microcontroller |
| 2x Buttons | Manual Control Panel |
| 2x LEDs | Manual Control Panel |
| 1x Servo | PWM Input |

## Design

## Timers

I have made a general purpose timer class, which includes all the types of timers needed within the course, this includes:

- Time Base
- PWM Input
- PWM Output
- Counting Pulse

This class targets 3 General Purpose timers, 2/3/4, as these are the only timers which have 4 channels, which is required for the PWM Input/Output.

### Differentiation

One of the main problems with using the Servo as an input device, alongside the fact that it is not intended to be used as such, is the fact that the servo can turn bi-directionally. As Rene pointed out, this could prove to be a problem in the future, as it would be hard to detect which direction the servo is turing in. However, in testing, I have found that the servo cannot turn fast enough to lose track of itself. This means that the servo can be used as a PWM input device, without the need for a differentiation circuit.

This is primarily due to the significant amount of force required to turn the servo, which limits the rotational velocity. Even when forcing the fastest rotational speed via PWM input, the servo itself cannot turn fast enough to lose track of itself.

## Watchdog

I have made a simple watchdog class, which is used to reset the microcontroller if the system is unresponsive. This is done by using the `IWDG` peripheral, which is a simple watchdog timer, which can be used to reset the microcontroller if the system is unresponsive.

The watchdog is reset every time the system is responsive, this is done by using the `Feed` function, which the system needs to trigger to indiciate that it is still responsive. If this is not activated in time, the system performs a hard reset.

## FreeRTOS

Making use of FreeRTOS is very easy. For my current plan, I have decided to use the following tasks:

| Task | Description |
| --- | --- |
| `SerialTask` | Handles serial input |
| `ManualTask` | Handles all manual control panel input |

### SerialTask

The `SerialTask` is responsible for handling serial input. This includes:

- Receiving commands

This can be achieved using the `scanf` function, as supplied by the `stdio.h` library.
The scanf functions allows me to enforce a certain format for the commands, which makes it easier to parse the commands.

The main reason for not including serial output in the task as well, is due to the blocking nature of scanf. This means that the task will be blocked until a command is received, which means that the task will not be able to handle any other tasks, until a command is received. This in turn, would mean that there is no feedback to the user, which would make the system seem unresponsive.

This however, should not impact the other parts of the system, as the serial input will not be responisble for calling the watchdog reset function.

## Implementation

## Results