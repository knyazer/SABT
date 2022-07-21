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
constexpr double N = 80;
constexpr double Nh = N / 2;

int main(int argc, char* args[]) {
    Renderer renderer;

    Camera cam;
    cam.setPosition({0, 0, 0});
    cam.setRotationByX(Angle::deg(180));
    cam.setRotationByY(Angle::deg(0));
    cam.setRotationByZ(Angle::deg(0));
    cam.setFOV(Angle::deg(80));

    renderer.createWindow("SABT demo", Rect(500, 500, 800, 800));

    OctreeRoot world;
    for (int i = 0; i < 14; i++)
        world.grow();
    for (int i = 8; i < 1000; i+= 8) {
        world.fill({0, 0, 8 + i}, 2, GREEN);
        world.fill({0, 0, 12 + i}, 2, BLUE);
    }


    // Main render cycle
    while (renderer.update()) {
        renderer.clear(GRAY);

        ll perfCounterTotal = 0, perfCounterFilled = 0, filledPixels = 0;

        for (int xi = 0; xi < N; xi++) {
            for (int yi = 0; yi < N; yi++) {
                ll perfCounter = 0;

                AlignedRect rect({static_cast<double>(xi - Nh) / Nh, static_cast<double>(yi - Nh) / Nh},
                                 {static_cast<double>((xi + 1) - Nh) / Nh, static_cast<double>((yi + 1) - Nh) / Nh });

                // Do we need to fill the current "pixel"?
                bool fill = false;
                Color color = GREEN;

                std::stack<OctreeBase*> stack;
                stack.push(&world);
                while (!stack.empty()) {
                    auto rawNode = stack.top();
                    stack.pop();

                    if (rawNode == nullptr)
                        throw std::runtime_error("Pointer to BaseOctree in call stack is nullptr");

                    // if node is full - finish, returning this node
                    if (rawNode->isFull()) {
                        color = rawNode->getColor(0);
                        fill = true;
                        break;
                    }

                    // If node is not full, it means it is not the OctreeUnit derived, so we can cast the Octree*.
                    auto node = dynamic_cast<Octree*>(rawNode);

                    // ignore if the node is empty
                    if (node->isEmpty())
                        continue;


                    // quick adequacy test
                    if (!node->hasChildren()) {
                        throw std::runtime_error("Cannot have no children while being SEMI.");
                    }

                    // if node is semi - continue iteration, push all the children sorted by the distance to node
                    // for each node check that it intersects with current beam (thick ray), and if so - push it to stack
                    Cube rootCube = world.getCubeFor(node);

                    using S_T = std::pair<std::pair<OctreeBase*, Cube>, double>;
                    std::vector<S_T> sorted;
                    for (size_t i = 0; i < 8; i++) {
                        OctreeBase* child = node->getChild(i);
                        if (child->isEmpty())
                            continue;

                        Cube cube = Octree::getCubeForChild(rootCube, i);
                        double distance = (Vec3f(cube.pos) - cam.getPosition()).size();

                        sorted.push_back({{child, cube}, distance});
                    }

                    std::sort(sorted.begin(), sorted.end(), [](const S_T& A, const S_T &B) {
                       return A.second < B.second;
                    });

                    for (auto zipped : sorted) {

                        perfCounter++;

                        auto *child = zipped.first.first;
                        Cube cube = zipped.first.second;

                        auto *projected = new Vec2f[8];
                        size_t pointsNumber = 0;
                        Vec3i* vertices = cube.getVertices();
                        for (size_t j = 0; j < 8; j++) {
                            Vec3i &vertex = vertices[j];
                            Vec2f proj = cam.project(Vec3f(vertex.x, vertex.y, vertex.z)); // TODO: add none project, not 42 42
                            if (proj.x != 42 || proj.y != 42)
                                projected[pointsNumber++] = proj;
                        }

                        if (pointsNumber == 0)
                            continue;

                        AlignedRect box(projected, pointsNumber);

                        if (rect.intersects(box) && GJK::GJK(rect, Polygon(projected, pointsNumber))) {
                            stack.push(child);
                            break;
                        }
                    }
                }

                perfCounterTotal += perfCounter;
                if (fill) {
                    Vec2f mid = rect.min;
                    Vec2f dim = rect.max - rect.min;
                    auto tr = [](double x) {return round(x * 400 + 400);};
                    renderer.drawRect(Rect(tr(mid.x), tr(mid.y), 800 / N, 800 / N), color);

                    perfCounterFilled += perfCounter;
                    filledPixels += 1;
                }
            }
        }

        std::cout << "total iterations: " << perfCounterTotal << std::endl;
        std::cout << "iterations per filled: " <<
            (filledPixels == 0 ? 0 : static_cast<double>(perfCounterFilled / filledPixels)) << std::endl;

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
        cam.rotateByX(Angle::deg(static_cast<double>(delta.y) / 10));
        cam.rotateByY(Angle::deg(static_cast<double>(delta.x) / 10));
    }

    return 0;
}