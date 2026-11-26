# Calibration notes

These notes record the values and observations from this particular build. They are not universal settings.

## 1. Confirm the sensor output

With the robot stationary, move each sensor over the light surface and the black line. The observed logic for this build was:

| Surface / position | Left | Right |
|---|---:|---:|
| Both sensors on the light surface / centered arrangement | 0 | 0 |
| Left sensor sees black, right sees light | 1 | 0 |
| Left sees light, right sensor sees black | 0 | 1 |
| Both see black | 1 | 1 |

The physical meaning depends on the sensor spacing and the line width. The build's observed steering mapping was:

- `0,0`: forward
- `0,1`: correct right
- `1,0`: correct left
- `1,1`: stop

If your sensor outputs differ, do not copy this mapping blindly.

## 2. Motor balance

The two motors did not run at the same speed with equal PWM. The following values were found to give a useful balance on this robot:

- Left forward PWM: **130**
- Right forward PWM: **72**
- Correction PWM: **120**

PWM is 8-bit, so the available range is 0–255. Change values in small steps and record each change. Test on the actual floor and battery condition because friction and battery voltage affect the result.

## 3. Mechanical setup recorded

- Track: closed rectangular path
- Track line width: approximately 2.7 cm
- Sensor centre-to-centre spacing: approximately 3 cm

The sensor spacing is slightly wider than the stated line width. Small changes in sensor height, alignment and angle can affect readings. Keep both sensors at the same height and parallel to the floor. Test at a low speed first.

## 4. Corner calibration

A two-sensor robot cannot identify every corner or junction unambiguously. Before adding a corner routine:

1. Approach a corner slowly.
2. Record left and right sensor values just before, during and just after the turn.
3. Repeat for all four corners.
4. Note whether the robot sees `00`, `01`, `10` or `11`, and for how long.
5. Only then design a turn routine.

The current firmware stops on `11`. This is a conservative default, not a completed rectangular-corner algorithm.
