//
// Created by knyaz on 6/4/2022.
//

#ifndef SABT_CONNECTED_STACK_H
#define SABT_CONNECTED_STACK_H

#include <memory>
#include <cassert>
#include <stdexcept>
#include <ostream>
#include <iostream>

#define INITIAL_SIZE 4
#define GROWTH_FACTOR 3
/**
 * The template which allows construction of connected stack with any parameter.
 * Connected stack generally acts similar to the usual stack, but with a few key differences.
 * First of all, you can connect it to other (parent) stack. It means that after "pop()" the pointer will continue to move
 * upwards until parent is not empty. And if the parent was also connected, then moves to the next one.
 * However, all the parent stacks are read-only, and after you push a value, this value
 * is pushed at the current pointer position.
 */
template<typename T>
class ConnectedStack {
protected:
    struct StackPointer {
        ConnectedStack<T> *root;
        size_t size;

        StackPointer() {
            root = nullptr;
            size = 0;
        }

        StackPointer(const StackPointer &other) {
            root = other.root;
            size = other.size;
        }

        StackPointer(ConnectedStack<T> *_root, size_t _size) : root(_root), size(_size) {}

        /// Moves the pointer upwards
        void step() {
            size--;

            // Attempt to go up the tree of stacks
            if (size == 0) {
                assert(root != nullptr && "Do not allow going up for the topmost stack");

                // *this = root->parent , but better?
                size = (root->parent).size;
                root = (root->parent).root;
            }
        }

        friend std::ostream &operator<<(std::ostream &os, const StackPointer &pointer) {
            os << "size: " << pointer.size;
            return os;
        }
    };

    StackPointer parent;

    T *data;
    size_t allocated;
    size_t currentSize;

    void grow() {
        // Setup new array
        size_t newSize = (allocated * GROWTH_FACTOR) + 1;
        T *newData = new T[newSize];

        // Copy data
        for (size_t i = 0; i < allocated; i++)
            newData[i] = data[i];

        // Save the pointer to old data array
        auto toDelete = data;

        // Reassign values
        data = newData;
        allocated = newSize;

        // Free memory
        delete toDelete;
    }

public:
    ConnectedStack() {
        data = new T[INITIAL_SIZE];
        allocated = INITIAL_SIZE;
        currentSize = 0;
    }

    void syncAllData(ConnectedStack<T> *other) {
        ConnectedStack<T> *ptr = other;
        size_t len = other->currentSize;

        size_t offset = currentSize;

        do {
            for (size_t i = 1; i <= len; i++)
                push(ptr->at(len - i));

            len = ptr->parent.size;
            ptr = ptr->parent.root;
        } while (ptr != nullptr);

        std::reverse(data + offset, data + currentSize);
    }

    void syncData(ConnectedStack<T> *other) {
        allocated = other->allocated;
        currentSize = other->currentSize;

        delete[] data;

        data = new T[allocated];
        for (size_t i = 0; i < currentSize; i++)
            data[i] = T(other->data[i]);
    }

    void setParent(ConnectedStack<T> *stack, ConnectedStack<T> *root) {
        parent = StackPointer(root, stack->parent.size);
    }

    /// Connect the stack to some place at other one
    void connect(ConnectedStack<T> *root, size_t ptr) {
        if (root->empty())
            throw std::runtime_error("Not allowed connect one stack to other empty stack - bad practice.");
        else {
            parent.root = root;
            parent.size = ptr + 1;
        }
    }

    void connectToEnd(ConnectedStack<T> *root) {
        if (root->size() == 0) {
            if (root->parent.size == 0)
                throw std::runtime_error("Fully empty stack, not allowed to connect.");

            parent.root = root->parent.root;
            parent.size = root->parent.size;
        }
        else
            connect(root, root->size() - 1);
    }

    bool empty() {
        return currentSize == 0;
    }

    bool parentEmpty() {
        return empty() && (parent.root == nullptr || parent.size == 0);
    }

    void push(T value) {
        if (currentSize < 0 || currentSize >= allocated)
            throw std::out_of_range("Attempt to access not allocated memory. Problems with stack growth.");

        data[currentSize] = value;
        currentSize++;

        if (currentSize >= allocated)
            grow();
    }

    T& at(size_t index) {
        if (index >= currentSize) {
            throw std::out_of_range("Attempt to access not available memory via 'at' operation.");
        }

        return data[index];
    }

    T front() {
        if (empty()) {
            if (parent.root == nullptr)
                throw std::runtime_error("Parent is nullptr and the stack is empty. Can't use 'front'.");
            if (parent.size == 0)
                throw std::runtime_error("Stack is empty and parent is empty. Can't use 'front'.");

            return parent.root->at(parent.size - 1);
        }

        if (currentSize < 1 || currentSize >= allocated)
            throw std::out_of_range("Current size of stack is wrong.");

        return data[currentSize - 1];
    }

    T back() {
        if (parent.root == nullptr)
            throw std::runtime_error("Cannot return the last element of parent if parent is nullptr, duh. Committing suicide.");
        if (parent.size == 0)
            throw std::runtime_error("Parent is zero size, weird, unable to access last element. Committing suicide cuz something might go wrong.");

        return parent.root->at(parent.size - 1);
    }

    void ascend() {
        if (parent.root == nullptr)
            throw std::runtime_error("Failed to ascend, parent is nullptr. Committing suicide.");

        parent.step();
    }

    void print(auto printFunction=[](T x){return x;}, long long len=-1) {
        if (len == -1)
            len = currentSize;

        for (size_t i = 1; i <= len; i++)
            std::cout << printFunction(data[len - i]) << std::endl;
    }

    void printAll(auto printFunction=[](T x){return x;}) {
        ConnectedStack<T> *ptr = this;
        size_t len = currentSize;

        do {
            ptr->print(printFunction, len);

            len = ptr->parent.size;
            ptr = ptr->parent.root;

            std::cout << " fin." << std::endl;
        } while (ptr != nullptr);
    }

    T pop() {
        T result = front();

        if (!empty())
            currentSize--;
        else
            parent.step();

        return result;
    }

    size_t size() const {
        return currentSize;
    }

    friend std::ostream &operator<<(std::ostream &os, const ConnectedStack &stack) {
        os << "parent: " << stack.parent << " currentSize: " << stack.currentSize
           << " allocated: " << stack.allocated;
        return os;
    }

    ~ConnectedStack() {
        delete data;
    }
};

#endif //SABT_CONNECTED_STACK_H