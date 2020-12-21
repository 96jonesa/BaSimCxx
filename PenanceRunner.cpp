//
// Created by Andy on 12/19/2020.
//

#include <string>
#include <algorithm>
#include <vector>
#include <memory>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "PenanceRunner.h"
#include "GameMap.h"
#include "Item.h"

PenanceRunner::PenanceRunner(int x, int y, std::string forced_movements)
    : x_(x), y_(y), destination_x_(x), destination_y_(y), forced_movements_(forced_movements), id_(next_id_++) {
}

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
      runners_to_remove.push_back(std::shared_ptr<PenanceRunner>(this));  // TODO: cant pass as shared_ptr because there is a free involved (double free)

      if (!is_dying_) {
        game_map.set_runners_alive(game_map.get_runners_alive() - 1);
      } else {
        if (IsNearEastTrap()) {
          if (game_map.get_east_trap_state() > 0) {
            game_map.set_east_trap_state(game_map.get_east_trap_state() - 1);
          }
        }

        if (IsNearWestTrap()) {
          if (game_map.get_west_trap_state() > 0) {
            game_map.set_west_trap_state(game_map.get_west_trap_state() - 1);
          }
        }
      }
    }
  } else {
    if (!is_dying_) {
      if (cycle_tick_ == 1) {
        DoTick1(game_map);
      } else if (cycle_tick_ == 2) {
        DoTick2Or5(game_map);
      } else if (cycle_tick_ == 3) {
        DoTick3(game_map);
      } else if (cycle_tick_ == 4) {
        DoTick4(game_map);
      } else if (cycle_tick_ == 5) {
        DoTick2Or5(game_map);
      } else if (cycle_tick_ == 6) {
        DoTick6(game_map);
      } else if (cycle_tick_ == 7) {
        DoTick7To10(game_map);
      } else if (cycle_tick_ == 8) {
        DoTick7To10(game_map);
      } else if (cycle_tick_ == 9) {
        DoTick7To10(game_map);
      } else if (cycle_tick_ == 10) {
        DoTick7To10(game_map);
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
  std::shared_ptr<Item> first_food_found;
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

        if (first_food_found == nullptr) {
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

bool PenanceRunner::TryEatAndCheckTarget(GameMap &game_map) {
  if (food_target_ != nullptr) {
    std::vector< std::shared_ptr<Item> > &item_zone = game_map.GetItemZone(food_target_->get_x() >> 3, food_target_->get_y() >> 3);
    int food_index = -1;

    for (std::size_t i = 0; i < item_zone.size(); i++) {
      std::shared_ptr<Item> &item = item_zone[i];
      if (item->get_id() == food_target_->get_id()) {
        food_index = i;
        break;
      }
    }

    if (food_index == -1) {
      food_target_ = nullptr;
      target_state_ = 0;
      return true;
    } else if ((x_ == food_target_->get_x()) && (y_ == food_target_->get_y())) {
      if (food_target_->get_is_good()) {
        if (IsNearEastTrap()) {
          if (game_map.get_east_trap_state() > 0) {
            is_dying_ = true;
          }
        } else if (IsNearWestTrap()) {
          if (game_map.get_west_trap_state() > 0) {
            is_dying_ = true;
          }
        }
      } else {
        blughhhh_countdown_ = 3;
        target_state_ = 0;

        if (cycle_tick_ > 5) {
          cycle_tick_ -= 5;
        }

        SetDestinationBlughhhh(game_map);
      }

      item_zone.erase(item_zone.begin() + food_index);
      return true;
    }
  }

  return false;
}

void PenanceRunner::CancelDestination() {
  destination_x_ = x_;
  destination_y_ = y_;
}

// TODO: this should probably be generalized
void PenanceRunner::SetDestinationBlughhhh(GameMap &game_map) {
  destination_x_ = x_;

  if (game_map.IsWave10()) {
    destination_y_ = EAST_TRAP_Y - 4;
  } else {
    destination_y_ = EAST_TRAP_Y + 4;
  }
}

void PenanceRunner::SetDestinationRandomWalk(GameMap &game_map) {
  if (x_ <= 27) {
    if ((y_ == 8) || (y_ == 9)) {
      destination_x_ = 30;
      destination_y_ = 8;
      return;
    } else if ((x_ == 25) && (y_ == 7)) {
      destination_x_ = 26;
      destination_y_ = 8;
      return;
    }
  } else if (x_ <= 32) {
    if (y_ <= 8) {
      destination_x_ = 30;
      destination_y_ = 6;
      return;
    }
  } else if ((y_ == 7) || (y_ == 8)) {
    destination_x_ -= 31;
    destination_y_ = 8;
    return;
  }

  int direction = RollMovement();
  forced_movements_index_++;

  if (direction == SOUTH) {
    destination_x_ = x_;
    destination_y_ = y_ - 5;
  } else if (direction == WEST) {
    destination_x_ = x_ - 5;

    if (destination_x_ < WEST_TRAP_X - 1) {
      destination_x_ = WEST_TRAP_X - 1;
    }

    destination_y_ = y_;
  } else {
    destination_x_ = x_ + 5;

    if (game_map.IsWave10()) {
      if (destination_x_ > EAST_TRAP_X - 1) {
        destination_x_ = EAST_TRAP_X - 1;
      }
    } else if (destination_x_ > EAST_TRAP_X) {
      destination_x_ = EAST_TRAP_X;
    }

    destination_y_ = y_;
  }
}

void PenanceRunner::DoTick1(GameMap &game_map) {
  if (y_ == 6) {
    despawn_countdown_ = 3;
    return;
  }

  if (blughhhh_countdown_ > 0) {
    blughhhh_countdown_--;
  } else {
    target_state_++;

    if (target_state_ > 3) {
      target_state_ = 1;
    }
  }

  bool ate_or_target_gone = TryEatAndCheckTarget(game_map);

  if ((blughhhh_countdown_ == 0) && (food_target_ == nullptr)) {
    CancelDestination();
  }

  if (!ate_or_target_gone) {
    DoMovement(game_map);
  }
}

void PenanceRunner::DoTick2Or5(GameMap &game_map) {
  if (target_state_ == 2) {
    TryTargetFood(game_map);
  }

  DoTick7To10(game_map);
}

void PenanceRunner::DoTick3(GameMap &game_map) {
  if (target_state_ == 3) {
    TryTargetFood(game_map);
  }

  DoTick7To10(game_map);
}

void PenanceRunner::DoTick4(GameMap &game_map) {
  if (target_state_ == 1) {
    TryTargetFood(game_map);
  }

  DoTick7To10(game_map);
}

void PenanceRunner::DoTick6(GameMap &game_map) {
  if (y_ == 6) {
    despawn_countdown_ = 3;
    return;
  }

  if (blughhhh_countdown_ > 0) {
    blughhhh_countdown_--;
  }

  if (target_state_ == 3) {
    TryTargetFood(game_map);
  }

  bool ate_or_target_gone = TryEatAndCheckTarget(game_map);

  if ((blughhhh_countdown_ == 0) && ((food_target_ == nullptr) || ate_or_target_gone)) {
    SetDestinationRandomWalk(game_map);
  }

  if (!ate_or_target_gone) {
    DoMovement(game_map);
  }
}

void PenanceRunner::DoTick7To10(GameMap &game_map) {
  bool ate_or_target_gone = TryEatAndCheckTarget(game_map);

  if (!ate_or_target_gone) {
    DoMovement(game_map);
  }
}

bool PenanceRunner::IsNearEastTrap() const {
  return ((std::abs(x_ - EAST_TRAP_X) < 2) && (std::abs(y_ - EAST_TRAP_Y) < 2));
}

bool PenanceRunner::IsNearWestTrap() const {
  return ((std::abs(x_ - WEST_TRAP_X) < 2) && (std::abs(y_ - WEST_TRAP_Y) < 2));
}

int PenanceRunner::RollMovement() {
  if (forced_movements_.size() > forced_movements_index_) {
    char movement = forced_movements_[forced_movements_index_];
    forced_movements_index_++;

    if (movement == 's') {
      return SOUTH;
    }

    if (movement == 'w') {
      return WEST;
    }

    if (movement == 'e') {
      return EAST;
    }
  }

  std::srand(std::time(nullptr));

  int random_number = std::rand() / ((RAND_MAX + 1u)/6);

  if (random_number < 4) {
    return SOUTH;
  }

  if (random_number == 4) {
    return WEST;
  }

  return EAST;
}

int PenanceRunner::next_id_ = 0;
