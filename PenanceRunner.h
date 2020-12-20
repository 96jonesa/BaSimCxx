//
// Created by Andy on 12/19/2020.
//

#ifndef BASIMCXX_PENANCERUNNER_H
#define BASIMCXX_PENANCERUNNER_H

class GameMap;

#include <string>
#include <memory>

#include "Item.h"
#include "GameMap.h"



class PenanceRunner {
  public:
    PenanceRunner(int x, int y);

    ~PenanceRunner();

    std::string ToString() const;

    void Tick(GameMap &game_map);

    void DoMovement(GameMap &game_map);

    void TryTargetFood(GameMap &game_map);

    bool TryEatAndCheckTarget();

    void CancelDestination();

    void SetDestinationBlughhhh();

    void SetDestinationRandomWalk();

    void DoTick1();

    void DoTick2Or5();

    void DoTick3();

    void DoTick4();

    void DoTick6();

    void DoTick7To10();

    /**
     * checks if this penance runner can be killed by the east trap at its
     * current location
     *
     * @param   game_map  the state of the game map
     * @return            true if this penance runner can be killed by the
     *                    east trap at its current location, else false
     */
    bool IsNearEastTrap(GameMap &gameMap) const;

    /**
     * checks if this penance runner can be killed by the west trap at its
     * current location
     *
     * @param   game_map  the state of the game map
     * @return            true if this penance runner can be killed by the
     *                    west trap at its current location, else false
     */
    bool IsNearWestTrap(GameMap &gameMap) const;

  private:
    static int next_id_;
    int x_, y_;
    int destination_x_, destination_y_;
    int cycle_tick_ = 1;
    int target_state_ = 0;
    std::shared_ptr<Item> food_target_;
    int blughhhh_countdown_ = 0;
    int stand_still_counter_ = 0;
    int despawn_countdown_ = -1;
    bool is_dying_ = false;
    // runner_rng
    int id_;
    int forced_movements_index_ = 0;
};


#endif  // BASIMCXX_PENANCERUNNER_H
