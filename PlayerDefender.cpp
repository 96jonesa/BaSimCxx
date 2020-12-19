//
// Created by Andy on 12/18/2020.
//

#include <cmath>
#include <string>
#include <vector>

#include "PlayerDefender.h"
#include "Item.h"

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
    // TODO
  }
}

void PlayerDefender::PathFind(int dest_x, int dest_y) {

}

bool PlayerDefender::IsInEastRepairRange(GameMap &game_map) const {
  return std::abs(x_ - EAST_TRAP_X) + std::abs(y - EAST_TRAP_Y) < 2;
}

bool PlayerDefender::IsInWestRepairRange(GameMap &game_map) const {
  return std::abs(x_ - WEST_TRAP_X) + std::abs(y - WEST_TRAP_Y) < 2;
}