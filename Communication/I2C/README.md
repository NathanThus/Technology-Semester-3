# Communication - I2C

The aim of this project is to create a multi-master I2C bus, with which I can play a game (pong). The system must contain 2 master devices and 1 slave, being the controllers and display respectively.

## Concepts

### Hardware based solution

While this does not adhere to the I2C specification, it is a simple solution to the problem. The hardware based solution is based on using GPIO pins to force control of the bus, by directly controlling another master.

## Collision Avoid

An interesting aspect of the I2C bus is the fact that _low_ bits have a higher priority than _high_ bits. This means that if two masters write to the same register, the master with the lowest address will win. This could be leveraged to our advantage.

### Register based control

With this concept, the goal of either master is to take control of the bus by writing to the control register. The control register is a 1 byte register, which is located at address 0x00. The control register is used to set the state of the bus. The following states are possible:

| Value | Description |
|-------|-------------|
| 0x00  | Bus free    |
| 0x01  | Master 1    |
| 0x02  | Master 2    |

The control register is read by all masters. If a master wants to take control of the bus, it writes to the control register. If the bus is free, the master takes control of the bus. If the bus is taken by another master, the master waits until the bus is free. If the bus is taken by itself, the master does nothing. In pseudocode, this would look like the following:

``` cpp
Loop forever
{
    Check my own register, if it is 0x00
    {
        Take control of the bus
        Verify I have control of the bus.
        If I have control of the bus
        {
            Do something
        }
        Relinquish control of the bus
    }
    else
    {
        Wait until the bus is free
    }
}
```
