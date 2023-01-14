#pragma once
#include <asm/ioctl.h>

#define MY_IOCTL_IN _IOW('w', 1, my_ioctl_data)

#define operation_add _IO('w', 2)
#define operation_subs _IO('w', 3)
#define operation_mult _IO('w', 4)
#define operation_div _IO('w', 5)

#define check_status _IO('w', 6)
#define error_ack _IO('w', 7)

typedef enum
{

addition, 
subtraction, 
multiplication, 
division

}operation_t;

typedef struct my_ioctl_data {
     int val1;
     int val2;
     operation_t desiredOperation;
}my_ioctl_data;
