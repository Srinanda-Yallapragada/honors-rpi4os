#include "io.h"

void main()
{
    uart_init();
    uart_writeText("Grind Time!\n");
    while (1);
}
