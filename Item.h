//
// Created by Andy on 12/18/2020.
//

#ifndef BASIMCXX_ITEM_H
#define BASIMCXX_ITEM_H

#include <string>

class Item {
  public:
    /**
     * constructor for constructing a "null" item
     */
    Item() : is_null_(true) { }

    /**
     * constructor specifying the x and y coordinates of this item,
     * whether or not this item is "good", and the type of this item
     *
     * @param x       the x coordinate of this item
     * @param y       the y coordinate of this item
     * @param is_good whether or not this item is "good"
     * @param type    the type of this item
     */
    Item(int x, int y, bool is_good, char type);

    /**
     * destructor that does nothing
     */
    ~Item() { };

    /**
     * returns a string representation of this item
     *
     * @return  string representation of this item
     */
    std::string ToString() const;

    /**
     * gets the x coordinate of this item
     *
     * @return  the x coordinate of this item
     */
    int get_x() const { return x_; }

    /**
     * sets the x coordinate of this item to the given value
     *
     * @param x the value to set the x coordinate of this item to
     */
    void set_x(int x) { x_ = x; }

    /**
     * gets the y coordinate of this item
     *
     * @return  the y coordinate of this item
     */
    int get_y() const { return y_; }

    /**
     * sets the y coordinate of this item to the given value
     *
     * @param x the value to set the y coordinate of this item to
     */
    void set_y(int y) { y_ = y; }

    /**
     * checks if this item is "good"
     *
     * @return  true if this item is "good", else false
     */
    bool get_is_good() { return is_good_; }

    /**
     * sets whether this item is "good" to the given value
     *
     * @param is_good the value to set whether this item is "good" to
     */
    void set_is_good(bool is_good) { is_good_ = is_good; }

    /**
     * gets the type of this item
     *
     * @return  the type of this item
     */
    char get_type() const { return type_; }

    /**
     * sets the type of this item to the given value
     *
     * @param type  the value to set the type of this item to
     */
    void set_type(char type) { type_ = type; }

    /**
     * gets the id of this item
     *
     * @return  the id of this item
     */
    int get_id() const { return id_; }

    /**
     * sets the id of this item to the given value
     *
     * @param id  the value to set the id of this item to
     */
    void set_id(int id) { id_ = id; }

    /**
     * gets whether this item is "null"
     *
     * @return  true if this item is "null", else false
     */
    bool get_is_null() const { return is_null_; }

    /**
     * sets whether this item is "null" to the given value
     *
     * @param is_null the value to set whether this item is "null" to
     */
    void set_is_null(bool is_null) { is_null_ = is_null; }

  private:
    static int next_id_;
    int x_, y_;
    bool is_good_;
    char type_;
    int id_;
    bool is_null_;
};  // class Item


#endif  // BASIMCXX_ITEM_H
