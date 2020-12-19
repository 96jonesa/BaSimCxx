//
// Created by Andy on 12/18/2020.
//

#ifndef BASIMCXX_PLAYERDEFENDER_H
#define BASIMCXX_PLAYERDEFENDER_H

#include <string>
#include <vector>

#include "Player.h"
#include "GameMap.h"

/**
 * class that represents a (player) defender
 */
class PlayerDefender : public Player {
  public:
  /**
   * constructor specifying the x and y coordinates of this (player) defender
   *
   * @param x the x coordinate of this (player) defender
   * @param y the y coordinate of this (player) defender
   */
    PlayerDefender(int x, int y) : Player(x, y) { }

    /**
     * virtual destructor that does nothing
     */
    virtual ~PlayerDefender() { }

    /**
     * returns a string representation of this (player) defender
     *
     * @return  string representation of this (player) defender
     */
    std::string ToString() const override;

    /**
     * updates this player's state by progressing by a game tick
     *
     * @param game_map  the state of the game map
     */
    void Tick(GameMap &game_map) override;

    /**
     * finds the path from this (player) defender's current location (x_, y_)
     * to the given destination (dest_x, dest_y) and modifies this (player)
     * defender's state accordingly
     *
     * @param game_map  the state of the game map
     * @param dest_x    the x coordinate of the destination
     * @param dest_y    the y coordinate of the destination
     */
    void PathFind(GameMap &game_map, int dest_x, int dest_y) override;

    /**
     * checks if this (player) defender can repair the east trap from their
     * current location
     *
     * @param   game_map  the state of the game map
     * @return            true if this (player) defender can repair the east
     *                    trap from their current location, else false
     */
    bool IsInEastRepairRange(GameMap &game_map) const;

    /**
     * checks if this (player) defender can repair the west trap from their
     * current location
     *
     * @param   game_map  the state of the game map
     * @return            true if this (player) defender can repair the west
     *                    trap from their current location, else false
     */
    bool IsInWestRepairRange(GameMap &game_map) const;

  private:
    char picking_up_food_ = 'n';
    bool picking_up_logs_ = false, picking_up_hammer_ = false;
    int repair_ticks_remaining_ = 0;
    int stand_still_counter_ = 0;
    int num_logs_ = 0;
    int num_tofu_ = 9, num_crackers_ = 9, num_worms_ = 9;
    bool has_hammer_ = false;
};  // class PlayerDefender


#endif  // BASIMCXX_PLAYERDEFENDER_H
