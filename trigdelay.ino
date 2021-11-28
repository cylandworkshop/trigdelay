const int out_0 = 8; // left bottom
const int out_1 = 9; // right bottom
const int out_0_led = 5;
const int out_1_led = 6;

const int in_0_led = 12;
const int in_1_led = 4;

const int in_0 = 2;
const int in_1 = 3;

const int led_green = 7;

#define N 2048 // buffer size in samples
#define DELAY_TIME 2 // ms

uint8_t buffer_0[N / 8];
uint8_t buffer_1[N / 8];

uint32_t head_0 = 0;
uint32_t head_1 = 0;

bool trig_delay(uint8_t* buffer, uint32_t* head, bool input, uint32_t delay_time) {
    if(delay_time > N) delay_time = N;
    
    if(input) {
        buffer[*head / 8] |= (1 << (*head % 8));
    } else {
        buffer[*head / 8] &= ~(1 << (*head % 8));
    }

    size_t tail = (N + *head - delay_time) % N;
    bool output = (bool)(1 & (buffer[tail / 8] >> (tail % 8)));
    *head = *head + 1;
    if(*head > N - 1) *head = 0;

    // printf("%d %d\n", input, output);
    // printf("-%d %d\n", head, buffer[head / 8]);
    // printf("t = %d: %d\n", tail, buffer[tail / 8]);

    return output;
}

void setup() {
  pinMode(out_0, OUTPUT);
  pinMode(out_0_led, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(in_0_led, OUTPUT);

  pinMode(in_0, INPUT_PULLUP);
}

uint32_t delay_0_time = 200;
uint32_t delay_1_time = 200;

void loop() {
  // 0
  bool in_0_value = digitalRead(in_0);
  digitalWrite(in_0_led, in_0_value);

  delay_0_time = analogRead(A0);

  bool out_0_value = trig_delay(buffer_0, &head_0, in_0_value, delay_0_time / DELAY_TIME);

  digitalWrite(out_0, out_0_value);
  digitalWrite(out_0_led, out_0_value);

  // 1

  bool in_1_value = digitalRead(in_1);
  digitalWrite(in_1_led, in_1_value);

  delay_1_time = analogRead(A1);

  bool out_1_value = trig_delay(buffer_1, &head_1, in_1_value, delay_1_time / DELAY_TIME);

  digitalWrite(out_1, out_1_value);
  digitalWrite(out_1_led, out_1_value);

  delay(DELAY_TIME);
}
