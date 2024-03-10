#include <raylib.h>
#include "boid.h"

int WinMain() {

	InitWindow(1024, 512, "Boids");
	SetTargetFPS(144);
	BoidGroup bg(300);
	while (!WindowShouldClose()){
		bg.update();
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawFPS(GetScreenWidth()-90, 10);
		bg.draw();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}