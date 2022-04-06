#pragma once

#include "shapes/Rectangle.h"

class ShapeRenderer
{
private:

public:

	ShapeRenderer()
	{

	}

	~ShapeRenderer()
	{

	}

	void begin();
	void draw(Rectangle rect);
	void end();
};

