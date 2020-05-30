#ifndef Node_hpp
#define Node_hpp

#include "Component.hpp"
#include <vector>

struct Node{
    string Node_name;
    double voltage;
    vector<Component> component;
}

#endif /* Node_hpp */
