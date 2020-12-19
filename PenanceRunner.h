//
// Created by Andy on 12/19/2020.
//

#ifndef BASIMCXX_PENANCERUNNER_H
#define BASIMCXX_PENANCERUNNER_H

#include <string>

#include "Item.h"
#include "GameMap.h"

class PenanceRunner {
  public:
    PenanceRunner();

    ~PenanceRunner();

    std::string ToString() const;

    void Tick(GameMap &game_map);

    void DoMovement();

    void TryTargetFood();

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

  private:
    static int next_id_;
    int x_, y_;
    int destination_x_, destination_y_;
    int cycle_tick_ = 1;
    int target_state_ = 0;
    Item food_target_;
    int blughhhh_countdown_ = 0;
    int stand_still_counter_ = 0;
    int despawn_countdown_ = -1;
    bool is_dying_ = false;
    // runner_rng
    int id_;
};


#endif  // BASIMCXX_PENANCERUNNER_H
