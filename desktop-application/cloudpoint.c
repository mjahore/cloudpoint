#define VER_MAJ	1
#define VER_MIN 0
#define VER_REV 1

#include <stdio.h>    // Standard input/output definitions
#include <stdlib.h>
#include <string.h>   // String function definitions
#include <unistd.h>   // for usleep()
#include <getopt.h>
#include <time.h>
#include "arduino-serial-lib.h"


int main(int argc, char *argv[])
{
    const int buf_max = 256;

    int fd = -1;
    float temperature, voltage;
    char *serialport = "/dev/cu.usbmodem1421";
    char filename[buf_max];
    int baudrate = 9600;  // default
    char quiet=0;
    char eolchar = '\n';
    int timeout = 5000;
    int not_ready = 1;
    char buf[buf_max];
    FILE *output, *gp_plot;
    int iter = 0;

    printf("CloudPoint Reader version %d.%d.%d\n", VER_MAJ, VER_MIN, VER_REV);
    printf("Michael J. A. Hore, Case Western Reserve University\n\n");

    printf("Opening GNUPlot...");
    gp_plot = popen("/usr/bin/gnuplot", "w");

    // Open GNUPlot for plotting data real time.
    if (!gp_plot) {
	printf("FAILED!\n");
    }

    // Close serial port if already open.
    if( fd!=-1 ) {
        serialport_close(fd);
        if(!quiet) printf("closed port %s\n",serialport);
    }
 
    // Open the Arduino board.
    fd = serialport_init(serialport, baudrate);
    if(!quiet) printf("opened port %s\n",serialport);
    serialport_flush(fd);

    printf("Enter filename: ");
    scanf("%s", filename);

    output = fopen(filename, "w");
    fprintf(output, "# Cloudpoint\n");
    fclose(output);
   
    // Do not proceed until we read a "real temperature"
    while(not_ready) {
	printf("Unplug the red 5V power wire!\n");
	printf("Waiting...");
 	sleep(5000);
        memset(buf,0,buf_max);  //
        serialport_read_until(fd, buf, eolchar, buf_max, timeout);
   	sscanf(buf, "%f\t%f\n", &temperature, &voltage);

        if (temperature > -10 && temperature < 150) {
		not_ready = 0;
	}
    }
 
    // Set plot properties
    fprintf(gp_plot, "set title \"%s\"\n", filename);
    fprintf(gp_plot, "set xlabel \"Temperature (C)\"\n");
    fprintf(gp_plot, "set ylabel \"Voltage (V)\"\n");
    fflush(gp_plot);

    // Proceed with the main loop to read from Arduino and write to disk.
    while(1) {
            memset(buf,0,buf_max);  //
            serialport_read_until(fd, buf, eolchar, buf_max, timeout);
            printf("%d\t%s", iter,buf);
	  
	    output = fopen(filename, "aw");
	    fprintf(output, "%s", buf);
            fclose(output);

	    // Every 10 seconds, update the data display.
	    if (iter % 10 == 0) {
               fprintf(gp_plot, "plot \"%s\" using 1:2\n", filename);
               fflush(gp_plot);
            }
	    // Wait until we read the next set of data.
	    // Pause for 1 second.
            iter++;
    }

    fclose(gp_plot);
    serialport_close(fd);
    exit(EXIT_SUCCESS);
} // end main

