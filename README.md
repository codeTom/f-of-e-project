# Time of flight sensor based 3D mapper
## About
This project builds a slow 3D lidar alternative using 2 servos and a time of flight sensor.

## Firmware
The firmware is based on a stripped down version of [Warp-firmware](https://github.com/physical-computation/Warp-firmware), source can be found in src.
### Servo control
Servo driver can control 2 servos, see `src/boot/ksdk1.1.0/devServos.{h,c}` for more details. To allow higher resolution for PWM control, the default PWM driver was modified and now uses 1/1000 percent instead of percent for duty cycle unit.

### Distance sensor
A basic VL53L0X V2 driver is implemented in `src/boot/ksdk1.1.0/devVL53.{c,h}` based on [this post](https://www.artfulbytes.com/vl53l0x-post).

### Communication
The firmware sends data using the default warpPrint() function and it can be read using the standard JLink connection.

## Hardware
Models directory contains the 3D printed attachments (source in `servoholder.scad`) and the box svg file for 3mm material thickness. The openscad file requires some external stls for servos and the sensor which were obtained from grabcad.com. Servo arm scad file is from [this repo](https://github.com/hugokernel/OpenSCAD_ServoArms).

## Software and processing
`python/live.py` will display live data as they arrive and also output it to terminal for future processing. Make sure the JLink server is running and connected while using the script. As the live display in matplotlib is very slow, it's recommended to log output from live.py to a file and use `python/postprocess.py` to display it after a full scan was completed.

## How to reproduce this project
### Materials list
 * FRDM board
 * 2 9g Servos
 * VL53L0X V2 sensor
 * Some wires, optionally a breadboard
 * 3D printer
 * (optional) Laser cutter and 3mm material
 * 2 M3 screws
 * 4 M2 screws
 * 2 M2 nuts

### Process
1. Print `servoholder.stl` and `sensorholder.stl`.
2. Attach distance sensor to sensorholder
3. Screw servo to servoholder
4. Glue the larger servohorn to servoholder (elevation servo)
5. Connect the distance sensor to i2c on the FRDM
6. Load firmware to FRDM (`make warp && make warp-load`)
7. Attach small servo horn to the servo in the holder
8. Connect servo in the holder to 5V, ground and PTB11 (temporary)
9. Observe servo motion and adjust rotation of the arm so that it goes from horizontal to horizontal
10. Power off FRDM
11. Screw the sensor holder to the servo arm (securing the arm to the servo)
12. (optional) Cut and assemble the box, screw azimuth servo to the box and place FRDM in the box
13. Attach the servo holder to the azimuth servo so that the axes of the servo intersect (to reduce errors slightly)
14. Move elevation servo data cable to PTB10 (routing the cable sensibly)
15. Connect elevation servo to 5V, ground, and PTB11
16. Plug FRDM back in
17. Connect ot FRDM with JLink
18. Start `python/live.py > data.csv` and see the scan progress
