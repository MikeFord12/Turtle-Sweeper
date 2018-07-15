# Turtle-Sweeper

# Background

This repository holds the code for the turtle sweeper team's GVSU final project. Code for each module, libraries, and then main logic code are included.

The purpose of this project is to provide a solution for tracking neonate (newborn) turtles in their natural wooded habitat that is more cost-effective than the current method. RFID technology can provide an inexpensive option to do this. The low cost of the tags will enable more neonates to be tracked at once, which given their high mortality rate, will provide better data on their migratory habits. RFID was also chosen due to the availability of long-lasting tags. Passive RFID tags can last years, while active VHF tags run out of battery life in about a month.

The goal is to design an easy to use system that can easily find and identify turtles, as well as keep track of their movement through seamless data logging. In order to be able to easily identify the turtles in the wild the Ultra High-Frequency range was chosen due to its longer read ranges. An Arduino Mega 2560 was selected as the microcontroller interface with the ThingMagic M6E Nano reader shield. To achieve higher gain and external antenna was added.  A 6mm passive RFID tag was selected, and based on Fiis Transmission estimations, the tag will read at the 10” ranges in free space and in dry conditions.

The UI was designed using modules compatible with the Arduino microcontroller to include event saving functionality, GPS location, LCD display, battery level indication, and keeping time. The mechanical design was inspired by a metal detector wand. The metal detector design will increase the reach of the user and place the antenna closer to the specimen. Overall this design will improve the ability to control the antenna and improve its portability. The increased portability will enable the user to cover more ground and search in areas that might be difficult to reach. 

# Setup

If anyone desires to clone and run this code, the included libraries must be present in the user’s Arduino IDE. The Arduino IDE used for development is Version 1.8.5. The SD, SoftwareSerial, and SPI libraries used are included in the Arduino IDE. The external libraries used are included in the Github repo. 

---------------------------------------------------------------------
**In order for these to work you must put these library folders in your “arduino->libraries” directory. If there is no libraries folder in your arduino directory, you must create one and put the library folders inside of there.**
---------------------------------------------------------------------

# Contact

For questions about this repository you can contact:  
Mike Ford – fordmich@mail.gvsu.edu  
David Bizzocchi – bizzoccd@mail.gvsu.edu
