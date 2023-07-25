#include "io.h"

void main()
{
    int basic_arr[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uart_init();
    uart_writeText("Hello World \n");
    while (1)
    {
        {
            volatile unsigned long long i;
            for (i = 0; i < 1000000000ULL; ++i)
            {
                for (int j = 0; j < 72; j++)
                {
                    basic_arr[j] += 1;
                }
            }
        }
    }
}
