#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include "controls.h"

int key_w_pressed = 0;
int key_s_pressed = 0;
int key_up_pressed = 0;
int key_dn_pressed = 0;

void handle_input(int kb) {
    struct input_event input;
    
    while (read(kb, &input, sizeof(struct input_event)) > 0) {
        if (input.type == EV_KEY) {
            int pressed = (input.value == 1 || input.value == 2);
            int released = (input.value == 0);

            if (pressed) {
                switch(input.code) {
                    case KEY_W:    key_w_pressed = 1; break;
                    case KEY_S:    key_s_pressed = 1; break;
                    case KEY_UP:   key_up_pressed = 1; break;
                    case KEY_DOWN: key_dn_pressed = 1; break;
                }
            } else if (released) {
                switch(input.code) {
                    case KEY_W:    key_w_pressed = 0; break;
                    case KEY_S:    key_s_pressed = 0; break;
                    case KEY_UP:   key_up_pressed = 0; break;
                    case KEY_DOWN: key_dn_pressed = 0; break;
                }
            }
        }
    }
}
