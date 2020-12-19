//
// Created by Andy on 12/18/2020.
//

#ifndef BASIMCXX_PLAYERDEFENDER_H
#define BASIMCXX_PLAYERDEFENDER_H

#include <string>
#include <vector>

#include "Player.h"
#include "GameMap.h"

// class that represents a (player) defender
class PlayerDefender : public Player {
  public:
    // constructor for initializing x_ and y_ to given values,
    // and initializing all other member variables via their
    // default constructors
    PlayerDefender(int x, int y) : Player(x, y) { }

    // virtual destructor that does nothing
    virtual ~PlayerDefender() { }

    // @returns string representation of this object
    std::string ToString() const override;

    // updates this player's state by progressing by a game tick
    void Tick(GameMap &game_map) override;

    // finds the path from this player's current location (x_, y_)
    // to the given destination (dest_x, dest_y) and updates this
    // player's state accordingly
    void PathFind(GameMap &game_map, int dest_x, int dest_y) override;

    // @returns true if can repair east trap from current location,
    //          else false
    bool IsInEastRepairRange(GameMap &game_map) const;

    // @returns true if can repair west trap from current location,
    //          else false
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
