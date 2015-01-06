#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "strhelpers.h"
#include "images.h"

/*
	Reads the message sent to server and tries to interprete it
	Analyses if it contains commands and calls such commands
*/
void readParameters(char * msg){
	char * commands[MAXTEXT];
	int size, i, type;
	char * name;

	int color = DEFAULTCOLOR;


	size = str_split(msg,commands);

	#ifdef DEBUG
		for(i = 0; i < size; i++){
			printf("READING: %s \n",commands[i]);
		}
	#endif

	// HISTOGRAM
	if(caseless_strcmp(commands[0],"histogram") == 0){

		for(i = 1; i < size; i++){

			//Pick color
			if(caseless_strcmp(commands[i],"-c") == 0 && commands[i+1] != NULL)
				color = readColor(commands[i+1]);
				
			//Open image file
			else if(caseless_strcmp(commands[i],"-i") == 0 && commands[i+1] != NULL){
				name = (char *) malloc(strlen(commands[i+1])+1);
				strcpy(name,commands[i+1]);
			}

			//Open directory
			else if(caseless_strcmp(commands[i],"-f") == 0 && commands[i+1] != NULL){
				//TODO
			}

		}

		//TODO DEBUG
		//Does not have file name
		if(name == NULL){
			printf("You must provide a file name. Type \"info\" for help.");
		}

		#ifdef DEBUG
			//printf("HISTOGRAM INFO: file: %s color: %d\n", name, color);
		#endif
		
	}
	
	//OTHER COMMANDS

	//TODO
}



/*
	Reads a string containing color information for the histogram
	and returns int containing sum of desired colors
*/
int readColor(char * c){
	int sum = 0, i;
	//avoid repeating colors
	int r = 0, g = 0, b = 0;

	//If string is null returns default
	// [ is this really necessary? ]
	if(c == NULL)
		return DEFAULTCOLOR;

	//Looking for chars running through the string
	for(i = 0; i < strlen(c); i++){
		if(!r && c[i] == 'r')
			sum += red;
		else if(!g && c[i] == 'g')
			sum += green;
		else if(!b && c[i] == 'b')
			sum += blue;
	}

	//Returns sum or if sum is 0, returns default
	return (sum != 0) ? sum : DEFAULTCOLOR;
}