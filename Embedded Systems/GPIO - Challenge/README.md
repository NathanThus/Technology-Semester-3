# GPIO Challenge - Nathan Thus

## Introduction
In this challenge the aim is to create a Manual Control Panel (Henceforth refered to as MCP), which uses various topics we have learnt so far in the semester. These include basic GPIO, Interrupts and rudementary Low Power Modes.

## The Challenge

## Hardware

## Design

## GPIO

## Interrupts


## Low Power Modes
For the lower power side of things, the aim is to demonstrate a small difference in power draw. That satisfies the criteria, without touching on sections that are too complicated for this challenge. While you could achieve lower power modes by disabling certain timers, which throw interrupts, this is not the aim of this challenge.

```cpp __WFI(); // This is more than enough to satisfy the criteria.```
This simply makes the processor wait for an interrupt, which is delivered by HAL_GetTick().

## Conclusion