#include "RFID_Drivers.h"

//Gracefully handles a reader that is already configured and already reading continuously
//Because Stream does not have a .begin() we have to do this outside the library
boolean setupNano(long baudRate)
{
        nano.begin(NeoSerial); //Tell the library to communicate over software serial port

        //Test to see if we are already connected to a module
        //This would be the case if the Arduino has been reprogrammed and the module has stayed powered
        NeoSerial.begin(baudRate); //For this test, assume module is already at our desired baud rate
        while(!NeoSerial); //Wait for port to open

        //About 200ms from power on the module will send its firmware version at 115200. We need to ignore this.
        while(NeoSerial.available()) NeoSerial.read();

        nano.getVersion();

        if (nano.msg[0] == ERROR_WRONG_OPCODE_RESPONSE)
        {
                //This happens if the baud rate is correct but the module is doing a ccontinuous read
                nano.stopReading();

                NeoSerial.println(F("Module continuously reading. Asking it to stop..."));

                delay(1500);
        }
        else
        {
                //The module did not respond so assume it's just been powered on and communicating at 115200bps
                NeoSerial.begin(115200); //Start software serial at 115200

                nano.setBaud(baudRate); //Tell the module to go to the chosen baud rate. Ignore the response msg

                NeoSerial.begin(baudRate); //Start the software serial port, this time at user's chosen baud rate
        }

        //Test the connection
        nano.getVersion();
        if (nano.msg[0] != ALL_GOOD) return (false); //Something is not right

        //The M6E has these settings no matter what
        nano.setTagProtocol(); //Set protocol to GEN2

        nano.setAntennaPort(); //Set TX/RX antenna ports to 1

        return (true); //We are ready to rock
}

void setupCommunication()
{
        NeoSerial.begin(115200);
        while (!NeoSerial); //Wait for the serial port to come online

        if (setupNano(38400) == false) //Configure nano to run at 38400bps
        {
                NeoSerial.println(F("Module failed to respond. Please check wiring."));
                while (1); //Freeze!
        }

        nano.setRegion(REGION_NORTHAMERICA); //Set to North America

        nano.setReadPower(500); //5.00 dBm. Higher values may caues USB port to brown out
        //Max Read TX Power is 27.00 dBm and may cause temperature-limit throttling
}
