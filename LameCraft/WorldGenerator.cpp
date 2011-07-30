#include "WorldGenerator.h"
#include "CraftWorld2.h"

#include <noisepp/Noise.h>
#include <noisepp/NoiseUtils.h>
#include <noisepp/NoiseBuilders.h>

void WorldGenerator::initRandompMap(int worldSize,int chunkSize, CraftWorld *world)
{
	int terrainType = 0;//rand() % 3;//0-2

	int testterrain = rand() % 10;

	if(testterrain < 4)
		terrainType = 0;
	if(testterrain >= 4 && testterrain < 7)
		terrainType = 1;
	if(testterrain >=7)
		terrainType = 2;
	
	initParametricRandompMap(worldSize, chunkSize, world, terrainType, false, true, true, true);
}

void WorldGenerator::initParametricRandompMap(int worldSize,int chunkSize, CraftWorld *world, int terrainType,bool makeFlat,bool makeTrees,bool makeWater,bool makeCaves)
{
	//inicjuj mape
	int WORLD_SIZE = worldSize;
	int CHUNK_SIZE = chunkSize;

	//srand(time(NULL));
	int seed = rand() % 10000;

	//terrain settings for default terrain
	unsigned char grass = 1;
	unsigned char rock = 3;
	unsigned char dirt = 2;
	unsigned char water = 4;
	unsigned char sandUnderWater = 7;

	if(terrainType == 1)//winter
	{
		grass = 26;
		rock = 3;
		dirt = 2;
		water = 32;
		sandUnderWater = 29;
	}

	if(terrainType == 2)//desert
	{
		grass = SandBlock::getID();
		rock = 3;
		dirt = DrySandBlock::getID();
		water = 0;//there is no water on desert ;)
		sandUnderWater = SandBlock::getID();
	}

	float *data = new float[worldSize * worldSize];

	//
	if(!makeFlat)
	{
		noisepp::PerlinModule perlin;
		perlin.setSeed(seed);
		perlin.setQuality(noisepp::NOISE_QUALITY_FAST_STD);

		//data = new float[worldSize * worldSize];



		noisepp::utils::PlaneBuilder2D builder;
		builder.setModule(perlin);
		builder.setSize(worldSize, worldSize);
		if(terrainType == 2)
			builder.setBounds(0.0, 0.0, 3.0, 3.0);
		else
			builder.setBounds(0.0, 0.0, 4.0, 4.0);
		builder.setDestination(data);
		builder.build ();

		int Height = 0;

		//build map
		for (int z = 0; z < WORLD_SIZE; ++z)
		{
			for (int x = 0; x < WORLD_SIZE; ++x)
			{
				Height = data[x + z*WORLD_SIZE]* WORLD_SIZE/12 + WORLD_SIZE/2;
				//int Height = m_HeightMap.GetValue(x, z) * WORLD_SIZE/12 + WORLD_SIZE/2;

				for (int y = 0; y < Height; ++y)
				{
					if(y == Height-1)
						world->GetBlock(x, y, z) = grass;//grass
					else if(y < Height-3)
						world->GetBlock(x, y, z) = rock;
					else
						world->GetBlock(x, y, z) = dirt;
				}

			}
		}

		//carve terrain

		/*
		float *data2 = new float[worldSize * worldSize];
		float *data3 = new float[worldSize * worldSize];

		perlin.setSeed(seed+1);
		noisepp::utils::PlaneBuilder2D builder2;
		builder2.setModule(perlin);
		builder2.setSize(worldSize, worldSize);
		builder2.setBounds(0.0, 0.0, 4.0, 4.0);
		builder2.setDestination(data2);
		builder2.build ();

		perlin.setSeed(seed+2);
		noisepp::utils::PlaneBuilder2D builder3;
		builder3.setModule(perlin);
		builder3.setSize(worldSize, worldSize);
		builder3.setBounds(0.0, 0.0, 4.0, 4.0);
		builder3.setDestination(data3);
		builder3.build ();

		int height1= 0;
		int height2= 0;

		for (int z = 0; z < WORLD_SIZE; ++z)
		{
			for (int x = 0; x < WORLD_SIZE; ++x)
			{
				height1 = data2[x + z*WORLD_SIZE]* 10 + WORLD_SIZE/2;
				height2 = data3[x + z*WORLD_SIZE]* 12 + WORLD_SIZE/2;

				if (height2 > height1)
				{
					//put at height1 grass
					if(world->GetBlock(x, height1, z) != 0)
						world->GetBlock(x, height1, z) = grass;

					//delete blocks
					for (int y = height1+1; y < height2; y++)
					{
						world->GetBlock(x, y, z) = 0;
					}
				}
			}
		}


		//delete tempdata
		delete []data2;
		delete []data3;*/

	}else
	{
		for (int z = 0; z < WORLD_SIZE; ++z)
		{
			for (int x = 0; x < WORLD_SIZE; ++x)
			{
				int Height  = 64;

				for (int y = 0; y < Height; ++y)
				{
					if(y == Height-1)
						world->GetBlock(x, y, z) = grass;//grass
					else if(y < Height-3)
						world->GetBlock(x, y, z) = rock;
					else
						world->GetBlock(x, y, z) = dirt;
				}
			}
		}
	}

	//caves?
	if(makeCaves)
	{
		noisepp::RidgedMultiModule NoiseSource;
		NoiseSource.setSeed(seed);//wygl¹da ciekawie
		NoiseSource.setQuality(noisepp::NOISE_QUALITY_FAST_STD);
		NoiseSource.setOctaveCount(4);
		NoiseSource.setFrequency(1.5f);//im mniejsza tym bardziej gêste i wiêksze 1.9
		NoiseSource.setLacunarity(2.5f);//im wiêksza liczba tym rzadsze obiekty 2.2

		noisepp::Pipeline3D *pipeline = new noisepp::Pipeline3D;
		noisepp::ElementID id = NoiseSource.addToPipeline(pipeline);
		noisepp::PipelineElement3D *element = pipeline->getElement(id);
		noisepp::Cache *cache = pipeline->createCache ();

		float nx, ny, nz;

		for (int z = 2; z < WORLD_SIZE - 4; ++z)
		{
			for (int x = 2; x < WORLD_SIZE - 4; ++x)
			{
				for (int y = 2; y < WORLD_SIZE/2; ++y)
				{
					nx = (float)x / WORLD_SIZE;
					ny = (float)y / WORLD_SIZE;
					nz = (float)z / WORLD_SIZE;

					if(element->getValue(nx,ny,nz,cache) > 0.8f)
						world->GetBlock(x,y,z) = 0;
				}
			}
		}

		 pipeline->freeCache (cache);
		 cache = 0;
		 delete pipeline;
		 pipeline = 0;
	}


	//watering
	if(makeWater)
	{
		int waterLevel = 58;

		for (int z = 0; z < WORLD_SIZE; ++z)
		{
			for (int x = 0; x < WORLD_SIZE; ++x)
			{
				int Height = data[x + z*WORLD_SIZE]* WORLD_SIZE/12 + WORLD_SIZE/2;
				//int Height = m_HeightMap.GetValue(x, z) * WORLD_SIZE/12 + WORLD_SIZE/2;

				if(Height < waterLevel)
				{
					world->GetBlock(x, Height-1, z) = sandUnderWater;//sand under water

					for (int y = Height;y < waterLevel;y++)
					{
						world->GetBlock(x, y, z) = water;//water
					}

				}else if(Height == waterLevel)
				{
					world->GetBlock(x, Height-1, z) = sandUnderWater;//beach sand
				}
			}
		}
	}

	delete []data;

	//make the most bottom layer of cube not destroyable
	for (int z = 0; z < WORLD_SIZE; ++z)
	{
		for (int x = 0; x < WORLD_SIZE; ++x)
		{
			world->GetBlock(x, 0, z) = IronBlock::getID();//last one in set
		}
	}
	
	int chunkx = WORLD_SIZE/CHUNK_SIZE;
	int chunkz = chunkx;
	
	for(int x = 0; x < chunkx; x++)
		for(int z = 0; z < chunkz; z++)
			reshapeChunk(world, terrainType, x, z, makeTrees);
}

void WorldGenerator::reshapeChunk(CraftWorld *world, int terrainType, int tx, int tz, bool makeTrees)
{
    int x = tx * 16;
    int z = tz * 16;
	
    int num = 0;
    if(terrainType == 2) num += 10;
    for(int i = 0; i < num; i++)
    {
        int nx = x + rand()%16 + 8;
        int ny = rand()%128;
        int nz = z + rand()%16 + 8;
		initCactus(world, nx, ny, nz);
    }
	num = 0;
	if(terrainType != 2 && makeTrees) num += 3;
    for(int i = 0; i < num; i++)
    {
        int nx = x + rand()%16;
		int nz = z + rand()%16;
        int ny = world->groundHeight(nx, nz);
		if(ny <= 0) continue;
		initTree(terrainType, world, nx, ny, nz);
    }
	
}

void WorldGenerator::initCactus(CraftWorld *world, int x, int y, int z)
{
	for(int i = 0; i < 10; i++)
    {
        int nx = (x + rand()%8) - rand()%8;
        int ny = (y + rand()%4) - rand()%4;
        int nz = (z + rand()%8) - rand()%8;
        if(world->GetBlock(nx, ny, nz) != 0) continue;
        int height = 1 + rand()%(rand()%3 + 1);
        for(int j = 0; j < height; j++)
			if(blockTypes[CactusBlock::getID()]->canStay(world, nx, ny + j, nz)) world->GetBlock(nx, ny + j, nz) = CactusBlock::getID();
    }
}

void WorldGenerator::initTree(int treeChoose, CraftWorld *world, int x, int y, int z)
{
	int WORLD_SIZE =128;
	int TrunkHeight = 4;//rand() % 5 + 4;

	//check if there is an water on there or another tree
	int flag = 1;
	for (int y1 = y; y1 < y + 1 + TrunkHeight && y1 < WORLD_SIZE; ++y1)
	{
		if(world->GetBlock(x, y1, z) == 4 || world->GetBlock(x, y1, z) == 3 || world->GetBlock(x, y1, z) == 8 || world->GetBlock(x, y1, z) == 9 || world->GetBlock(x, y1, z) == 31 || world->GetBlock(x, y1, z) == 38 || world->GetBlock(x, y1, z) == 25 || world->GetBlock(x, y1, z) == 32)
		flag = 0;
	}

	//if(flag == 0)continue;
	if(flag == 1)
	{
		//Create the tree trunk
		for (int y1 = y + 1; y1 < y + 1 + TrunkHeight && y1 < WORLD_SIZE; ++y1)
		{
			block_t& Block = world->GetBlock(x, y1, z);
			if (Block == 0)
			{
				if(treeChoose == 0) Block = 8;
				else Block = 31;
			}
		}

		//create my leaves
		for(int yy = 0;yy < 3;yy++)
			for(int xx = 0;xx < 3;xx++)
				for(int zz = 0;zz < 3;zz++)
				{
					int x1 = xx + x - 1;// :D - what a shitty code
					int y1 = yy + y + TrunkHeight - 1;
					int z1 = zz + z - 1;

					if (x1 < 0 || y1 < 0 || z1 < 0) continue;
					if (x1 >= WORLD_SIZE || y1 >= WORLD_SIZE || z1 >= WORLD_SIZE) continue;

					block_t& Block = world->GetBlock(x1, y1, z1);
					if (Block == 0)
					{
						if(treeChoose == 0) Block = 9;
						else Block = 38;
					}
				}
	}
}
