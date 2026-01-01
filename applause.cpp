#include <driver/i2s.h>

#define I2S_WS 25     // LRCL
#define I2S_SD 22     // DOUT
#define I2S_SCK 26    // BCLK

#define SAMPLE_RATE 16000
int iled = 0;
int led = 13;
void i2s_init() {
  i2s_config_t config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num  = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
}

void setup() {
  pinMode(led,OUTPUT);
  Serial.begin(115200);
  i2s_init();
  Serial.println("INMP441 test started...");
}

void loop() {
  int16_t buffer[1024];
  size_t bytesRead;

  // อ่านข้อมูลจาก INMP441
  i2s_read(I2S_NUM_0, buffer, sizeof(buffer), &bytesRead, portMAX_DELAY);

  long sum = 0;
  for (int i = 0; i < 1024; i++) {
    sum += abs(buffer[i]);
  }

  int amplitude = sum / 1024;

  Serial.println(amplitude);
  if (amplitude > 150 && iled == 0){
      digitalWrite(led,1);
      iled = iled + 1;
      delay(500);
  }else if(amplitude > 150 && iled == 1 ){
      digitalWrite(led,0);
      iled = iled - 1;
      delay(500);
  }   // แสดงค่าความดัง

  delay(50);                   // กัน Serial spam
}
