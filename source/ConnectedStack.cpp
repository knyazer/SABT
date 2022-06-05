//
// Created by knyaz on 6/4/2022.
//

#include <ConnectedStack.h>

template<typename T>
void ConnectedStack<T>::StackPointer::step() {
    if (ptr != 0)
        ptr--;
    else {
        // Attempt to go up the tree of stacks (generally only one step)
        while (ptr == 0) {
            assert(root == nullptr && "Do not allow going up for the topmost stack");

            // *this = root->parent , but better?
            ptr = (root->parent)->ptr;
            root = std::make_shared<ConnectedStack<T>>((root->parent).root);
        }
    }
}

template<typename T>
ConnectedStack<T>::ConnectedStack() {
    data = std::make_unique<T>(T(INITIAL_SIZE));
    allocated = INITIAL_SIZE;
    currentSize = 0;
}

template<typename T>
bool ConnectedStack<T>::empty() {
    return currentSize == 0;
}

template<typename T>
void ConnectedStack<T>::grow() {
    // Setup new array
    size_t newSize = (allocated * GROWTH_FACTOR) + 1;
    std::unique_ptr<T> newData = std::make_unique<T>(T(newSize));

    // Copy data
    std::copy(data.get(), data.get() + allocated * sizeof(T), newData.get());

    // Reassign values
    data = newData;
    allocated = newSize;
}

template<typename T>
void ConnectedStack<T>::push(T value) {
    assert(currentSize >= allocated && "Attempt to access not allocated memory. Problems with stack growth");

    data[currentSize] = value;
    currentSize++;

    if (currentSize >= allocated)
        grow();
}

template<typename T>
T ConnectedStack<T>::at(size_t index) {
    assert(index >= 0 && index < size() && "Stack access out of bounds");

    return data[index];
}

template<typename T>
T ConnectedStack<T>::front() {
    if (empty())
        return parent.root.at(parent.ptr - 1);

    assert(currentSize >= 1 && currentSize <= size() && "Stack front is out of bounds. Impossible!");

    return data[currentSize - 1];
}

template<typename T>
T ConnectedStack<T>::pop() {
    T result = front();

    if (!empty())
        currentSize--;
    else
        parent.step();

    return result;
}

template<typename T>
size_t ConnectedStack<T>::size() {
    return currentSize;
}
