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

#define PIN_DATA_1      2
#define PIN_DATA_2      3
#define PIN_CLK         4
#define PIN_LAYER_1_EN  A0
#define PIN_LAYER_2_EN  A1
#define PIN_LAYER_3_EN  A2
#define PIN_LAYER_4_EN  A3

// Variable to store the led cube
volatile uint8_t ledcube[4][4] = {
  {0b00000010, 0b00000000, 0b00000000, 0b00000110},
  {0b00001111, 0b00000000, 0b00000000, 0b00000101},
  {0b00001010, 0b00000000, 0b00000000, 0b00001111},
  {0b00000110, 0b00000000, 0b00000000, 0b00000100}
};

volatile uint8_t frame_expired = 0;

void setup() {
  // put your setup code here, to run once:
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

  // Wait five second
  for (i = 0; i < 50; i++) {
    wait_for_new_frame();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t layer, i;
  
  // Spiral up animation
  wait_for_new_frame();
  clear_cube();
  for (layer = 0; layer < 4; layer++) {
    // Fill middle square
    wait_for_new_frame();
    ledcube[layer][1] |= 0b00000010;
    wait_for_new_frame();
    ledcube[layer][1] |= 0b00000100;
    wait_for_new_frame();
    ledcube[layer][2] |= 0b00000100;
    wait_for_new_frame();
    ledcube[layer][2] |= 0b00000010;
    // Fill outside square
    for (i = 2; i > 0; i--) {
      wait_for_new_frame();
      ledcube[layer][i] |= 0b00000001;
    }
    for (i = 0; i < 3; i++) {
      wait_for_new_frame();
      ledcube[layer][0] |= 0b00000001 << i;
    }
    for (i = 0; i < 3; i++) {
      wait_for_new_frame();
      ledcube[layer][i] |= 0b00001000;
    }
    for (i = 0; i < 4; i++) {
      wait_for_new_frame();
      ledcube[layer][3] |= 0b00001000 >> i;
    }
  }
  
  
}

void wait_for_new_frame(void) {
  while (!frame_expired) {
    __asm__ __volatile__("nop");
  }
  frame_expired = 0;
}

void clear_cube(void) {
  uint8_t i, j;
  // Clear the buffer
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      ledcube[i][j] = 0;
    }
  }
}

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

