#ifndef __INPUT_H__
#define __INPUT_H__

#include <libdragon.h>

#define INPUT_UP 0
#define INPUT_DOWN 1
#define INPUT_LEFT 2
#define INPUT_RIGHT 3

typedef struct SI_condat input_t;

input_t input_get();
void input_timer();

#endif //__INPUT_H__
