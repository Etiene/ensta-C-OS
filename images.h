#define MAXTEXT 100
#define DEFAULTCOLOR 7 // r + g + b
#define DEBUG 1


#define IMGPATH "images/"

typedef enum{
	red = 1,
	green = 2,
	blue = 4,
} color;

typedef enum{
	image,
	folder
} type;

/* Un header targa basique. */
typedef struct targa_header_ {
  uint8_t  idlength;
  uint8_t  colourmaptype;
  uint8_t  datatypecode;
  uint8_t useless[9];
  uint16_t width;
  uint16_t height;
  uint8_t desc[2];
} targa_header;


/* Une structure pour decrire les elements d'image utiles au traitement */
typedef struct image_desc_
{
  char *fname;               /* Libellé du fichier image (path)                 */
  uint16_t width;
  uint16_t height;
  uint8_t * pRed;               /* Référence sur le plan mémoire de couleur rouge  */
  uint8_t * pGreen;              /* Référence sur le plan mémoire de couleur bleue  */
  uint8_t * pBlue;             /* Référence sur le plan mémoire de couleur verte  */
} image_desc ;


/*
	Reads the message sent to server and tries to interprete it
	Analyses if it contains commands and calls such commands
	@param msg: a string containing the message sent to server
	@return: nothing

	Expected msg format:
		<command> <option> [<parameter> [<option> [<parameter>]...]] 

	Available commands: 
		histogram

	Available Options:
		-c  : picks color
			Parameters:
				r 	: shows red histogram 
				g	: shows green histogram 
				b	: shows blue histogram 
			Default value:
				rgb

		-i 	: read image
			Parameters:
				image file name

		-f 	: read folder
			Parameters:
				directory path

		-s  : picks size
			Parameters:
				size limit for the images in KBytes

	
	Example:
		histogram -i 1.tga -c r

		histogram -f images -s 400 -c rgb

*/
void readParameters(char * msg, char * responseImagePath,char * error);


/*
	Reads a string containing color information for the histogram
	and returns int containing sum of desired colors
	@param c: string containing color
	@return: number of sum of colors
*/
int readColor(char * c);

/*
	Function provided in TD
*/
	int writeImage(image_desc desc, targa_header head, char* fName);


/*
	Function provided in TD
*/
int readImage(image_desc *pDesc, targa_header *pHeader, char * fName);

/*
	Makes histograms of images,
*/
void makeHistogram(image_desc * img, targa_header * head, char * name);

/*
	fills color graphic of image histogram for each color
*/
void fillColors(int width, int height, uint8_t * red, uint8_t * green, uint8_t * blue, int * points, int color_r, int color_g, int color_b);

/*
	fills color graphic of image histogram with 3 colors at the same time
*/
void fillColors2(int width, int height, uint8_t * red, uint8_t * green, uint8_t * blue, int * r, int * g, int * b);

/*
	fills color graphic in bars of image color rates
*/
void fillColors3(int width, int height, uint8_t * red, uint8_t * green, uint8_t * blue, int * r, int * g, int * b);

/*
	Makes histogram of images in a folder by size_limit (in Kbyties). 
	First bar is smaller than, second bar is higher than
	Also makes graphic of color rates of images in a folder
*/
void makeFolderHistogram(DIR * FD, char * dir, int size_limit);

