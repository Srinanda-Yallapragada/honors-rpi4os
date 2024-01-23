#include "io.h"

void main()
{
    uart_init();
    uart_writeText("Hello World!\n");
    while (1);
}
