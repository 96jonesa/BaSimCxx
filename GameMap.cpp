//
// Created by Andy on 12/18/2020.
//

#include "GameMap.h"

GameMap::GameMap(bool is_wave_10) : is_wave_10_(is_wave_10) {
  if (is_wave_10) {
    layout_ = WAVE_10_MAP;
    width_tiles_ = 64;
    height_tiles_ = 48;
  } else {
    layout_ = WAVE_1_9_MAP;
    width_tiles_ = 64;
    height_tiles_ = 48;
  }

  item_zones_width_ = 1 + ((width_tiles_ - 1) >> 3);
  item_zones_height_ = 1 + ((height_tiles_ - 1) >> 3);

  for (int x_zone = 0; x_zone < item_zones_width_; x_zone++) {
    for (int y_zone = 0; y_zone < item_zones_height_; y_zone++) {
      vector<Item> items;
      item_zones_[x_zone + item_zones_width_ * y_zone] = items;
    }
  }

  east_trap_state_ = 2;
  west_trap_state_ = 2;
}