#ifndef __INPUT_H__
#define __INPUT_H__

#include <libdragon.h>

#define INPUT_UP 0
#define INPUT_DOWN 1
#define INPUT_LEFT 2
#define INPUT_RIGHT 3

#define VRU_READ 0x09
#define VRU_WRITE 0x0A
#define VRU_READ_STATUS 0x0B
#define VRU_WRITE_CONFIG 0x0C
#define VRU_WRITE_INIT 0x0D

typedef struct SI_condat input_t;

input_t input_get();
void input_timer();
void input_vru();

#endif //__INPUT_H__
