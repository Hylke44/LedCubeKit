#ifndef LEDCUBE_H
#define LEDCUBE_H

#include "stdint.h"

/*
 For controlling the Led Cube a XYZ coordinate system is used.
       ^ Z
       |
       |_____________
      /|            /|
     / |           / |
    /  |          /  |
   /_____________/   |
   |   |        |    |
   |   |________|____|______ -> X
   |   /        |    /
   |  /         |   /
   | /          |  /
   |/           | /
   |____________|/
  /
 /
|/ Y


Three different planes can be used:
XY plane:
        ____________________ -> X
       /             /
      /             /
     /             /
    /             /
   /_____________/
  /
 /
|/ Y


XZ plane:
       ^ Z
       |
       |_____________
       |             |
       |             |
       |             |
       |             |
       |             |
       |_____________|______ -> X
	   
	  
YZ plane:
       ^ Z
       |
       |
      /|
     / |       
    /  |        
   /   |
   |   |   
   |   |
   |   /
   |  /
   | / 
   |/
   |
  /
 /
|/ Y


Lines are used in the same direction as the axis.
*/


/*********    GENERAL FUNCTIONS    *********/

// Use this typedef to make a ledcube buffer.
// Can be used to call set_cube() function
// Example:
//  cube_t cube = ({  {0b0110, 0b1001, 0b1001, 0b0110},
//                    {0b1001, 0b0000, 0b0000, 0b1001},
//                    {0b1001, 0b0000, 0b0000, 0b1001},
//                    {0b0110, 0b1001, 0b1001, 0b0110}});
typedef uint8_t cube_t[4][4];

// Initializes all the things for a ledcube
// Needs to be called once at the beginning of the code
// Example:
//  ledcube_init();
void ledcube_init(void);

// Waits for a number of new refresh frame.
// The code is "paused" when calling this function
// Each second has 10 frames.
// frames: number of new frames to wait
// Example:
//  wait_for_new_frames(2);
void wait_for_new_frames(uint8_t frames);


/*********    CUBE FUNCTION    *********/

// Turns all the leds off
// Example:
//  clear_cube();
void clear_cube(void);

// Turns all the leds on
// Example:
//  fill_cube();
void fill_cube(void);

// Turns the leds of the cube on according to the specified value
// Use the cube buffer to set the leds and then pass it to this function.
// cube: the led cube buffer to set the leds to
// Example:
//  set_cube(cube);
void set_cube(cube_t cube);


/*********    PLANE FUNCTIONS     *********/

// Turns on a single specified layer on the xy plane
// z: the plane to turn on [0 - 3]
// Example:
//  fill_plane_xy(1);
void fill_plane_xy(uint8_t z);

// Turns on a single specified layer on the xz plane
// y: the plane to turn on [0 - 3]
// Example:
//  fill_plane_xz(1);
void fill_plane_xz(uint8_t y);

// Turns on a single specified layer on the yz plane
// x: the plane to turn on [0 - 3]
// Example:
//  fill_plane_yz(1);
void fill_plane_yz(uint8_t x);

// Turns off a signle specified layer on the xy plane
// z: the plane to turn off [0 - 3]
// Example:
//  clear_plane_xy(1);
void clear_plane_xy(uint8_t z);

// Turns off a signle specified layer on the xz plane
// y: the plane to turn off [0 - 3]
// Example:
//  clear_plane_xz(1);
void clear_plane_xz(uint8_t y);

// Turns off a signle specified layer on the yz plane
// x: the plane to turn off [0 - 3]
// Example:
//  clear_plane_yz(1);
void clear_plane_yz(uint8_t x);


/*********    LINE FUNCTIONS    *********/

// Turns on a single specified line on the x axis
// y: the line y coordinate to turn on [0 - 3]
// z: the line z coordinate to turn on [0 - 3]
// Example:
//  fill_line_x(1, 2);
void fill_line_x(uint8_t y, uint8_t z);

// Turns on a single specified line on the y axis
// x: the line y coordinate to turn on [0 - 3]
// z: the line z coordinate to turn on [0 - 3]
// Example:
//  fill_line_y(1, 2);
void fill_line_y(uint8_t x, uint8_t z);

// Turns on a single specified line on the z axis
// x: the line x coordinate to turn on [0 - 3]
// y: the line y coordinate to turn on [0 - 3]
// Example:
//  fill_line_z(1, 2);
void fill_line_z(uint8_t x, uint8_t y);

// Turns off a single specified line on the x axis
// y: the line y coordinate to turn off [0 - 3]
// z: the line z coordinate to turn off [0 - 3]
// Example:
//  clear_line_x(1, 2);
void clear_line_x(uint8_t y, uint8_t z);

// Turns off a single specified line on the y axis
// x: the line y coordinate to turn off [0 - 3]
// z: the line z coordinate to turn off [0 - 3]
// Example:
//  clear_line_y(1, 2);
void clear_line_y(uint8_t x, uint8_t z);

// Turns off a single specified line on the z axis
// x: the line x coordinate to turn off [0 - 3]
// y: the line y coordinate to turn off [0 - 3]
// Example:
//  clear_line_z(1, 2);
void clear_line_z(uint8_t x, uint8_t y);


/*********    SHIFTING FUNCTIONS    *********/

// Shifts all the leds one layer up allong the z axis
// Example:
//  shift_cube_up();
void shift_cube_up(void);

// Shifts all the leds one layer down allong the z axis
// Example:
//  shift_cube_down();
void shift_cube_down(void);

// Shifts all the leds one layer left allong the x axis
// Example:
//  shift_cube_left();
void shift_cube_left(void);

// Shifts all the leds one layer right allong the x axis
// Example:
//  shift_cube_right();
void shift_cube_right(void);

// Shifts all the leds one layer to the front allong the y axis
// Example:
//  shift_cube_front();
void shift_cube_front(void);

// Shifts all the leds one layer to the back allong the y axis
// Example:
//  shift_cube_back();
void shift_cube_back(void);


#endif // LEDCUBE_H
