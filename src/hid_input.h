#include <stdio.h>	
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <linux/input.h>


#ifndef HID_INPUT_H
#define HID_INPUT_H


// define array indexes for the button_state array.
#define IDX_W 0
#define IDX_A 1
#define IDX_S 2
#define IDX_D 3
#define IDX_SPACE 4

#define STATE_CHANGE_UP 0
#define STATE_CHANGE_DOWN 1
#define STATE_CHANGE_HOLD 2


/** 
 Open device descriptor for read operations
 @param device: filesystem path to the device, e.g /dev/input/event3
 @return integer file descriptor of the opened device
 */
int hid_open_device(char *device);


/** 
 @param button: the code for the pressed button, #defines in <linux/input.h>
 @param new_state: the state to which the key has changed. 
 @param state_array: contains the states of all the key being tracked, will be modified
 */
void hid_translate_state_change(uint16_t button, char new_state, char state_array[]);


/** 
 Intended to be run in a reparate thread, polls specified input device for keypresses
 pthread requres args and return values to be of type void *
 @param device: string (char *)containing the filesystem path to the device
 @return: NULL
 */
void *hid_polling_loop(void *device);




#endif /* end of include guard: HID_INPUT_H */
