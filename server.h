#define MAXNAME 10
#define MAXTEXT 100

void read_header(int sock, char * username);
void print_msg(char *talker, char * chat);
void send_img(int socket,char * path);
void transforme_image(char * argtab[]);