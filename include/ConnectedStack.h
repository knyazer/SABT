//
// Created by knyaz on 6/4/2022.
//

#ifndef SABT_CONNECTED_STACK_H
#define SABT_CONNECTED_STACK_H

#include <memory>
#include <cassert>

#define INITIAL_SIZE 10
#define GROWTH_FACTOR 1.5

template<typename T>
class ConnectedStack {
    struct StackPointer {
        std::shared_ptr<ConnectedStack<T>> root;
        size_t ptr;

        ///< Moves the pointer upwards
        void step();
    };

    StackPointer parent;

    std::unique_ptr<T> data;
    size_t currentSize;
    size_t allocated;

    void grow();

public:
    ConnectedStack();

    void push(T value);
    T front();
    T pop();
    T at(size_t index);

    size_t size();
    bool empty();
};

#endif //SABT_CONNECTED_STACK_H
