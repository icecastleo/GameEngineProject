#pragma once

#include <vector>
class Entity;

class TransformSystem
{
public:
	TransformSystem();
	~TransformSystem();

	void update(float delta, std::vector<Entity *> entities);
};

