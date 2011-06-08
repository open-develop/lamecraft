#ifndef CraftWorld_H
#define CraftWorld_H

#include <string>

#include <Aurora/Math/Frustum.h>

#include "Blocks.h"
#include "SimpleMeshChunk2.h"

typedef struct
{
	int distance;
	int chunk;
}TransparentOrder;

class CraftWorld
{
public:

	CraftWorld();
	~CraftWorld();

	block_t& GetBlock (const int x, const int y, const int z) ;
	block_t& GetBlockLight (const int x, const int y, const int z);
	block_t BlockAtPoint(Vector3 pos);

	//map creation
	void initRandompMap(int worldSize,int chunkSize);
	void initRandompMap(int worldSize,int chunkSize,int terrainType,bool makeFlat,bool makeTrees,bool makeWater,bool makeCaves);
	void initWorldBlocksLight();
	void initTrees(int treeChoose);
	void setTextureSize(int texture,int chunk);
	void buildblocksVerts();

	//chunks handling
	void createChunks(const int StartX, const int StartY, const int StartZ);
	void createTransparentChunks(const int StartX, const int StartY, const int StartZ);
	void rebuildChunk(int id);
	void rebuildTransparentChunk(int id);
	void rebuildNearestChunks(int id,Vector3 pos);
	void createWorldChunkss();

	//load/save options
	void SaveWorld(const char *filename);
	void SaveCompressedWorld(std::string filename);
	void LoadWorld(const char *filename);
	void LoadCompressedWorld(std::string filename);

	//lightening
	void InitLightValues();
	void SetWolrdTime(float time);

	//
	void buildMap();
	void destroyMap();

	//rendering
	void drawWorld(Frustum &camFrustum,bool camUpdate);
	void drawCubes(int i);

	//pickin,collision etc
	int groundHeight (const int x, const int z);
	int getChunkId(Vector3 pos);
	int BlockSoundAtPos(Vector3 pos);

	bool SolidAtPointForPlayer(Vector3 pos);
	bool SolidAtPoint(Vector3 pos);
	bool PlayerInWater(Vector3 pos);
	bool BlockTransparent(const int x, const int y, const int z);
	bool BlockEditable(const int x, const int y, const int z);


	int GetDrawntTrianglesCount() { return drawnTriangles; }

	Vector3 GetPlayerPos();
	void UpdatePlayerZoneBB(Vector3 playerPosition);
	void UpdateWorldTime(float dt);



	char worldName[50];
	int createdChunksCount;
	Vector3	fogColor;

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

	std::vector<BaseBlock> blockTypes;

	float *data;

	//player zone aabb
	BoundingBox playerZoneBB;
	Vector3 playerZoneSize;
	Vector3 playerPos;
	int chunksCreatedInFrameCount;
	int transparentchunksCreatedInFrameCount;
	float updateChunkTimer;
	bool updateChunksSwitch;

	float worldTime;

	//information
	int drawnTriangles;

	//transparent order rendering
	TransparentOrder transOrder[512];
	int transOrderCont;
	float lightShadowFactor;
	Vector3 lightFactor;

	//lightining
	Vector3 lightColor;
	Vector3	ambientColor;
	float lightAngle;
	float factor1;
	float factor2;
	float factor3;
};


#endif
