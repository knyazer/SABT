//
// Created by knyaz on 6/8/2022.
//
// Simple enum defining possible types of filling of a particular octree

#ifndef SABT_FILLING_H
#define SABT_FILLING_H

enum Filling {
    FULL, // Means that node has no EMPTY or SEMI children inside it
    EMPTY, // Means that all the children are EMPTY
    SEMI, // All other configurations
};

#endif //SABT_FILLING_H
