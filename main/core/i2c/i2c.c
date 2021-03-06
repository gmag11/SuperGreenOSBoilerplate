/*
 * Copyright (C) 2018  SuperGreenLab <towelie@supergreenlab.com>
 * Author: Constantin Clauzel <constantin.clauzel@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include "i2c.h"
#include "driver/i2c.h"

#include "../kv/kv.h"
#include "../log/log.h"


#define MASTER_TX_BUF_DISABLE  0
#define MASTER_RX_BUF_DISABLE  0
#define MASTER_FREQ_HZ         100000

/*static void print_scan() {
  int i;
  esp_err_t espRc;
  printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
  printf("00:         ");
  for (i=3; i< 0x78; i++) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, 1
    i2c_master_stop(cmd);

    espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
    if (i%16 == 0) {
      printf("\n%.2x:", i);
    }
    if (espRc == 0) {
      printf(" %.2x", i);
    } else {
      printf(" --");
    }
    i2c_cmd_link_delete(cmd);
  }
  printf("\n");
}*/

void i2c_task(void *param) {
  int sda = get_i2c_sda();
  int scl = get_i2c_scl();
  while(true) {

    vTaskDelay(2000 / portTICK_RATE_MS);
  }
}

void init_i2c() {
  int sda = get_i2c_sda();
  int scl = get_i2c_scl();
  // Call `init` driver methods

  xTaskCreate(i2c_task, "I2C", 4096, NULL, 10, NULL);
}

static bool i2c_started = false;

void start_i2c() {
  if (i2c_started) return;

  int sda = get_i2c_sda();
  int scl = get_i2c_scl();

  i2c_started = true;
  i2c_config_t conf;
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = sda;
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  conf.scl_io_num = scl;
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  conf.master.clk_speed = MASTER_FREQ_HZ;
  i2c_param_config(I2C_NUM_0, &conf);
  i2c_driver_install(I2C_NUM_0, conf.mode,
      MASTER_RX_BUF_DISABLE,
      MASTER_TX_BUF_DISABLE, 0);
}

void stop_i2c() {
  if (!i2c_started) return;
  i2c_started = false;
  i2c_driver_delete(I2C_NUM_0);
}
