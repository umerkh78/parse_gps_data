#include <stdio.h>
#include "parse_gps_data.h"

void parse_gps_data(char arr[]);
static bool checksum(char array[]);
static void check(char arry[],int abs_ary[16]);
static void set_display_data(gps_t* data,int abs_ary[16],char arry[],char* param[]);

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

static void check(char arry[],int abs_ary[16])
{
    /*This function checks empty parameters in the input string and sets their size 
    to 0 while also storing the sizes of other parameters in the abs_array variable*/
    size_t sze =  strlen(arry);

    //absent is a check to see how many commas(parameters) have passed
    //previous saves last location of comma to calculate size
    int i = abs_ary[0],absent=0,prev=0;
    bool asterik = false;

    while (arry[i] != '\0') {
        // keep iterating until comma comes
        while (arry[i] != ',' && arry[i] != '*' && i < sze) {
            ++i;
        }
        if (arry[i] == '*') {
            asterik = true;         //Boolean variable to check the arrival of asterik(checksum parameter)
        }
        
        if (arry[i]==',' || arry[i]=='*') {
            // checks if comma is present and identify if some parameter is absent
            abs_ary[absent] = i-prev;
            prev = i+1;
            ++absent;
        }
        if (arry[i+1] == '\0') {
            //check if the next character is the newline character signifying end of string
            if (asterik) {
                //if asterik has passed(checksum has passed) save the size of the string
                abs_ary[absent] = i-prev;
                prev = i+1;
            }
            else {
                //If asterik hasn't passed means a few parameters are missing in between
                while (absent <= 15) {
                    //Set size of all those parameters and checksum equal to zero
                    abs_ary[absent] = 0;
                    ++absent;
                }
            }
        }
        ++i;
    }
}

static void set_display_data(gps_t* data,int abs_ary[16],char arry[],char* param[])
{
    /*This function stores NMEA string into various parameters provided by the GPS struct
    and then displays them on esp-idf monitor*/
	size_t sz =  strlen(arry);
	char sentenceData[sz];
	strcpy(sentenceData,arry);                      //Copy the original string to avoid any change

	char* token = strtok(sentenceData, ", *");      //Tokenize the copied string
    int fieldIndex = 0;
    
    while (token != NULL) {
        switch (fieldIndex) {
            /*We check various cases of the field index iterator to store data in the
            required struct parameter adn then display it*/
            case 0:
                if (abs_ary[fieldIndex] == 0) {
                    //If size of this parameter is zero, mark it as missing
                	data->sat_type = "Missing";
                }
                else {
                    //If size not zero then assign value using strdup which uses malloc to allocate memory
                	data->sat_type = strdup(token);
                }
                ESP_LOGI("","%s: %s\n",param[fieldIndex],data->sat_type);   //Display the parameter and its contents
                break;
            case 1:
                if (abs_ary[fieldIndex] == 0) {
                	data->timestamp = "Missing";
                }
                else {
                	data->timestamp = strdup(token);
                }
                ESP_LOGI("","%s: %s\n",param[fieldIndex],data->timestamp);
                break;
            case 2:
                if (abs_ary[fieldIndex] == 0) {
                	data->latitude = "Missing";
                }
                else {
                	data->latitude = strdup(token);
                }
                ESP_LOGI("","%s: %s\n",param[fieldIndex],data->latitude);
                break;
            case 3:
                if (abs_ary[fieldIndex] == 0) {
                	data->lat_dir = "Missing";
                }
                else {
                	data->lat_dir = strdup(token);
                }
                ESP_LOGI("","%s: %s\n",param[fieldIndex],data->lat_dir);
                break;
            case 4:
                if (abs_ary[fieldIndex] == 0) {
                	data->longitude = "Missing";
                }
                else {
                	data->longitude = strdup(token);
                }
                ESP_LOGI("","%s: %s\n",param[fieldIndex],data->longitude);
                break;
            case 5:
                if (abs_ary[fieldIndex] == 0) {
                	data->long_dir = "Missing";
                }
                else {
                	data->long_dir = strdup(token);
                }
                ESP_LOGI("","%s: %s\n",param[fieldIndex],data->long_dir);
                break;
            case 6:
                if (abs_ary[fieldIndex] == 0) {
                	data->precision = "Missing";
                }
                else {
                	data->precision = strdup(token);
                }
                ESP_LOGI("","%s: %s\n",param[fieldIndex],data->precision);
                break;
            case 7:
                if (abs_ary[fieldIndex] == 0) {
                	data->sat_no = "Missing";
                }
                else {
                	data->sat_no = strdup(token);
                }
                ESP_LOGI("","%s: %s\n",param[fieldIndex],data->sat_no);
                break;
            case 8:
                if (abs_ary[fieldIndex] == 0) {
                	data->hdp = "Missing";
                }
                else {
                	data->hdp = strdup(token);
                }
                ESP_LOGI("","%s: %s\n",param[fieldIndex],data->hdp);
                break;
            case 9:
                if (abs_ary[fieldIndex] == 0) {
                	data->alt = "Missing";
                }
                else {
                	data->alt = strdup(token);
                }
                ESP_LOGI("","%s: %s\n",param[fieldIndex],data->alt);
                break;
            case 10:
                if (abs_ary[fieldIndex] == 0) {
                	data->unit1 = "Missing";
                }
                else {
                	data->unit1 = strdup(token);
                }
                ESP_LOGI("","%s: %s\n",param[fieldIndex],data->unit1);
                break;
            case 11:
                if (abs_ary[fieldIndex] == 0) {
                	data->geo_sep = "Missing";
                }
                else {
                	data->geo_sep = strdup(token);
                }
                ESP_LOGI("","%s: %s\n",param[fieldIndex],data->geo_sep);
                break;
            case 12:
                if (abs_ary[fieldIndex] == 0) {
                	data->unit2 = "Missing";
                }
                else {
                	data->unit2 = strdup(token);
                }
                ESP_LOGI("","%s: %s\n",param[fieldIndex],data->unit2);
                break;
            case 13:
                if (abs_ary[fieldIndex] == 0) {
                	data->aoc = strdup("Missing");
                }
                else {
                	data->aoc = strdup(token);
                }
                ESP_LOGI("","%s: %s\n",param[fieldIndex],data->aoc);
                break;
            case 14:
                if (abs_ary[fieldIndex] == 0) {
                	data->cor_stat_id = "Missing";
                }
                else {
                	data->cor_stat_id = strdup(token);
                }
                ESP_LOGI("","%s: %s\n",param[fieldIndex],data->cor_stat_id);
                break;
            case 15:
                if (abs_ary[fieldIndex] == 0) {
                	data->checksum = "Missing";
                }
                else {
                	data->checksum = strdup(token);
                }
                ESP_LOGI("","%s: %s\n",param[fieldIndex],data->checksum);
                break;
             //Add cases for additional fields
            default:
                break;
        }
        if(abs_ary[fieldIndex+1] != 0){         //If size of next element is non-zero then tokenize string
        	token = strtok(NULL, ",*");
        	fieldIndex++;
        }
        else{                                   //If size of next element is zero then do not tokenize as it 
        	fieldIndex++;                       //misses the parameters(due to presence of consecutive delimiters)
        }
    }
    free_data(data,abs_ary);                   //Free all dynamically allocated memory and point it to NULL
}
