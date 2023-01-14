#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "mydriver.h"


char dane[] = "Hello_from_app";
char *readBuff; 
char *writeBuff; 
int main(void) {

	printf("Hello from app!\n");
	
	int fd = open("/dev/my_device_driver", O_RDWR);
	
	if (fd < 0) {	printf("Unable to open driver\n"); 	return 0;	}
	
	printf("Opened the driver!\n");
	
	readBuff = (char*)malloc(50);
	writeBuff = "2";
	//my_ioctl_data data ={5,10,addition};
	
	if (write(fd, writeBuff, 4 ) < 0) {  printf("Unable to write to driver\n");	return 0;	}
	
	writeBuff = "10";
	if (write(fd, writeBuff, 4 ) < 0) {  printf("Unable to write to driver\n");	return 0;	}
	
	
	printf("Writing IOCTL!\n");	
	
	if (ioctl(fd, operation_add, 0) < 0) {printf("Unable to handle IOCTL\n"); perror("Error:");	return 0;	}
	
		
	if (read(fd, readBuff, 50 ) < 0) {  printf("Unable to read from driver\n");	return 0;	}
		
	printf("RECIVED FROM DRIVER:  ");
	printf("%s",readBuff);
	
   	free(readBuff);
   	/*-------------------------------------------------------------------------------------*/
   	writeBuff = "2";
	//my_ioctl_data data ={5,10,addition}
	
	if (write(fd, writeBuff, 4 ) < 0) {  printf("Unable to write to driver\n");	return 0;	}
	
	writeBuff = "0";
	if (write(fd, writeBuff, 4 ) < 0) {  printf("Unable to write to driver\n");	return 0;	}
	
	
	printf("Writing IOCTL!\n");	
	
	if (ioctl(fd, operation_div, 0) < 0) {printf("Unable to handle IOCTL\n"); perror("Error:");	return 0;	}
	
		if (ioctl(fd, check_status, 0) < 0) {printf("Unable to handle IOCTL\n"); perror("Error:");	return 0;	}
		
		if (ioctl(fd, error_ack, 0) < 0) {printf("Unable to handle IOCTL\n"); perror("Error:");	return 0;	}
	
		
	
	
   	free(readBuff);
   	
   	
   	

	printf("Time to close\n");
	
	close(fd);
	
	printf("Closed :) Bye\n");
	return 0;
}
