# Python scripts for the Datalogger
Some helper scripts to import data from SD card and prepare them for analysis.

The device logs IMU and GPS data in the same file. This is done to avoid the time delay introduced by closing and opening two files constantly. The separate_adafruit_gps.py script separates the data into IMU and GPS folders. 

If you're as lazy as I am, you can use import_all.py to auto-import the SD card data into a folder named by the current date in a directory specified.

operate_all.py combines the usage of both the previous files into one.