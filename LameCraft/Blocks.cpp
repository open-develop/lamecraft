#include "Blocks.h"


GrassBlock::GrassBlock()
{
	ID = 1;

	textureRow = 0;

	upPlane = 0;
	downPlane = 2;
	sidePlane = 3;
	editable = true;
	transparent = false;
	soundType = 0;
}


DirtBlock::DirtBlock()
{
	ID = 2;

	textureRow = 0;

	upPlane = 2;
	downPlane = 2;
	sidePlane = 2;
	editable = true;
	transparent = false;
	soundType = 1;
}


RockBlock::RockBlock()
{
	ID = 3;

	textureRow = 1;

	upPlane = 0;
	downPlane = 0;
	sidePlane = 0;
	editable = true;
	transparent = false;
	soundType = 2;
}



WaterBlock::WaterBlock()
{
	ID = 4;

	textureRow = 3;

	upPlane = 2;
	downPlane = 2;
	sidePlane = 2;
	editable = true;
	transparent = true;
	soundType = -1;
}



GoldBlock::GoldBlock()
{
	ID = 5;

	textureRow = 6;

	upPlane = 2;
	downPlane = 2;
	sidePlane = 2;
	editable = true;
	transparent = false;
	soundType = 2;
}



LadderBlock::LadderBlock()
{
	ID = 6;

	textureRow = 3;

	upPlane = 0;
	downPlane = 0;
	sidePlane = 0;
	editable = true;
	transparent = true;
	soundType = 3;
	//w sumie mo¿na go zrobiæ przezroczystym...
}



SandBlock::SandBlock()
{
	ID = 7;

	textureRow = 1;

	upPlane = 2;
	downPlane = 2;
	sidePlane = 2;
	editable = true;
	transparent = false;
	soundType = 1;
}



WoodBlock::WoodBlock()
{
	ID = 8;

	textureRow = 2;

	upPlane = 1;
	downPlane = 1;
	sidePlane = 2;
	editable = true;
	transparent = false;
	soundType = 3;
}


LeavesBlock::LeavesBlock()
{
	ID = 9;

	textureRow = 2;

	upPlane = 3;
	downPlane = 3;
	sidePlane = 3;
	editable = true;
	transparent = true;
	soundType = 0;
}



TNTBlock::TNTBlock()
{
	ID = 10;

	textureRow = 4;

	upPlane = 3;
	downPlane = 3;
	sidePlane = 3;
	editable = true;
	transparent = false;
	soundType = 3;
}



BlackWoolBlock::BlackWoolBlock()
{
	ID = 11;

	textureRow = 3;

	upPlane = 6;
	downPlane = 6;
	sidePlane = 6;
	editable = true;
	transparent = false;
	soundType = 3;
}



RedWoolBlock::RedWoolBlock()
{
	ID = 12;

	textureRow = 1;

	upPlane = 4;
	downPlane = 4;
	sidePlane = 4;
	editable = true;
	transparent = false;
	soundType = 3;
}



DarkGreyWoolBlock::DarkGreyWoolBlock()
{
	ID = 13;

	textureRow = 4;

	upPlane = 5;
	downPlane = 5;
	sidePlane = 5;
	editable = true;
	transparent = false;
	soundType = 3;
}




BrownWoolBlock::BrownWoolBlock()
{
	ID = 14;

	textureRow = 3;

	upPlane = 4;
	downPlane = 4;
	sidePlane = 4;
	editable = true;
	transparent = false;
	soundType = 3;
}



LightBlackWoolBlock::LightBlackWoolBlock()
{
	ID = 15;

	textureRow = 3;

	upPlane = 7;
	downPlane = 7;
	sidePlane = 7;
	editable = true;
	transparent = false;
	soundType = 3;
}




PinkWoolBlock::PinkWoolBlock()
{
	ID = 16;

	textureRow = 1;

	upPlane = 5;
	downPlane = 5;
	sidePlane = 5;
	editable = true;
	transparent = false;
	soundType = 3;
}



LightGreenWoolBlock::LightGreenWoolBlock()
{
	ID = 17;

	textureRow = 4;

	upPlane = 6;
	downPlane = 6;
	sidePlane = 6;
	editable = true;
	transparent = false;
	soundType = 3;
}


YellowWoolBlock::YellowWoolBlock()
{
	ID = 18;

	textureRow = 3;

	upPlane = 5;
	downPlane = 5;
	sidePlane = 5;
	editable = true;
	transparent = false;
	soundType = 3;
}


BlueWoolBlock::BlueWoolBlock()
{
	ID = 19;

	textureRow = 0;

	upPlane = 6;
	downPlane = 6;
	sidePlane = 6;
	editable = true;
	transparent = false;
	soundType = 3;
}


LightBlueWoolBlock::LightBlueWoolBlock()
{
	ID = 20;

	textureRow = 0;

	upPlane = 7;
	downPlane = 7;
	sidePlane = 7;
	editable = true;
	transparent = false;
	soundType = 3;
}


FioletWoolBlock::FioletWoolBlock()
{
	ID = 21;

	textureRow = 1;

	upPlane = 6;
	downPlane = 6;
	sidePlane = 6;
	editable = true;
	transparent = false;
	soundType = 3;
}


PastelWoolBlock::PastelWoolBlock()
{
	ID = 22;

	textureRow = 2;

	upPlane = 6;
	downPlane = 6;
	sidePlane = 6;
	editable = true;
	transparent = false;
	soundType = 3;
}


LightFioletWoolBlock::LightFioletWoolBlock()
{
	ID = 23;

	textureRow = 1;

	upPlane = 7;
	downPlane = 7;
	sidePlane = 7;
	editable = true;
	transparent = false;
	soundType = 3;
}


OrangeWoolBlock::OrangeWoolBlock()
{
	ID = 24;

	textureRow = 2;

	upPlane = 7;
	downPlane = 7;
	sidePlane = 7;
	editable = true;
	transparent = false;
	soundType = 3;
}


CactusBlock::CactusBlock()
{
	ID = 25;

	textureRow = 5;

	upPlane = 0;
	downPlane = 2;
	sidePlane = 1;
	editable = true;
	transparent = true;
	soundType = 3;
}

AllSnowBlock::AllSnowBlock()
{
	ID = 26;

	textureRow = 0;

	upPlane = 4;
	downPlane = 4;
	sidePlane = 4;
	editable = true;
	transparent = false;
	soundType = 0;
}


BrickBlock::BrickBlock()
{
	ID = 27;

	textureRow = 4;

	upPlane = 7;
	downPlane = 7;
	sidePlane = 7;
	editable = true;
	transparent = false;
	soundType = 2;
}


CageBlock::CageBlock()
{
	ID = 28;

	textureRow = 6;

	upPlane = 1;
	downPlane = 1;
	sidePlane = 1;
	editable = true;
	transparent = true;
	soundType = 2;
}



SnowBlock::SnowBlock()
{
	ID = 29;

	textureRow = 0;

	upPlane = 4;
	downPlane = 2;
	sidePlane = 5;
	editable = true;
	transparent = false;
	soundType = 0;
}




DarkWoodBlock::DarkWoodBlock()
{
	ID = 30;

	textureRow = 2;

	upPlane = 1;
	downPlane = 1;
	sidePlane = 4;
	editable = true;
	transparent = false;
	soundType = 3;
}


WhiteWoodBlock::WhiteWoodBlock()
{
	ID = 31;

	textureRow = 2;

	upPlane = 1;
	downPlane = 1;
	sidePlane = 5;
	editable = true;
	transparent = false;
	soundType = 3;
}


IceBlock::IceBlock()
{
	ID = 32;

	textureRow = 6;

	upPlane = 0;
	downPlane = 0;
	sidePlane = 0;
	editable = true;
	transparent = false;
	soundType = 2;
}

CaneBlock::CaneBlock()
{
	ID = 33;

	textureRow = 5;

	upPlane = 4;
	downPlane = 4;
	sidePlane = 3;
	editable = true;
	transparent = true;
	soundType = 2;
}

BoxBlock::BoxBlock()
{
	ID = 34;

	textureRow = 4;

	upPlane = 0;
	downPlane = 0;
	sidePlane = 0;
	editable = true;
	transparent = false;
	soundType = 3;
}


ShelfBlock::ShelfBlock()
{
	ID = 35;

	textureRow = 4;

	upPlane = 0;
	downPlane = 0;
	sidePlane = 1;
	editable = true;
	transparent = false;
	soundType = 3;
}

WallBlock::WallBlock()
{
	ID = 36;

	textureRow = 4;

	upPlane = 2;
	downPlane = 2;
	sidePlane = 2;
	editable = true;
	transparent = false;
	soundType = 2;
}


DrySandBlock::DrySandBlock()
{
	ID = 37;

	textureRow = 4;

	upPlane = 4;
	downPlane = 4;
	sidePlane = 4;
	editable = true;
	transparent = false;
	soundType = 1;
}


SnowLewesBlock::SnowLewesBlock()
{
	ID = 38;

	textureRow = 2;

	upPlane = 0;
	downPlane = 0;
	sidePlane = 0;
	editable = true;
	transparent = true;
	soundType = 0;
}


MudBlock::MudBlock()
{
	ID = 39;

	textureRow = 5;

	upPlane = 5;
	downPlane = 5;
	sidePlane = 5;
	editable = true;
	transparent = false;
	soundType = 1;
}


GlassBlock::GlassBlock()
{
	ID = 40;

	textureRow = 3;

	upPlane = 1;
	downPlane = 1;
	sidePlane = 1;
	editable = true;
	transparent = true;
	soundType = 2;
}


BrownCoailBlock::BrownCoailBlock()
{
	ID = 41;

	textureRow = 6;

	upPlane = 3;
	downPlane = 3;
	sidePlane = 3;
	editable = true;
	transparent = false;
	soundType = 2;
}

BlackCoailBlock::BlackCoailBlock()
{
	ID = 42;

	textureRow = 6;

	upPlane = 4;
	downPlane = 4;
	sidePlane = 4;
	editable = true;
	transparent = false;
	soundType = 2;
}

BlueCoailBlock::BlueCoailBlock()
{
	ID = 43;

	textureRow = 6;

	upPlane = 5;
	downPlane = 5;
	sidePlane = 5;
	editable = true;
	transparent = false;
	soundType = 2;
}


RedCoailBlock::RedCoailBlock()
{
	ID = 44;

	textureRow = 6;

	upPlane = 6;
	downPlane = 6;
	sidePlane = 6;
	editable = true;
	transparent = false;
	soundType = 2;
}


SlamRockBlock::SlamRockBlock()
{
	ID = 45;

	textureRow = 6;

	upPlane = 7;
	downPlane = 7;
	sidePlane = 7;
	editable = true;
	transparent = false;
	soundType = 2;
}


WaterRockBlock::WaterRockBlock()
{
	ID = 46;

	textureRow = 5;

	upPlane = 6;
	downPlane = 6;
	sidePlane = 6;
	editable = true;
	transparent = false;
	soundType = 2;
}

JackOLatern::JackOLatern()
{
	ID = 47;

	textureRow = 7;

	upPlane = 0;
	downPlane = 1;
	sidePlane = 2;
	editable = true;
	transparent = false;
	soundType = 2;
	lightSource = true;
}


//must be always last

IronBlock::IronBlock()
{
	ID = 48;

	textureRow = 1;

	upPlane = 1;
	downPlane = 1;
	sidePlane = 1;
	editable = false;
	transparent = false;
	soundType = 2;
}

