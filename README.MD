# arduino-display-temp

Takes temperature (ºC) from NTC resistor and displays in a 128x32 OLED

### About this

I make this project because is funny playing with Arduino. Nothing special, only back-up code

### Devices

I only tested this on a STM32 blue pill board (yeah, a whole ARM 32-bit Cortex-M3 75MHz processor for this because is more cheap than an arduino nano)

### Required components

NTC thermistor (recommended 10k ohm), a resistor (recommended 10k ohm), thermistor datasheet->(IMPORTANT)<-

Wiring:

```
[GND]------[NTC]------[r10K]------[3.3v || 5v]
                   |
                  [A0]
```

### Values

```c++
	float Vin = 3.3;     // [V in]
	float Rt = 10000;    // Resistor t [ohm] [10k]
	float R0 = 10000;    // value of rct in T0 [ohm] [10k]

	// [...]
	float Bta = 4050; // BETA VALUE [IN THERMISTOR DATASHEET, VERY IMPORTANT!!!!!]
```

### Hardcoded takes[]

`takes[]` is hardcoded to 20 values, it's for average calculation (ntc may vary a lot every take) and makes this unreadable and unestable

### Delet dis

- I don't code in C or C++ I tried to do my best

- Pending use of (millis() - lastupdate > interval) to avoid blocking the main thread