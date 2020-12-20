//
// Created by Andy on 12/18/2020.
//

#include <string>
#include <cmath>

#include "GameMap.h"
#include "Item.h"

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

std::string GameMap::ToString() const {
  return "";
}

bool GameMap::IsWave10() const {
  return is_wave_10_;
}

void GameMap::AddItem(Item &item) {
  GetItemZone(item.get_x() >> 3, item.get_y() >> 3).push_back(item);
}

std::vector<Item> &GameMap::GetItemZone(int x_zone, int y_zone) const {
  return item_zones_[x_zone + y_zone * item_zones_width_];
}

int GameMap::GetTileFlag(int x, int y) const {
  return layout_[x + y * width_tiles_];
}

bool GameMap::CanMoveEast(int x, int y) const {
  return (GetTileFlag(x + 1, y) & (MOVE_WEST_MASK | MOVE_FULL_MASK)) == 0;
}

bool GameMap::CanMoveWest(int x, int y) const {
  return (GetTileFlag(x - 1, y) & (MOVE_EAST_MASK | MOVE_FULL_MASK)) == 0;
}

bool GameMap::CanMoveNorth(int x, int y) const {
  return (GetTileFlag(x, y + 1) & (MOVE_SOUTH_MASK | MOVE_FULL_MASK)) == 0;
}

bool GameMap::CanMoveSouth(int x, int y) const {
  return (GetTileFlag(x, y - 1) & (MOVE_NORTH_MASK | MOVE_FULL_MASK)) == 0;
}

bool GameMap::HasLineOfSight(int x1, int y1, int x2, int y2) const {
  int dx = x2 - x1;
  int dx_abs = std::abs(dx);
  int dy = y2 - y1;
  int dy_abs = std::abs(dy);

  if (dx_abs > dy_abs) {
    int x_tile = x1;
    int y = y1 << 16;
    int slope = std::trunc((1.0 * (dy << 16)) / dx_abs);
    int x_inc;
    int x_mask;

    if (dx > 0) {
      x_inc = 1;
      x_mask = LOS_WEST_MASK | LOS_FULL_MASK;
    } else {
      x_inc = -1;
      x_mask = LOS_EAST_MASK | LOS_FULL_MASK;
    }

    int y_mask;
    y += 0x8000;

    if (dy < 0) {
      y--;
      y_mask = LOS_NORTH_MASK | LOS_FULL_MASK;
    } else {
      y_mask = LOS_SOUTH_MASK | LOS_FULL_MASK;
    }

    while (x_tile != x2) {
      x_tile += x_inc;
      int y_tile = y >> 16;

      if ((GetTileFlag(x_tile, y_tile) & x_mask) != 0) {
        return false;
      }

      y += slope;
      int new_y_tile = y >> 16;

      if ((new_y_tile != y_tile) && ((GetTileFlag(x_tile, new_y_tile) & y_mask) != 0)) {
        return false;
      }
    }
  } else {
    int y_tile = y1;
    int x = x1 << 16;
    int slope = std::trunc((1.0 * (dx << 16)) / dy_abs);
    int y_inc;
    int y_mask;

    if (dy > 0) {
      y_inc = 1;
      y_mask = LOS_SOUTH_MASK | LOS_FULL_MASK;
    } else {
      y_inc = -1;
      y_mask = LOS_NORTH_MASK | LOS_FULL_MASK;
    }

    int x_mask;
    x += 0x8000;

    if (dx < 0) {
      x--;
      x_mask = LOS_EAST_MASK | LOS_FULL_MASK;
    } else {
      x_mask = LOS_WEST_MASK | LOS_FULL_MASK;
    }

    while (y_tile != y2) {
      y_tile += y_inc;
      int x_tile = x >> 16;
      if ((GetTileFlag(x_tile, y_tile) & y_mask) != 0) {
        return false;
      }

      x += slope;
      int new_x_tile = x >> 16;

      if ((new_x_tile != x_tile) && ((GetTileFlag(new_x_tile, y_tile) & x_mask) != 0)) {
        return false;
      }
    }
  }

  return true;
}
