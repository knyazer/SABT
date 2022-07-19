//
// Created by knyaz on 7/3/2022.
//

#include <Renderer.h>
#include <SABT.h>
#include <iostream>
#include <stack>

using namespace graphics;

using std::cout, std::endl;

#define eps 1e-6

int main(int argc, char* args[]) {
    Renderer renderer;

    Camera cam;
    cam.setPosition({0, 0, 0});
    cam.setRotationByX(Angle::deg(0));
    cam.setRotationByY(Angle::deg(0));
    cam.setRotationByZ(Angle::deg(0));
    cam.setFOV(Angle::deg(80));

    renderer.createWindow("SABT", Rect(500, 500, 800, 800));

    OctreeRoot world;
    for (int i = 0; i < 10; i++)
        world.grow();
    world.fill({0, 0, -5}, 0);


    // Main render cycle
    while (renderer.update()) {
        renderer.clear(GRAY);

        for (int xi = 0; xi < 40; xi++) {
            for (int yi = 0; yi < 40; yi++) {
                AlignedRect rect({0.05 * xi - 1, 0.05 * yi - 1},
                                 {0.05 * (xi + 1) - 1, 0.05 * (yi + 1) - 1});

                // Do we need to fill the current "pixel"?
                bool fill = false;

                std::stack<Octree*> stack;
                stack.push(&world);
                while (!stack.empty()) {
                    auto node = stack.top();
                    stack.pop();

                    // ignore if the node is empty
                    if (node->isEmpty())
                        continue;

                    // if node is full - finish, returning this node
                    if (node->isFull()) {
                        fill = true;
                        break;
                    }

                    // quick adequacy test
                    if (!node->hasChildren()) {
                        throw std::runtime_error("Cannot have no children while being SEMI.");
                    }

                    // if node is semi - continue iteration, push all the children sorted by the distance to node
                    // for each node check that it intersects with current beam (thick ray), and if so - push it to stack
                    for (int i = 0; i < 8; i++) { // TODO: make for each cycle for Triplet
                        Octree* child = &node->getChild(i);
                        Cube cube = world.getCubeFor(child);

                        std::vector<Vec2f> projected;
                        for (auto vertex : cube.getVertices()) {
                            projected.push_back(cam.project(vertex)); // TODO: add none project, not 42 42
                        }

                        // TODO: add bounding box test
                        if (GJK::GJK(rect, Polygon(projected)))
                            stack.push(child);
                    }

                }
            }
        }

        // position controls
        if (renderer.pressed['j'])
            cam.move({0, 0, -0.1});
        if (renderer.pressed['k'])
            cam.move({0, 0, 0.1});
        if (renderer.pressed['h'])
            cam.move({0.1, 0, 0});
        if (renderer.pressed['l'])
            cam.move({-0.1, 0, 0});
        if (renderer.pressed['i'])
            cam.move({0, 0.1, 0});
        if (renderer.pressed['m'])
            cam.move({0, -0.1, 0});

        // rotation controls
        auto delta = renderer.getMouseDelta();
        cam.rotateByX(Angle::deg(static_cast<double>(-delta.y) / 10));
        cam.rotateByY(Angle::deg(static_cast<double>(delta.x) / 10));
    }

    return 0;
}