#include "ledcube.h"
#include "arduino.h"
#include "stdint.h"

#define PIN_DATA_1      2
#define PIN_DATA_2      3
#define PIN_CLK         4
#define PIN_LAYER_1_EN  A0
#define PIN_LAYER_2_EN  A1
#define PIN_LAYER_3_EN  A2
#define PIN_LAYER_4_EN  A3


// Variable to store the led cube
static volatile uint8_t ledcube[4][4] = {
  {0b00000010, 0b00000000, 0b00000000, 0b00000110},
  {0b00001111, 0b00000000, 0b00000000, 0b00000101},
  {0b00001010, 0b00000000, 0b00000000, 0b00001111},
  {0b00000110, 0b00000000, 0b00000000, 0b00000100}
};

static volatile uint8_t frame_expired = 0;

// Interupt routine. This functions controls all the led outputs.
ISR(TIMER1_COMPA_vect) {
  static uint8_t layer = 0, frame_counter = 0;
  uint8_t i;

  // Increase the layer
  layer++;
  if (layer == 4) {
    layer = 0;
  }
  // Turn off all layers. Leds will flash if not turned off
  digitalWrite(PIN_LAYER_1_EN, LOW);
  digitalWrite(PIN_LAYER_2_EN, LOW);
  digitalWrite(PIN_LAYER_3_EN, LOW);
  digitalWrite(PIN_LAYER_4_EN, LOW);
  // Write new data to the shift registers
  for (i = 0; i < 4; i++) {
    // Set data pins
    if (ledcube[layer][2] & (1 << i)) {
      digitalWrite(PIN_DATA_1, HIGH);
    } else {
      digitalWrite(PIN_DATA_1, LOW);
    }
    if (ledcube[layer][0] & (1 << i)) {
      digitalWrite(PIN_DATA_2, HIGH);
    } else {
      digitalWrite(PIN_DATA_2, LOW);
    }
    // Toggle clock
    digitalWrite(PIN_CLK, HIGH);
    digitalWrite(PIN_CLK, LOW);
  }
  for (i = 0; i < 4; i++) {
    // Set data pins
    if (ledcube[layer][3] & (1 << i)) {
      digitalWrite(PIN_DATA_1, HIGH);
    } else {
      digitalWrite(PIN_DATA_1, LOW);
    }
    if (ledcube[layer][1] & (1 << i)) {
      digitalWrite(PIN_DATA_2, HIGH);
    } else {
      digitalWrite(PIN_DATA_2, LOW);
    }
    // Toggle clock
    digitalWrite(PIN_CLK, HIGH);
    digitalWrite(PIN_CLK, LOW);
  }
  // Enable the new layer
  if (layer == 0) {
    digitalWrite(PIN_LAYER_1_EN, HIGH);
  } else if (layer == 1) {
    digitalWrite(PIN_LAYER_2_EN, HIGH);
  } else if (layer == 2) {
    digitalWrite(PIN_LAYER_3_EN, HIGH);
  } else if (layer == 3) {
    digitalWrite(PIN_LAYER_4_EN, HIGH);
  }

  // Frame counter
  // Trigger new frame at 10 frames/second
  frame_counter++;
  if (frame_counter == 24) {
    frame_counter = 0;
    frame_expired = 1;
  }
}


/*********    GENERAL FUNCTIONS    *********/

void ledcube_init(void) {
  uint8_t i;
  
  pinMode(PIN_DATA_1, OUTPUT);
  pinMode(PIN_DATA_2, OUTPUT);
  pinMode(PIN_CLK, OUTPUT);
  pinMode(PIN_LAYER_1_EN, OUTPUT);
  pinMode(PIN_LAYER_2_EN, OUTPUT);
  pinMode(PIN_LAYER_3_EN, OUTPUT);
  pinMode(PIN_LAYER_4_EN, OUTPUT);
  // The builtin led looks nice with some casings
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Use timer1 for interrupt routine. At each interrupt the shift registers are
  // set and the cube updated. The frequency at which this happens must be greater
  // then 60Hz. Because there are four layers the routine must be called at 240Hz.
  // Timer1 is used so the functions delay() can still be used normally.
  cli();//stop interrupts
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0;
  // set timer count for 1khz increments
  OCR1A = 259;
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bit for 256 prescaler
  TCCR1B |= (1 << CS12);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();//allow interrupts

  // Wait one second
  wait_for_new_frames(5);
}

void wait_for_new_frames(uint8_t frames) {
  uint8_t i;
  for (i = 0; i < frames; i++) {
    while (!frame_expired) {
      __asm__ __volatile__("nop");
    }
    frame_expired = 0;
  }
}


/*********    CUBE FUNCTION    *********/

void clear_cube(void) {
  uint8_t i, j;
  // Clear the buffer
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      ledcube[i][j] = 0;
    }
  }
}

void fill_cube(void) {
  uint8_t i, j;
  // Fill the buffer
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      ledcube[i][j] = 0b1111;
    }
  }
}

void set_cube(cube_t cube) {
  uint8_t i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      ledcube[i][j] = cube[i][j];
    }
  }
}


/*********    PLANE FUNCTIONS     *********/


void fill_plane_xy(uint8_t z) {
  uint8_t i;
  for (i = 0; i < 4; i++) {
    ledcube[z][i] = 0b1111;
  }
}

void fill_plane_xz(uint8_t y) {
  uint8_t i;
  for (i = 0; i < 4; i++) {
    ledcube[i][y] = 0b1111;
  }
}

void fill_plane_yz(uint8_t x) {
  uint8_t i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      ledcube[i][j] = ledcube[i][j] | (0b1 << x);
    }
  }
}

void clear_plane_xy(uint8_t z) {
  uint8_t i;
  for (i = 0; i < 4; i++) {
    ledcube[z][i] = 0b0000;
  }
}

void clear_plane_xz(uint8_t y) {
  uint8_t i;
  for (i = 0; i < 4; i++) {
    ledcube[i][y] = 0b0000;
  }
}

void clear_plane_yz(uint8_t x) {
  uint8_t i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      ledcube[i][j] = ledcube[i][j] & ~(0b1 << x);
    }
  }
}


/*********    LINE FUNCTIONS    *********/

void fill_line_x(uint8_t y, uint8_t z) {
  ledcube[z][y] = 0b1111;
}

void fill_line_y(uint8_t x, uint8_t z) {
  uint8_t i;
  for (i = 0; i < 4; i++) {
    ledcube[z][i] = ledcube[z][i] | (0b1 << x);
  }
}

void fill_line_z(uint8_t x, uint8_t y) {
  uint8_t i;
  for (i = 0; i < 4; i++) {
    ledcube[y][i] = ledcube[y][i] | (0b1 << x);
  }
}

void clear_line_x(uint8_t y, uint8_t z) {
  ledcube[z][y] = 0b1111;
}

void clear_line_y(uint8_t x, uint8_t z) {
  uint8_t i;
  for (i = 0; i < 4; i++) {
    ledcube[z][i] = ledcube[z][i] & ~(0b1 << x);
  }
}

void clear_line_z(uint8_t x, uint8_t y) {
  uint8_t i;
  for (i = 0; i < 4; i++) {
    ledcube[i][y] = ledcube[i][y] & ~(0b1 << x);
  }
}


/*********    SHIFTING FUNCTIONS    *********/

void shift_cube_up(void) {
  uint8_t i;
  for (i = 3; i > 0; i--) {
    ledcube[i][0] = ledcube[i-1][0];
    ledcube[i][1] = ledcube[i-1][1];
    ledcube[i][2] = ledcube[i-1][2];
    ledcube[i][3] = ledcube[i-1][3];
  }
  ledcube[0][0] = 0;
  ledcube[0][1] = 0;
  ledcube[0][2] = 0;
  ledcube[0][3] = 0;
}

void shift_cube_down(void) {
  uint8_t i;
  for (i = 0; i < 3; i++) {
    ledcube[i][0] = ledcube[i+1][0];
    ledcube[i][1] = ledcube[i+1][1];
    ledcube[i][2] = ledcube[i+1][2];
    ledcube[i][3] = ledcube[i+1][3];
  }
  ledcube[3][0] = 0;
  ledcube[3][1] = 0;
  ledcube[3][2] = 0;
  ledcube[3][3] = 0;
}

void shift_cube_right(void) {
  uint8_t i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      ledcube[i][j] = (ledcube[i][j] >> 1) & 0b00001111;
    }
  }
}

void shift_cube_left(void) {
  uint8_t i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      ledcube[i][j] = (ledcube[i][j] << 1) & 0b00001111;
    }
  }
}

void shift_cube_front(void) {
  uint8_t i;
  for (i = 0; i < 3; i++) {
    ledcube[0][i] = ledcube[0][i+1];
    ledcube[1][i] = ledcube[1][i+1];
    ledcube[2][i] = ledcube[2][i+1];
    ledcube[3][i] = ledcube[3][i+1];
  }
  ledcube[0][3] = 0;
  ledcube[1][3] = 0;
  ledcube[2][3] = 0;
  ledcube[3][3] = 0;
}


void shift_cube_back(void) {
  uint8_t i;
  for (i = 3; i > 0; i--) {
    ledcube[0][i] = ledcube[0][i-1];
    ledcube[1][i] = ledcube[1][i-1];
    ledcube[2][i] = ledcube[2][i-1];
    ledcube[3][i] = ledcube[3][i-1];
  }
  ledcube[0][0] = 0;
  ledcube[1][0] = 0;
  ledcube[2][0] = 0;
  ledcube[3][0] = 0;
}

