#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>

void main()
{
char *buffer;
int bytes_read,fd,buffer_length;
buffer_length = (sizeof(char*)*256);
fd=0;
//buffer= malloc(sizeof(char*)* buffer_length);
/* allocate memory for character buffers HERE before you use them */

fd = open("/dev/process_list", O_RDONLY);
if(fd==0)
	printf("Errorin fd");
while(1){
	buffer = malloc(buffer_length);
	bytes_read = read(fd, buffer, buffer_length);
	if(bytes_read <0)
		break;
	printf("%s\n", buffer);	
	free(buffer);

}


close(fd);
}