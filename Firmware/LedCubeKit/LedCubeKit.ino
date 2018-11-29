//##############################################################################                   
//#     ______      # PROJECT:
//#    -      -     #   Led Cube Kit
//#   /  |  |\ \    # DESCRIPTION:
//#  |  _|__|_\ |   #   A DIY kit for making a 4x4x4 ledcube.
//#  | \ |  |   |   #   This code only provides simple animations
//#   \ \|  |  /    #   so the cube can be tested.
//#    -______-     #   It is up to you to make more, better, flashing 
//#                 #   and amazing creations.
//##############################################################################

extern "C" {
  #include "ledcube.h"
}

void setup() {
  // put your setup code here, to run once:
  ledcube_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t i, x, y, z, times;
  cube_t cube;
  
  // yz plane sliding left and right
  clear_cube();
  fill_plane_yz(0);
  wait_for_new_frames(2);
  for (times = 0; times < 3; times++) {
    shift_cube_left();
    wait_for_new_frames(2);
  }
  for (times = 0; times < 3; times++) {
    shift_cube_right();
    wait_for_new_frames(2);
  }

  
  // Filling cube per yz plane
  clear_cube();
  for (x = 0; x < 3; x++) {
    fill_plane_yz(x);
    wait_for_new_frames(2);
  }

  // Empty cube per yz plane
  fill_cube();
  wait_for_new_frames(2);
  for (x = 0; x < 3; x++) {
    clear_plane_yz(x);
    wait_for_new_frames(2);
  }

  // xy plane sliding down and up
  clear_cube();
  fill_plane_xy(0);
  wait_for_new_frames(2);
  for (times = 0; times < 3; times++) {
    shift_cube_up();
    wait_for_new_frames(2);
  }
  for (times = 0; times < 3; times++) {
    shift_cube_down();
    wait_for_new_frames(2);
  }

  // Filling cube per xy plane
  clear_cube();
  for (z = 0; z < 3; z++) {
    fill_plane_xy(z);
    wait_for_new_frames(2);
  }

  // Empty cube per xy plane
  fill_cube();
  wait_for_new_frames(2);
  for (z = 0; z < 3; z++) {
    clear_plane_xy(z);
    wait_for_new_frames(2);
  }
  
  // xz plane sliding front and back
  clear_cube();
  fill_plane_xz(3);
  wait_for_new_frames(2);
  for (times = 0; times < 3; times++) {
    shift_cube_front();
    wait_for_new_frames(2);
  }
  for (times = 0; times < 3; times++) {
    shift_cube_back();
    wait_for_new_frames(2);
  }

  // Filling cube per xz plane
  clear_cube();
  for (y = 0; y < 3; y++) {
    fill_plane_xz(3-y);
    wait_for_new_frames(2);
  }

  // Empty cube per xz plane
  fill_cube();
  wait_for_new_frames(2);
  for (y = 0; y < 3; y++) {
    clear_plane_xz(3-y);
    wait_for_new_frames(2);
  }
  
  // Wave
  clear_cube();
  for (times = 0; times < 2; times++){
    fill_line_x(0,0);
    wait_for_new_frames(2);
    shift_cube_back();
    fill_line_x(0,0);
    wait_for_new_frames(2);
    shift_cube_back();
    fill_line_x(0,1);
    wait_for_new_frames(2);
    shift_cube_back();
    fill_line_x(0,2);
    wait_for_new_frames(2);
    shift_cube_back();
    fill_line_x(0,3);
    wait_for_new_frames(2);
    shift_cube_back();
    fill_line_x(0,3);
    wait_for_new_frames(2);
    shift_cube_back();
    fill_line_x(0,2);
    wait_for_new_frames(2);
    shift_cube_back();
    fill_line_x(0,1);
    wait_for_new_frames(2);
    shift_cube_back();
  }
  fill_line_x(0,0);
  wait_for_new_frames(2);
  shift_cube_back();
  wait_for_new_frames(2);
  shift_cube_back();
  wait_for_new_frames(2);
  shift_cube_back();
  wait_for_new_frames(2);
  
  // Shifting planes
  clear_cube();
  fill_plane_xy(0);
  fill_plane_xz(0);
  fill_plane_yz(0);
  wait_for_new_frames(2);
  for (times = 0; times < 2; times++) {
    for (i = 0; i < 3; i++) {
      shift_cube_up();
      fill_plane_xz(0);
      fill_plane_yz(0);
      wait_for_new_frames(2);
    }
    for (i = 0; i < 3; i++) {
      shift_cube_left();
      fill_plane_xy(3);
      fill_plane_xz(0);
      wait_for_new_frames(2);
    }
    for (i = 0; i < 3; i++) {
      shift_cube_back();
      fill_plane_xy(3);
      fill_plane_yz(3);
      wait_for_new_frames(2);
    }
    for (i = 0; i < 3; i++) {
      shift_cube_down();
      fill_plane_xz(3);
      fill_plane_yz(3);
      wait_for_new_frames(2);
    }
    for (i = 0; i < 3; i++) {
      shift_cube_right();
      fill_plane_xy(0);
      fill_plane_xz(3);
      wait_for_new_frames(2);
    }
    for (i = 0; i < 3; i++) {
      shift_cube_front();
      fill_plane_xy(0);
      fill_plane_yz(0);
      wait_for_new_frames(2);
    }
  }
  

}

