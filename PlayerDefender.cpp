//
// Created by Andy on 12/18/2020.
//

#include <cmath>
#include <string>
#include <vector>

#include "PlayerDefender.h"
#include "Player.h"
#include "Item.h"
#include "GameMap.h"

std::string PlayerDefender::ToString() const {
  return "";
}

void PlayerDefender::Tick(GameMap &game_map) {
  stand_still_counter_++;

  int prev_x = x_;
  int prev_y = y_;

  if (repair_ticks_remaining_ > 0) {
    if (repair_ticks_remaining_ == 1) {
      num_logs_--;

      if (IsInEastRepairRange(game_map)) {
        game_map.set_east_trap_state(2);
      } else {
        game_map.set_west_trap_state(2);
      }
    }

    repair_ticks_remaining_--;
    path_queue_pos_ = 0;
    picking_up_food_ = 'n';
  } else if (picking_up_food_ != 'n') {
    std::vector<Item> &item_zone = game_map.GetItemZone(x_ >> 3, x_ >> 3);

    for (int i = 0; i < item_zone.size(); i++) {
      Item item = item_zone[i];
      if ((x_ == item.get_x()) && (y_ == item.get_y()) && (item.get_type() == picking_up_food_)) {
        item_zone.erase(item_zone.begin() + i);

        if (picking_up_food_ == 't') {
          num_tofu_++;
        } else if (picking_up_food_ == 'c') {
          num_crackers_++;
        } else {
          num_worms_++;
        }

        break;
      }
    }

    picking_up_food_ = 'n';
    path_queue_pos_ = 0;
  } else if (picking_up_logs_) {
    bool wave_is_10 = game_map.IsWave10();

    if ((wave_is_10 && (x_ == WAVE10_NW_LOGS_X) && (y_ == WAVE10_NW_LOGS_Y)) || (!wave_is_10 && (x_ == WAVE1_NW_LOGS_X) && (y_ == WAVE1_NW_LOGS_Y))) {
      if (game_map.get_nw_logs_state()) {
        num_logs_++;
        game_map.set_nw_logs_state(false);
      }
    } else if ((wave_is_10 && (x_ == WAVE10_SE_LOGS_X) && (y_ == WAVE10_SE_LOGS_Y)) || (!wave_is_10 && (x_ == WAVE1_SE_LOGS_X) && (y_ == WAVE1_SE_LOGS_Y))) {
      if (game_map.get_se_logs_state()) {
        num_logs_++;
        game_map.set_se_logs_state(false);
      }
    }

    picking_up_logs_ = false;
  } else if (picking_up_hammer_) {
    if (game_map.get_hammer_state() && (x_ == HAMMER_X) && (y_ = HAMMER_Y)) {
      has_hammer_ = true;
      game_map.set_hammer_state(false);
    }

    picking_up_hammer_ = false;
  } else if (path_queue_pos_ > 0) {
    path_queue_pos_--;
    x_ = path_queue_x_[path_queue_pos_];
    y_ = path_queue_y_[path_queue_pos_];

    if (path_queue_pos_ > 0) {
      path_queue_pos_--;
      x_ = path_queue_x_[path_queue_pos_];
      y_ = path_queue_y_[path_queue_pos_];
    }
  }

  if ((prev_x != x_) || (prev_y != y_)) {
    stand_still_counter_ = 0;
  }
}

void PlayerDefender::PathFind(GameMap &game_map, int dest_x, int dest_y) {
  std::vector<int> new_shortest_distances;
  shortest_distances_ = new_shortest_distances;

  std::vector<int> new_way_points;
  way_points_ = new_way_points;

  int width_tiles = game_map.get_width_tiles();
  int height_tiles = game_map.get_height_tiles();
  std::vector<int> layout = game_map.get_layout();

  for (int i = 0; i < width_tiles * height_tiles; i++) {
    shortest_distances_.push_back(99999999);
    way_points_.push_back(0);
  }

  way_points_[x_ + y_ * width_tiles] = 99;
  shortest_distances_[x_ + y_ * width_tiles] = 0;
  path_queue_pos_ = 0;
  int path_queue_end = 0;
  path_queue_x_[path_queue_end] = x_;
  path_queue_y_[path_queue_end] = y_;
  path_queue_end++;

  int current_x;
  int current_y;
  bool found_destination = false;

  while (path_queue_pos_ != path_queue_end) {
    current_x = path_queue_x_[path_queue_pos_];
    current_y = path_queue_y_[path_queue_pos_];
    path_queue_pos_++;

    if ((current_x == dest_x) && (current_y == dest_y)) {
      found_destination = true;
      break;
    }

    int new_distance = shortest_distances_[current_x + current_y * width_tiles] + 1;
    int index = current_x - 1 + current_y * width_tiles;

    if ((current_x > 0) && (way_points_[index] == 0) && ((layout[index] & 19136776) == 0)) {
      path_queue_x_[path_queue_end] = current_x - 1;
      path_queue_y_[path_queue_end] = current_y;
      path_queue_end++;
      way_points_[index] = 2;
      shortest_distances_[index] = new_distance;
    }

    index = current_x + 1 + current_y * width_tiles;

    if ((current_x < width_tiles - 1) && (way_points_[index] == 0) && ((layout[index] & 19136896) == 0)) {
      path_queue_x_[path_queue_end] = current_x + 1;
      path_queue_y_[path_queue_end] = current_y;
      path_queue_end++;
      way_points_[index] = 8;
      shortest_distances_[index] = new_distance;
    }

    index = current_x + (current_y - 1) * width_tiles;

    if ((current_y > 0) && (way_points_[index] == 0) && ((layout[index] & 19136770) == 0)) {
      path_queue_x_[path_queue_end] = current_x;
      path_queue_y_[path_queue_end] = current_y - 1;
      path_queue_end++;
      way_points_[index] = 1;
      shortest_distances_[index] = new_distance;
    }

    index = current_x + (current_y + 1) * width_tiles;

    if ((current_y < height_tiles - 1) && (way_points_[index] == 0) && ((layout[index] & 19136800) == 0)) {
      path_queue_x_[path_queue_end] = current_x;
      path_queue_y_[path_queue_end] = current_y + 1;
      path_queue_end++;
      way_points_[index] = 4;
      shortest_distances_[index] = new_distance;
    }

    index = current_x - 1 + (current_y - 1) * width_tiles;

    if ((current_x > 0) &&
        (current_y > 0) &&
        (way_points_[index] == 0) &&
        ((layout[index] & 19136782) == 0) &&
        ((layout[current_x - 1 + current_y * width_tiles] & 19136776) == 0) &&
        ((layout[current_x + (current_y - 1) * width_tiles] & 19136770) == 0)) {
      path_queue_x_[path_queue_end] = current_x - 1;
      path_queue_y_[path_queue_end] = current_y - 1;
      path_queue_end++;
      way_points_[index] = 3;
      shortest_distances_[index] = new_distance;
    }

    index = current_x + 1 + (current_y - 1) * width_tiles;

    if ((current_x < width_tiles - 1) &&
        (current_y > 0) &&
        (way_points_[index] == 0) &&
        ((layout[index] & 19136899) == 0) &&
        ((layout[current_x + 1 + current_y * width_tiles] & 19136896) == 0) &&
        ((layout[current_x + (current_y - 1) * width_tiles] & 19136770) == 0)) {
      path_queue_x_[path_queue_end] = current_x + 1;
      path_queue_y_[path_queue_end] = current_y - 1;
      path_queue_end++;
      way_points_[index] = 9;
      shortest_distances_[index] = new_distance;
    }

    index = current_x - 1 + (current_y + 1) * width_tiles;

    if ((current_x > 0) &&
        (current_y < height_tiles - 1) &&
        (way_points_[index] == 0) &&
        ((layout[index] & 19136824) == 0) &&
        ((layout[current_x - 1 + current_y * width_tiles] & 19136776) == 0) &&
        ((layout[current_x + (current_y + 1) * width_tiles] & 19136800) == 0)) {
      path_queue_x_[path_queue_end] = current_x - 1;
      path_queue_y_[path_queue_end] = current_y + 1;
      path_queue_end++;
      way_points_[index] = 6;
      shortest_distances_[index] = new_distance;
    }

    index = current_x + 1 + (current_y + 1) * width_tiles;

    if ((current_x < width_tiles - 1) &&
        (current_y < height_tiles - 1) &&
        (way_points_[index] == 0) &&
        ((layout[index] & 19136992) == 0) &&
        ((layout[current_x + 1 + current_y * width_tiles] & 19136896) == 0) &&
        ((layout[current_x + (current_y + 1) * width_tiles] & 19136800) == 0)) {
      path_queue_x_[path_queue_end] = current_x + 1;
      path_queue_y_[path_queue_end] = current_y + 1;
      path_queue_end++;
      way_points_[index] = 12;
      shortest_distances_[index] = new_distance;
    }
  }

  if (!found_destination) {
    int best_distance_start = 0x7FFFFFFF;
    int best_distance_end = 0x7FFFFFFF;
    int deviation = 10;

    for (int x = dest_x - deviation; x <= dest_x + deviation; x++) {
      for (int y = dest_y - deviation; y <= dest_y + deviation; y++) {
        if ((x >= 0) && (y >= 0) && (x < width_tiles) && (y < height_tiles)) {
          int distance_start = shortest_distances_[x + y * width_tiles];

          if (distance_start < 100) {
            int dx = dest_x - x;
            int dy = dest_y - y;
            int distance_end = dx * dx + dy * dy;

            if ((distance_end < best_distance_end) ||
                ((distance_end == best_distance_end) && (distance_start < best_distance_start))) {
              best_distance_start = distance_start;
              best_distance_end = distance_end;
              current_x = x;
              current_y = y;
              found_destination = true;
            }
          }
        }
      }
    }

    if (!found_destination) {
      path_queue_pos_ = 0;
      return;
    }
  }

  path_queue_pos_ = 0;

  while ((current_x != x_) || (current_y != y_)) {
    int way_point = way_points_[current_x + current_y * width_tiles];
    path_queue_x_[path_queue_pos_] = current_x;
    path_queue_y_[path_queue_pos_] = current_y;
    path_queue_pos_++;

    if ((way_point & 2) != 0) {
      current_x++;
    } else if ((way_point & 8) != 0) {
      current_x--;
    }

    if ((way_point & 1) != 0) {
      current_y++;
    } else if ((way_point & 4) != 0) {
      current_y--;
    }
  }
}

bool PlayerDefender::IsInEastRepairRange(GameMap &game_map) const {
  return std::abs(x_ - EAST_TRAP_X) + std::abs(y_ - EAST_TRAP_Y) < 2;
}

bool PlayerDefender::IsInWestRepairRange(GameMap &game_map) const {
  return std::abs(x_ - WEST_TRAP_X) + std::abs(y_ - WEST_TRAP_Y) < 2;
}
