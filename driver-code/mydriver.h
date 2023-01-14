#pragma once
#include <asm/ioctl.h>
#include "../../include/uapi/misc/mydriver.h"

#define sysbusAddr 0x100e0000
#define STATUS_REG_OFFSET  0x00
#define OPERATION_REG_OFFSET  0x04
#define DAT0_REG_OFFSET  0x08
#define DAT1_REG_OFFSET  0x0c
#define RESULT_REG_OFFSET  0x10

#define OPERATION_ADD  (1 << 0)
#define OPERATION_SUB  (1 << 1)
#define OPERATION_MULT 1 <<  (2)
#define OPERATION_DIV  (1 << 3)

#define STATUS_INVALID_OPERATION  (1 << 0)
#define STATUS_DIV_BY_ZERO  (1 << 1)

#define ERROR_ACK  0xFF

