#include "Blocks.h"


GrassBlock::GrassBlock()
{
	textureRow = 0;

	upPlane = 0;
	downPlane = 2;
	sidePlane = 3;
	editable = true;
	transparent = false;
	soundType = 0;
}

block_t GrassBlock::getID(){ return 1;}

DirtBlock::DirtBlock()
{
	textureRow = 0;

	upPlane = 2;
	downPlane = 2;
	sidePlane = 2;
	editable = true;
	transparent = false;
	soundType = 1;
}

block_t DirtBlock::getID(){ return 2;}


RockBlock::RockBlock()
{
	textureRow = 1;

	upPlane = 0;
	downPlane = 0;
	sidePlane = 0;
	editable = true;
	transparent = false;
	soundType = 2;
}

block_t RockBlock::getID(){ return 3;}


WaterBlock::WaterBlock()
{
	textureRow = 3;

	upPlane = 2;
	downPlane = 2;
	sidePlane = 2;
	editable = true;
	transparent = true;
	soundType = -1;
}

block_t WaterBlock::getID(){ return 4;}

GoldBlock::GoldBlock()
{
	textureRow = 6;

	upPlane = 2;
	downPlane = 2;
	sidePlane = 2;
	editable = true;
	transparent = false;
	soundType = 2;
}

block_t GoldBlock::getID(){ return 5;}

LadderBlock::LadderBlock()
{
	textureRow = 3;

	upPlane = 0;
	downPlane = 0;
	sidePlane = 0;
	editable = true;
	transparent = true;
	soundType = 3;
}
block_t LadderBlock::getID(){ return 6;}


SandBlock::SandBlock()
{
	textureRow = 1;

	upPlane = 2;
	downPlane = 2;
	sidePlane = 2;
	editable = true;
	transparent = false;
	soundType = 1;
}
block_t SandBlock::getID(){ return 7;}


WoodBlock::WoodBlock()
{
	textureRow = 2;

	upPlane = 1;
	downPlane = 1;
	sidePlane = 2;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t WoodBlock::getID(){ return 8;}

LeavesBlock::LeavesBlock()
{
	textureRow = 2;

	upPlane = 3;
	downPlane = 3;
	sidePlane = 3;
	editable = true;
	transparent = true;
	soundType = 0;
}
block_t LeavesBlock::getID(){ return 9;}


TNTBlock::TNTBlock()
{
	textureRow = 4;

	upPlane = 3;
	downPlane = 3;
	sidePlane = 3;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t TNTBlock::getID(){ return 10;}


BlackWoolBlock::BlackWoolBlock()
{
	textureRow = 3;

	upPlane = 6;
	downPlane = 6;
	sidePlane = 6;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t BlackWoolBlock::getID(){ return 11;}


RedWoolBlock::RedWoolBlock()
{
	textureRow = 1;

	upPlane = 4;
	downPlane = 4;
	sidePlane = 4;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t RedWoolBlock::getID(){ return 12;}


DarkGreyWoolBlock::DarkGreyWoolBlock()
{
	textureRow = 4;

	upPlane = 5;
	downPlane = 5;
	sidePlane = 5;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t DarkGreyWoolBlock::getID(){ return 13;}



BrownWoolBlock::BrownWoolBlock()
{
	textureRow = 3;

	upPlane = 4;
	downPlane = 4;
	sidePlane = 4;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t BrownWoolBlock::getID(){ return 14;}


LightBlackWoolBlock::LightBlackWoolBlock()
{
	textureRow = 3;

	upPlane = 7;
	downPlane = 7;
	sidePlane = 7;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t LightBlackWoolBlock::getID(){ return 15;}



PinkWoolBlock::PinkWoolBlock()
{
	textureRow = 1;

	upPlane = 5;
	downPlane = 5;
	sidePlane = 5;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t PinkWoolBlock::getID(){ return 16;}


LightGreenWoolBlock::LightGreenWoolBlock()
{
	textureRow = 4;

	upPlane = 6;
	downPlane = 6;
	sidePlane = 6;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t LightGreenWoolBlock::getID(){ return 17;}

YellowWoolBlock::YellowWoolBlock()
{
	textureRow = 3;

	upPlane = 5;
	downPlane = 5;
	sidePlane = 5;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t YellowWoolBlock::getID(){ return 18;}

BlueWoolBlock::BlueWoolBlock()
{
	textureRow = 0;

	upPlane = 6;
	downPlane = 6;
	sidePlane = 6;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t BlueWoolBlock::getID(){ return 19;}

LightBlueWoolBlock::LightBlueWoolBlock()
{
	textureRow = 0;

	upPlane = 7;
	downPlane = 7;
	sidePlane = 7;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t LightBlueWoolBlock::getID(){ return 20;}

FioletWoolBlock::FioletWoolBlock()
{
	textureRow = 1;

	upPlane = 6;
	downPlane = 6;
	sidePlane = 6;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t FioletWoolBlock::getID(){ return 21;}

PastelWoolBlock::PastelWoolBlock()
{
	textureRow = 2;

	upPlane = 6;
	downPlane = 6;
	sidePlane = 6;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t PastelWoolBlock::getID(){ return 22;}

LightFioletWoolBlock::LightFioletWoolBlock()
{
	textureRow = 1;

	upPlane = 7;
	downPlane = 7;
	sidePlane = 7;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t LightFioletWoolBlock::getID(){ return 23;}

OrangeWoolBlock::OrangeWoolBlock()
{
	textureRow = 2;

	upPlane = 7;
	downPlane = 7;
	sidePlane = 7;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t OrangeWoolBlock::getID(){ return 24;}

CactusBlock::CactusBlock()
{
	textureRow = 5;

	upPlane = 0;
	downPlane = 2;
	sidePlane = 1;
	editable = true;
	transparent = true;
	soundType = 3;
}
block_t CactusBlock::getID(){ return 25;}

AllSnowBlock::AllSnowBlock()
{
	textureRow = 0;

	upPlane = 4;
	downPlane = 4;
	sidePlane = 4;
	editable = true;
	transparent = false;
	soundType = 0;
}
block_t AllSnowBlock::getID(){ return 26;}

BrickBlock::BrickBlock()
{
	textureRow = 4;

	upPlane = 7;
	downPlane = 7;
	sidePlane = 7;
	editable = true;
	transparent = false;
	soundType = 2;
}
block_t BrickBlock::getID(){ return 27;}

CageBlock::CageBlock()
{
	textureRow = 6;

	upPlane = 1;
	downPlane = 1;
	sidePlane = 1;
	editable = true;
	transparent = true;
	soundType = 2;
}
block_t CageBlock::getID(){ return 28;}


SnowBlock::SnowBlock()
{
	textureRow = 0;

	upPlane = 4;
	downPlane = 2;
	sidePlane = 5;
	editable = true;
	transparent = false;
	soundType = 0;
}
block_t SnowBlock::getID(){ return 29;}



DarkWoodBlock::DarkWoodBlock()
{
	textureRow = 2;

	upPlane = 1;
	downPlane = 1;
	sidePlane = 4;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t DarkWoodBlock::getID(){ return 30;}

WhiteWoodBlock::WhiteWoodBlock()
{
	textureRow = 2;

	upPlane = 1;
	downPlane = 1;
	sidePlane = 5;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t WhiteWoodBlock::getID(){ return 31;}

IceBlock::IceBlock()
{
	textureRow = 6;

	upPlane = 0;
	downPlane = 0;
	sidePlane = 0;
	editable = true;
	transparent = false;
	soundType = 2;
}
block_t IceBlock::getID(){ return 32;}

CaneBlock::CaneBlock()
{
	textureRow = 5;

	upPlane = 4;
	downPlane = 4;
	sidePlane = 3;
	editable = true;
	transparent = true;
	soundType = 2;
}
block_t CaneBlock::getID(){ return 33;}

BoxBlock::BoxBlock()
{
	textureRow = 4;

	upPlane = 0;
	downPlane = 0;
	sidePlane = 0;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t BoxBlock::getID(){ return 34;}

ShelfBlock::ShelfBlock()
{
	textureRow = 4;

	upPlane = 0;
	downPlane = 0;
	sidePlane = 1;
	editable = true;
	transparent = false;
	soundType = 3;
}
block_t ShelfBlock::getID(){ return 35;}

WallBlock::WallBlock()
{
	textureRow = 4;

	upPlane = 2;
	downPlane = 2;
	sidePlane = 2;
	editable = true;
	transparent = false;
	soundType = 2;
}
block_t WallBlock::getID(){ return 36;}

DrySandBlock::DrySandBlock()
{
	textureRow = 4;

	upPlane = 4;
	downPlane = 4;
	sidePlane = 4;
	editable = true;
	transparent = false;
	soundType = 1;
}
block_t DrySandBlock::getID(){ return 37;}

SnowLewesBlock::SnowLewesBlock()
{
	textureRow = 2;

	upPlane = 0;
	downPlane = 0;
	sidePlane = 0;
	editable = true;
	transparent = true;
	soundType = 0;
}
block_t SnowLewesBlock::getID(){ return 38;}

MudBlock::MudBlock()
{
	textureRow = 5;

	upPlane = 5;
	downPlane = 5;
	sidePlane = 5;
	editable = true;
	transparent = false;
	soundType = 1;
}
block_t MudBlock::getID(){ return 39;}

GlassBlock::GlassBlock()
{
	textureRow = 3;

	upPlane = 1;
	downPlane = 1;
	sidePlane = 1;
	editable = true;
	transparent = true;
	soundType = 2;
}
block_t GlassBlock::getID(){ return 40;}

BrownCoailBlock::BrownCoailBlock()
{
	textureRow = 6;

	upPlane = 3;
	downPlane = 3;
	sidePlane = 3;
	editable = true;
	transparent = false;
	soundType = 2;
}
block_t BrownCoailBlock::getID(){ return 41;}

BlackCoailBlock::BlackCoailBlock()
{
	textureRow = 6;

	upPlane = 4;
	downPlane = 4;
	sidePlane = 4;
	editable = true;
	transparent = false;
	soundType = 2;
}
block_t BlackCoailBlock::getID(){ return 42;}

BlueCoailBlock::BlueCoailBlock()
{
	textureRow = 6;

	upPlane = 5;
	downPlane = 5;
	sidePlane = 5;
	editable = true;
	transparent = false;
	soundType = 2;
}
block_t BlueCoailBlock::getID(){ return 43;}

RedCoailBlock::RedCoailBlock()
{
	textureRow = 6;

	upPlane = 6;
	downPlane = 6;
	sidePlane = 6;
	editable = true;
	transparent = false;
	soundType = 2;
}
block_t RedCoailBlock::getID(){ return 44;}

SlamRockBlock::SlamRockBlock()
{
	textureRow = 6;

	upPlane = 7;
	downPlane = 7;
	sidePlane = 7;
	editable = true;
	transparent = false;
	soundType = 2;
}
block_t SlamRockBlock::getID(){ return 45;}

WaterRockBlock::WaterRockBlock()
{
	textureRow = 5;

	upPlane = 6;
	downPlane = 6;
	sidePlane = 6;
	editable = true;
	transparent = false;
	soundType = 2;
}
block_t WaterRockBlock::getID(){ return 46;}

JackOLatern::JackOLatern()
{
	textureRow = 7;

	upPlane = 0;
	downPlane = 1;
	sidePlane = 2;
	editable = true;
	transparent = false;
	soundType = 2;
	lightSource = true;
}
block_t JackOLatern::getID(){ return 47;}

//must be always last

IronBlock::IronBlock()
{
	textureRow = 1;

	upPlane = 1;
	downPlane = 1;
	sidePlane = 1;
	editable = false;
	transparent = false;
	soundType = 2;
}
block_t BaseBlock::getID(){ return 48;}
