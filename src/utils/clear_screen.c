#include "clear_screen.h"
#include "config.h"
#include <stdlib.h>

void clear_screen(void)
{
    if (!is_terminal_interactive())
    {
        return;
    }
    system("clear");
}
