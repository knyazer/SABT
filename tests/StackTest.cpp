//
// Created by knyaz on 6/5/2022.
//

#include "gtest/gtest.h"

#include <ConnectedStack.h>
#include <stdexcept>

#define N 10000
#define M 5

TEST(Stack, PushPopTest) {
    ConnectedStack<size_t> stack;

    for (size_t i = 0; i < N; i++) {
        stack.push(i);
        ASSERT_EQ(stack.pop(), i);
    }
}

TEST(Stack, ManyPushManyPopTest) {
    ConnectedStack<size_t> stack;

    for (size_t i = 0; i < N; i++) {
        stack.push(i);
    }

    for (size_t i = 0; i < N; i++) {
        ASSERT_EQ(stack.pop(), N - i - 1);
    }
}

TEST(Stack, BasicThrowTest) {
    ConnectedStack<size_t> stack;

    EXPECT_THROW({
                     try {
                         stack.front();
                     }
                     catch (const std::exception &e) {
                         EXPECT_STREQ("Parent is nullptr and the stack is empty. Can't use 'front'.", e.what());
                         throw;
                     }
                 }, std::exception);

    EXPECT_THROW({
                     try {
                         stack.at(5);
                     }
                     catch (const std::exception &e) {
                         EXPECT_STREQ("Attempt to access not available memory via 'at' operation.", e.what());
                         throw;
                     }
                 }, std::exception);

    EXPECT_THROW({
                     try {
                         stack.at(30);
                     }
                     catch (const std::exception &e) {
                         EXPECT_STREQ("Attempt to access not available memory via 'at' operation.", e.what());
                         throw;
                     }
                 }, std::exception);
}

TEST(Stack, SequentialMinimumSizeConnectionTest) {
    auto current = new ConnectedStack<size_t>();

    for (size_t i = 0; i < N; i++) {
        current->push(i);

        auto newStack = new ConnectedStack<size_t>();
        newStack->connect(std::make_shared<ConnectedStack<size_t>>(*current), 0);

        current = newStack;
    }

    current->push(N);

    for (size_t i = 0; i <= N; i++) {
        ASSERT_EQ(current->pop(), N - i);
    }
}

TEST(Stack, SequentialConnectionTest) {
    auto current = new ConnectedStack<size_t>();

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            current->push(i * (j + 1));
        }

        auto newStack = new ConnectedStack<size_t>();
        newStack->connect(std::make_shared<ConnectedStack<size_t>>(*current), M - 1);

        current = newStack;
    }

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            ASSERT_EQ(current->pop(), (N - i - 1) * (M - j));
        }
    }
}

TEST(Stack, SequentialMiddleConnectionTest) {
    auto current = new ConnectedStack<size_t>();

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            current->push(i * (j + 1));
        }

        auto newStack = new ConnectedStack<size_t>();
        newStack->connect(std::make_shared<ConnectedStack<size_t>>(*current), (size_t) (M / 2));

        current = newStack;
    }

    for (size_t i = 0; i < N; i++) {
        for (size_t j = M / 2 + 1; j >= 1; j--) {
            ASSERT_EQ(current->pop(), (N - i - 1) * j);
        }
    }
}

TEST(Stack, SequentialPushPopConnectionTest) {
    auto current = new ConnectedStack<size_t>();

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            current->push(i * (j + 1));
        }

        for (size_t j = 0; j < (M - 2); j++) {
            ASSERT_EQ(current->pop(), i * (M - j));
        }

        auto newStack = new ConnectedStack<size_t>();
        newStack->connect(std::make_shared<ConnectedStack<size_t>>(*current), 1);

        current = newStack;
    }

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 2; j >= 1; j--) {
            ASSERT_EQ(current->pop(), (N - i - 1) * j);
        }

        for (size_t j = 0; j < M; j++) {
            current->push(42 * i * j);
        }

        for (size_t j = 0; j < M; j++) {
            ASSERT_EQ(current->pop(), 42 * i * (M - j - 1));
        }
    }
}

TEST(Stack, OneRootManyNodesConnectionTest) {
    auto root = new ConnectedStack<size_t>();
    root->push(42);

    ConnectedStack<size_t> nodes[N];

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            nodes[i].push((i + 1) * (j + 1));
        }

        nodes[i].connect(std::make_shared<ConnectedStack<size_t>>(*root), 0);
    }

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            ASSERT_EQ(nodes[i].pop(), (i + 1) * (M - j));
        }

        ASSERT_EQ(nodes[i].pop(), 42);

        ASSERT_ANY_THROW(nodes[i].pop());
    }
}