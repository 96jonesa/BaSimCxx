//
// Created by Andy on 12/18/2020.
//

#include <string>
#include <cmath>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>

#include "GameMap.h"
#include "Item.h"
#include "Player.h"

GameMap::GameMap(bool is_wave_10, int max_runners_alive, int total_runners, std::vector<std::string> runner_movements)
    : is_wave_10_(is_wave_10),
      max_runners_alive_(max_runners_alive),
      total_runners_(total_runners),
      runner_movements_(runner_movements.begin(), runner_movements.end()) {
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
     item_zones_[x_zone + item_zones_width_ * y_zone] = std::shared_ptr< std::vector< std::shared_ptr<Item> > >(new std::vector< std::shared_ptr<Item> >());
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

void GameMap::AddItem(std::shared_ptr<Item> item) {
  GetItemZone(item->get_x() >> 3, item->get_y() >> 3).push_back(item);
}

std::vector< std::shared_ptr<Item> > &GameMap::GetItemZone(int x_zone, int y_zone) {
  return *item_zones_[x_zone + y_zone * item_zones_width_];
}

int GameMap::GetTileFlag(int x, int y) {
  return layout_[x + y * width_tiles_];
}

bool GameMap::CanMoveEast(int x, int y) {
  return (GetTileFlag(x + 1, y) & (MOVE_WEST_MASK | MOVE_FULL_MASK)) == 0;
}

bool GameMap::CanMoveWest(int x, int y) {
  return (GetTileFlag(x - 1, y) & (MOVE_EAST_MASK | MOVE_FULL_MASK)) == 0;
}

bool GameMap::CanMoveNorth(int x, int y) {
  return (GetTileFlag(x, y + 1) & (MOVE_SOUTH_MASK | MOVE_FULL_MASK)) == 0;
}

bool GameMap::CanMoveSouth(int x, int y) {
  return (GetTileFlag(x, y - 1) & (MOVE_NORTH_MASK | MOVE_FULL_MASK)) == 0;
}

bool GameMap::HasLineOfSight(int x1, int y1, int x2, int y2) {
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

bool GameMap::TileBlocksPenance(int x, int y) {
  for (std::shared_ptr<Player> &player : players_) {
    if ((x == player->get_x()) && (y = player->get_y())) {
      return true;
    }
  }

  if (y == 22) {
    if ((x >= 20) && (x <= 22)) {
      return true;
    }

    if ((!is_wave_10_) && (x >= 39) && (x <= 41)) {
      return true;
    }
  } else if ((x == 46) && (y >= 9) && (y <= 12)) {
    return true;
  } else if ((!is_wave_10_) && (x == 27) && (y == 24)) {
    return true;
  }

  return false;
}

void GameMap::Tick() {
  tick_counter_++;
  runners_to_remove_.clear();

  for (int i = 0; i < runners_.size(); i++) {
    runners_[i]->Tick(*this);
  }

  for (int i = 0; i < runners_to_remove_.size(); i++) {
    std::shared_ptr<PenanceRunner> runner = runners_to_remove_[i];
    int index;

    for (int j = 0; j < runners_.size(); j++) {
      if (runner->get_id() == runners_[i]->get_id()) {
        index = i;
        break;
      }
    }

    runners_.erase(runners_.begin() + index);
  }

  // hammer and logs respawn
  if ((tick_counter_ > 1) && (tick_counter_ % 10 == 1)) {
    nw_logs_state_ = true;
    se_logs_state_ = true;
    hammer_state_ = true;
  }

  // defender food changes
  if ((tick_counter_ > 2) && (tick_counter_ % 50 == 2)) {
    std::srand(std::time(nullptr));
    int random_number = std::rand() / ((RAND_MAX + 1u)/2);

    if (defender_food_ == 't') {
      if (random_number == 0) {
        defender_food_ = 'c';
      } else {
        defender_food_ = 'w';
      }
    } else if (defender_food_ == 'c') {
      if (random_number == 0) {
        defender_food_ = 'w';
      } else {
        defender_food_ = 't';
      }
    } else {
      if (random_number == 0) {
        defender_food_ = 't';
      } else {
        defender_food_ = 'c';
      }
    }
  }

  if ((tick_counter_ > 1) &&
      (tick_counter_ % 10 == 1) &&
      (runners_alive_ < max_runners_alive_) &&
      (runners_killed_ + runners_alive_ < total_runners_)) {
    std::string movements = "";

    if (runner_movements_.size() > runner_movements_index_) {
      movements = runner_movements_[runner_movements_index_];
      runner_movements_index_++;
    }

    std::shared_ptr<PenanceRunner> runner = std::make_shared<PenanceRunner>(WAVE1_RUNNER_SPAWN_X, WAVE1_RUNNER_SPAWN_Y, movements);
    runners_.push_back(runner);
    runners_alive_++;
  }
}
