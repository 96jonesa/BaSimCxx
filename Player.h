//
// Created by Andy on 12/18/2020.
//

#ifndef BASIMCXX_PLAYER_H
#define BASIMCXX_PLAYER_H

#include <string>

// class that represents a player
class Player {
  public:
    // constructor for initializing x_ and y_ to given values,
    // and initializing all other member variables via their
    // default constructors
    Player(int x, int y) : x_(x), y_(y) { }

    // @returns string representation of this object
    virtual std::string ToString() const = 0;

    // updates this player's state by progressing by a game tick
    virtual void Tick(GameMape &game_map);

    // finds the path from this player's current location (x_, y_)
    // to the given destination (dest_x, dest_y) and updates this
    // player's state accordingly
    virtual void PathFind(int dest_x, int dest_y);

    // virtual destructor that does nothing
    virtual ~Player() { }

  protected:
    int x_, y_;
    int path_queue_pos_;
    std::vector<int> path_queue_x_, path_queue_y_;
    std::vector<int> shortest_distances_;
    std::vector<int> way_points_;
};  // class Player


#endif  // BASIMCXX_PLAYER_H
