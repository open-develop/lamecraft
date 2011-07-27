#ifndef WorldGenerator_H
#define WorldGenerator_H

class CraftWorld;

class WorldGenerator
{
	public:
		void initRandompMap(int worldSize,int chunkSize, CraftWorld *world);
		void initParametricRandompMap(int worldSize,int chunkSize, CraftWorld *world, int terrainType,bool makeFlat,bool makeTrees,bool makeWater,bool makeCaves);
		void initTree(int treeChoose, CraftWorld *world, int x, int y, int z);
		
		void reshapeChunk(CraftWorld *world, int terrainType, int x, int z, bool makeTrees);
		
		void initCactus(CraftWorld *world, int x, int y, int z);

};

#endif
