//
// Created by Andy on 12/19/2020.
//

#ifndef BASIMCXX_PENANCERUNNER_H
#define BASIMCXX_PENANCERUNNER_H

class GameMap;

#include <string>
#include <memory>
#include <cstdlib>

#include "Item.h"
#include "GameMap.h"

const int SOUTH = 0;
const int WEST = 1;
const int EAST = 2;

class PenanceRunner {
  public:
    /**
     * constructor specifying the x and y coordinates and the forced movements
     * of this penance runner
     *
     * @param x                 the x coordinate of this penance runner
     * @param y                 the y coordinate of this penance runner
     * @param forced_movements  the forced movements of this penance runner
     */
    PenanceRunner(int x, int y, std::string forced_movements);

    /**
     * destructor that does nothing
     */
    ~PenanceRunner() { };

    /**
     * returns a string representation of this penance runner
     *
     * @return  string representation of this penance runner
     */
    std::string ToString() const;

    /**
     * updates this penance runner's state by progressing by a game tick
     *
     * @param game_map  the state of the game map
     */
    void Tick(GameMap &game_map);

    /**
     * moves this penance runner for a game tick
     *
     * @param game_map  the state of the game map
     */
    void DoMovement(GameMap &game_map);

    /**
     * tries to find and set a new food target for this penance runner
     *
     * @param game_map  the state of the game map
     */
    void TryTargetFood(GameMap &game_map);

    /**
     * checks if this penance runner's food target is still valid and,
     * if it is still valid and this penance runner is at the location of
     * its food target, tries to eat it
     *
     * @param game_map  the state of the game map
     */
    bool TryEatAndCheckTarget(GameMap &game_map);

    /**
     * sets this penance runner's destination to its current location
     */
    void CancelDestination();

    /**
     * sets this penance runner's destination as if it just ate a "bad" food
     *
     * @param game_map  the state of the game map
     */
    void SetDestinationBlughhhh(GameMap &game_map);

    /**
     * sets this penance runner's destination according to a sample from its
     * random movement distribution
     *
     * @param game_map  the state of the game map
     */
    void SetDestinationRandomWalk(GameMap &game_map);

    /**
     * performs this penance runner's tick 1 actions
     *
     * @param game_map  the state of the game map
     */
    void DoTick1(GameMap &game_map);

    /**
     * performs this penance runner's tick 2 / tick 5 actions
     *
     * @param game_map  the state of the game map
     */
    void DoTick2Or5(GameMap &game_map);

    /**
     * performs this penance runner's tick 3 actions
     *
     * @param game_map  the state of the game map
     */
    void DoTick3(GameMap &game_map);

    /**
     * performs this penance runner's tick 4 actions
     *
     * @param game_map  the state of the game map
     */
    void DoTick4(GameMap &game_map);

    /**
     * performs this penance runner's tick 6 actions
     *
     * @param game_map  the state of the game map
     */
    void DoTick6(GameMap &game_map);

    /**
     * performs this penance runner's tick 7 / tick 8 / tick 9 / tick 10 actions
     *
     * @param game_map  the state of the game map
     */
    void DoTick7To10(GameMap &game_map);

    /**
     * checks if this penance runner can be killed by the east trap at its
     * current location
     *
     * @return  true if this penance runner can be killed by the
     *          east trap at its current location, else false
     */
    bool IsNearEastTrap() const;

    /**
     * checks if this penance runner can be killed by the west trap at its
     * current location
     *
     * @return  true if this penance runner can be killed by the
     *          west trap at its current location, else false
     */
    bool IsNearWestTrap() const;

    /**
     * samples this runner's random movement distribution in order to determine
     * its next movement direction
     *
     * @return  the next movement direction of this runner
     */
    int RollMovement();

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
    std::string forced_movements_;
    int id_;
    std::size_t forced_movements_index_ = 0;
};


#endif  // BASIMCXX_PENANCERUNNER_H
