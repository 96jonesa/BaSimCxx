//
// Created by Andy on 12/18/2020.
//

#ifndef BASIMCXX_PLAYER_H
#define BASIMCXX_PLAYER_H

class GameMap;

#include <string>
#include <vector>
#include <map>

#include "GameMap.h"

/**
 * class that represents a player
 */
class Player {
  public:
    /**
     * constructor specifying the x and y coordinates of this player
     *
     * @param x the x coordinate of this player
     * @param y the y coordinate of this player
     */
    Player(int x, int y) : x_(x), y_(y) { }

    /**
     * returns a string representation of this player
     *
     * @return  string representation of this player
     */
    virtual std::string ToString() const = 0;

    /**
     * updates this player's state by progressing by a game tick
     *
     * @param game_map
     */
    virtual void Tick(GameMap &game_map) = 0;

    /**
     * finds the path from this player's current location (x_, y_)
     * to the given destination (dest_x, dest_y) and modifies this
     * player's state accordingly
     *
     * @param game_map  the state of the game map
     * @param dest_x    the x coordinate of the destination
     * @param dest_y    the y coordinate of the destination
     */
    virtual void PathFind(GameMap &game_map, int dest_x, int dest_y) = 0;

    /**
     * virtual destructor that does nothing
     */
    virtual ~Player() { }

    /**
     * gets the x coordinate of this player
     *
     * @return  the x coordinate of this player
     */
    int get_x() const { return x_; }

    /**
     * sets the x coordinate of this player to the given value
     *
     * @param x the value to set the x coordinate of this player to
     */
    void set_x(int x) { x_ = x; }

    /**
     * gets the y coordinate of this player
     *
     * @return  the y coordinate of this player
     */
    int get_y() const { return y_; }

    /**
     * sets the y coordinate of this player to the given value
     *
     * @param y the value to set the y coordinate of this player to
     */
    void set_y(int y) { y_ = y; }

  protected:
    int x_, y_;
    int path_queue_pos_ = 0;
    std::map<int, int> path_queue_x_, path_queue_y_;
    std::vector<int> shortest_distances_;
    std::vector<int> way_points_;
};  // class Player


#endif  // BASIMCXX_PLAYER_H
