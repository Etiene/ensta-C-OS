#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "strhelpers.h"
#include "images.h"
	
int readImage(image_desc *pDesc, targa_header *pHead, char * fName)
{
  FILE * fDesc;
  int i;
  targa_header head;
  /* Open fDesc file */
  fDesc = fopen(fName,"r");

  /* Lecture du header */
  fread(pHead, sizeof(targa_header), 1, fDesc);
  head = *pHead; // on le met dans une vari. temporarire pour faciliter l'acces aux champs

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

void fillColors(int width, int height, uint8_t * red, uint8_t * green, uint8_t * blue, int * points, int color_r, int color_g, int color_b){
	int i, j;
	int division_factor;
	int highest_number = 0;

	//resizing
	for(i=0;i<256;i++){
		if(points[i]>=highest_number){
			highest_number = points[i];
		}
	}

	division_factor = highest_number/(height-30);



	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			//axis
			if(i==20 || j == 20){
				red[i+ j*width ] = 0;
				green[i + j*width ] = 0;
				blue[i + j*width ] = 0;
			}
			//filling with white
			else
			{
				red[i + j*width] = 255;
				green[i + j*width] = 255;
				blue[i + j*width] = 255;
			}
		}
	}

	//graphic
	for(i=0;i<256;i++){
		for(j=0; j < points[i]; j++){
			red[21+i + (21+j/division_factor)*width] = color_r;
			green[21+i + (21+j/division_factor)*width] = color_g;
			blue[21+i + (21+j/division_factor)*width] = color_b;
		}
		
	}
}


void makeHistogram(image_desc * img, targa_header * head, char * name){
	image_desc * hist_desc = (image_desc *) malloc (sizeof(image_desc));
	targa_header hist_header = *head;
	int r[256];
	int g[256];
	int b[256];
	int y[256];
	int i,j;
	uint8_t * red = (uint8_t *) img->pRed;
	uint8_t * green = (uint8_t *) img->pGreen;
	uint8_t * blue = (uint8_t *) img->pBlue;
	int width = (int) img->width;
	int height = (int) img->height;
	char red_hist[300];
	char green_hist[300];
	char blue_hist[300];
	char y_hist[300];

	//Histogram points
	//columns
	for(i = 0; i < 256; i++){
		r[i] = 0;
		g[i] = 0;
		b[i] = 0;
		y[i] = 0;
	}

	for(i = 0; i < height*width ; i++, red++, green++, blue++){
		y[(*red)/3+(*green)/3+(*blue)/3]+=1;
		r[*red]+=1;
		g[*green]+=1;
		b[*blue]+=1;
	}

	width = 296;
	height = 300;
	hist_header.width = width;
	hist_header.height = height;
	hist_desc->width = width;
	hist_desc->height = height;
	
	hist_desc->pRed   = malloc(sizeof(uint8_t)*hist_desc->width*hist_desc->height);
	hist_desc->pGreen = malloc(sizeof(uint8_t)*hist_desc->width*hist_desc->height);
	hist_desc->pBlue  = malloc(sizeof(uint8_t)*hist_desc->width*hist_desc->height);

	red = (uint8_t *) hist_desc->pRed;
	green = (uint8_t *) hist_desc->pGreen;
	blue = (uint8_t *) hist_desc->pBlue;

	strcpy(red_hist,"histograms/red_");
	strcat(red_hist,name);

	strcpy(green_hist,"histograms/green_");
	strcat(green_hist,name);

	strcpy(blue_hist,"histograms/blue_");
	strcat(blue_hist,name);

	strcpy(y_hist,"histograms/y_");
	strcat(y_hist,name);

	fillColors(width,height,red,green,blue,r,255,0,0);
	writeImage(*hist_desc,hist_header,red_hist);
	fillColors(width,height,red,green,blue,g,0,255,0);
	writeImage(*hist_desc,hist_header,green_hist);
	fillColors(width,height,red,green,blue,b,0,0,255);
	writeImage(*hist_desc,hist_header,blue_hist);
	fillColors(width,height,red,green,blue,y,70,70,70);
	writeImage(*hist_desc,hist_header,y_hist);
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
			printf("READING %d:  %s \n",i,commands[i]);
		}
	#endif

	// HISTOGRAM
	if(caseless_strcmp(commands[0],"histogram") == 0){
printf("HISTOGRAM");
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
					readImage(img,header,name);
					makeHistogram(img,header,commands[i+1]);
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
			printf("HISTOGRAM INFO: file: %s color: %d\n", name, color);
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