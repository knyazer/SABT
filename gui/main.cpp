//
// Created by knyaz on 7/3/2022.
//
#include <stdio.h>
#include <Renderer.h>

using namespace graphics;

int main(int argc, char* args[]) {
    Renderer renderer;

    renderer.init();
    renderer.createWindow("SABT", Rect(500, 500, 800, 800));

    for (int i = 0; i < 1000; i++) {
        renderer.clear(GRAY);
        renderer.drawRect(Rect(100 + i, 100, 100, 100), BLUE);
        renderer.update();

        if (renderer.isStopped())
            break;
    }

    return 0;
}