# Elevator

Elevator is a simple elevator simulator written in C++.

## Primary Function

Manage 8 ACME Elevators

## Secondary Functions

- Send the elevators to the correct floor
- Handle user input inside the elevator
- Open & close doors in a timely manner
  - Verify that there is no obstacle in the way
  - Time out after 5 seconds
- Update the floor indicator
- Manage Elevator Request Button Events
  - Up / Down
  - Reset Button Backlight
- Play sound via speaker
- Elevator is sent on proximity or path basis
- Measure cable tension for safety
  - Lock elevator in case of failure

## Why is the system being built?

The current system isn't safe. It's old and needs to be replaced. The new system will be safer and more reliable.
The system is improved by the inclusion of a new elevator control system. The new system will be able to handle more elevators and will be more reliable, alongside the cable tension measurement system.

## Actors

- User
- Engineering / Maintenance Staff
- Emergency Services

Due to the fact the latter two aren't mentioned in the document, I will refrain from making a use case diagram for them.

## Use Cases

- Call elevator
- Select floor
- Keep door open via blocking
  
## Use Case Diagram

## Sources

![Sketch1](IMG20230227132257.jpg "Sketch")

![Sketch2](IMG20230227132311.jpg "Sketch")
