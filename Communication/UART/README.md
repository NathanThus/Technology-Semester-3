# UART

## Transmitter

Takes the incoming bit from the computer, flips the byte, adds stop & start. Then sends it to the reciever. Bit by bit.

## Reciever

1 Timer for recieving, 1 timer for sampling (Far faster)
Reads transmitter pins and stores the bits in a buffer.
Turns into byte
Falling Edge interrupt
Keep track of bit count.

## Both

Timer Interrupt.
