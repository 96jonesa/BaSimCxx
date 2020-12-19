//
// Created by Andy on 12/18/2020.
//

#ifndef BASIMCXX_ITEM_H
#define BASIMCXX_ITEM_H

#include <string>

class Item {
  public:
    Item() : is_null_(true) { }

    Item(int x, int y, bool is_good, char type);

    ~Item() { };

    std::string ToString() const;

    int get_x() const { return x_; }

    void set_x(int x) { x_ = x; }

    int get_y() const { return y_; }

    void set_y(int y) { y_ = y; }

    bool get_is_good() { return is_good_; }

    void set_is_good(bool is_good) { is_good_ = is_good; }

    char get_type() const { return type_; }

    void set_type(char type) { type_ = type; }

    int get_id() const { return id_; }

    void set_id(int id) { id_ = id; }

    bool get_is_null() const { return is_null_; }

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
