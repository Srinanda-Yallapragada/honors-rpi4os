#include "io.h"
#include "shell.h"
void main() {
  uart_init();
  uart_writeText("\n Welcome to Srinanda's honors thesis OS! \nOS_name>");
  while (1) {
    shell();
  }
}
