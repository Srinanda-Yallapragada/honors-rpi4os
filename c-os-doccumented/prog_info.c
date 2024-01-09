#include "io.h"

// assumes uart has been initialized
void info() {
  uart_writeText("here is some information about the system! \n");
  uart_writeText("Did you know possible working names for the operating "
                 "systems here were RH2D and CZPZ? \n");
  uart_writeText(" Rust Hard To Do and C-easy P-easy! \n");
}
