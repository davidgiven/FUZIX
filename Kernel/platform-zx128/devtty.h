#ifndef __DEVTTY_DOT_H__
#define __DEVTTY_DOT_H__

void tty_pollirq(void);
static void keydecode(void);

#define KEY_ROWS	8
#define KEY_COLS	8
extern uint8_t keymap[8];

#endif
