#include <stdio.h>
#include "parse_gps_data.h"

void parse_gps_data(char arr[]);
static bool checksum(char array[]);

void parse_gps_data(char arr[])
{

}


static bool gga_check(char ary[])
{
    /*This function checks the GGA parameter to check if the right
    inout string has been entered. GGA is the first parameter of 
    NMEA format GPS data.*/
	if (ary[0] == ',') {
        //Checks if the first parameter is empty
		return false;
	}
	else {
		size_t sz = strlen(ary);                   //get input array size
		char dum_ary[sz];                           //create an array of input array size
		strcpy(dum_ary,ary);                        //copy contents into the dummy array

		char* value = strtok(dum_ary,",");          //tokenize the string to get first parameter
		char chcker[] = "$GPGGA";

		return !(strcmp(value,chcker)); 
            /*Compares the value of first parameter with $GPGGA and returns true if 
            it matches(correct string) and false if it doesn't(incorrect string)*/
	}
}

static bool checksum(char array[])
{
    /*This function verifies checksum of the given data by comparing it with checksum
    present at the end of NMEA format. Checksum is calculated by doing XOR of all the 
    characters between the $ and * of the NMEA format GPS data*/
	int x = 1;
	int chcksum = 0;

	while (array[x] != '*') {
        /*This calculates XOR of data between $ and * of the NMEA format GPS data*/

		chcksum ^= array[x];
		++x;
	}
	++x;

	int receivedChecksum = 0;
	sscanf(&array[x], "%2X", &receivedChecksum);        //Reading and storing the checksum from NMEA in HEX Format

	return receivedChecksum==chcksum;                   //Compare the two and return the boolean
}
