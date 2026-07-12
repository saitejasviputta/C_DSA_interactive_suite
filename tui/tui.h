#ifndef TUI_H
#define TUI_H

#ifndef _WIN32
void tui_run(void);
#else
static inline void tui_run(void) {}
#endif

#endif