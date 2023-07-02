#include <stdio.h>

void app_main(void)
{
    char ary[] = "$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E";
    parse_gps_data(ary);
}