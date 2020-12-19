//
// Created by Andy on 12/18/2020.
//

#ifndef BASIMCXX_ITEM_H
#define BASIMCXX_ITEM_H


class Item {
  public:
    Item();

    ~Item();

    int get_x();

    int get_y();

    char get type();

  private:
    char type;
    int x_, y_;
};  // class Item


#endif  //BASIMCXX_ITEM_H
