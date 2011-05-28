#ifndef CraftWorld_H
#define CraftWorld_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>


//framework
#include <Aurora/Math/Frustum.h>

#include "SimpleMeshChunk2.h"
//#include "NoiseMap.h"

typedef unsigned char block_t;

class WBlock
{
public:

	block_t ID;

	int textureRow;

	float upPlane;
	float downPlane;
	float sidePlane;

	bool editable;
	bool transparent;

	CraftPSPVertex *vertices;

	int soundType;//0 grass,1 gravel,2 stone,3 wood

	WBlock()
	{
		ID = 0;

		textureRow = 0;

		upPlane = 0;
		downPlane = 0;
		sidePlane = 0;
		editable = false;
		transparent = true;
		vertices = NULL;
		soundType = -1;
	}

	virtual ~WBlock()
	{
		if(vertices != NULL)
		{
			free(vertices);
		}
	}

};

class GrassBlock : public WBlock
{
public:

	GrassBlock()
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

};

class DirtBlock : public WBlock
{
public:

	DirtBlock()
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

};

class RockBlock : public WBlock
{
public:

	RockBlock()
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

};

class WaterBlock : public WBlock
{
public:

	WaterBlock()
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

};

class GoldBlock : public WBlock
{
public:

	GoldBlock()
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

};

class LadderBlock : public WBlock
{
public:

	LadderBlock()
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
};



class SandBlock : public WBlock
{
public:

	SandBlock()
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

};

class WoodBlock : public WBlock
{
public:

	WoodBlock()
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
};

class LeavesBlock : public WBlock
{
public:

	LeavesBlock()
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
};

class TNTBlock : public WBlock
{
public:

	TNTBlock()
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
};

class BlackWoolBlock : public WBlock
{
public:

	BlackWoolBlock()
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
};

class RedWoolBlock : public WBlock
{
public:

	RedWoolBlock()
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
};

class DarkGreyWoolBlock : public WBlock
{
public:

	DarkGreyWoolBlock()
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
};

class BrownWoolBlock : public WBlock
{
public:

	BrownWoolBlock()
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
};

class LightBlackWoolBlock : public WBlock
{
public:

	LightBlackWoolBlock()
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
};

class PinkWoolBlock : public WBlock
{
public:

	PinkWoolBlock()
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
};

class LightGreenWoolBlock : public WBlock
{
public:

	LightGreenWoolBlock()
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
};

class YellowWoolBlock : public WBlock
{
public:

	YellowWoolBlock()
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
};

class BlueWoolBlock : public WBlock
{
public:

	BlueWoolBlock()
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
};

class LightBlueWoolBlock : public WBlock
{
public:

	LightBlueWoolBlock()
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
};

class FioletWoolBlock : public WBlock
{
public:

	FioletWoolBlock()
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
};

class PastelWoolBlock : public WBlock
{
public:

	PastelWoolBlock()
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
};

class LightFioletWoolBlock : public WBlock
{
public:

	LightFioletWoolBlock()
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
};

class OrangeWoolBlock : public WBlock
{
public:

	OrangeWoolBlock()
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
};

class CactusBlock : public WBlock
{
public:

	CactusBlock()
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
};

class AllSnowBlock : public WBlock
{
public:

	AllSnowBlock()
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
};

class BrickBlock : public WBlock
{
public:

	BrickBlock()
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
};

class CageBlock : public WBlock
{
public:

	CageBlock()
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
};

class SnowBlock : public WBlock
{
public:

	SnowBlock()
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

};

class DarkWoodBlock : public WBlock
{
public:

	DarkWoodBlock()
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

};

class WhiteWoodBlock : public WBlock
{
public:

	WhiteWoodBlock()
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

};

class IceBlock : public WBlock
{
public:

	IceBlock()
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

};

class CaneBlock : public WBlock
{
public:

	CaneBlock()
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

};

class BoxBlock : public WBlock
{
public:

	BoxBlock()
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

};

class ShelfBlock : public WBlock
{
public:

	ShelfBlock()
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

};

class WallBlock : public WBlock
{
public:

	WallBlock()
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
};

//
class DrySandBlock : public WBlock
{
public:

	DrySandBlock()
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
};

class SnowLewesBlock : public WBlock
{
public:

	SnowLewesBlock()
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
};

class MudBlock : public WBlock
{
public:

	MudBlock()
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
};

class GlassBlock : public WBlock
{
public:

	GlassBlock()
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
};

class BrownCoailBlock : public WBlock
{
public:

	BrownCoailBlock()
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
};

class BlackCoailBlock : public WBlock
{
public:

	BlackCoailBlock()
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
};

class BlueCoailBlock : public WBlock
{
public:

	BlueCoailBlock()
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
};

class RedCoailBlock : public WBlock
{
public:

	RedCoailBlock()
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
};

class SlamRockBlock : public WBlock
{
public:

	SlamRockBlock()
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
};

class WaterRockBlock : public WBlock
{
public:

	WaterRockBlock()
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
};

//must be always last
class IronBlock : public WBlock
{
public:

	IronBlock()
	{
		ID = 47;

		textureRow = 1;

		upPlane = 1;
		downPlane = 1;
		sidePlane = 1;
		editable = false;
		transparent = false;
		soundType = 2;
	}

};

class CraftWorld
{
public:

	CraftWorld();
	~CraftWorld();

	block_t& GetBlock (const int x, const int y, const int z) ;
	block_t& GetBlockLight (const int x, const int y, const int z);

	bool BlockTransparent(const int x, const int y, const int z);
	bool BlockEditable(const int x, const int y, const int z);

	void initmap(int worldSize,int chunkSize,const char *mapFile);
	void initRandompMap(int worldSize,int chunkSize);
	void initRandompMap(int worldSize,int chunkSize,int terrainType,bool makeFlat,bool makeTrees,bool makeWater,bool makeCaves);
	
	void initWorldBlocksLight();
	void setWorldLightValues(const float WorldTime);

	void initTrees(int treeChoose);

	void setTextureSize(int texture,int chunk);

	void buildMap();
	void destroyMap();

	void buildblocksVerts();

	int createdChunksCount;
	void createChunks(const int StartX, const int StartY, const int StartZ);
	void createTransparentChunks(const int StartX, const int StartY, const int StartZ);

	void rebuildChunk(int id);
	void rebuildTransparentChunk(int id);
	void rebuildNearestChunks(int id,Vector3 pos);

	void createWorldChunkss();

	void drawWorld(Frustum &camFrustum);
	void drawCubes(int i);

	//pickin,collision etc
	int getHeightAt(int x,int z);
	int groundHeight (const int x, const int z);
	int getChunkId(Vector3 pos);

	int BlockSoundAtPos(Vector3 pos);
	bool SolidAtPointForPlayer(Vector3 pos);
	bool SolidAtPoint(Vector3 pos);
	bool PlayerInWater(Vector3 pos);
	block_t BlockAtPoint(Vector3 pos);

	int GetDrawntTrianglesCount() { return drawnTriangles; }

	Vector3 GetPlayerPos();
	void UpdatePlayerZoneBB(Vector3 playerPosition);
	void UpdateWorldTime(float dt);

	void SaveWorld(const char *filename);
	void SaveCompressedWorld(std::string filename);
	
	void LoadWorld(const char *filename);
	void LoadCompressedWorld(std::string filename);

	char worldName[50];

private:

	int WORLD_SIZE;
	int CHUNK_SIZE;



	int texutreSize;
	int textureChunk;
	float percent;

	std::vector<SimpleMeshChunk*> mChunks;
	std::vector<SimpleMeshChunk*> mWaterChunks;


	block_t* m_Blocks;
	block_t* m_BlockLight;


	//NoiseMap		    m_HeightMap;
	unsigned char mineMap[128][128];
	float *data;

	std::vector<WBlock> blockTypes;

	Vector3 m_LightColor;
	Vector3	m_AmbientColor;
	Vector3	m_FogColor;
	float	m_LightAngle;
	float Factor;

	//player zone aabb
	BoundingBox playerZoneBB;
	Vector3 playerZoneSize;
	Vector3 playerPos;
	int chunksCreatedInFrameCount;
	int transparentchunksCreatedInFrameCount;
	float updateChunkTimer;
	bool updateChunksSwitch;

	float worldTime;

	//small kesz
	/*Vector3 BlockColory1;    //Top face
	Vector3 BlockColory2;    //Bottom face
	Vector3 BlockColorx1;    //Sunset face
	Vector3 BlockColorx2;    //Sunrise face
	Vector3 BlockColorz;     //Front/back faces

	Vector3 lastBlockColory1;    //Top face
	Vector3 lastBlockColory2;    //Bottom face
	Vector3 lastBlockColorx1;    //Sunset face
	Vector3 lastBlockColorx2;    //Sunrise face
	Vector3 lastBlockColorz;     //Front/back faces
	float 	lastBaseLight;*/

	//information
	int drawnTriangles;

	//different terrains
	//0 - default,1 - snowy,2 - desert,3 - tropical

};


#endif
