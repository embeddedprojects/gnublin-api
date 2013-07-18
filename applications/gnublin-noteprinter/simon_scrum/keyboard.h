#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdio.h>
#include <usb.h>

usb_dev_handle* keyboard_init();
void keyboard_close(usb_dev_handle* udev);
void getInput(usb_dev_handle* udev, char *buffer);
#endif
