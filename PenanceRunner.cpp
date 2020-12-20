//
// Created by Andy on 12/19/2020.
//

#include <string>

#include "PenanceRunner.h"

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
      std::vector<PenanceRunner> &runners_to_remove = game_map.get_runners_to_remove();
      runners_to_remove.push_back(this);

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
        DoTick2Or5()
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

void PenanceRunner::TryTargetFood() {

}

bool PenanceRunner::TryEatAndCheckTarget() {

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

}

bool PenanceRunner::IsNearWestTrap(GameMap &gameMap) const {

}
