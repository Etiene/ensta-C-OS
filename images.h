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
void readParameters(char * msg);


/*
	Reads a string containing color information for the histogram
	and returns int containing sum of desired colors
	@param c: string containing color
	@return: number of sum of colors
*/
int readColor(char * c);
