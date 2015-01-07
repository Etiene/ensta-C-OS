#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gfx.h"
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


void drawHistogram(int * r, int * g, int * b, int * y){
	int ysize = 300;
	int xsize = 300;

	char c;

	// Open a new window for drawing.
	gfx_open(xsize,ysize,"Example Graphics Program");

	// Set the current drawing color to green.
	gfx_color(0,200,100);

	// Draw a triangle on the screen.
	gfx_line(100,100,200,100);
	gfx_line(200,100,150,150);
	gfx_line(150,150,100,100);

	while(1) {
		// Wait for the user to press a character.
		c = gfx_wait();

		// Quit if it is the letter q.
		if(c=='q') break;
	}
}

void makeHistogram(image_desc * img){
	int r[256];
	int g[256];
	int b[256];
	int y[256];
	int i,j;
	uint8_t * red = (uint8_t *) img->pRed;
	uint8_t * green = (uint8_t *) img->pGreen;
	uint8_t * blue = (uint8_t *) img->pBlue;

	//columns
	for(i = 0; i < img->width; i++){
		r[i] = 0;
		g[i] = 0;
		b[i] = 0;
		y[i] = 0;
		//pixels
		for(j = 0; j < img->height; j++){
			r[i] += *(red + i*img->width + j);
			g[i] += *(green + i*img->width + j);
			b[i] += *(blue + i*img->width + j);
			y[i] += (*(red + i*img->width + j) + *(green + i*img->width + j) + *(blue + i*img->width + j)) / 3;
		}
	}

	drawHistogram(r,g,b,y);
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
					makeHistogram(img);
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