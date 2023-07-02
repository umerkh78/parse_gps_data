#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void parse_gps_data(char arr[]);

//Struct to save parameters of NMEA Message
typedef struct {
    char* sat_type;
    char* timestamp;
    char* latitude;
    char* lat_dir;
    char* longitude;
    char* long_dir;
    char* precision;
    char* sat_no;
    char* hdp;
    char* alt;
    char* unit1;
    char* geo_sep;
    char* unit2;
    char* aoc;
    char* cor_stat_id;
    char* checksum;
} gps_t;

#ifdef __cplusplus
}
#endif