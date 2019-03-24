#include <iostream>
#include <fstream>
void logmsg(char * msg) {

	FILE * logfile;
	logfile = fopen("D:\\herp.txt", "a+");
	fprintf(logfile, "%s\n", msg);
	fclose(logfile);
}