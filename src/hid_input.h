#include <stdio.h>	
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>


#ifndef HID_INPUT_H
#define HID_INPUT_H


// define array indexes for the button_state array.
#define NUM_INDEXED_KEYS 9
#define IDX_FWD 0         
#define IDX_LEFT 1
#define IDX_BACK 2
#define IDX_RIGHT 3
#define IDX_ALLSTOP 4
#define IDX_TRANS_INC 5
#define IDX_TRANS_DEC 6
#define IDX_ROT_INC 7
#define IDX_ROT_DEC 8

#define STATE_CHANGE_UP 0
#define STATE_CHANGE_DOWN 1
#define STATE_CHANGE_HOLD 2

/** 
 The array contains the current states of the key that are being tracked
 (based on #define IDX_<key>). Declared as static to prevent writes for outside
 this module. 
 */
char key_state[NUM_INDEXED_KEYS];




/** 
 Getter fuction for the key state array (for use in files outside this module)
 */
char *hid_get_key_state();


/** 
 Open device descriptor for read operations
 @param device: filesystem path to the device, e.g /dev/input/event3
 @return integer file descriptor of the opened device
 */
int hid_open_device(char *device);


/** 
 Since only some of the keypresses are of interest, not all
 of them are tracked. This function translates the relevant key codes
 used in <linux/input.h> to array indexes and applies new state values.
 @param button: the code for the pressed button, #defines in <linux/input.h>
 @param new_state: the state to which the key has changed. 
 @param state_array: contains the states of all the key being tracked, will be modified
 */
void hid_translate_state_change(uint16_t button, char new_state, char state_array[]);

/** 
 Used to set recently declared key_state array into a know state, fill with 0x00
 @param key_state: array to be cleared, length is NUM_INDEXED_KEYS
 */
/*
 HACK: should be replaced with memset()
 */
void hid_clear_key_state(char key_state[]);


/** 
 Intended to be run in a reparate thread, polls specified input device for keypresses
 pthread requres args and return values to be of type void *
 @param device: string (char *)containing the filesystem path to the device
 @return: NULL
 */
void *hid_polling_loop(void *device);




#endif /* end of include guard: HID_INPUT_H */
