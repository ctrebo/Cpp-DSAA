#ifndef EDGE_H
#define EDGE_H

class Edge {
private:
    int value_ {};
    int weight_{};

public:
    Edge(int value, int weight=0);
    
    void setWeight(int weight);
    void setValue(int value);

    int getWeight() const;
    int getValue() const;
};


#endif
