#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>


#define TAILLE  400
#define LAR_BAR 100
#define SPACE   40

using namespace std;
typedef struct ImageDesc {
	//char  idlength;
	char  colourmaptype;
	char  datatypecode;
	short int colourmaporigin;
	short int colourmaplength;
	char  colourmapdepth;
	short int x_origin;
	short int y_origin;
	short w;
	short h;
	char  bitsperpixel;
  	char  imagedescriptor;
} imageDesc;

typedef struct Image {
	imageDesc desc;
	uint8_t*  pixels;
} image;

int main ( int argc, char** argv );
image hist3 ( image data );
image hist2 ( image data );
image hist1 ( image data, int cha );
void saveImage(image img, char* path);
void sort(int a[], int array_size);
image sizeHist(char *path, int* ranges, int size);
image sizeImgHist(char *path, int* ranges, int size);
void colorsHist(void);
void frequence(image data, int channel, int* fre);


void colorsHist (void)
{
	int files =0, i=0, j=0, h=0, w=0;
	char childs[100][256];
	char path[256] = "img/";
	char aux[256];
		// conta quantos arquivos tga tem no diretorio
	DIR *dir;
	FILE* fp;
	int dt[3];

	int freq[256];
	double bluetemp = 0, greentemp = 0, redtemp = 0;
	int greens=0, blues=0, reds=0;

	struct dirent *ent;
	if ((dir = opendir (path)) != NULL) {
	  while ((ent = readdir (dir)) != NULL) {
	    if (strstr(ent->d_name, ".tga\0"))
	    	files++;
	  }
	  closedir (dir);
	} else {
	  perror ("");
	}

	// armazena os path no childs
	if ((dir = opendir (path)) != NULL) {
		i = 0;
		while ((ent = readdir (dir)) != NULL) {
			if (strstr(ent->d_name, ".tga\0")) {
				strcpy(childs[i++], ent->d_name);
			}
		}
		closedir (dir);
	} else {
	  perror ("");
	}
	for (j=0; j< files; j++)
	{

		
	bluetemp = 0;
	greentemp = 0;
	redtemp = 0;
		image data;

		strcpy(aux, "img/");

		strcat(aux, childs[j]);

		fp = fopen(aux, "r");

		fread(&data.desc, sizeof(imageDesc), 1, fp);

		size_t size = data.desc.w * data.desc.h * sizeof(uint8_t);



		data.pixels = (uint8_t *) malloc(3*size);

		fread(data.pixels, sizeof(uint8_t), 3 * size, fp);


		frequence(data, 0, freq);
		for (i=0;i<256;i++)
			bluetemp += i*freq[i];
		frequence(data, 1, freq);
		for (i=0;i<256;i++)
			greentemp += i*freq[i];
		frequence(data, 2, freq);
		for (i=0;i<256;i++)
			redtemp += i*freq[i];

		if(bluetemp > greentemp && bluetemp > redtemp)
			blues++;
		else if (greentemp > bluetemp && greentemp > redtemp)
			greens++;
		else if (redtemp > bluetemp && redtemp > greentemp)
			reds++;
		else
			printf("\nchupameucu\n");


	}
	printf("\nblues: %d\ngreens: %d\nreds: %d\n", blues, greens, reds);
	dt[0] = blues*TAILLE/files;
	dt[1] = greens*TAILLE/files;
	dt[2] = reds*TAILLE/files;
	////////////////////////////////////////


	w = 3 * LAR_BAR + 2 * SPACE;
	h = TAILLE;

	image out;
	out.pixels = (uint8_t *) malloc(3 * w * h * sizeof(uint8_t));

	for (i = 0; i < w; i++) {
		for (j = 0; j < h; j++) {
			if (i % (LAR_BAR + SPACE) < LAR_BAR && j < dt[i / (LAR_BAR + SPACE)]) {
				if(i / (LAR_BAR + SPACE) < 1)
				{
				out.pixels[3 * (i + j * w)    ] =  255;
				out.pixels[3 * (i + j * w) + 1] = 0;
				out.pixels[3 * (i + j * w) + 2] = 0;
				}
				else if (i / (LAR_BAR + SPACE) < 2)
				{
				out.pixels[3 * (i + j * w)    ] =  0;
				out.pixels[3 * (i + j * w) + 1] = 255;
				out.pixels[3 * (i + j * w) + 2] = 0;
				}
				else if (i / (LAR_BAR + SPACE) < 3)
				{
				out.pixels[3 * (i + j * w)    ] =  0;
				out.pixels[3 * (i + j * w) + 1] = 0;
				out.pixels[3 * (i + j * w) + 2] = 255;
				}
			} else {
				out.pixels[3 * (i + j * w)    ] = 255;
				out.pixels[3 * (i + j * w) + 1] = 255;
				out.pixels[3 * (i + j * w) + 2] = 255;
			}
		}
	}


	ImageDesc data;

	data.colourmaptype = NULL;
	data.datatypecode = NULL;
	data.colourmaporigin = 2;
	data.colourmaplength = 0;
	data.colourmapdepth = NULL;
	data.x_origin = 0;
	data.y_origin = 0;
	data.bitsperpixel = (char) 24;
	data.imagedescriptor = NULL;

	out.desc = data;
	out.desc.w = w;
	out.desc.h = h;

	saveImage(out, "hist/colorHist.tga");


	///////////////////////////////////

}

image sizeHist(char *path, int* ranges, int size) {
	int i, j, w, h, tam, jaca = 0, files = 0;
	char childs[100][256];
	int hist[size + 1];

	for (i = 0; i < size + 1; i++)
		hist[i] = 0;

	sort(ranges, size);

	// conta quantos arquivos tga tem no diretorio
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (path)) != NULL) {
	  while ((ent = readdir (dir)) != NULL) {
	    if (strstr(ent->d_name, ".tga\0"))
	    	files++;
	  }
	  closedir (dir);
	} else {
	  perror ("");
	}

	// armazena os path no childs
	if ((dir = opendir (path)) != NULL) {
		i = 0;
		while ((ent = readdir (dir)) != NULL) {
			if (strstr(ent->d_name, ".tga\0")) {
				strcpy(childs[i++], ent->d_name);
			}
		}
		closedir (dir);
	} else {
	  perror ("");
	}


	for (i = 0; i < files; i++) {
		char aux[256] = "img/";
		strcat(aux, childs[i]);
		FILE* fp = fopen(aux, "r");
		fseek(fp, 0L, SEEK_END);
		tam = ftell(fp);

		for (j = 0; j < size; j++) {
			if (tam <= ranges[j]) {
				hist[j]++;
				jaca++;
				break;
			}
		}
	}

	w = LAR_BAR * (size + 1) + SPACE * size;
	h = 256;

	hist[size] = files - jaca;

	for (i = 0; i <= size; i++)
		printf("%d\n", hist[i]);

	for (i = 0; i <= size; i++)
		hist[i] = hist[i] * 256 / files;

	image out;
	out.pixels = (uint8_t *) malloc(3 * w * h * sizeof(uint8_t));

	for (i = 0; i < w; i++) {
		for (j = 0; j < h; j++) {
			if (i % (LAR_BAR + SPACE) < LAR_BAR && j < hist[i / (LAR_BAR + SPACE)]) {
				out.pixels[3 * (i + j * w)    ] = 100;
				out.pixels[3 * (i + j * w) + 1] = 100;
				out.pixels[3 * (i + j * w) + 2] = 0;
			} else {
				out.pixels[3 * (i + j * w)    ] = 255;
				out.pixels[3 * (i + j * w) + 1] = 255;
				out.pixels[3 * (i + j * w) + 2] = 255;
			}
		}
	}

	ImageDesc data;

	data.colourmaptype = NULL;
	data.datatypecode = NULL;
	data.colourmaporigin = 2;
	data.colourmaplength = 0;
	data.colourmapdepth = NULL;
	data.x_origin = 0;
	data.y_origin = 0;
	data.bitsperpixel = (char) 24;
	data.imagedescriptor = NULL;

	out.desc = data;
	out.desc.w = w;
	out.desc.h = h;

	saveImage(out, "hist/hist_fsize.tga");
}

image sizeImgHist(char *path, int* ranges, int size) {
	int i, j, w, h, tam, jaca = 0, files = 0;
	char childs[100][256];
	int hist[size + 1];

	for (i = 0; i < size + 1; i++)
		hist[i] = 0;

	sort(ranges, size);

	// conta quantos arquivos tga tem no diretorio
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (path)) != NULL) {
	  while ((ent = readdir (dir)) != NULL) {
	    if (strstr(ent->d_name, ".tga\0"))
	    	files++;
	  }
	  closedir (dir);
	} else {
	  perror ("");
	}

	// armazena os path no childs
	if ((dir = opendir (path)) != NULL) {
		i = 0;
		while ((ent = readdir (dir)) != NULL) {
			if (strstr(ent->d_name, ".tga\0")) {
				strcpy(childs[i++], ent->d_name);
			}
		}
		closedir (dir);
	} else {
	  perror ("");
	}


	for (i = 0; i < files; i++) {
		char aux[256] = "img/";
		strcat(aux, childs[i]);
		FILE* fp = fopen(aux, "r");
		imageDesc desc;
		fread(&desc, sizeof(desc), 1, fp);

		for (j = 0; j < size; j++) {
			if (desc.w * desc.h <= ranges[j]) {
				hist[j]++;
				jaca++;
				break;
			}
		}
	}

	w = LAR_BAR * (size + 1) + SPACE * size;
	h = 256;

	hist[size] = files - jaca;

	for (i = 0; i <= size; i++)
		hist[i] = hist[i] * 256 / files;

	image out;
	out.pixels = (uint8_t *) malloc(3 * w * h * sizeof(uint8_t));

	for (i = 0; i < w; i++) {
		for (j = 0; j < h; j++) {
			if (i % (LAR_BAR + SPACE) < LAR_BAR && j < hist[i / (LAR_BAR + SPACE)]) {
				out.pixels[3 * (i + j * w)    ] = 100;
				out.pixels[3 * (i + j * w) + 1] = 100;
				out.pixels[3 * (i + j * w) + 2] = 0;
			} else {
				out.pixels[3 * (i + j * w)    ] = 255;
				out.pixels[3 * (i + j * w) + 1] = 255;
				out.pixels[3 * (i + j * w) + 2] = 255;
			}
		}
	}

	ImageDesc data;

	data.colourmaptype = NULL;
	data.datatypecode = NULL;
	data.colourmaporigin = 2;
	data.colourmaplength = 0;
	data.colourmapdepth = NULL;
	data.x_origin = 0;
	data.y_origin = 0;
	data.bitsperpixel = (char) 24;
	data.imagedescriptor = NULL;

	out.desc = data;
	out.desc.w = w;
	out.desc.h = h;

	saveImage(out, "hist/hist_isize.tga");
}

image hist3 ( image dat ) {
	long int lr = 0, lg = 0, lb = 0, max=0;
	int i=0, j=0, w=0, h=0, m = 0;
	long int dt[3];

	imageDesc data = dat.desc;

	for (i = 0; i < 3 * data.w * data.h; i += 3) {
		lb += dat.pixels[i  ];
		lg += dat.pixels[i+1];
		lr += dat.pixels[i+2];

		m++;
	}

	lr = (((lr*TAILLE) / 255) / data.w) / data.h;
	lg = (((lg*TAILLE) / 255) / data.w) / data.h;
	lb = (((lb*TAILLE) / 255) / data.w) / data.h;

	dt[0] = lb;
	dt[1] = lg;
	dt[2] = lr;

	w = 3 * LAR_BAR + 2 * SPACE;
	h = TAILLE;

	image out;
	out.pixels = (uint8_t *) malloc(3 * w * h * sizeof(uint8_t));

	for (i = 0; i < w; i++) {
		for (j = 0; j < h; j++) {
			if (i % (LAR_BAR + SPACE) < LAR_BAR && j < dt[i / (LAR_BAR + SPACE)]) {
				if(i / (LAR_BAR + SPACE) < 1)
				{
				out.pixels[3 * (i + j * w)    ] =  255;
				out.pixels[3 * (i + j * w) + 1] = 0;
				out.pixels[3 * (i + j * w) + 2] = 0;
				}
				else if (i / (LAR_BAR + SPACE) < 2)
				{
				out.pixels[3 * (i + j * w)    ] =  0;
				out.pixels[3 * (i + j * w) + 1] = 255;
				out.pixels[3 * (i + j * w) + 2] = 0;
				}
				else if (i / (LAR_BAR + SPACE) < 3)
				{
				out.pixels[3 * (i + j * w)    ] =  0;
				out.pixels[3 * (i + j * w) + 1] = 0;
				out.pixels[3 * (i + j * w) + 2] = 255;
				}
			} else {
				out.pixels[3 * (i + j * w)    ] = 255;
				out.pixels[3 * (i + j * w) + 1] = 255;
				out.pixels[3 * (i + j * w) + 2] = 255;
			}
		}
	}

	out.desc = data;
	out.desc.w = w;
	out.desc.h = h;

	saveImage(out, "hist/hist_3.tga");

	return out;
}

void frequence(image data, int channel, int* fre) {
	int i;

	for (i = 0; i < 256; i++)
		fre[i] = 0;

	for (i = 0; i < 3 * data.desc.w * data.desc.h; i += 3)
		fre[data.pixels[i + channel]]++;
}

image hist1(image data, int channel) {
	int fre[256], cor[3];
	int i, j, w, h, t = 0;
	image img;

	if (channel == 0) {
		cor[0] = 255;
		cor[1] = 0;
		cor[2] = 0;
	} else if (channel == 1) {
		cor[1] = 255;
		cor[0] = 0;
		cor[2] = 0;
	} else if (channel == 2) {
		cor[2] = 255;
		cor[1] = 0;
		cor[0] = 0;
	}

	frequence(data, channel, fre);
	img.desc = data.desc;

	w = 256;
	h = 256;

	img.desc.w = w;
	img.desc.h = h;
	img.pixels = (uint8_t*) malloc(3 * w * h);

	for (i = 0; i < 256; i++) {
		if (t < fre[i])
			t = fre[i];
	}

	for (i = 0; i < 256; i++) {
		fre[i] = (fre[i] * h) / t;
	}

	for (i = 0; i < w; i++)
		for (j = 0; j < h; j++) {
			if (j < fre[i]) {
				img.pixels[3 * (i + j * w)    ] = cor[0];
				img.pixels[3 * (i + j * w) + 1] = cor[1];
				img.pixels[3 * (i + j * w) + 2] = cor[2];
			} else {
				img.pixels[3 * (i + j * w)    ] = 255;
				img.pixels[3 * (i + j * w) + 1] = 255;
				img.pixels[3 * (i + j * w) + 2] = 255;
			}
		}

	char path[17];
	sprintf(path, "hist/hist_1_%d.tga", channel);
	saveImage(img, path);

	return img;
}

void saveImage(image img, char* path) {
	FILE *fp = fopen(path, "w");
	fwrite(&img.desc, sizeof(imageDesc), 1, fp);
	fwrite( img.pixels, sizeof(uint8_t), img.desc.w * img.desc.h * 3, fp);
	fclose(fp);
}

image hist2(image data)
{
	int total[4][256];
	int i, j, t, h, w;
	image img;

	img.desc = data.desc;

	w = 256;
	h = 256;

	img.desc.w = w;
	img.desc.h = h;
	img.pixels = (uint8_t*) malloc(3 * w * h);



	for (i=0;i<3;i++)
		frequence(data, i, total[i]);
	for (i=0; i<256; i++)
		total[3][i] = (total[0][i] + total[1][i] + total[2][i]) / 3;

	for (i = 0; i < 256; i++) {
			if (t < total[3][i])
				t = total[3][i];
		}

	for (i = 0; i < 256; i++) {
		total[3][i] = (total[3][i] * h) / t;
	}


	for (i = 0; i < w; i++)
		for (j = 0; j < h; j++) {
			if (j < total[3][i]) {
				img.pixels[3 * (i + j * w)    ] = 0;
				img.pixels[3 * (i + j * w) + 1] = 0;
				img.pixels[3 * (i + j * w) + 2] = 0;
			} else {
				img.pixels[3 * (i + j * w)    ] = 255;
				img.pixels[3 * (i + j * w) + 1] = 255;
				img.pixels[3 * (i + j * w) + 2] = 255;
			}
		}

	saveImage(img, "hist/hist_2.tga");

	return img;

}

void sort(int a[], int array_size) {
     int i, j, temp;
     for (i = 0; i < (array_size - 1); ++i)
     {
          for (j = 0; j < array_size - 1 - i; ++j )
          {
               if (a[j] > a[j+1])
               {
                    temp = a[j+1];
                    a[j+1] = a[j];
                    a[j] = temp;
               }
          }
     }
}