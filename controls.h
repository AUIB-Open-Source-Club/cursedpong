#ifndef INPUT_H
#define INPUT_H

#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <unistd.h>

extern int key_w_pressed;
extern int key_s_pressed;
extern int key_up_pressed;
extern int key_dn_pressed;

void handle_input(int fd);

#endif // !INPUT
