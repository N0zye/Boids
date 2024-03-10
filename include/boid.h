#pragma once
#include <raylib.h>
#include <raymath.h>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <vector>

#define PERSEPTION_RADIUS 35
#define AVOIDANCE_RADIUS 20
#define BOID_MAX_SPEED 100

struct Boid {
	Vector2 pos;
	Vector2 vel;
	Boid(float x, float y) {
		pos.x = x;
		pos.y = y;
		vel.x = 0; //v0 = vx = BOID_MAX_SPEED
		vel.y = BOID_MAX_SPEED;
	}

	void update(double deltaTime, std::vector<Boid> neighbouringBoids) {
	
		Vector2 acceleration = Vector2Zero();

		std::vector<Vector2> forces = _calcForces(neighbouringBoids);
		
		for (int i = 0; i < 3; i++) {
			acceleration = Vector2Add(acceleration, _limitForce(forces.at(i)));
		}
		vel = Vector2Add(vel, Vector2Scale(acceleration, deltaTime));
		pos = Vector2Add(pos, Vector2Scale(vel, deltaTime));
		_limitMovment();
	}

	void draw() {
		DrawCircle(pos.x, pos.y, 3, BLACK);
	}
private:

	std::vector<Vector2> _calcForces(std::vector<Boid> neighbouringBoids){
		std::vector<Vector2> forces(3); // 0 = separation; 1 = Alignment; 2 Cohesion
		for (Boid b : neighbouringBoids) {
			if (Vector2Distance(pos, b.pos) <= AVOIDANCE_RADIUS) { // sepratation 
				forces.at(0) = Vector2Add(forces.at(0), Vector2Scale(Vector2Subtract(pos, b.pos), 2));
			}
			forces.at(1) = Vector2Add(forces.at(1), Vector2Add(vel, b.vel)); // Alignment
			forces.at(2) = Vector2Add(forces.at(2), b.pos); // Cohesion add all the positions
		}
		forces.at(2) = Vector2Scale(forces.at(2), (float)(1/neighbouringBoids.size())); // calc the center of mass
		forces.at(2) = Vector2Subtract(forces.at(2), pos);
		forces.at(2) = { 0, 0 };
		return forces;
	}
	Vector2 _limitForce(Vector2 force) {
		return Vector2Subtract(Vector2Scale(Vector2Normalize(Vector2ClampValue(force, 0, BOID_MAX_SPEED / 3 * 2)), BOID_MAX_SPEED), vel);
	}
	void _limitMovment() {
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

	std::vector<Boid> _getNeighbouringBoids(Boid boid) {
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
	BoidGroup(int number_of_boids) {
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
	void draw() {
		for (Boid &boid : boids) {
			boid.draw();
		}
	}
	void update() {
		double dt = GetFrameTime();
		for (Boid &boid : boids) {
			boid.update(dt, _getNeighbouringBoids(boid));
		}
	}
};