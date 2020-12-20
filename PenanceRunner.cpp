//
// Created by Andy on 12/19/2020.
//

#include <string>
#include <algorithm>
#include <vector>
#include <memory>

#include "PenanceRunner.h"
#include "GameMap.h"
#include "Item.h"

PenanceRunner::PenanceRunner(int x, int y)
    : x_(x), y_(y), destination_x_(x), destination_y_(y), id_(next_id_++) { }

std::string PenanceRunner::ToString() const {
  return "";
}

void PenanceRunner::Tick(GameMap &game_map) {
  cycle_tick_++;

  if (cycle_tick_ > 10) {
    cycle_tick_ = 1;
  }

  stand_still_counter_++;

  if (despawn_countdown_ != -1) {
    despawn_countdown_--;

    if (despawn_countdown_ == 0) {
      std::vector< std::shared_ptr<PenanceRunner> > &runners_to_remove = game_map.get_runners_to_remove();
      runners_to_remove.push_back(std::shared_ptr<PenanceRunner>(this));

      if (!is_dying_) {
        game_map.set_runners_alive(game_map.get_runners_alive() - 1);
      } else {
        if (IsNearEastTrap(game_map)) {
          if (game_map.get_east_trap_state() > 0) {
            game_map.set_east_trap_state(game_map.get_east_trap_state() - 1);
          }
        }

        if (IsNearWestTrap(game_map)) {
          if (game_map.get_west_trap_state() > 0) {
            game_map.set_west_trap_state(game_map.get_west_trap_state() - 1);
          }
        }
      }
    }
  } else {
    if (!is_dying_) {
      if (cycle_tick_ == 1) {
        DoTick1();
      } else if (cycle_tick_ == 2) {
        DoTick2Or5();
      } else if (cycle_tick_ == 3) {
        DoTick3();
      } else if (cycle_tick_ == 4) {
        DoTick4();
      } else if (cycle_tick_ == 5) {
        DoTick2Or5();
      } else if (cycle_tick_ == 6) {
        DoTick6();
      } else if (cycle_tick_ == 7) {
        DoTick7To10();
      } else if (cycle_tick_ == 8) {
        DoTick7To10();
      } else if (cycle_tick_ == 9) {
        DoTick7To10();
      } else if (cycle_tick_ == 10) {
        DoTick7To10();
      }
    }

    if (is_dying_) {
      if (stand_still_counter_ > 2) {
        game_map.set_runners_killed(game_map.get_runners_killed() + 1);
        game_map.set_runners_alive(game_map.get_runners_alive() - 1);
        despawn_countdown_ = 2;
      }
    }
  }
}

void PenanceRunner::DoMovement(GameMap &game_map) {
  int start_x = x_;

  if (destination_x_ > start_x) {
    if ((!game_map.TileBlocksPenance(start_x + 1, y_)) && game_map.CanMoveEast(start_x, y_)) {
      x_++;
      stand_still_counter_ = 0;
    }
  } else if (destination_x_ < start_x) {
    if ((!game_map.TileBlocksPenance(start_x - 1, y_)) && game_map.CanMoveWest(start_x, y_)) {
      x_--;
      stand_still_counter_ = 0;
    }
  }
  if (destination_y_ > y_) {
    if ((!game_map.TileBlocksPenance(start_x, y_ + 1)) &&
        (!game_map.TileBlocksPenance(x_, y_ + 1)) &&
        game_map.CanMoveNorth(start_x, y_) &&
        game_map.CanMoveNorth(x_, y_)) {
      y_++;
      stand_still_counter_ = 0;
    }
  } else if (destination_y_ < y_) {
    if ((!game_map.TileBlocksPenance(start_x, y_ - 1)) &&
        (!game_map.TileBlocksPenance(x_, y_ - 1)) &&
        game_map.CanMoveSouth(start_x, y_) &&
        game_map.CanMoveSouth(x_, y_)) {
      y_--;
      stand_still_counter_ = 0;
    }
  }
}

void PenanceRunner::TryTargetFood(GameMap &game_map) {
  int x_zone = x_ >> 3;
  int y_zone = y_ >> 3;
  std::shared_ptr<Item> first_food_found(new Item());
  int end_x_zone = std::max(x_zone - 1, 0);
  int end_y_zone = std::max(y_zone - 1, 0);
  int item_zones_width = game_map.get_item_zones_width();
  int item_zones_height = game_map.get_item_zones_height();
  int runner_sniff_distance = game_map.get_runner_sniff_distance();

  for (int x = std::min(x_zone + 1, item_zones_width - 1); x >= end_x_zone; x--) {
    for (int y = std::min(y_zone + 1, item_zones_height - 1); y >= end_y_zone; y--) {
      std::vector< std::shared_ptr<Item> > &item_zone = game_map.GetItemZone(x, y);

      for (int food_index = item_zone.size() - 1; food_index >= 0; food_index--) {
        std::shared_ptr<Item> food = item_zone[food_index];

        if (!game_map.HasLineOfSight(x_, y_, food->get_x(), food->get_y())) {
          continue;
        }

        if (first_food_found->get_is_null()) {
          first_food_found = food;
        }

        if (std::max(std::abs(x_ - food->get_x()), std::abs(y_ - food->get_y())) <= runner_sniff_distance) {
          food_target_ = food;
          destination_x_ = first_food_found->get_x();
          destination_y_ = first_food_found->get_y();
          target_state_ = 0;
          return;
        }
      }
    }
  }
}

bool PenanceRunner::TryEatAndCheckTarget() {
  return true;
}

void PenanceRunner::CancelDestination() {

}

void PenanceRunner::SetDestinationBlughhhh() {

}

void PenanceRunner::SetDestinationRandomWalk() {

}

void PenanceRunner::DoTick1() {

}

void PenanceRunner::DoTick2Or5() {

}

void PenanceRunner::DoTick3() {

}

void PenanceRunner::DoTick4() {

}

void PenanceRunner::DoTick6() {

}

void PenanceRunner::DoTick7To10() {

}

bool PenanceRunner::IsNearEastTrap(GameMap &gameMap) const {
  return true;
}

bool PenanceRunner::IsNearWestTrap(GameMap &gameMap) const {
  return true;
}
