# UART

## Introduction

The purpose of this project is to create a software UART solution, using the GPIO pins for reading the data. This would in turn, allow itself to be used as an in-field debugging tool, allowing me to attach the device to the lines of a serial communication, and read the data being sent.

## Requirements

The following requirements are in place for the project:

- I must be able to read data at a baudrate of AT LEAST 9600.
- I must support multiple data configurations, including:
  - 7-8 data bits
  - 1-2 stop bits
  - Even/Odd/No parity

The system is built on two seperate modules, namely the transmitter and the reciever. The transmitter is responsible for recieving data via USB serial, and then converting it into a bitstream. The reciever is responsible for reading the bitstream, and converting it into a byte.

The transmitter can be verified using the Analog Discovery 2, which additionally allows for a fair bit of debugging. The receiver can be debugged using PuTTY, or another serial monitor.

## Theory

### Timing

Timing is absolutely crucial in this project. While optimisations are usually not the bread and butter of these kind of programming challenges, they are absolutely crucial in this case. The reason for this is that the timing of the bits is crucial to the correct operation of the system. If the timing is off, the system will not be able to read the data correctly, resulting in garbage data.

Some preemptive optimisations I can think of include:

- Using a for loop instead of while loops
- Reading registers instead of arduino function calls

### Sampling

<!-- stuff -->

## Designs

![Reciever State Machine](out/Diagrams/State%20Machine%20Reciever.png)

![Transmitter State Machine](out/Diagrams/State%20Machine%20Transmitter.png)

## Implementation

### Transmitter

#### Proof

![300 Baud](Transmitter_Pictures/Baud_300_Receiver.png)

![9600 Data](Transmitter_Pictures/Baud_9600_Preset_Data_Transmitter.png)

![9600 Even Parity](Transmitter_Pictures/Baud_9600_Parity_Even_Transmitter.png)

![9600 Odd Parity](Transmitter_Pictures/Baud_9600_Parity_Odd_Transmitter.png)

![9600 Two Stop Bits](Transmitter_Pictures/Baud_9600_Two_Stop.png)

![9600 Seven Data Bits](Transmitter_Pictures/Baud_9600_7Bit.png.png)

### Reciever

1 Timer for recieving, 1 timer for sampling (Far faster)
Reads transmitter pins and stores the bits in a buffer.
Turns into byte
Keep track of bit count.

### Micros vs Interrupts

lorem ipsum dolor sit amet consectetur adipisicing elit. Quisquam, quos.

