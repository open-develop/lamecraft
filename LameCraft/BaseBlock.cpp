#include "BaseBlock.h"


BaseBlock::BaseBlock()
{
	ID = 0;

	textureRow = 0;
	upPlane = 0;
	downPlane = 0;
	sidePlane = 0;

	vertices = NULL;

	soundType = -1;

	editable = false;
	transparent = true;
	lightSource = false;
}

BaseBlock::~BaseBlock()
{
	if(vertices != NULL)
	{
		free(vertices);
	}
}
