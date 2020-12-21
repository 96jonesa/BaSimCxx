//
// Created by Andy on 12/20/2020.
//

#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <boost/algorithm/string.hpp>
#include <memory>

#include "Player.h"
#include "PlayerDefender.h"
#include "GameMap.h"
#include "Item.h"
#include "PenanceRunner.h"

bool IsValidTile(std::vector<int> &tile);

int GetWalkDistance(int x1, int y1, int x2, int y2);

bool IsInRange(int x1, int y1, int x2, int y2, int range);

std::vector< std::vector< std::vector<int> > > SolveWave7ThreeTiles();

std::vector<std::string> AppendMovement(std::vector<std::string> &current_movements, std::vector<std::string> &movement_options);

std::vector<std::string> ParseMovementsInput(std::string movements_input);

bool IsRunnerOnStack(GameMap &game_map);

std::string TileString(std::vector<int> &tile);

std::string MovementsString(std::vector<std::string> &tile);

bool CheckWave7ThreeTiles(std::vector<int> &tile1, std::vector<int> &tile2, std::vector<int> &tile3);

int main(int argc, char **argv) {
  std::cout << "Hello, world!" << std::endl;

  std::vector< std::vector< std::vector<int> > > wave7_three_tile_solution = SolveWave7ThreeTiles();

  std::cout << "number of solutions: " << wave7_three_tile_solution.size() << std::endl;

  std::cout << "solutions:" << std::endl;

  for (int i = 0; i < wave7_three_tile_solution.size(); i++) {
    std::vector< std::vector<int> > solution = wave7_three_tile_solution[i];
    std::cout << "    [" << TileString(solution[0]) << ", " << TileString(solution[1]) << ", " << TileString(solution[2]) << "]" << std::endl;
  }
}

bool IsValidTile(std::vector<int> &tile) {
  int x = tile[0];
  int y = tile[1];

  if ((x == 47) && (y == 31)) {
    return true;
  }

  if ((x == 47) && (y == 32)) {
    return true;
  }

  if ((x == 46) && ((y >= 28) && (y <= 33))) {
    return true;
  }

  if (((x >= 43) && (x <= 46)) && ((y >= 25) && (y <= 27))) {
    return true;
  }

  if ((x == 47) && (y == 25)) {
    return true;
  }

  if ((x == 47) && (y == 26)) {
    return true;
  }

  if ((x == 48) && (y == 25)) {
    return true;
  }

  if (((x >= 34) && (x <= 45)) && ((y >= 28) && (y <= 36))) {
    return true;
  }

  if (((x >= 34) && (x <= 43)) && ((y >= 37) && (y <= 38))) {
    return true;
  }

  if ((x == 44) && (y == 37)) {
    return true;
  }

  return false;
}

int GetWalkDistance(int x1, int y1, int x2, int y2) {
  return std::max(std::abs(x1 - x2), std::abs(y1 - y2));
}

bool IsInRange(int x1, int y1, int x2, int y2, int range) {
  return (GetWalkDistance(x1, y1, x2, y2) <= range);
}

std::vector< std::vector< std::vector<int> > > SolveWave7ThreeTiles() {
  std::vector< std::vector< std::vector<int> > > tiles;

  int x1_min = EAST_TRAP_X - 10;
  int x1_max = EAST_TRAP_X + 10;
  int y1_min = EAST_TRAP_Y;
  int y1_max = EAST_TRAP_Y + 10;
  int x2_min = EAST_TRAP_X - 10;
  int x2_max = EAST_TRAP_X + 10;
  int y2_min = EAST_TRAP_Y;
  int y2_max = EAST_TRAP_Y + 15;
  int x3_min = EAST_TRAP_X - 1;
  int x3_max = EAST_TRAP_X + 1;
  int y3_min = EAST_TRAP_Y - 1;
  int y3_max = EAST_TRAP_Y + 1;

  for (int x1 = x1_min; x1 <= x1_max; x1++) {
    for (int y1 = y1_min; y1 <= y1_max; y1++) {
      std::vector<int> tile1 = {x1, y1};

      if (IsValidTile(tile1) && IsInRange(x1, y1, EAST_TRAP_X, EAST_TRAP_Y + 1, 4)) {
        for (int x2 = x2_min; x2 <= x2_max; x2++) {
          for (int y2 = y2_min; y2 <= y2_max; y2++) {
            std::vector<int> tile2 = {x2, y2};

            if (IsValidTile(tile2) && IsInRange(x1, y1, x2, y2, 5)) {
              for (int x3 = x3_min; x3 <= x3_max; x3++) {
                for (int y3 = y3_min; y3 <= y3_max; y3++) {
                  std::vector<int> tile3 = {x3, y3};

                  if (IsValidTile(tile3) && IsInRange(x3, y3, EAST_TRAP_X, EAST_TRAP_Y, 1) && (GetWalkDistance(x3, y3, x2, y2) == 10)) {
                    if (CheckWave7ThreeTiles(tile1, tile2, tile3)) {
                      std::vector< std::vector<int> > these_tiles = {tile1, tile2, tile3};
                      tiles.push_back(these_tiles);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  return tiles;
}

std::vector<std::string> AppendMovement(std::vector<std::string> &current_movements, std::vector<std::string> &movement_options) {
  std::vector<std::string> result;

  for (int i = 0; i < current_movements.size(); i++) {
    for (int j = 0; j < movement_options.size(); j++) {
      result.push_back(current_movements[i] + "-" + movement_options[j]);
    }
  }

  return result;
}

std::vector<std::string> ParseMovementsInput(std::string movements_input) {
  std::vector<std::string> movements;
  boost::split(movements, movements_input, boost::is_any_of("-"));

  for (int i = 0; i < movements.size(); i++) {
    std::string moves = movements[i];

    for (int j = 0; j < moves.size(); j++) {
      char move = moves[j];

      if ((move != 's') && (move != 'w') && (move != 'e')) {
        movements.clear();
        return movements;
      }
    }
  }

  return movements;
}

bool IsRunnerOnStack(GameMap &game_map) {
  std::vector< std::shared_ptr<PenanceRunner> > &runners = game_map.get_runners();

  for (int i = 0; i <runners.size(); i++) {
    std::shared_ptr<PenanceRunner> runner = runners[i];

    if ((runner->get_x() == EAST_TRAP_X) && (runner->get_y() == EAST_TRAP_Y + 1)) {
      return true;
    }
  }

  return false;
}

std::string TileString(std::vector<int> &v) {
  std::string result = "[" + std::to_string(v[0]);

  for (int i = 1; i < v.size(); i++) {
    result += ", " + std::to_string(v[i]);
  }

  result += "]";

  return result;
}

std::string MovementsString(std::vector<std::string> &v) {
  std::string result = "[" + v[0];

  for (int i = 1; i < v.size(); i++) {
    result += ", " + v[i];
  }

  result += "]";

  return result;
}

bool CheckWave7ThreeTiles(std::vector<int> &tile1, std::vector<int> &tile2, std::vector<int> &tile3) {
  std::vector<std::string> movements_input_vector = {"se"};
  std::vector<std::string> movement_options = {"ss", "sw", "se", "ws", "ww", "we", "es", "ew", "ee"};

  movements_input_vector = AppendMovement(movements_input_vector, movement_options);
  movements_input_vector = AppendMovement(movements_input_vector, movement_options);
  movements_input_vector = AppendMovement(movements_input_vector, movement_options);
  movements_input_vector = AppendMovement(movements_input_vector, movement_options);
  movements_input_vector = AppendMovement(movements_input_vector, movement_options);

  for (int movements_index = 0; movements_index < movements_input_vector.size(); movements_index++) {
    std::vector<std::string> movements = ParseMovementsInput(movements_input_vector[movements_index]);
    int max_runners_alive = 5;
    int total_runners = 6;

    GameMap game_map(false, max_runners_alive, total_runners, movements);
    std::shared_ptr<Player> defender = std::make_shared<PlayerDefender>(WAVE1_DEFENDER_SPAWN_X, WAVE1_DEFENDER_SPAWN_Y);
    game_map.AddPlayer(defender);

    bool first_runner = false;
    int second_drop_tick = 100;

    for (int i = 0; i < 100; i++) {
      game_map.Tick();
      defender.Tick(game_map);

      int tick_counter = game_map.get_tick_counter();

      if (tick_counter == 24) {  // trap food
        std::shared_ptr<Item> food1 = std::make_shared<Item>(EAST_TRAP_X, EAST_TRAP_Y + 1, true, 't');
        std::shared_ptr<Item> food2 = std::make_shared<Item>(EAST_TRAP_X, EAST_TRAP_Y + 1, true, 't');
        std::shared_ptr<Item> food3 = std::make_shared<Item>(EAST_TRAP_X, EAST_TRAP_Y + 1, true, 't');
        game_map.AddItem(food1);
        game_map.AddItem(food2);
        game_map.AddItem(food3);
      } else if (tick_counter == 27) {  // trail food
        std::shared_ptr<Item> food1 = std::make_shared<Item>(EAST_TRAP_X - 6, EAST_TRAP_Y + 5, false, 'w');
        game_map.AddItem(food1);
      } else if (tick_counter == 29) {  // main stack
        std::shared_ptr<Item> food1 = std::make_shared<Item>(EAST_TRAP_X - 10, EAST_TRAP_Y + 9, true, 't');
        std::shared_ptr<Item> food2 = std::make_shared<Item>(EAST_TRAP_X - 10, EAST_TRAP_Y + 9, true, 't');
        std::shared_ptr<Item> food3 = std::make_shared<Item>(EAST_TRAP_X - 10, EAST_TRAP_Y + 9, true, 't');
        std::shared_ptr<Item> food4 = std::make_shared<Item>(EAST_TRAP_X - 10, EAST_TRAP_Y + 9, true, 't');
        std::shared_ptr<Item> food5 = std::make_shared<Item>(EAST_TRAP_X - 10, EAST_TRAP_Y + 9, true, 't');
        game_map.AddItem(food1);
        game_map.AddItem(food2);
        game_map.AddItem(food3);
        game_map.AddItem(food4);
        game_map.AddItem(food5);
      } else if ((!first_runner) && IsRunnerOnStack()) {
        first_runner = true;
        defender->set_x(EAST_TRAP_X);
        defender->set_y(EAST_TRAP_Y + 1);
      } else if (tick_counter == 61) {  // move off trap food on tick 61
        defender->PathFind(tile1[0], tile1[1]);
      } else if ((defender->get_x() == tile1[0]) && (defender->get_y() == tile1[1])) {  // drop bait food asap, then move to tile2
        std::shared_ptr<Item> food1 = std::make_shared<Item>(tile1[0], tile1[1], true, 'w');
        game_map.AddItem(food1);
        defender->PathFind(tile2[0], tile1[0]);
      } else if ((defender->get_x() == tile2[0]) && (defender->get_y() == tile2[1])) {  // drop bait food asap, then teleport away to avoid bumping
        std::shared_ptr<Item> food1 = std::make_shared<Item>(tile2[0], tile2[1], true, 'w');
        game_map.AddItem(food1);
        defender->set_x(WAVE1_DEFENDER_SPAWN_X);
        defender->set_y(WAVE1_DEFENDER_SPAWN_Y);
        second_drop_tick = tick_counter;
      } else if (tick_counter == second_drop_tick + 5) {  // should take 5 ticks to get to tile3 from tile2, then teleport away to not block
        defender->set_x(tile3[0]);
        defender->set_y(tile3[1]);
        std::shared_ptr<Item> food1 = std::make_shared<Item>(tile3[0], tile3[1], true, 'w');
        std::shared_ptr<Item> food2 = std::make_shared<Item>(tile3[0], tile3[1], true, 'w');
        std::shared_ptr<Item> food3 = std::make_shared<Item>(tile3[0], tile3[1], true, 'w');
        std::shared_ptr<Item> food4 = std::make_shared<Item>(tile3[0], tile3[1], true, 'w');
        std::shared_ptr<Item> food5 = std::make_shared<Item>(tile3[0], tile3[1], true, 'w');
        game_map.AddItem(food1);
        game_map.AddItem(food2);
        game_map.AddItem(food3);
        game_map.AddItem(food4);
        game_map.AddItem(food5);
        defender->set_x(WAVE1_DEFENDER_SPAWN_X);
        defender->set_y(WAVE1_DEFENDER_SPAWN_Y);
      }
    }

    if (game_map.get_runners_killed() < 6) {
      std::cout << TileString(tile1) << ", " << TileString(tile2) << ", " << TileString(tile3);
      std::cout << " failed with " + std::to_string(game_map.get_runners_killed()) << " runners killed on pattern ";
      std::cout << MovementsString(movements) << std::endl;

      return false;
    }
  }

  std::cout << TileString(tile1) << ", " << TileString(tile2) << ", " << TileString(tile3);
  std::cout << " succeeded" << std::endl;

  return true;
}
