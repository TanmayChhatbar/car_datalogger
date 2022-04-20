# Car Datalogger
The device logs IMU and GPS data onto an SD card (or slower, serial)

The screen offers critical stats on where data is being written, whether GPS has a fix, etc.

# Hardware
- TTGO TS V1.4 1.8in, with built-in MPU9250 IMU and a buzzer.
- Adafruit Ultimate GPS
- Some wires
- Basic soldering skills
- Optional 3D Printable case
- Optional custom car mount, near the CG 
  - could be easily adapted from the mount included on the repo
  - or just use double-sided tape

# Software
## code
The project is made in PlatformIO. PlatformIO should take care of all the dependencies and download all that is needed on its own.

## data analysis tools
The logs for GPS and IMU are combined into one file. You can use the included python script to separate the data.
You can use the MATLAB tools to plot the trajectory on a map, or visualize the IMU data as needed.

# TODO

![](media/datalogger.gif)
![](media/photo.png)
