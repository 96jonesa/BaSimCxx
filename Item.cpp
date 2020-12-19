//
// Created by Andy on 12/18/2020.
//

#include <string>

#include "Item.h"

Item::Item(int x, int y, bool is_good, char type)
    : x_(x), y_(y), is_good_(is_good), type_(type), id_(next_id_++) { }

std::string Item::ToString() const {
  return "";
}

int Item::next_id_ = 0;
