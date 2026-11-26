# Troubleshooting

## Robot moves in the wrong direction

- Lift the chassis so the wheels are clear.
- Test one motor at a time.
- Confirm which output pair drives the left motor and which drives the right motor.
- Check the forward polarity documented in `WIRING.md`.
- If a motor spins backward, power down and swap that motor's two output wires, or deliberately change the control logic.

## Robot turns away from the line

The steering map depends on the physical placement of the sensors. Confirm which sensor sees black when the robot drifts left and when it drifts right. Update the two correction branches only after confirming the readings.

## Robot moves straight but drifts gradually

The motors may have different speeds. Adjust the left and right forward PWM values independently in small increments. Also check for wheel friction, loose tyres, unequal gearboxes and chassis alignment.

## Robot sometimes ignores the line

- Clean the sensor lenses and track.
- Keep both sensors at the same height.
- Check that the sensor modules are firmly mounted and do not wobble.
- Recheck sensor readings under the actual room lighting.
- Adjust the sensor module potentiometers if the board provides them, then repeat the full sensor truth-table test.
- Check battery voltage under motor load.

## ESP32 resets when motors start

This often points to supply droop, motor noise or wiring problems. Check the regulator and battery current capability, common ground, wiring length and motor-driver supply. Do not solve this by feeding a raw battery into the ESP32 3V3 pin.

## Robot stops at a rectangular corner

The current code stops when both sensors read HIGH. Record the sensor pattern at the corner before changing the logic. A two-sensor setup may need a dedicated timed turn/search routine or an additional sensor to handle sharp corners reliably.
