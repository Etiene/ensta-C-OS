
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include "strhelpers.h"
#include "images.h"

int main(int argc, char * argv[]){
	char msg[300];
	int i;
	strcpy(msg,"");
	for(i=1;i<argc;i++){
		strcat(msg,argv[i]);
		strcat(msg," ");
	}	

	printf("%s\n",msg);
	readParameters(msg);

	return 0;
}