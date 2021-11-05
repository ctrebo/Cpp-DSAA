#include "edge.h"

Edge::Edge(int value, int weight): weight_ {weight}, value_ {value} {}


void Edge::setWeight(int weight) {
    weight_ = weight;
}
void Edge::setValue(int value) {
    value_ = value;
}

int Edge::getWeight() const {
    return weight_;
}
int Edge::getValue() const {
    return value_;
}
