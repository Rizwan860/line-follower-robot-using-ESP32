# ESP32 Line Following Robot

A small two-wheel line-following robot built around an **ESP32 DevKit V1**, two reflective IR sensors and a **DRV8833** motor driver.

This project grew through practical testing: checking the sensors, verifying each motor direction, balancing the two wheel speeds, and then tuning the robot on a closed rectangular track. The goal of this repository is to share the build clearly enough that another beginner can understand it, reproduce it and improve it.

> **Build status:** The sensor readings and motor operation were tested, and the current straight-line following calibration was reported working. Rectangular-track corner handling may need further tuning for your particular track and sensor placement.

## What it does

- Reads a left and right digital IR sensor.
- Drives two DC geared motors through a DRV8833.
- Makes a basic steering correction when one sensor detects the dark line.
- Uses separately calibrated PWM values to compensate for unequal motor speeds.

## Hardware used

| Part | Details |
|---|---|
| Controller | ESP32 DevKit V1 |
| Line sensors | 2 × HW-201 reflective IR modules |
| Motor driver | DRV8833 |
| Motors | 2 × 3 V DC geared motors |
| Battery | 1-cell Li-ion pack (nominal 3.7 V, 2000 mAh as used in the build) |
| Track | Black line on a light surface; rectangular closed loop |

## Pin connections

| Function | ESP32 GPIO |
|---|---:|
| Left IR sensor OUT | GPIO 35 |
| Right IR sensor OUT | GPIO 34 |
| DRV8833 AIN1 | GPIO 18 |
| DRV8833 AIN2 | GPIO 19 |
| DRV8833 BIN1 | GPIO 21 |
| DRV8833 BIN2 | GPIO 22 |
| DRV8833 nSLEEP | GPIO 23 |

Motor outputs:
- DRV8833 OUT1 / OUT2 → left motor
- DRV8833 OUT3 / OUT4 → right motor

Connect all grounds together: ESP32 GND, sensor GND, motor-driver GND and battery negative.

See [`docs/WIRING.md`](docs/WIRING.md) for more detail.

## Sensor logic observed during testing

The installed sensors were observed to produce these digital values:

| Left sensor | Right sensor | Meaning / action |
|---:|---:|---|
| 0 | 0 | Move forward |
| 0 | 1 | Correct right |
| 1 | 0 | Correct left |
| 1 | 1 | Stop (safe default) |

This table is specific to the sensor modules and mounting arrangement used in this build. Verify the output of your own sensors before running the motors. Some modules have adjustable potentiometers and may use the opposite output polarity.

## Current motor calibration

The following values are the latest values reported as working for this particular robot:

| Setting | Value |
|---|---:|
| Left motor forward PWM | 130 / 255 |
| Right motor forward PWM | 72 / 255 |
| Correction PWM | 120 / 255 |
| PWM frequency | 1 kHz |
| PWM resolution | 8 bit |
| Main loop delay | 200 µs |

The two motors did not run at the same speed with equal PWM, so the left and right values are intentionally different. These are starting values for this build, not universal values for all motors.

## Getting started

1. Install the Arduino IDE and the ESP32 board package.
2. Open [`firmware/ESP32_Line_Follower.ino`](firmware/ESP32_Line_Follower.ino).
3. Select the correct ESP32 board and COM port.
4. Check every connection and raise the robot so its wheels are clear of the table for the first motor test.
5. Upload the sketch.
6. Place the robot on the track and observe whether each correction moves it back toward the line.

The sketch uses the Arduino-ESP32 LEDC API (`ledcAttach` / `ledcWrite`) available in current ESP32 Arduino core versions. If your installed core is old and does not recognize `ledcAttach`, update the ESP32 board package or adapt the PWM setup for that core version.

## Important limitations

This is a simple two-sensor controller, not a full PID line follower. With only two sensors, some junctions, sharp corners and line-loss situations can produce ambiguous readings. The `1,1` state is currently configured to stop rather than guess a direction. A rectangular track may therefore need a dedicated corner strategy after recording the sensor pattern at each corner.

## Safety notes

- **Do not connect a raw Li-ion cell directly to the ESP32 3V3 pin.** Use a suitable regulated supply for the ESP32 and sensors.
- A fully charged 1-cell Li-ion battery reaches about 4.2 V. Confirm that the motor voltage rating and driver/supply arrangement are appropriate; the motors in this build are described as 3 V motors.
- Use a protected cell and a proper 1-cell Li-ion CC/CV charger. Never leave a charging cell unattended.
- Check motor stall current and the driver/battery current limits. A stalled motor can draw much more current than it draws while spinning freely.
- Stop testing if a motor, driver, battery or regulator becomes hot, or if the ESP32 repeatedly resets.

## Repository layout

```text
ESP32_Line_Follower_Robot/
├── README.md
├── LICENSE
├── .gitignore
├── firmware/
│   └── ESP32_Line_Follower.ino
└── docs/
    ├── WIRING.md
    ├── CALIBRATION.md
    └── TROUBLESHOOTING.md
```

## Next improvements

- Add a controlled corner routine after measuring sensor readings at the four corners.
- Add adjustable speed and correction settings.
- Add optional serial diagnostics with a controlled print interval.
- Consider a 3- or 5-sensor array for more reliable corner and junction detection.
- Consider a PID controller after the sensor geometry and motor response are measured.

## Author

**Shaik Rizwan Basha**  
GitHub: [Rizwan860](https://github.com/Rizwan860)

Built as a hands-on embedded systems project. Suggestions, issue reports and improvements are welcome.

## License

Released under the MIT License. See [`LICENSE`](LICENSE).
