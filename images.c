#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

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
  printf("[mem_targa] Header : %u %u %u %u %u\n", head.idlength, head.colourmaptype, head.datatypecode, head.width, head.height);


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

void fillColors2(int width, int height, uint8_t * red, uint8_t * green, uint8_t * blue, int * r, int * g, int * b){
	int i, j;
	int division_factor;
	int highest_number = 0;


	//resizing
	for(i=0;i<256;i++){
		if(r[i]>=highest_number){
			highest_number = r[i];
		}
		if(g[i]>=highest_number){
			highest_number = g[i];
		}
		if(b[i]>=highest_number){
			highest_number = b[i];
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
		for(j=0; j < r[i]; j++){
			red[21+i + (21+j/division_factor)*width] += 100;
			green[21+i + (21+j/division_factor)*width] += 200;
			blue[21+i + (21+j/division_factor)*width] += 200;
		}
		for(j=0; j < g[i]; j++){
			red[21+i + (21+j/division_factor)*width] += 200;
			green[21+i + (21+j/division_factor)*width] += 100;
			blue[21+i + (21+j/division_factor)*width] += 200;
		}
		for(j=0; j < b[i]; j++){
			red[21+i + (21+j/division_factor)*width] += 200;
			green[21+i + (21+j/division_factor)*width] += 200;
			blue[21+i + (21+j/division_factor)*width] += 100;
		}

	}
}

void fillColors3(int width, int height, uint8_t * red, uint8_t * green, uint8_t * blue, int * r, int * g, int * b){
	int i, j;
	int division_factor, sum_red = 0, sum_green = 0, sum_blue = 0;
	int highest_number;


	for(i = 0; i < 256 ; i++){
		sum_red += r[i]*i;
		sum_green += g[i]*i;
		sum_blue += b[i]*i;
	}

	//resizing
	highest_number = sum_red;
	if(sum_green>highest_number)
		highest_number = sum_green;
	if(sum_blue>highest_number)
		highest_number = sum_blue;

	division_factor = highest_number/(height-40);



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
	for(i=0; i < sum_red/division_factor; i++){
		//width of bar is 20 pixels
		for(j=80; j<100; j++){
			green[j + (21+i)*width] = 0;
			blue[j + (21+i)*width] = 0;
		}
		
	}

	for(i=0; i < sum_green/division_factor; i++){
		//width of bar is 20 pixels
		for(j=160; j<180; j++){
			red[j + (21+i)*width] = 0;
			blue[j + (21+i)*width] = 0;
		}
		
	}

	for(i=0; i < sum_blue/division_factor; i++){
		//width of bar is 20 pixels
		for(j=240; j<260; j++){
			red[j + (21+i)*width] = 0;
			green[j + (21+i)*width] = 0;
		}
		
	}
}




void makeHistogram(image_desc * img, targa_header * head, char * name){
	image_desc * hist_desc = (image_desc *) malloc (sizeof(image_desc));
	targa_header hist_header = *head;
	int r[256], g[256],  b[256], y[256];
	int i,j, sum_red = 0, sum_green = 0, sum_blue = 0;
	uint8_t * red = (uint8_t *) img->pRed;
	uint8_t * green = (uint8_t *) img->pGreen;
	uint8_t * blue = (uint8_t *) img->pBlue;
	int width = (int) img->width;
	int height = (int) img->height;
	char red_hist[300], green_hist[300], blue_hist[300], y_hist[300], rgb_hist[300], rgb2_hist[300];

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
	strcpy(rgb2_hist,"histograms/rgb2_");
	strcat(rgb2_hist,name);
	strcpy(rgb_hist,"histograms/rgb_");
	strcat(rgb_hist,name);


	fillColors(width,height,red,green,blue,r,255,0,0);
	writeImage(*hist_desc,hist_header,red_hist);
	fillColors(width,height,red,green,blue,g,0,255,0);
	writeImage(*hist_desc,hist_header,green_hist);
	fillColors(width,height,red,green,blue,b,0,0,255);
	writeImage(*hist_desc,hist_header,blue_hist);
	fillColors(width,height,red,green,blue,y,70,70,70);
	writeImage(*hist_desc,hist_header,y_hist);

	fillColors2(width,height,red,green,blue,r,g,b);
	writeImage(*hist_desc,hist_header,rgb_hist);

	fillColors3(width,height,red,green,blue,r,g,b);
	writeImage(*hist_desc,hist_header,rgb2_hist);
}

void makeFolderHistogram(DIR * FD, char * dir, int size_limit){
	image_desc * hist_desc = (image_desc *) malloc (sizeof(image_desc));
	targa_header * hist_header = (targa_header *) malloc (sizeof(targa_header));
	struct dirent* in_file;
	FILE    *entry_file;
	struct stat st;
	int size, sum_red = 0, sum_green = 0, sum_blue = 0;
	char file_name[300];
	int smaller = 0, higher = 0, width = 296, height = 300, i, j;
	uint8_t * red, * green, * blue;
	int multiply_factor = 1, division_factor = 0;


	printf("image size %d\n", size_limit);


	  while ((in_file = readdir(FD))){
		if (!strcmp (in_file->d_name, "."))
            continue;
        if (!strcmp (in_file->d_name, ".."))    
            continue;

        printf("open file %s. \n",in_file->d_name);
      	strcpy(file_name,dir);
		strcat(file_name,"/");
        strcat(file_name,in_file->d_name);
        entry_file = fopen(file_name, "r");
        if (entry_file == NULL)
        {
        	printf("Could not open file %s. \n",file_name);
        }
        else {
        	printf("open file %s. \n",file_name);
        	stat(file_name, &st);
			size = st.st_size;   
			if(size <= size_limit*1024){
				printf("smaller");
				smaller += 1;
			}
			else{
				printf("higher");
				higher += 1;
			}
				
			readImage(hist_desc,hist_header,file_name);
			red = (uint8_t *) hist_desc->pRed;
			green = (uint8_t *) hist_desc->pGreen;
			blue = (uint8_t *) hist_desc->pBlue;
			for(i = 0; i < hist_desc->width * hist_desc->height; i++){
				sum_red += *red++;
				sum_green += *green++;
				sum_blue += *blue++;
			}

			printf("%d\n",size);   
			fclose(entry_file);  
		}

	}

	printf("higher than %dk: %d, smaller: %d.\n",size_limit,higher,smaller);

	hist_header->width = width;
	hist_header->height = height;
	hist_desc->width = width;
	hist_desc->height = height;
	
	hist_desc->pRed   = malloc(sizeof(uint8_t)*hist_desc->width*hist_desc->height);
	hist_desc->pGreen = malloc(sizeof(uint8_t)*hist_desc->width*hist_desc->height);
	hist_desc->pBlue  = malloc(sizeof(uint8_t)*hist_desc->width*hist_desc->height);
	red = (uint8_t *) hist_desc->pRed;
	green = (uint8_t *) hist_desc->pGreen;
	blue = (uint8_t *) hist_desc->pBlue;

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

	
	//resizing
	multiply_factor = (higher>smaller)? (height-40)/higher : (height-40)/smaller;

	//graphic
	
	for(i=0; i < smaller*multiply_factor; i++){
		//width of bar is 20 pixels
		for(j=100; j<125; j++){
			red[j + (21+i)*width] = 70;
			green[j + (21+i)*width] = 70;
			blue[j + (21+i)*width] = 70;
		}
		
	}

	for(i=0; i < higher*multiply_factor; i++){
		//width of bar is 20 pixels
		for(j=200; j<220; j++){
			red[j + (21+i)*width] = 70;
			green[j + (21+i)*width] = 70;
			blue[j + (21+i)*width] = 70;
		}
		
	}

	hist_header->desc[0]=24;
	hist_header->datatypecode=2;
	hist_header->idlength = 0;
	hist_header->colourmaptype = 0;


	writeImage(*hist_desc,*hist_header,"test.tga");


	red = (uint8_t *) hist_desc->pRed;
	green = (uint8_t *) hist_desc->pGreen;
	blue = (uint8_t *) hist_desc->pBlue;

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

	higher = sum_red;
	if(sum_green>higher)
		higher = sum_green;
	if(sum_blue>higher)
		higher = sum_blue;

	division_factor = higher/(height-40);

	//graphic
	for(i=0; i < sum_red/division_factor; i++){
		//width of bar is 20 pixels
		for(j=80; j<100; j++){
			green[j + (21+i)*width] = 0;
			blue[j + (21+i)*width] = 0;
		}
		
	}

	for(i=0; i < sum_green/division_factor; i++){
		//width of bar is 20 pixels
		for(j=160; j<180; j++){
			red[j + (21+i)*width] = 0;
			blue[j + (21+i)*width] = 0;
		}
		
	}

	for(i=0; i < sum_blue/division_factor; i++){
		//width of bar is 20 pixels
		for(j=240; j<260; j++){
			red[j + (21+i)*width] = 0;
			green[j + (21+i)*width] = 0;
		}
		
	}

	writeImage(*hist_desc,*hist_header,"test2.tga");

}

/*
	Reads the message sent to server and tries to interprete it
	Analyses if it contains commands and calls such commands
*/
void readParameters(char * msg){
	char * commands[MAXTEXT];
	int size, i, type, image_size = 0;
	char * name;
	int color = DEFAULTCOLOR;
	int name_ = 0;
	int readcolor = 0, readsize = 0, readimage = 0, readfolder = 0;



	size = str_split(msg,commands);

	#ifdef DEBUG
		for(i = 0; i < size; i++){
			printf("READING %d:  %s \n",i,commands[i]);
		}
	#endif

	// HISTOGRAM
	if(caseless_strcmp(commands[0],"histogram") == 0){
		for(i = 1; i < size; i++){

			//Pick color
			if(caseless_strcmp(commands[i],"-c") == 0 && commands[i+1] != NULL)
				readcolor = 1;

			// size
			if(caseless_strcmp(commands[i],"-s") == 0 && commands[i+1] != NULL)
				readsize = 1;
				
			//Open image file
			else if(caseless_strcmp(commands[i],"-i") == 0 && commands[i+1] != NULL)
				readimage = 1;

			//Open directory
			else if(caseless_strcmp(commands[i],"-f") == 0 && commands[i+1] != NULL)
				readfolder;
			

		}

		if(readcolor)
			color = readColor(commands[i+1]);

		if(readsize)
			image_size = atoi(commands[i+1]);

		if(readimage){
			FILE * fp;
				
			name = (char *) malloc(strlen(IMGPATH));
			name_ = 1;
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

		if(readfolder){
			DIR* FD;
				
			name_ = 1;
			if (NULL == (FD = opendir (commands[i+1]))) {
				printf("Could not open dir %s. \n",commands[i+1]);
			}else{
				makeFolderHistogram(FD,commands[i+1],image_size);
			}
		}

		// DEBUG
		//Does not have file name
		if(!name_){
			printf("You must provide a file name. Type \"info\" for help.\n");
		}
		
	}
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
