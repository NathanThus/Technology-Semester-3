# Watchdog

_KR Register_

| Value | Description |
| --- | --- |
| 0xAAAA | Watchdog Avoid Reset |
| 0x5555 | Watchdog Setup, grants access to RLR, PR & WLR |
| 0xCCCC | Watchdog Start |

```cpp
IDWG->KR =  0x5555; // Setup
IDWG->PR =  0b111;  // Every 256 cycles, subtract 1 from counter.
IDWG->RLR = 0x0FFF; // Reload
IDWG->KR =  0xCCCC; // Start
IDWG->KR =  0xAAAA; // Avoid Reset

(40kHz / PRESCALER) / RLR_VAL = seconds before watchdog reset.
// Assuming a PRESCALER of 256 & RLR_VAL of 2500, the watchdog will reset the system after 1 second.
// (40kHz / 256) / 2500 = 1 second
```
