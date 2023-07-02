#include <stdio.h>
#include "parse_gps_data.h"

void parse_gps_data(char arr[]);

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
