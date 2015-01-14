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

	
	Example:
		histogram -i flag.tga -c r

		histogram -f /home/myname/images -c rb

*/
void readParameters(char * msg, char * responseImagePath,char * error);


/*
	Reads a string containing color information for the histogram
	and returns int containing sum of desired colors
	@param c: string containing color
	@return: number of sum of colors
*/
int readColor(char * c);

/* A documenter */
void freeImage(image_desc *pdesc);
/* A documenter */
int writeImage(image_desc desc, targa_header head, char* fName);
/* A documenter */
int readImage(image_desc *pDesc, targa_header *pHeader, char * fName);

void makeHistogram(image_desc * img, targa_header * head, char * name);
