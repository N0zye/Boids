#include "boid.h"
#include <raylib.h>

int WinMain()
{

    InitWindow(1024, 512, "Boids");
    SetTargetFPS(60);

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = Vector3 { 0.0f, 10.0f, 10.0f };
    camera.target = Vector3 { 0.0f, 0.0f, 0.0f };
    camera.up = Vector3 { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    BoidGroup bg(300);
    while (!WindowShouldClose()) {
        bg.update();
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        bg.draw();
        EndMode3D();

        DrawFPS(GetScreenWidth() - 90, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
