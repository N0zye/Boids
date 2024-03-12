#pragma once
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <raylib.h>
#include <raymath.h>
#include <vector>

#define PERSEPTION_RADIUS 40
#define AVOIDANCE_RADIUS 20
#define BOID_MAX_SPEED 100

struct Boid {
    Vector2 pos;
    Vector2 vel;
    Boid(float x, float y)
    {
        pos.x = x;
        pos.y = y;
        vel.x = 0; // v0 = vx = BOID_MAX_SPEED
        vel.y = BOID_MAX_SPEED;
    }

    void update(double deltaTime, std::vector<Boid> neighbouringBoids)
    {

        Vector2 acceleration = Vector2Zero();

        acceleration = Vector2Add(acceleration, Vector2Subtract(_calcForce(neighbouringBoids), vel));

        vel = Vector2Add(vel, Vector2Scale(acceleration, deltaTime));
        pos = Vector2Add(pos, Vector2Scale(vel, deltaTime));
        _limitMovment();
    }

    void draw()
    {
        DrawCircle(pos.x, pos.y, 3, BLACK);
    }

private:
    Vector2 _calcForce(std::vector<Boid> neighbouringBoids)
    {
        Vector2 separation = Vector2Zero(), alignment = Vector2Zero(), cohesion = Vector2Zero();
        Vector2 cm = Vector2Zero();
        for (Boid& b : neighbouringBoids) {
            if (Vector2Distance(b.pos, pos) <= AVOIDANCE_RADIUS) {
                separation = Vector2Add(separation, Vector2Subtract(pos, b.pos));
            }
            alignment = Vector2Add(alignment, b.vel);
            cm = Vector2Add(cm, b.pos);
        }

        cm = Vector2Scale(cm, 1 / neighbouringBoids.size());
        cohesion = Vector2Subtract(cm, pos);

        separation = Vector2Normalize(separation);
        alignment = Vector2Normalize(alignment);
        cohesion = Vector2Normalize(cohesion);

        Vector2 final = Vector2Add(separation, alignment);
        final = Vector2Add(final, cohesion);

        final = Vector2ClampValue(Vector2Scale(Vector2Normalize(final), BOID_MAX_SPEED), 0, BOID_MAX_SPEED); // btw the clamp is not needed
        return final;
    }

    void _limitMovment()
    {
        // Check if the object goes out of the screen horizontally
        if (pos.x < 0)
            pos.x = GetScreenWidth() - 1;
        else if (pos.x >= GetScreenWidth())
            pos.x = 0;

        // Check if the object goes out of the screen vertically
        if (pos.y < 0)
            pos.y = GetScreenHeight() - 1;
        else if (pos.y >= GetScreenHeight())
            pos.y = 0;
    }
};

class BoidGroup {
private:
    std::vector<Boid> boids;

    std::vector<Boid> _getNeighbouringBoids(Boid boid)
    {
        std::vector<Boid> neighbouringBoids;
        for (Boid b : boids) {
            if (&b == &boid) {
                continue;
            }
            if (Vector2Distance(boid.pos, b.pos) <= PERSEPTION_RADIUS) {
                neighbouringBoids.push_back(b);
            }
        }
        return neighbouringBoids;
    }

public:
    BoidGroup(int number_of_boids)
    {
        srand(time(NULL));
        for (int i = 0; i < number_of_boids; i++) {
            bool uniquePositionFound = false;
            Vector2 newPos;
            while (!uniquePositionFound) {
                newPos.x = (float)(rand() % GetScreenWidth());
                newPos.y = (float)(rand() % GetScreenHeight());

                // Check if the new position overlaps with existing positions
                bool overlap = false;
                for (const auto& boid : boids) {
                    Vector2 objPos = boid.pos;
                    if (newPos.x == objPos.x && newPos.y == objPos.y) {
                        overlap = true;
                        break;
                    }
                }

                // If no overlap found, add the object with the new position
                if (!overlap) {
                    uniquePositionFound = true;
                }
            }
            boids.emplace_back(newPos.x, newPos.y);
        }
    }

    void draw()
    {
        for (Boid& boid : boids) {
            boid.draw();
        }
    }

    void update()
    {
        double dt = GetFrameTime();
        for (Boid& boid : boids) {
            boid.update(dt, _getNeighbouringBoids(boid));
        }
    }
};
