# Pet Food Dispenser with Level-Sensing Light Bar (ESP32) - Build Guide

Remix credit: based on the 3D Printable Pet Food Dispenser With Level Sensing by Penguingineer (https://www.instructables.com/3D-Printable-Pet-Foot-Dispenser-With-Level-Sensing/, CC BY 4.0), updated/refurbished for modern use with a current parts list, a safer sensor supply and a rewritten build guide.

## What you are building

A gravity pet-food dispenser: the reservoir on top refills the open tray as your pet eats. A hinged arm rests on the food and turns a spring-centred single-axis joystick, which works here as a potentiometer. An ESP32 reads it and lights a WS2812B strip behind a white diffuser bar: green when there is plenty of food, yellow when it is getting low, flashing red when it needs a refill.

Printed parts (8 files):

- `auto feeder main body v2.stl`, `auto feeder tray v2.stl`, `auto feeder food lid v2.stl`, `auto feeder electronics lid v2.stl`
- `Level Sensor Arm.stl`, `Level Sensor Base.stl`
- `light bar body v2.stl`, `light bar diffuser v2.stl`
- `Pet Food Dispenser Assembly.step` - full assembly for reference

**Important: the STL files are stored in metres.** In most slicers they import about 1000 times too small (the main body shows as 0.23 mm wide). Scale every STL by **100000 %** (x1000) on import; the main body should then measure about 233 x 177 x 140 mm. The STEP file carries its units and imports at the right size. You need a printer with at least about 250 x 250 x 200 mm of build space for the body and the tray.

The original prints the diffuser bar in white PLA+ with two walls and no infill so the light shines through; everything else is black PLA+.

## Parts you buy separately

The shop parts are in the Bill of Material PDF. One part is not in it:

- **Single-axis joystick module, centre-return version** (the green one) from Elation Sports Technologies, the original author's company. The level-sensor base is shaped for this module. A bottom-return (black) or no-return (orange) version changes how the arm behaves.

## Step 1 - Install the software and flash the board first

1. Install the Arduino IDE (2.x).
2. In **File > Preferences > Additional boards manager URLs** add `https://espressif.github.io/arduino-esp32/package_esp32_index.json` and install **esp32 by Espressif Systems**.
3. Select the board **DOIT ESP32 DEVKIT V1**.
4. Install **Adafruit NeoPixel** with the Library Manager.
5. Open `Pet_Feeder_Level_Sensor.ino` from the project repository and upload it over a micro-USB data cable.

## Step 2 - Wiring

| From | To |
|---|---|
| 5 V supply + (via the DC jack screw terminal) | ESP32 VIN and LED strip 5V |
| 5 V supply - | ESP32 GND and LED strip GND |
| LED strip DIN | ESP32 GPIO 32 |
| Joystick VCC | **ESP32 3V3** |
| Joystick GND | ESP32 GND |
| Joystick signal (wiper) | ESP32 GPIO 35 |

Cut the strip to **18 LEDs** (the sketch drives 18 pixels) and wire to the end with the DIN arrow pointing away from the wire.

**Change from the original:** the original wiring picture feeds the joystick from the 5 V rail. GPIO 35 is an analog input that must stay at or below 3.3 V, so a 5 V supply on the potentiometer can push the pin over its limit. Power the joystick from 3V3 as in the table. This changes the raw readings, so re-tune the sketch as in Step 4.

Check the polarity on the screw terminal before you plug in the supply (+ and - are marked). Do not power the board from USB and the 5 V supply at the same time while you work on it.

## Step 3 - Assembly

1. Fit the joystick in the slot of the level-sensor base so its lever turns with the level-sensor arm, then mount the arm.
2. Lay the strip in the light bar body, LEDs facing the diffuser, and close it with the diffuser.
3. Put the ESP32 under the electronics lid and route the wires to the light bar and the sensor.
4. Set the reservoir (main body) on the tray. The parts lift apart for filling and cleaning; a few dots of hot glue fix any part you want to stay put.

## Step 4 - Calibrate and first run

The sketch maps the raw joystick reading with `map(potValue, 745, 0, 0, 18)` (line 37): a reading of 745 means empty, 0 means full. That 745 was measured on the original 5 V wiring.

1. Add `Serial.begin(115200);` at the start of `setup()` and `Serial.println(potValue);` after the `analogRead` line, then upload.
2. Open the serial monitor at 115200 baud. Note the reading with the arm hanging at the **empty** position and with the arm lifted to the **full** position.
3. Put those two numbers in place of `745` and `0` in line 37 and upload again.
4. Behaviour: 9 or more of 18 LEDs lit shows green, 1 to 8 shows yellow, below that the whole bar flashes red once per second.

The original write-up says a power cycle "re-zeroes" the joystick. The sketch has no zeroing code; use the steps above instead.

## Troubleshooting

- Wrong colours or a random number of LEDs: make sure the strip is a 5 V WS2812B/GRB strip and the strip GND is shared with the ESP32 GND.
- Bar always red: the joystick reading never rises; check the wiper wire on GPIO 35 and the values in line 37.
- Prints look tiny in the slicer: scale x1000, see above.

## Files in this project

- Print: the eight STL files listed above (scale x1000); `Pet Food Dispenser Assembly.step` for reference
- Firmware: `Pet_Feeder_Level_Sensor.ino` in the project repository
- Bill of Material PDF with shop links for each part
