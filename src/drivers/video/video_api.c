#include <core/core-logging.h>
#include <resources/psf2/psf2.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "video_api.h"

video_driver_t video_driver;

void video_clear(uint32_t color)
{
  uint32_t* buffer = video_driver.video_buffer;

  if (buffer == NULL) {
    return;
  }

  uint32_t buffer_size = video_driver.video_width * video_driver.video_height;

  for (uint32_t i = 0; i < buffer_size; i++) {
    buffer[i] = color;
  }
}

void video_clear_region(uint32_t color,
                        int32_t x,
                        int32_t y,
                        int32_t width,
                        int32_t height)
{
  if (width < 0 || height < 0) {
    return;
  }

  uint32_t* buffer = video_driver.video_buffer;

  if (buffer == NULL) {
    return;
  }

  for (int32_t y_pos = 0, y_index = y * video_driver.video_width;
       y_pos < height;
       y_pos++, y_index += video_driver.video_width) {

    if (y < 0 && y + y_pos < 0) {
      continue;
    }

    if (y + y_pos >= video_driver.video_height) {
      break;
    }

    for (int32_t x_pos = 0, x_index = x + y_index; x_pos < width;
         x_pos++, x_index++) {

      if (x < 0 && x + x_pos < 0) {
        continue;
      }

      if (x + x_pos >= video_driver.video_width) {
        break;
      }

      buffer[x_index] = color;
    }
  }
}

void video_blit(uint32_t* src_buffer,
                int32_t src_x,
                int32_t src_y,
                int32_t src_width,
                int32_t src_height,
                int32_t src_buffer_width,
                int32_t src_buffer_height,
                int32_t dst_x,
                int32_t dst_y)
{
  if (src_width < 0 || src_height < 0 || src_buffer_width < 0 ||
      src_buffer_height < 0 || src_x + src_width > src_buffer_width ||
      src_y + src_height > src_buffer_height) {
    return;
  }

  uint32_t* buffer = video_driver.video_buffer;

  if (buffer == NULL) {
    return;
  }

  for (int32_t y_pos = 0,
               y_index = dst_y * video_driver.video_width,
               src_y_index = src_y * src_buffer_width;
       y_pos < src_height;
       y_pos++,
               y_index += video_driver.video_width,
               src_y_index += src_buffer_width) {

    if (dst_y < 0 && dst_y + y_pos < 0) {
      continue;
    }

    if (dst_y + y_pos >= video_driver.video_height) {
      break;
    }

    for (int32_t x_pos = 0,
                 x_index = dst_x + y_index,
                 src_x_index = src_y_index + src_x;
         x_pos < src_width;
         x_pos++, x_index++) {

      if (dst_x < 0 && dst_x + x_pos < 0) {
        continue;
      }

      if (dst_x + x_pos >= video_driver.video_width) {
        break;
      }

      buffer[x_index] = src_buffer[src_x_index + x_pos];
    }
  }
}

void video_swap_buffers()
{
  if (video_driver.swap_buffers != NULL) {
    video_driver.swap_buffers();
  }
}