# Wiring and hardware notes

## ESP32 to IR sensors

| IR module | ESP32 |
|---|---|
| Left OUT | GPIO 35 |
| Right OUT | GPIO 34 |
| VCC | Suitable sensor supply (check the module rating) |
| GND | GND |

GPIO 34 and GPIO 35 are input-only pins on the ESP32, which is suitable for reading digital sensor outputs. They do not provide internal pull-up or pull-down resistors. Confirm that each sensor module provides a valid digital output level.

## ESP32 to DRV8833

| DRV8833 | ESP32 |
|---|---:|
| AIN1 | GPIO 18 |
| AIN2 | GPIO 19 (PWM) |
| BIN1 | GPIO 21 (PWM) |
| BIN2 | GPIO 22 |
| nSLEEP | GPIO 23 |

| DRV8833 output | Motor |
|---|---|
| OUT1 / OUT2 | Left motor |
| OUT3 / OUT4 | Right motor |

The code assumes the forward polarity that was verified on this robot: left motor forward uses AIN1 LOW and PWM on AIN2; right motor forward uses PWM on BIN1 and BIN2 LOW. If your motor wires are reversed, verify direction with the wheels lifted and correct the wiring or logic.

## Power and common ground

- Connect battery negative, driver GND, ESP32 GND and sensor GND together.
- Feed the motor driver VM from a supply suitable for the motors and within the DRV8833 board's ratings.
- Feed the ESP32 through a suitable regulated input. Do not connect a raw Li-ion cell directly to the ESP32 3V3 pin.
- A 1-cell Li-ion battery is about 3.7 V nominal and up to 4.2 V when fully charged. That can exceed the stated 3 V motor rating. Use an appropriate regulated motor supply or motors rated for the available voltage.
- Check the particular DRV8833 breakout-board pin labels and power arrangement; breakout boards are not all laid out identically.

## Noise reduction

Motor switching can cause voltage dips and electrical noise. A suitable bulk capacitor near the motor-driver supply and good wiring/layout can help, but they do not replace a supply that can handle motor startup and stall current. A small ceramic capacitor across each motor's terminals can also reduce brush noise.

## Before powering up

1. Check polarity and continuity.
2. Keep the robot's wheels off the ground for the first motor test.
3. Confirm that the driver and motor supply are within their ratings.
4. Confirm the ESP32 supply is regulated and stable.
5. Stop immediately if anything becomes hot or the ESP32 repeatedly resets.
