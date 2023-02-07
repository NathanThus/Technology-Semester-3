# POUND LOCK

Pound lock is a design challenge, based around creating various diagrams for a pound lock.

## Users
Operator - The individual that operates the actual pound lock

## Use Cases
- The User needs to be able to raise water level
- The User needs to be able to lower water level
- The User needs to be able to engage an emergency stop
- The User needs to be able to relenquish an emergency stop

## Use Case Diagram

## Components
- Valves
- Water Level Sensor
- Traffic Light
- Control Panel
- Gate

### Details
A gate has:

- 3 Valves
- 2 Traffic Lights (1 Inner, 1 Outer)

Gates serve as the primary class for the program, controlling their own opening actions.

They can determine if:

- The water level is right to open the next valve.
- Open the next valve.
- Switch the state of the traffic light.

The gate has an array of valves, allowing the use of an enum to indicate each valve. However, it has a seperate Inbound & Outbound traffic light, for ease of programming & clarity.

## Class Diagram

## State Diagram


### Notes
In the PDF, section 12.4, there's an inconsistency in the naming of the Waterlevels. While the 2 prior refer to the valves, this one refers to water. It could have easily been Above_Low_Valve