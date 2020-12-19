//
// Created by Andy on 12/18/2020.
//

#ifndef BASIMCXX_ITEM_H
#define BASIMCXX_ITEM_H


class Item {
  public:
    Item(int x, int y, bool is_good, char type) : x_(x), y_(y), is_good_(is_good), type_(type) { }

    ~Item();

    int get_x() const { return x_; }

    void set_x(int x) { x_ = x; }

    int get_y() const { return y_; }

    void set_y(int y) { y_ = y; }

    bool get_is_good() { return is_good_; }

    void set_is_good(bool is_good) { is_good_ = is_good; }

    char get_type() const { return type_; }

    void set_type(char type) { type_ = type; }

  private:
    int x_, y_;
    bool is_good_;
    char type_;
};  // class Item


#endif  // BASIMCXX_ITEM_H
