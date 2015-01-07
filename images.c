#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "strhelpers.h"
#include "images.h"


int readImage(image_desc *pDesc, targa_header *pHead, FILE * fDesc)
{
	int i;
	targa_header head;
	/* Open fDesc file */
	// fDesc = fopen(fName,"r");

	/* Lecture du header */
	fread(pHead, sizeof(targa_header), 1, fDesc);
	head = *pHead; // on le met dans une vari. temporarire pour faciliter l'acces aux champs

	printf("[mem_targa] Header : %u %u %u %u %u\n", head.idlength, head.colourmaptype, head.datatypecode, head.width, head.height);

	// Initialisation de la structure image
	pDesc->width = head.width;
	pDesc->height = head.height;
	pDesc->pRed   = malloc(sizeof(uint8_t)*head.width*head.height);
	pDesc->pGreen = malloc(sizeof(uint8_t)*head.width*head.height);
	pDesc->pBlue  = malloc(sizeof(uint8_t)*head.width*head.height);

	// Remplir pixel par pixel les 3 couches de couleurs
	for (i=0; i<head.width*head.height; i++) {
		// read 1 int : blue
		fread(pDesc->pBlue+i, sizeof(uint8_t), 1, fDesc);
		// read 1 int : green
		fread(pDesc->pGreen+i, sizeof(uint8_t), 1, fDesc);
		// read 1 int : red
		fread(pDesc->pRed+i, sizeof(uint8_t), 1, fDesc);
	}
	return 1;

}

int writeImage (image_desc iDesc, targa_header head, char * fName)
{
	FILE *fDesc;
	uint8_t *pBlue, *pGreen, *pRed, *pBlueEnd;
	int i;

	/* Open output image file */
	if ((fDesc = fopen (fName, "w")) == NULL)
	{
		fprintf (stderr, "Cannot create the file \"%s\".\n", fName);
		return 0;
	}

	/* Ecrire le header dans fDesc*/
	head.width = iDesc.width;
	head.height = iDesc.height;
	printf("[read mem_targa] Header : %u %u %u %u %u \n", 
		 head.idlength, head.colourmaptype, head.datatypecode, head.width, head.height);

	fwrite (&head, sizeof (targa_header), 1, fDesc);

	// Variables temporaires 
	pBlue = iDesc.pBlue;
	pGreen = iDesc.pGreen;
	pRed = iDesc.pRed;
	/* On va lire head.width*head.height pixels pour les mettre dans le fichier fDesc */
	for (i=0; i<head.width*head.height; i++){
		fwrite(pBlue+i, sizeof(uint8_t), 1, fDesc);
		fwrite(pGreen+i, sizeof(uint8_t), 1, fDesc);
		fwrite(pRed+i, sizeof(uint8_t), 1, fDesc);
	}
	fclose (fDesc);
	return 1;
}


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
				FILE * fp;
				
				name = (char *) malloc(strlen(IMGPATH));
				strcat(name,IMGPATH);
				strcat(name,commands[i+1]);

				fp = fopen(name,"rb");
				if(fp == NULL){
					printf("Could not open file %s. \n",name);
				}else{
					targa_header * header = (targa_header *) malloc (sizeof(targa_header));
					image_desc * img = (image_desc *) malloc (sizeof(image_desc));
					printf("File %s is open. \n",name);
					readImage(img,header,fp);
					writeImage(*img,*header,"test.tga");
					fclose(fp);
					free(header);
					free(img);
				}
				
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