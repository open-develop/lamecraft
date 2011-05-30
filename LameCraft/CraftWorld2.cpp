#include "CraftWorld2.h"
#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Utils/Logger.h>

#include <string.h>
#include <zlib.h>  

#include <noisepp/Noise.h>
#include <noisepp/NoiseUtils.h>
#include <noisepp/NoiseBuilders.h>

CraftWorld::CraftWorld()
{
	WORLD_SIZE = 0;
	CHUNK_SIZE = 0;
	//
	playerZoneSize = Vector3(48,32,48);
	updateChunksSwitch = false;
	worldTime = 0.0f;
	updateChunkTimer = 0.0f;
	chunksCreatedInFrameCount = 0;
	transparentchunksCreatedInFrameCount = 0;

	//air
	blockTypes.push_back(BaseBlock());

	//normal blocks
	blockTypes.push_back(GrassBlock());
	blockTypes.push_back(DirtBlock());
	blockTypes.push_back(RockBlock());
	blockTypes.push_back(WaterBlock());
	blockTypes.push_back(GoldBlock());
	blockTypes.push_back(LadderBlock());
	blockTypes.push_back(SandBlock());
	blockTypes.push_back(WoodBlock());
	blockTypes.push_back(LeavesBlock());

	blockTypes.push_back(TNTBlock());
	blockTypes.push_back(BlackWoolBlock());
	blockTypes.push_back(RedWoolBlock());
	blockTypes.push_back(DarkGreyWoolBlock());
	blockTypes.push_back(BrownWoolBlock());
	blockTypes.push_back(LightBlackWoolBlock());
	blockTypes.push_back(PinkWoolBlock());
	blockTypes.push_back(LightGreenWoolBlock());
	blockTypes.push_back(YellowWoolBlock());

	blockTypes.push_back(BlueWoolBlock());
	blockTypes.push_back(LightBlueWoolBlock());
	blockTypes.push_back(FioletWoolBlock());
	blockTypes.push_back(PastelWoolBlock());
	blockTypes.push_back(LightFioletWoolBlock());
	blockTypes.push_back(OrangeWoolBlock());
	blockTypes.push_back(CactusBlock());
	blockTypes.push_back(SnowBlock());
	blockTypes.push_back(BrickBlock());

	blockTypes.push_back(CageBlock());
	blockTypes.push_back(AllSnowBlock());
	blockTypes.push_back(DarkWoodBlock());
	blockTypes.push_back(WhiteWoodBlock());
	blockTypes.push_back(IceBlock());
	blockTypes.push_back(CaneBlock());
	blockTypes.push_back(BoxBlock());
	blockTypes.push_back(ShelfBlock());
	blockTypes.push_back(WallBlock());

	blockTypes.push_back(DrySandBlock());
	blockTypes.push_back(SnowLewesBlock());
	blockTypes.push_back(MudBlock());
	blockTypes.push_back(GlassBlock());
	blockTypes.push_back(BrownCoailBlock());
	blockTypes.push_back(BlackCoailBlock());
	blockTypes.push_back(BlueCoailBlock());
	blockTypes.push_back(RedCoailBlock());

	blockTypes.push_back(SlamRockBlock());
	//blockTypes.push_back(WaterRockBlock());

	//can't destroy this one
	blockTypes.push_back(IronBlock());
}

CraftWorld::~CraftWorld()
{
	//delete arrays
	delete [] m_Blocks;
	delete [] m_BlockLight;

	//delete chunks
	for(unsigned int i = 0;i < mChunks.size();i++)
	{
		delete mChunks[i];
	}
	mChunks.clear();

	//water chunks
	for(unsigned int i = 0;i < mWaterChunks.size();i++)
	{
		delete mWaterChunks[i];
	}
	mWaterChunks.clear();


	//delete block vertices
	blockTypes.clear();

}

void CraftWorld::SaveCompressedWorld(std::string  filename)
{
	//first save version and name on normal file
	FILE * pFile;
	pFile = fopen(filename.c_str(),"wb");

	if(pFile == NULL)
		return;

	int version = 2;//save with compression

	//version
	fwrite(&version,sizeof(int),1,pFile);

	//name
	fwrite(worldName,sizeof(char),50,pFile);

	//close file
	fclose(pFile);

	//now save compressed map info
	filename = filename + "c";

	//open file
	gzFile saveFile;
	saveFile = gzopen(filename.c_str(),"wb");
	if(saveFile == 0)
		return;

	//player pos
	gzwrite(saveFile, &playerPos.x,sizeof(float));
	gzwrite(saveFile, &playerPos.y,sizeof(float));
	gzwrite(saveFile, &playerPos.z,sizeof(float));
		
	//size
	gzwrite(saveFile, &WORLD_SIZE,sizeof(int));

	//chunksize
	gzwrite(saveFile, &CHUNK_SIZE,sizeof(int));

	//cubes
	gzwrite(saveFile, m_Blocks,sizeof(unsigned char)*(WORLD_SIZE * WORLD_SIZE * WORLD_SIZE));

	//light
	gzwrite(saveFile, m_BlockLight,sizeof(unsigned char)*(WORLD_SIZE * WORLD_SIZE * WORLD_SIZE));
		
	//close file
	gzclose(saveFile);
	saveFile=0;
}



void CraftWorld::SaveWorld(const char *filename)
{
	FILE * pFile;
	pFile = fopen(filename,"wb");

	if(pFile == NULL)
		return;

	int version = 1;

	//version
	fwrite(&version,sizeof(int),1,pFile);

	//name
	fwrite(worldName,sizeof(char),50,pFile);

	//size
	fwrite(&WORLD_SIZE,sizeof(int),1,pFile);

	//chunksize
	fwrite(&CHUNK_SIZE,sizeof(int),1,pFile);

	//cubes
	fwrite(m_Blocks,sizeof(unsigned char),(WORLD_SIZE * WORLD_SIZE * WORLD_SIZE),pFile);

	//light
	fwrite(m_BlockLight,sizeof(unsigned char),(WORLD_SIZE * WORLD_SIZE * WORLD_SIZE),pFile);

	//close file
	fclose(pFile);
}

void CraftWorld::LoadWorld(const char *filename)
{
	FILE * pFile;
	pFile = fopen(filename,"rb");

	if(pFile == NULL)
		return;

	int version = 1;

	//version
	fread(&version,sizeof(int),1,pFile);

	//name
	fread(worldName,sizeof(char),50,pFile);

	//size
	fread(&WORLD_SIZE,sizeof(int),1,pFile);

	//chunksize
	fread(&CHUNK_SIZE,sizeof(int),1,pFile);

	//cubes
	m_Blocks = new block_t[WORLD_SIZE * WORLD_SIZE * WORLD_SIZE];
	fread(m_Blocks,sizeof(unsigned char),(WORLD_SIZE * WORLD_SIZE * WORLD_SIZE),pFile);

	//light
	m_BlockLight = new block_t[WORLD_SIZE * WORLD_SIZE * WORLD_SIZE];
	fread(m_BlockLight,sizeof(unsigned char),(WORLD_SIZE * WORLD_SIZE * WORLD_SIZE),pFile);

	//close file
	fclose(pFile);
}

void CraftWorld::LoadCompressedWorld(std::string  filename)
{
	//load normal save info
	FILE * pFile;
	pFile = fopen(filename.c_str(),"rb");

	if(pFile == NULL)
		return;

	int version = 1;

	//version
	fread(&version,sizeof(int),1,pFile);

	//name
	fread(worldName,sizeof(char),50,pFile);
	//close file
	fclose(pFile);

	//load compressed map data
	filename = filename + "c";

	//open file
	gzFile saveFile;
	saveFile = gzopen(filename.c_str(),"rb");
	if(saveFile == 0)
		return;

	//player pos
	gzread(saveFile, &playerPos.x,sizeof(float));
	gzread(saveFile, &playerPos.y,sizeof(float));
	gzread(saveFile, &playerPos.z,sizeof(float));

	//size
	gzread(saveFile, &WORLD_SIZE,sizeof(int));

	//chunksize
	gzread(saveFile, &CHUNK_SIZE,sizeof(int));

	//cubes
	m_Blocks = new block_t[WORLD_SIZE * WORLD_SIZE * WORLD_SIZE];
	gzread(saveFile, m_Blocks,sizeof(unsigned char)*(WORLD_SIZE * WORLD_SIZE * WORLD_SIZE));

	//light
	m_BlockLight = new block_t[WORLD_SIZE * WORLD_SIZE * WORLD_SIZE];
	gzread(saveFile, m_BlockLight,sizeof(unsigned char)*(WORLD_SIZE * WORLD_SIZE * WORLD_SIZE));

	//close file
	gzclose(saveFile);
	saveFile=0;
}

void CraftWorld::initRandompMap(int worldSize,int chunkSize)
{
	//inicjuj mape
	WORLD_SIZE = worldSize;
	CHUNK_SIZE = chunkSize;

	m_Blocks = new block_t[WORLD_SIZE * WORLD_SIZE * WORLD_SIZE];
	memset(m_Blocks, 0, sizeof(block_t) * WORLD_SIZE * WORLD_SIZE * WORLD_SIZE);

	m_BlockLight = new block_t[WORLD_SIZE * WORLD_SIZE * WORLD_SIZE];
	memset(m_BlockLight, 255, sizeof(block_t) * WORLD_SIZE * WORLD_SIZE * WORLD_SIZE);

	//srand(time(NULL));
	int seed = rand() % 10000;

	//terrain settings for default terrain
	unsigned char grass = 1;
	unsigned char rock = 3;
	unsigned char dirt = 2;
	unsigned char water = 4;
	unsigned char sandUnderWater = 7;

	int terrainType = 0;//rand() % 3;//0-2

	int testterrain = rand() % 10;

	if(testterrain < 4)
		terrainType = 0;
	if(testterrain >= 4 && testterrain < 7)
		terrainType = 1;
	if(testterrain >=7)
		terrainType = 2;

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
		grass = 37;
		rock = 3;
		dirt = 39;
		water = 0;//there is no water on desert ;)
		sandUnderWater = 7;
	}
	//

	noisepp::PerlinModule perlin;
	perlin.setSeed(seed);
	perlin.setOctaveCount(4);
	perlin.setQuality(noisepp::NOISE_QUALITY_FAST_STD);

	data = new float[worldSize * worldSize];

	noisepp::utils::PlaneBuilder2D builder;
	builder.setModule(perlin);
	builder.setSize(worldSize, worldSize);
	if(terrainType == 2)
		builder.setBounds(0.0, 0.0, 3.0, 3.0);
	else
		builder.setBounds(0.0, 0.0, 4.0, 4.0);
	builder.setDestination(data);
	builder.build ();

	//build map
	for (int z = 0; z < WORLD_SIZE; ++z)
	{
		for (int x = 0; x < WORLD_SIZE; ++x)
		{
			int Height  = data[x + z*WORLD_SIZE]* WORLD_SIZE/12 + WORLD_SIZE/2;
			//int Height = m_HeightMap.GetValue(x, z) * WORLD_SIZE/12 + WORLD_SIZE/2;

			for (int y = 0; y < Height; ++y)
			{
				if(y == Height-1)
					GetBlock(x, y, z) = grass;//grass
				else if(y < Height/2)
					GetBlock(x, y, z) = rock;
				else
					GetBlock(x, y, z) = dirt;
			}

		}
	}

	//caves?
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
	float max = 0.0f;

	for (int z = 2; z < WORLD_SIZE - 4; ++z)
	{
		for (int x = 2; x < WORLD_SIZE - 4; ++x)
		{
			for (int y = 2; y < WORLD_SIZE/2; ++y)
			{
				nx = (float)x / WORLD_SIZE;
				ny = (float)y / WORLD_SIZE;
				nz = (float)z / WORLD_SIZE;

				float test = element->getValue(nx,ny,nz,cache);

				if(test > max)
					max = test;

				if(test > 0.8f)
					if(GetBlock(x,y,z) != 0)
						GetBlock(x,y,z) = 0;

			}

		}
	}

	 pipeline->freeCache (cache);
	 cache = 0;
	 delete pipeline;
	 pipeline = 0;


	//watering
	int waterLevel = 58;

	for (int z = 0; z < WORLD_SIZE; ++z)
	{
		for (int x = 0; x < WORLD_SIZE; ++x)
		{
			int Height = data[x + z*WORLD_SIZE]* WORLD_SIZE/12 + WORLD_SIZE/2;
			//int Height = m_HeightMap.GetValue(x, z) * WORLD_SIZE/12 + WORLD_SIZE/2;

			if(Height < waterLevel)
			{
				GetBlock(x, Height-1, z) = sandUnderWater;//sand under water

				for (int y = Height;y < waterLevel;y++)
				{
					GetBlock(x, y, z) = water;//water
				}

			}else if(Height == waterLevel)
			{
				GetBlock(x, Height-1, z) = sandUnderWater;//beach sand
			}
		}
	}

	//make the most bottom layer of cube not destroyable
	for (int z = 0; z < WORLD_SIZE; ++z)
	{
		for (int x = 0; x < WORLD_SIZE; ++x)
		{
			GetBlock(x, 0, z) = 46;//last one in set
		}
	}

	delete []data;

	//init trees
	initTrees(terrainType);
}

void CraftWorld::initRandompMap(int worldSize,int chunkSize,int terrainType,bool makeFlat,bool makeTrees,bool makeWater,bool makeCaves)
{
	//inicjuj mape
	WORLD_SIZE = worldSize;
	CHUNK_SIZE = chunkSize;

	m_Blocks = new block_t[WORLD_SIZE * WORLD_SIZE * WORLD_SIZE];
	memset(m_Blocks, 0, sizeof(block_t) * WORLD_SIZE * WORLD_SIZE * WORLD_SIZE);

	m_BlockLight = new block_t[WORLD_SIZE * WORLD_SIZE * WORLD_SIZE];
	memset(m_BlockLight, 255, sizeof(block_t) * WORLD_SIZE * WORLD_SIZE * WORLD_SIZE);

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
		grass = 37;
		rock = 3;
		dirt = 39;
		water = 0;//there is no water on desert ;)
		sandUnderWater = 7;
	}
	//
	if(!makeFlat)
	{
		noisepp::PerlinModule perlin;
		perlin.setSeed(seed);
		perlin.setOctaveCount(4);
		perlin.setQuality(noisepp::NOISE_QUALITY_FAST_STD);

		data = new float[worldSize * worldSize];

		noisepp::utils::PlaneBuilder2D builder;
		builder.setModule(perlin);
		builder.setSize(worldSize, worldSize);
		if(terrainType == 2)
			builder.setBounds(0.0, 0.0, 3.0, 3.0);
		else
			builder.setBounds(0.0, 0.0, 4.0, 4.0);
		builder.setDestination(data);
		builder.build ();

		//build map
		for (int z = 0; z < WORLD_SIZE; ++z)
		{
			for (int x = 0; x < WORLD_SIZE; ++x)
			{
				int Height  = data[x + z*WORLD_SIZE]* WORLD_SIZE/12 + WORLD_SIZE/2;
				//int Height = m_HeightMap.GetValue(x, z) * WORLD_SIZE/12 + WORLD_SIZE/2;

				for (int y = 0; y < Height; ++y)
				{
					if(y == Height-1)
						GetBlock(x, y, z) = grass;//grass
					else if(y < Height/2)
						GetBlock(x, y, z) = rock;
					else
						GetBlock(x, y, z) = dirt;
				}

			}
		}
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
						GetBlock(x, y, z) = grass;//grass
					else if(y < Height/2)
						GetBlock(x, y, z) = rock;
					else
						GetBlock(x, y, z) = dirt;
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
		float max = 0.0f;

		for (int z = 2; z < WORLD_SIZE - 4; ++z)
		{
			for (int x = 2; x < WORLD_SIZE - 4; ++x)
			{
				for (int y = 2; y < WORLD_SIZE/2; ++y)
				{
					nx = (float)x / WORLD_SIZE;
					ny = (float)y / WORLD_SIZE;
					nz = (float)z / WORLD_SIZE;

					float test = element->getValue(nx,ny,nz,cache);

					if(test > max)
						max = test;

					if(test > 0.8f)
						if(GetBlock(x,y,z) != 0)
							GetBlock(x,y,z) = 0;

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
					GetBlock(x, Height-1, z) = sandUnderWater;//sand under water

					for (int y = Height;y < waterLevel;y++)
					{
						GetBlock(x, y, z) = water;//water
					}

				}else if(Height == waterLevel)
				{
					GetBlock(x, Height-1, z) = sandUnderWater;//beach sand
				}
			}
		}

		delete []data;
	}

	//make the most bottom layer of cube not destroyable
	for (int z = 0; z < WORLD_SIZE; ++z)
	{
		for (int x = 0; x < WORLD_SIZE; ++x)
		{
			GetBlock(x, 0, z) = 46;//last one in set
		}
	}



	//init trees
	if(makeTrees)
		initTrees(terrainType);
}

void CraftWorld::initTrees(int treeChoose)
{
	int NumTrees = 100;

	for (int i = 0; i < NumTrees; ++i)
	{
		//Choose the tree position
		int x = rand() % WORLD_SIZE;
		int z = rand() % WORLD_SIZE;
		int y = groundHeight(x, z);

		if (y <= 0) continue;

		int TrunkHeight = 4;//rand() % 5 + 4;


		//check if there is an water on there or another tree
		int flag = 1;
		for (int y1 = y; y1 < y + 1 + TrunkHeight && y1 < WORLD_SIZE; ++y1)
		{
			if(GetBlock(x, y1, z) == 4 || GetBlock(x, y1, z) == 8 || GetBlock(x, y1, z) == 9 || GetBlock(x, y1, z) == 31 || GetBlock(x, y1, z) == 38 || GetBlock(x, y1, z) == 25 || GetBlock(x, y1, z) == 32)
				flag = 0;
		}

		if(flag == 0)continue;


		if(treeChoose == 0)//normal tree
		{
			//Create the tree trunk
			for (int y1 = y + 1; y1 < y + 1 + TrunkHeight && y1 < WORLD_SIZE; ++y1)
			{
				block_t& Block = GetBlock(x, y1, z);
				if (Block == 0) Block = 8;
			}

			//create my leaves
			for(int yy = 0;yy < 3;yy++)
			{
				for(int xx = 0;xx < 3;xx++)
				{
					for(int zz = 0;zz < 3;zz++)
					{
						int x1 = xx + x - 1;// :D - what a shitty code
						int y1 = yy + y + TrunkHeight - 1;
						int z1 = zz + z - 1;

						if (x1 < 0 || y1 < 0 || z1 < 0) continue;
						if (x1 >= WORLD_SIZE || y1 >= WORLD_SIZE || z1 >= WORLD_SIZE) continue;

						block_t& Block = GetBlock(x1, y1, z1);
						if (Block == 0) Block = 9;
					}
				}
			}
		}

		if(treeChoose == 1)//winter tree
		{
			//Create the tree trunk
			for (int y1 = y + 1; y1 < y + 1 + TrunkHeight && y1 < WORLD_SIZE; ++y1)
			{
				block_t& Block = GetBlock(x, y1, z);
				if (Block == 0) Block = 31;
			}

			//create my leaves
			for(int yy = 0;yy < 3;yy++)
			{
				for(int xx = 0;xx < 3;xx++)
				{
					for(int zz = 0;zz < 3;zz++)
					{
						int x1 = xx + x - 1;// :D - what a shitty code
						int y1 = yy + y + TrunkHeight - 1;
						int z1 = zz + z - 1;

						if (x1 < 0 || y1 < 0 || z1 < 0) continue;
						if (x1 >= WORLD_SIZE || y1 >= WORLD_SIZE || z1 >= WORLD_SIZE) continue;

						block_t& Block = GetBlock(x1, y1, z1);
						if (Block == 0) Block = 38;
					}
				}
			}
		}

		if(treeChoose == 2)//desert cactus ;)
		{
			//Create the tree trunk
			for (int y1 = y + 1; y1 < y + 1 + TrunkHeight && y1 < WORLD_SIZE; ++y1)
			{
				block_t& Block = GetBlock(x, y1, z);
				if (Block == 0) Block = 25;
			}
		}
	}
}

void CraftWorld::initWorldBlocksLight()
{
	int x, y, z;
	block_t Light;
	block_t DeltaLight = 16;

	for (z = 0; z < WORLD_SIZE; ++z)
	{
		for (x = 0; x < WORLD_SIZE; ++x)
		{
			Light = 255;

			for (y = WORLD_SIZE - 1; y >= 0; --y)
			{
				GetBlockLight(x, y, z) = Light;

				if (GetBlock(x, y, z) != 0)
				{
					if (Light >= DeltaLight)
						Light -= DeltaLight;
					if (Light < 85)
						Light = 85;
				}

			}
		}
	}
}

void CraftWorld::buildMap()
{
	createWorldChunkss();
}

void CraftWorld::destroyMap()
{
	for(unsigned int i = 0;i < mChunks.size();i++)
	{
		mChunks[i]->reset();
	}

	for(unsigned int i = 0;i < mWaterChunks.size();i++)
	{
		mWaterChunks[i]->reset();
	}
}

void CraftWorld::setTextureSize(int texture,int chunk)
{
	texutreSize = texture;
	textureChunk = chunk;

	percent = (float)textureChunk / (float)texutreSize;
}

block_t& CraftWorld::GetBlock (const int x, const int y, const int z) 
{
	return m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_SIZE];
}

block_t& CraftWorld::GetBlockLight (const int x, const int y, const int z)
{
	return m_BlockLight[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_SIZE];
}

bool CraftWorld::BlockTransparent(const int x, const int y, const int z)
{
	return blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_SIZE]].transparent;
}

bool CraftWorld::BlockEditable(const int x, const int y, const int z)
{
	if (x < 0 || y < 0 || z < 0  || x >= WORLD_SIZE || y >= WORLD_SIZE || z >= WORLD_SIZE) return false;

	return blockTypes[m_Blocks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_SIZE]].editable;
}

void CraftWorld::buildblocksVerts()
{
	std::vector<Vector3*> mPosition;
	std::vector<Vector3*> mColour;
	std::vector<Vector2*> mtextures;
	std::vector<Vector3*> mTriangle;

	int iVertex = 0;

	float x = -0.5f;
	float y = -0.5f;
	float z = -0.5f;

	//create vertices for each block type
	for(unsigned int i = 1;i < blockTypes.size();i++)
	{
		BaseBlock *blockType = &blockTypes[i];

		float down = 1.0f - percent * (blockType->textureRow + 1);
		float up = down + percent;

		float left = percent * blockType->upPlane;
		float right = left + percent;

		iVertex = 0;

		//light
		float BlockLight  = 1.0f;  //For the two x faces
		float BlockLight1 = BlockLight * 0.9f;		//For the two z faces
		float BlockLight2 = BlockLight * 0.8f;		//For the two y faces


		//x
		left = percent * blockType->sidePlane;
		right = left + percent;

		mPosition.push_back(new Vector3(x, y, z+1));	mtextures.push_back(new Vector2(right, down)); mColour.push_back(new Vector3(BlockLight,BlockLight,BlockLight));
		mPosition.push_back(new Vector3(x, y+1, z+1));	mtextures.push_back(new Vector2(right, up)); mColour.push_back(new Vector3(BlockLight,BlockLight,BlockLight));
		mPosition.push_back(new Vector3(x, y+1, z));		mtextures.push_back(new Vector2(left, up)); mColour.push_back(new Vector3(BlockLight,BlockLight,BlockLight));
		mPosition.push_back(new Vector3(x, y,   z));		mtextures.push_back(new Vector2(left, down)); mColour.push_back(new Vector3(BlockLight,BlockLight,BlockLight));

		mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
		mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

		iVertex += 4;

		//x

		left = percent * blockType->sidePlane;
		right = left + percent;

		mPosition.push_back(new Vector3(x+1, y,   z));	mtextures.push_back(new Vector2(right, down));mColour.push_back(new Vector3(BlockLight,BlockLight,BlockLight));
		mPosition.push_back(new Vector3(x+1, y+1, z));	mtextures.push_back(new Vector2(right, up));mColour.push_back(new Vector3(BlockLight,BlockLight,BlockLight));
		mPosition.push_back(new Vector3(x+1, y+1, z+1));	mtextures.push_back(new Vector2(left, up));mColour.push_back(new Vector3(BlockLight,BlockLight,BlockLight));
		mPosition.push_back(new Vector3(x+1, y,   z+1));	mtextures.push_back(new Vector2(left, down));mColour.push_back(new Vector3(BlockLight,BlockLight,BlockLight));

		mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
		mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

		iVertex += 4;

		//up
		left = percent * blockType->downPlane;
		right = left + percent;

		mPosition.push_back(new Vector3(x,   y, z));		mtextures.push_back(new Vector2(left, up)); mColour.push_back(new Vector3(BlockLight2,BlockLight2,BlockLight2));
		mPosition.push_back(new Vector3(x+1, y, z));		mtextures.push_back(new Vector2(right, up)); mColour.push_back(new Vector3(BlockLight2,BlockLight2,BlockLight2));
		mPosition.push_back(new Vector3(x+1, y, z+1));	mtextures.push_back(new Vector2(right, down)); mColour.push_back(new Vector3(BlockLight2,BlockLight2,BlockLight2));
		mPosition.push_back(new Vector3(x,   y, z+1));	mtextures.push_back(new Vector2(left, down)); mColour.push_back(new Vector3(BlockLight2,BlockLight2,BlockLight2));

		mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
		mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

		iVertex += 4;

		//down
		left = percent * blockType->upPlane;
		right = left + percent;

		mPosition.push_back(new Vector3(x,   y+1, z+1));		mtextures.push_back(new Vector2(left, up)); mColour.push_back(new Vector3(BlockLight2,BlockLight2,BlockLight2));
		mPosition.push_back(new Vector3(x+1, y+1, z+1));		mtextures.push_back(new Vector2(right, up)); mColour.push_back(new Vector3(BlockLight2,BlockLight2,BlockLight2));
		mPosition.push_back(new Vector3(x+1, y+1, z));		mtextures.push_back(new Vector2(right, down)); mColour.push_back(new Vector3(BlockLight2,BlockLight2,BlockLight2));
		mPosition.push_back(new Vector3(x,   y+1, z));		mtextures.push_back(new Vector2(left, down)); mColour.push_back(new Vector3(BlockLight2,BlockLight2,BlockLight2));

		mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
		mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

		iVertex += 4;

		left = percent * blockType->sidePlane;
		right = left + percent;

		mPosition.push_back(new Vector3(x,   y+1, z));		mtextures.push_back(new Vector2(right, up)); mColour.push_back(new Vector3(BlockLight1,BlockLight1,BlockLight1));
		mPosition.push_back(new Vector3(x+1, y+1, z));		mtextures.push_back(new Vector2(left, up)); mColour.push_back(new Vector3(BlockLight1,BlockLight1,BlockLight1));
		mPosition.push_back(new Vector3(x+1, y,   z));		mtextures.push_back(new Vector2(left, down)); mColour.push_back(new Vector3(BlockLight1,BlockLight1,BlockLight1));
		mPosition.push_back(new Vector3(x,   y,   z));		mtextures.push_back(new Vector2(right, down)); mColour.push_back(new Vector3(BlockLight1,BlockLight1,BlockLight1));

		mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
		mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

		iVertex += 4;

		left = percent * blockType->sidePlane;
		right = left + percent;

		mPosition.push_back(new Vector3(x,   y,   z+1));		mtextures.push_back(new Vector2(left, down)); mColour.push_back(new Vector3(BlockLight1,BlockLight1,BlockLight1));
		mPosition.push_back(new Vector3(x+1, y,   z+1));		mtextures.push_back(new Vector2(right, down)); mColour.push_back(new Vector3(BlockLight1,BlockLight1,BlockLight1));
		mPosition.push_back(new Vector3(x+1, y+1, z+1));		mtextures.push_back(new Vector2(right, up)); mColour.push_back(new Vector3(BlockLight1,BlockLight1,BlockLight1));
		mPosition.push_back(new Vector3(x,   y+1, z+1));		mtextures.push_back(new Vector2(left, up)); mColour.push_back(new Vector3(BlockLight1,BlockLight1,BlockLight1));

		mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
		mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

		iVertex += 4;


		blockTypes[i].vertices = (CraftPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(CraftPSPVertex));
		//build verts
		//vertices
		int vert = 0;
		unsigned int size =  mTriangle.size();
		for(unsigned int j = 0;j < size;j++)
		{
			blockTypes[i].vertices[vert].u = mtextures[mTriangle[j]->x]->x;
			blockTypes[i].vertices[vert].v = mtextures[mTriangle[j]->x]->y;
			blockTypes[i].vertices[vert].color = GU_COLOR( mColour[mTriangle[j]->x]->x,mColour[mTriangle[j]->x]->y,mColour[mTriangle[j]->x]->z,1.0f);
			blockTypes[i].vertices[vert].x = mPosition[mTriangle[j]->x]->x;
			blockTypes[i].vertices[vert].y = mPosition[mTriangle[j]->x]->y;
			blockTypes[i].vertices[vert].z = mPosition[mTriangle[j]->x]->z;
			vert++;

			blockTypes[i].vertices[vert].u = mtextures[mTriangle[j]->y]->x;
			blockTypes[i].vertices[vert].v = mtextures[mTriangle[j]->y]->y;
			blockTypes[i].vertices[vert].color = GU_COLOR( mColour[mTriangle[j]->y]->x,mColour[mTriangle[j]->y]->y,mColour[mTriangle[j]->y]->z,1.0f);
			blockTypes[i].vertices[vert].x = mPosition[mTriangle[j]->y]->x;
			blockTypes[i].vertices[vert].y = mPosition[mTriangle[j]->y]->y;
			blockTypes[i].vertices[vert].z = mPosition[mTriangle[j]->y]->z;
			vert++;

			blockTypes[i].vertices[vert].u = mtextures[mTriangle[j]->z]->x;
			blockTypes[i].vertices[vert].v = mtextures[mTriangle[j]->z]->y;
			blockTypes[i].vertices[vert].color = GU_COLOR( mColour[mTriangle[j]->z]->x,mColour[mTriangle[j]->z]->y,mColour[mTriangle[j]->z]->z,1.0f);
			blockTypes[i].vertices[vert].x = mPosition[mTriangle[j]->z]->x;
			blockTypes[i].vertices[vert].y = mPosition[mTriangle[j]->z]->y;
			blockTypes[i].vertices[vert].z = mPosition[mTriangle[j]->z]->z;
			vert++;
		}

		//clear the cache or there will be some errors
		sceKernelDcacheWritebackInvalidateRange(blockTypes[i].vertices,( mTriangle.size() * 3) * sizeof(CraftPSPVertex));
		//sceKernelDcacheWritebackInvalidateAll();

		for(unsigned int aa = 0;aa < mPosition.size();aa++)
		{
			delete mPosition[aa];
			delete mtextures[aa];
			delete mColour[aa];
		}
		mPosition.clear();
		mtextures.clear();
		mColour.clear();

		for(unsigned int aa = 0;aa < mTriangle.size();aa++)
			delete 		mTriangle[aa];
		mTriangle.clear();
	}
}

void CraftWorld::createChunks(const int StartX, const int StartY, const int StartZ)
{
	SimpleMeshChunk* MeshChunk = new SimpleMeshChunk();

	int iVertex = 0;
	block_t Block;
	bool transparentBlock;

	/* Only create visible faces of each chunk */
	bool DefaultBlock = false;
	int SX = 0;
	int SY = 0;
	int SZ = 0;
	int MaxSize = WORLD_SIZE;


	MeshChunk->chunkStartZ = StartZ;
	MeshChunk->chunkStartY = StartY;
	MeshChunk->chunkStartX = StartX;

	MeshChunk->bBox = BoundingBox(Vector3(StartX,StartY,StartZ),Vector3(StartX + CHUNK_SIZE,StartY + CHUNK_SIZE,StartZ + CHUNK_SIZE));

	//if we are in player zone on the start then build map
	if(playerZoneBB.intersect(MeshChunk->bBox))
	{
		createdChunksCount++;
		//light
		for (int z = StartZ; z < CHUNK_SIZE + StartZ; ++z)
		{
			for (int y = StartY; y < CHUNK_SIZE + StartY; ++y)
			{
				for (int x = StartX; x < CHUNK_SIZE + StartX; ++x)
				{
					Block = GetBlock(x,y,z);
					if(BlockTransparent(x,y,z) == true)continue;//if block is transparent don't continue

					//texture stuff
					BaseBlock *blockType = &blockTypes[Block];

					float down = 1.0f - percent * (blockType->textureRow + 1);
					float up = down + percent;

					float left = percent * blockType->upPlane;
					float right = left + percent;

					//light
					float BlockLight  = GetBlockLight(x, y, z) / 255.0f;  //For the two x faces
					float BlockLight1 = BlockLight * 0.9f;		//For the two z faces
					float BlockLight2 = BlockLight * 0.8f;		//For the two y faces

					//x-1
					transparentBlock = DefaultBlock;
					if (x > SX)transparentBlock = BlockTransparent(x-1,y,z);

					if (transparentBlock == true)
					{
						left = percent * blockType->sidePlane;
						right = left + percent;

						MeshChunk->position(x, y,   z+1);	MeshChunk->textureCoord(right, down); MeshChunk->colour(BlockLight,BlockLight,BlockLight);
						MeshChunk->position(x, y+1, z+1);	MeshChunk->textureCoord(right, up); MeshChunk->colour(BlockLight,BlockLight,BlockLight);
						MeshChunk->position(x, y+1, z);		MeshChunk->textureCoord(left, up); MeshChunk->colour(BlockLight,BlockLight,BlockLight);
						MeshChunk->position(x, y,   z);		MeshChunk->textureCoord(left, down); MeshChunk->colour(BlockLight,BlockLight,BlockLight);

						MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
						MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

						iVertex += 4;
					}


					//x+1
					transparentBlock = DefaultBlock;
					if (x < SX + MaxSize - 1)transparentBlock = BlockTransparent(x+1,y,z);

					if (transparentBlock == true)
					{
						left = percent * blockType->sidePlane;
						right = left + percent;

						MeshChunk->position(x+1, y,   z);	MeshChunk->textureCoord(right, down);MeshChunk->colour(BlockLight,BlockLight,BlockLight);
						MeshChunk->position(x+1, y+1, z);	MeshChunk->textureCoord(right, up);MeshChunk->colour(BlockLight,BlockLight,BlockLight);
						MeshChunk->position(x+1, y+1, z+1);	MeshChunk->textureCoord(left, up);MeshChunk->colour(BlockLight,BlockLight,BlockLight);
						MeshChunk->position(x+1, y,   z+1);	MeshChunk->textureCoord(left, down);MeshChunk->colour(BlockLight,BlockLight,BlockLight);

						MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
						MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

						iVertex += 4;
					}

					//y-1
					transparentBlock = DefaultBlock;
					if (y > SY) transparentBlock = BlockTransparent(x,y-1,z);

					if (transparentBlock == true)
					{
						//up
						left = percent * blockType->downPlane;
						right = left + percent;

						MeshChunk->position(x,   y, z);		MeshChunk->textureCoord(left, up); MeshChunk->colour(BlockLight2,BlockLight2,BlockLight2);
						MeshChunk->position(x+1, y, z);		MeshChunk->textureCoord(right, up); MeshChunk->colour(BlockLight2,BlockLight2,BlockLight2);
						MeshChunk->position(x+1, y, z+1);	MeshChunk->textureCoord(right, down); MeshChunk->colour(BlockLight2,BlockLight2,BlockLight2);
						MeshChunk->position(x,   y, z+1);	MeshChunk->textureCoord(left, down); MeshChunk->colour(BlockLight2,BlockLight2,BlockLight2);

						MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
						MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

						iVertex += 4;
					}


					//y+1
					transparentBlock = DefaultBlock;
					if (y < SY + MaxSize - 1)transparentBlock = BlockTransparent(x,y+1,z);

					if (transparentBlock == true)
					{
						//down
						left = percent * blockType->upPlane;
						right = left + percent;

						MeshChunk->position(x,   y+1, z+1);		MeshChunk->textureCoord(left, up); MeshChunk->colour(BlockLight2,BlockLight2,BlockLight2);
						MeshChunk->position(x+1, y+1, z+1);		MeshChunk->textureCoord(right, up); MeshChunk->colour(BlockLight2,BlockLight2,BlockLight2);
						MeshChunk->position(x+1, y+1, z);		MeshChunk->textureCoord(right, down); MeshChunk->colour(BlockLight2,BlockLight2,BlockLight2);
						MeshChunk->position(x,   y+1, z);		MeshChunk->textureCoord(left, down); MeshChunk->colour(BlockLight2,BlockLight2,BlockLight2);

						MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
						MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

						iVertex += 4;
					}

					//z-1
					transparentBlock = DefaultBlock;
					if (z > SZ) transparentBlock = BlockTransparent(x,y,z-1);

					if (transparentBlock == true)
					{
						left = percent * blockType->sidePlane;
						right = left + percent;

						MeshChunk->position(x,   y+1, z);		MeshChunk->textureCoord(right, up); MeshChunk->colour(BlockLight1,BlockLight1,BlockLight1);
						MeshChunk->position(x+1, y+1, z);		MeshChunk->textureCoord(left, up); MeshChunk->colour(BlockLight1,BlockLight1,BlockLight1);
						MeshChunk->position(x+1, y,   z);		MeshChunk->textureCoord(left, down); MeshChunk->colour(BlockLight1,BlockLight1,BlockLight1);
						MeshChunk->position(x,   y,   z);		MeshChunk->textureCoord(right, down); MeshChunk->colour(BlockLight1,BlockLight1,BlockLight1);

						MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
						MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

						iVertex += 4;
					}


					//z+1
					transparentBlock = DefaultBlock;
					if (z < SZ + MaxSize - 1)transparentBlock = BlockTransparent(x,y,z+1);

					if (transparentBlock == true)
					{
						left = percent * blockType->sidePlane;
						right = left + percent;

						MeshChunk->position(x,   y,   z+1);		MeshChunk->textureCoord(left, down); MeshChunk->colour(BlockLight1,BlockLight1,BlockLight1);
						MeshChunk->position(x+1, y,   z+1);		MeshChunk->textureCoord(right, down); MeshChunk->colour(BlockLight1,BlockLight1,BlockLight1);
						MeshChunk->position(x+1, y+1, z+1);		MeshChunk->textureCoord(right, up); MeshChunk->colour(BlockLight1,BlockLight1,BlockLight1);
						MeshChunk->position(x,   y+1, z+1);		MeshChunk->textureCoord(left, up); MeshChunk->colour(BlockLight1,BlockLight1,BlockLight1);

						MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
						MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

						iVertex += 4;
					}

				}
			}
		}
		MeshChunk->end();
	}else
	{
		MeshChunk->end();
		MeshChunk->created = false;
		//MeshChunk->lastTriangleCount = -1;
	}

	MeshChunk->id = mChunks.size();
	mChunks.push_back(MeshChunk);
}


void CraftWorld::createTransparentChunks(const int StartX, const int StartY, const int StartZ)
{
	SimpleMeshChunk* MeshChunk = new SimpleMeshChunk();

	int iVertex = 0;
	block_t Block;
	block_t Block1;
	bool transparentBlock;

	/* Only create visible faces of each chunk */
	bool DefaultBlock = false;
	int SX = 0;
	int SY = 0;
	int SZ = 0;
	int MaxSize = WORLD_SIZE;


	MeshChunk->chunkStartZ = StartZ;
	MeshChunk->chunkStartY = StartY;
	MeshChunk->chunkStartX = StartX;

	MeshChunk->bBox = BoundingBox(Vector3(StartX,StartY,StartZ),Vector3(StartX + CHUNK_SIZE,StartY + CHUNK_SIZE,StartZ + CHUNK_SIZE));

	//if we are in player zone on the start then build map
	if(playerZoneBB.intersect(MeshChunk->bBox))
	{
		for (int z = StartZ; z < CHUNK_SIZE + StartZ; ++z)
		{
			for (int y = StartY; y < CHUNK_SIZE + StartY; ++y)
			{
				for (int x = StartX; x < CHUNK_SIZE + StartX; ++x)
				{
					Block = GetBlock(x,y,z);
					if (BlockTransparent(x,y,z) == false || Block == 0) continue;

					BaseBlock *blockType = &blockTypes[Block];

					float down = 1.0f - percent * (blockType->textureRow + 1);
					float up = down + percent;

					float left = percent * blockType->upPlane;
					float right = left + percent;

					//x-1
					transparentBlock = DefaultBlock;
					Block1 = 1;
					if (x > SX)
					{
						transparentBlock = BlockTransparent(x-1,y,z);
						Block1 = GetBlock(x-1,y,z);
					}

					if (transparentBlock == false || Block1 == 0)
					{
						left = percent * blockType->sidePlane;
						right = left + percent;

						MeshChunk->position(x, y,   z+1);	MeshChunk->textureCoord(right, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x, y+1, z+1);	MeshChunk->textureCoord(right, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x, y+1, z);		MeshChunk->textureCoord(left, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x, y,   z);		MeshChunk->textureCoord(left, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );

						MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
						MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

						iVertex += 4;
					}

					//x+1
					transparentBlock = DefaultBlock;
					Block1 = 1;
					if (x < SX + MaxSize - 1)
					{
						transparentBlock = BlockTransparent(x+1,y,z);
						Block1 = GetBlock(x+1,y,z);
					}

					if (transparentBlock == false || Block1 == 0)
					{
						left = percent * blockType->sidePlane;
						right = left + percent;

						MeshChunk->position(x+1, y,   z);	MeshChunk->textureCoord(right, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x+1, y+1, z);	MeshChunk->textureCoord(right, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x+1, y+1, z+1);	MeshChunk->textureCoord(left, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x+1, y,   z+1);	MeshChunk->textureCoord(left, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );

						MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
						MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

						iVertex += 4;
					}

					//y-1
					transparentBlock = DefaultBlock;
					Block1 = 1;
					if (y > SY)
					{
						transparentBlock = BlockTransparent(x,y-1,z);
						Block1 = GetBlock(x,y-1,z);
					}

					if (transparentBlock == false || Block1 == 0)
					{
						//up
						left = percent * blockType->downPlane;
						right = left + percent;

						MeshChunk->position(x,   y, z);		MeshChunk->textureCoord(left, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x+1, y, z);		MeshChunk->textureCoord(right, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x+1, y, z+1);	MeshChunk->textureCoord(right, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x,   y, z+1);	MeshChunk->textureCoord(left, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );

						MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
						MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

						iVertex += 4;
					}


					//y+1
					transparentBlock = DefaultBlock;
					Block1 = 1;
					if (y < SY + MaxSize - 1)
					{
						transparentBlock = BlockTransparent(x,y+1,z);
						Block1 = GetBlock(x,y+1,z);
					}

					if (transparentBlock == false || Block1 == 0)
					{
						//down
						left = percent * blockType->upPlane;
						right = left + percent;

						MeshChunk->position(x,   y+1, z+1);		MeshChunk->textureCoord(left, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x+1, y+1, z+1);		MeshChunk->textureCoord(right, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x+1, y+1, z);		MeshChunk->textureCoord(right, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x,   y+1, z);		MeshChunk->textureCoord(left, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );

						MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
						MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

						iVertex += 4;
					}

					//z-1
					transparentBlock = DefaultBlock;
					Block1 = 1;
					if (z > SZ)
					{
						transparentBlock = BlockTransparent(x,y,z-1);
						Block1 = GetBlock(x,y,z-1);
					}

					if (transparentBlock == false || Block1 == 0)
					{
						left = percent * blockType->sidePlane;
						right = left + percent;

						MeshChunk->position(x,   y+1, z);		MeshChunk->textureCoord(right, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x+1, y+1, z);		MeshChunk->textureCoord(left, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x+1, y,   z);		MeshChunk->textureCoord(left, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x,   y,   z);		MeshChunk->textureCoord(right, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );

						MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
						MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

						iVertex += 4;
					}


					//z+1
					transparentBlock = DefaultBlock;
					Block1 = 1;
					if (z < SZ + MaxSize - 1)
					{
						transparentBlock = BlockTransparent(x,y,z+1);
						Block1 = GetBlock(x,y,z+1);
					}

					if (transparentBlock == false || Block1 == 0)
					{
						left = percent * blockType->sidePlane;
						right = left + percent;

						MeshChunk->position(x,   y,   z+1);		MeshChunk->textureCoord(left, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x+1, y,   z+1);		MeshChunk->textureCoord(right, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x+1, y+1, z+1);		MeshChunk->textureCoord(right, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
						MeshChunk->position(x,   y+1, z+1);		MeshChunk->textureCoord(left, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );

						MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
						MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

						iVertex += 4;
					}

				}
			}
		}
		MeshChunk->end();
	}else
	{
		MeshChunk->end();
		MeshChunk->created = false;
		//MeshChunk->lastTriangleCount = -1;
	}


	//MeshChunk->end();
	MeshChunk->id = mChunks.size();
	mWaterChunks.push_back(MeshChunk);
}

void CraftWorld::rebuildChunk(int id)
{
	SimpleMeshChunk* MeshChunk = mChunks[id];

	int iVertex = 0;
	block_t Block;
	bool transparentBlock;

	/* Only create visible faces of each chunk */
	bool DefaultBlock = false;
	int SX = 0;
	int SY = 0;
	int SZ = 0;
	int MaxSize = WORLD_SIZE;

	int StartZ = MeshChunk->chunkStartZ;
	int StartY = MeshChunk->chunkStartY;
	int StartX = MeshChunk->chunkStartX;

	MeshChunk->reset();

	for (int z = StartZ; z < CHUNK_SIZE + StartZ; ++z)
	{
		for (int y = StartY; y < CHUNK_SIZE + StartY; ++y)
		{
			for (int x = StartX; x < CHUNK_SIZE + StartX; ++x)
			{
				Block = GetBlock(x,y,z);
				if(BlockTransparent(x,y,z) == true)continue;

				BaseBlock *blockType = &blockTypes[Block];

				float down = 1.0f - percent * (blockType->textureRow + 1);
				float up = down + percent;

				float left = percent * blockType->upPlane;
				float right = left + percent;

				//block colour
				float BlockLight  = GetBlockLight(x, y, z) / 255.0f;  //For the two x faces
				float BlockLight1 = BlockLight * 0.9f;		//For the two z faces
				float BlockLight2 = BlockLight * 0.8f;		//For the two y faces


				//x-1
				transparentBlock = DefaultBlock;
				if (x > SX) transparentBlock = BlockTransparent(x-1,y,z);

				if (transparentBlock == true)
				{
					left = percent * blockType->sidePlane;
					right = left + percent;

					MeshChunk->position(x, y,   z+1);	MeshChunk->textureCoord(right, down); MeshChunk->colour(BlockLight, BlockLight, BlockLight);
					MeshChunk->position(x, y+1, z+1);	MeshChunk->textureCoord(right, up); MeshChunk->colour(BlockLight, BlockLight, BlockLight);
					MeshChunk->position(x, y+1, z);		MeshChunk->textureCoord(left, up); MeshChunk->colour(BlockLight, BlockLight, BlockLight);
					MeshChunk->position(x, y,   z);		MeshChunk->textureCoord(left, down); MeshChunk->colour(BlockLight, BlockLight, BlockLight);

					MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
					MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

					iVertex += 4;
				}

				//x+1
				transparentBlock = DefaultBlock;
				if (x < SX + MaxSize - 1) transparentBlock = BlockTransparent(x+1,y,z);

				if (transparentBlock == true)
				{
					left = percent * blockType->sidePlane;
					right = left + percent;

					MeshChunk->position(x+1, y,   z);	MeshChunk->textureCoord(right, down);MeshChunk->colour(BlockLight, BlockLight, BlockLight);
					MeshChunk->position(x+1, y+1, z);	MeshChunk->textureCoord(right, up);MeshChunk->colour(BlockLight, BlockLight, BlockLight);
					MeshChunk->position(x+1, y+1, z+1);	MeshChunk->textureCoord(left, up);MeshChunk->colour(BlockLight, BlockLight, BlockLight);
					MeshChunk->position(x+1, y,   z+1);	MeshChunk->textureCoord(left, down);MeshChunk->colour(BlockLight, BlockLight, BlockLight);

					MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
					MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

					iVertex += 4;
				}

				//y-1
				transparentBlock = DefaultBlock;
				if (y > SY) transparentBlock = BlockTransparent(x,y-1,z);

				if (transparentBlock == true)
				{
					//up
					left = percent * blockType->downPlane;
					right = left + percent;

					MeshChunk->position(x,   y, z);		MeshChunk->textureCoord(left, up); MeshChunk->colour(BlockLight2 , BlockLight2 , BlockLight2 );
					MeshChunk->position(x+1, y, z);		MeshChunk->textureCoord(right, up); MeshChunk->colour(BlockLight2 , BlockLight2 , BlockLight2 );
					MeshChunk->position(x+1, y, z+1);	MeshChunk->textureCoord(right, down); MeshChunk->colour(BlockLight2 , BlockLight2 , BlockLight2 );
					MeshChunk->position(x,   y, z+1);	MeshChunk->textureCoord(left, down); MeshChunk->colour(BlockLight2 , BlockLight2 , BlockLight2 );

					MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
					MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

					iVertex += 4;
				}


				//y+1
				transparentBlock = DefaultBlock;
				if (y < SY + MaxSize - 1) transparentBlock = BlockTransparent(x,y+1,z);

				if (transparentBlock == true)
				{
					//down
					left = percent * blockType->upPlane;
					right = left + percent;

					MeshChunk->position(x,   y+1, z+1);		MeshChunk->textureCoord(left, up); MeshChunk->colour(BlockLight2 , BlockLight2 , BlockLight2 );
					MeshChunk->position(x+1, y+1, z+1);		MeshChunk->textureCoord(right, up); MeshChunk->colour(BlockLight2 , BlockLight2 , BlockLight2 );
					MeshChunk->position(x+1, y+1, z);		MeshChunk->textureCoord(right, down); MeshChunk->colour(BlockLight2 , BlockLight2 , BlockLight2 );
					MeshChunk->position(x,   y+1, z);		MeshChunk->textureCoord(left, down); MeshChunk->colour(BlockLight2 , BlockLight2 , BlockLight2 );

					MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
					MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

					iVertex += 4;
				}

				//z-1
				transparentBlock = DefaultBlock;
				if (z > SZ) transparentBlock = BlockTransparent(x,y,z-1);

				if (transparentBlock == true)
				{
					left = percent * blockType->sidePlane;
					right = left + percent;

					MeshChunk->position(x,   y+1, z);		MeshChunk->textureCoord(right, up); MeshChunk->colour(BlockLight1 , BlockLight1 , BlockLight1 );
					MeshChunk->position(x+1, y+1, z);		MeshChunk->textureCoord(left, up); MeshChunk->colour(BlockLight1 , BlockLight1 , BlockLight1 );
					MeshChunk->position(x+1, y,   z);		MeshChunk->textureCoord(left, down); MeshChunk->colour(BlockLight1 , BlockLight1 , BlockLight1 );
					MeshChunk->position(x,   y,   z);		MeshChunk->textureCoord(right, down); MeshChunk->colour(BlockLight1 , BlockLight1 , BlockLight1 );

					MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
					MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

					iVertex += 4;
				}


				//z+1
				transparentBlock = DefaultBlock;
				if (z < SZ + MaxSize - 1) transparentBlock = BlockTransparent(x,y,z+1);

				if (transparentBlock == true)
				{
					left = percent * blockType->sidePlane;
					right = left + percent;

					MeshChunk->position(x,   y,   z+1);		MeshChunk->textureCoord(left, down); MeshChunk->colour(BlockLight1 , BlockLight1 , BlockLight1 );
					MeshChunk->position(x+1, y,   z+1);		MeshChunk->textureCoord(right, down); MeshChunk->colour(BlockLight1 , BlockLight1 , BlockLight1 );
					MeshChunk->position(x+1, y+1, z+1);		MeshChunk->textureCoord(right, up); MeshChunk->colour(BlockLight1 , BlockLight1 , BlockLight1 );
					MeshChunk->position(x,   y+1, z+1);		MeshChunk->textureCoord(left, up); MeshChunk->colour(BlockLight1 , BlockLight1 , BlockLight1 );

					MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
					MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

					iVertex += 4;
				}

			}
		}
	}


	MeshChunk->end();
}

void CraftWorld::rebuildTransparentChunk(int id)
{
	SimpleMeshChunk* MeshChunk = mWaterChunks[id];

	int iVertex = 0;
	block_t Block;
	block_t Block1;
	bool transparentBlock;

	/* Only create visible faces of each chunk */
	bool DefaultBlock = false;
	int SX = 0;
	int SY = 0;
	int SZ = 0;
	int MaxSize = WORLD_SIZE;

	int StartZ = MeshChunk->chunkStartZ;
	int StartY = MeshChunk->chunkStartY;
	int StartX = MeshChunk->chunkStartX;

	MeshChunk->reset();

	for (int z = StartZ; z < CHUNK_SIZE + StartZ; ++z)
	{
		for (int y = StartY; y < CHUNK_SIZE + StartY; ++y)
		{
			for (int x = StartX; x < CHUNK_SIZE + StartX; ++x)
			{
				Block = GetBlock(x,y,z);
				if (BlockTransparent(x,y,z) == false || Block == 0) continue;

				BaseBlock *blockType = &blockTypes[Block];

				float down = 1.0f - percent * (blockType->textureRow + 1);
				float up = down + percent;

				float left = percent * blockType->upPlane;
				float right = left + percent;

				//x-1
				transparentBlock = DefaultBlock;
				Block1 = 1;
				if (x > SX)
				{
					transparentBlock = BlockTransparent(x-1,y,z);
					Block1 = GetBlock(x-1,y,z);
				}

				if (transparentBlock == false || Block1 == 0)
				{
					left = percent * blockType->sidePlane;
					right = left + percent;

					MeshChunk->position(x, y,   z+1);	MeshChunk->textureCoord(right, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x, y+1, z+1);	MeshChunk->textureCoord(right, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x, y+1, z);		MeshChunk->textureCoord(left, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x, y,   z);		MeshChunk->textureCoord(left, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );

					MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
					MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

					iVertex += 4;
				}

				//x+1
				transparentBlock = DefaultBlock;
				Block1 = 1;
				if (x < SX + MaxSize - 1)
				{
					transparentBlock = BlockTransparent(x+1,y,z);
					Block1 = GetBlock(x+1,y,z);
				}

				if (transparentBlock == false || Block1 == 0)
				{
					left = percent * blockType->sidePlane;
					right = left + percent;

					MeshChunk->position(x+1, y,   z);	MeshChunk->textureCoord(right, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x+1, y+1, z);	MeshChunk->textureCoord(right, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x+1, y+1, z+1);	MeshChunk->textureCoord(left, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x+1, y,   z+1);	MeshChunk->textureCoord(left, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );

					MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
					MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

					iVertex += 4;
				}

				//y-1
				transparentBlock = DefaultBlock;
				Block1 = 1;
				if (y > SY)
				{
					transparentBlock = BlockTransparent(x,y-1,z);
					Block1 = GetBlock(x,y-1,z);
				}

				if (transparentBlock == false || Block1 == 0)
				{
					//up
					left = percent * blockType->downPlane;
					right = left + percent;

					MeshChunk->position(x,   y, z);		MeshChunk->textureCoord(left, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x+1, y, z);		MeshChunk->textureCoord(right, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x+1, y, z+1);	MeshChunk->textureCoord(right, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x,   y, z+1);	MeshChunk->textureCoord(left, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );

					MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
					MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

					iVertex += 4;
				}


				//y+1
				transparentBlock = DefaultBlock;
				Block1 = 1;
				if (y < SY + MaxSize - 1)
				{
					transparentBlock = BlockTransparent(x,y+1,z);
					Block1 = GetBlock(x,y+1,z);
				}

				if (transparentBlock == false || Block1 == 0)
				{
					//down
					left = percent * blockType->upPlane;
					right = left + percent;

					MeshChunk->position(x,   y+1, z+1);		MeshChunk->textureCoord(left, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x+1, y+1, z+1);		MeshChunk->textureCoord(right, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x+1, y+1, z);		MeshChunk->textureCoord(right, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x,   y+1, z);		MeshChunk->textureCoord(left, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );

					MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
					MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

					iVertex += 4;
				}

				//z-1
				transparentBlock = DefaultBlock;
				Block1 = 1;
				if (z > SZ)
				{
					transparentBlock = BlockTransparent(x,y,z-1);
					Block1 = GetBlock(x,y,z-1);
				}

				if (transparentBlock == false || Block1 == 0)
				{
					left = percent * blockType->sidePlane;
					right = left + percent;

					MeshChunk->position(x,   y+1, z);		MeshChunk->textureCoord(right, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x+1, y+1, z);		MeshChunk->textureCoord(left, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x+1, y,   z);		MeshChunk->textureCoord(left, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x,   y,   z);		MeshChunk->textureCoord(right, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );

					MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
					MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

					iVertex += 4;
				}


				//z+1
				transparentBlock = DefaultBlock;
				Block1 = 1;
				if (z < SZ + MaxSize - 1)
				{
					transparentBlock = BlockTransparent(x,y,z+1);
					Block1 = GetBlock(x,y,z+1);
				}

				if (transparentBlock == false || Block1 == 0)
				{
					left = percent * blockType->sidePlane;
					right = left + percent;

					MeshChunk->position(x,   y,   z+1);		MeshChunk->textureCoord(left, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x+1, y,   z+1);		MeshChunk->textureCoord(right, down); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x+1, y+1, z+1);		MeshChunk->textureCoord(right, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );
					MeshChunk->position(x,   y+1, z+1);		MeshChunk->textureCoord(left, up); MeshChunk->colour(1.0f , 1.0f , 1.0f );

					MeshChunk->triangle(iVertex, iVertex+1, iVertex+2);
					MeshChunk->triangle(iVertex+2, iVertex+3, iVertex);

					iVertex += 4;
				}

			}
		}
	}


	MeshChunk->end();
}

void CraftWorld::rebuildNearestChunks(int id,Vector3 pos)
{
	Vector3 test = mChunks[id]->bBox.onBorder(pos);
	Vector3 temp = pos;
	if(test.x != 0)
	{
		temp.x +=test.x;
		//sprawdz czy jest tam powietrze czy coœ innego
		if(GetBlock(temp.x,temp.y,temp.z) != 0)
		{
			int chunkNumber = getChunkId(temp);

			if(chunkNumber != -1)
			{
				rebuildChunk(chunkNumber);
				rebuildTransparentChunk(chunkNumber);
			}
		}
	}


	if(test.y != 0)
	{
		temp = pos;
		temp.y +=test.y;
		//sprawdz czy jest tam powietrze czy coœ innego
		if(GetBlock(temp.x,temp.y,temp.z) != 0)
		{
			int chunkNumber = getChunkId(temp);

			if(chunkNumber != -1)
				rebuildChunk(chunkNumber);
		}
	}

	if(test.z != 0)
	{
		temp = pos;
		temp.z +=test.z;
		//sprawdz czy jest tam powietrze czy coœ innego
		if(GetBlock(temp.x,temp.y,temp.z) != 0)
			{
			int chunkNumber = getChunkId(temp);

			if(chunkNumber != -1)
				rebuildChunk(chunkNumber);
		}
	}

}

void CraftWorld::createWorldChunkss()
{
	createdChunksCount = 0;

	for (int z = 0; z < WORLD_SIZE; z += CHUNK_SIZE)
	{
		for (int y = 0; y < WORLD_SIZE; y += CHUNK_SIZE)
		{
			for (int x = 0; x < WORLD_SIZE; x += CHUNK_SIZE)
			{
				createChunks(x,y,z);
				createTransparentChunks(x,y,z);
			}
		}
	}

}

void CraftWorld::drawCubes(int i)
{
	sceGuColor(0xFFFFFFFF);
	sceGuEnable(GU_TEXTURE_2D);
	if(blockTypes[i].transparent)
		sceGuEnable(GU_BLEND);
	sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF |GU_COLOR_8888| GU_VERTEX_32BITF | GU_TRANSFORM_3D, 36, 0, blockTypes[i].vertices);
	if(blockTypes[i].transparent)
		sceGuDisable(GU_BLEND);
	sceGuDisable(GU_TEXTURE_2D);
}

void CraftWorld::drawWorld(Frustum &camFrustum)
{
	drawnTriangles = 0;

	sceGuColor(0xFFFFFFFF);
	sceGuEnable(GU_TEXTURE_2D);
	for(unsigned int i = 0;i < mChunks.size();i++)
	{
		if(mChunks[i]->trienglesCount > 0 || !mChunks[i]->created)
		{
			if(playerZoneBB.intersect(mChunks[i]->bBox))
			{
				if(camFrustum.BoxInFrustum(mChunks[i]->bBox) == Frustum::Intersects)
				{
					//check if wee need to build vertices of this chunk
					if(!mChunks[i]->created && /*mChunks[i]->lastTriangleCount != 0 &&*/ chunksCreatedInFrameCount == 0)
					{
						rebuildChunk(i);
						chunksCreatedInFrameCount++;
					}

					mChunks[i]->drawChunk();
					drawnTriangles += mChunks[i]->trienglesCount;
				}
			}else if(mChunks[i]->created)
			{
				mChunks[i]->reset();
			}
		}
	}


	sceGuColor(0x55FFFFFF);
	sceGuEnable(GU_BLEND);
	for(unsigned int i = 0;i < mWaterChunks.size();i++)
	{
		if(mWaterChunks[i]->trienglesCount > 0 || !mWaterChunks[i]->created)
		{
			if(playerZoneBB.intersect(mWaterChunks[i]->bBox))
			{
				if(camFrustum.BoxInFrustum(mWaterChunks[i]->bBox) == Frustum::Intersects)
				{
					//check if wee need to build vertices of this chunk
					if(!mWaterChunks[i]->created /*&& mChunks[i]->lastTriangleCount != 0*/ && transparentchunksCreatedInFrameCount == 0)
					{
						rebuildTransparentChunk(i);
						transparentchunksCreatedInFrameCount++;
					}

					mWaterChunks[i]->drawChunk();
					drawnTriangles += mChunks[i]->trienglesCount;
				}
			}else if(mWaterChunks[i]->created)
			{
				mWaterChunks[i]->reset();
			}
		}
	}
	sceGuDisable(GU_BLEND);
	sceGuDisable(GU_TEXTURE_2D);
}

void CraftWorld::UpdatePlayerZoneBB(Vector3 playerPosition)
{
	playerPos = playerPosition;
	playerZoneBB = BoundingBox(Vector3(playerPosition.x - playerZoneSize.x,playerPosition.y - playerZoneSize.y,playerPosition.z - playerZoneSize.z),Vector3(playerPosition.x + playerZoneSize.x,playerPosition.y + playerZoneSize.y,playerPosition.z + playerZoneSize.z));
}

Vector3 CraftWorld::GetPlayerPos()
{
	return playerPos;
}

void CraftWorld::UpdateWorldTime(float dt)
{
	worldTime += dt;

	//stuff that goes on tick
	//1 tick = 1 second

	//rebuilding chunks
	updateChunkTimer += dt;
	if(updateChunkTimer > 0.3f)//update time 1/4 second
	{
		chunksCreatedInFrameCount = 0;
		transparentchunksCreatedInFrameCount = 0;
		updateChunkTimer = 0.0f;
	}

	if(worldTime > 1.0f)
	{
		worldTime = 0.0f;
	}
}

int CraftWorld::groundHeight(const int x, const int z)
{
	if (x < 0 || z < 0 || x >= WORLD_SIZE || z >= WORLD_SIZE) return -1;

	for (int y = WORLD_SIZE - 1; y >= 0; --y)
	{
		block_t Block = GetBlock(x, y, z);
		if (Block != 0 && Block != 4) return (y);
	}

	return -1;
}

int CraftWorld::getChunkId(Vector3 pos)
{
	for(unsigned int i = 0;i < mChunks.size();i++)
	{
		if(mChunks[i]->bBox.contains(pos))
			return mChunks[i]->id;
	}

	return -1;
}

bool CraftWorld::SolidAtPoint(Vector3 pos)
{
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;

	//outside the world so collision
	if (x <= 0 || y <= 0 || z <= 0  || x >= WORLD_SIZE-1 || y >= WORLD_SIZE-1 || z >= WORLD_SIZE-1) return true;

	//don't collide with water and air
	if(GetBlock(x,y,z) == 0 || GetBlock(x,y,z) == 4)
	{
		//
		return false;
	}

	//collision
	return true;
}

int CraftWorld::BlockSoundAtPos(Vector3 pos)
{
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;

	//outside the world so collision
	if (x <= 0 || y <= 0 || z <= 0  || x >= WORLD_SIZE-1 || y >= WORLD_SIZE-1 || z >= WORLD_SIZE-1) return -1;

	return blockTypes[GetBlock(x,y,z)].soundType;
}

bool CraftWorld::SolidAtPointForPlayer(Vector3 pos)
{
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;

	//outside the world so collision
	if (x <= 0 || y <= 0 || z <= 0  || x >= WORLD_SIZE-1 || y >= WORLD_SIZE-1 || z >= WORLD_SIZE-1) return true;

	//don't collide with water and air
	if(GetBlock(x,y,z) == 0 || GetBlock(x,y,z) == 4)
	{
		//4 corners
		if(SolidAtPoint(Vector3(pos.x-0.15f,pos.y,pos.z-0.15f))) return true;
		if(SolidAtPoint(Vector3(pos.x+0.15f,pos.y,pos.z+0.15f))) return true;
		if(SolidAtPoint(Vector3(pos.x+0.15f,pos.y,pos.z-0.15f))) return true;
		if(SolidAtPoint(Vector3(pos.x-0.15f,pos.y,pos.z+0.15f))) return true;
		//
		return false;
	}

	//collision
	return true;
}

bool CraftWorld::PlayerInWater(Vector3 pos)
{
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;

	if (x < 0 || y < 0 || z < 0  || x >= WORLD_SIZE-1 || y >= WORLD_SIZE-1 || z >= WORLD_SIZE-1) return false;

	if(GetBlock(x,y,z) == 4)
		return true;

	return false;
}

block_t CraftWorld::BlockAtPoint(Vector3 pos)
{
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;

	if (x < 0 || y < 0 || z < 0  || x >= WORLD_SIZE || y >= WORLD_SIZE || z >= WORLD_SIZE) return -1;

	return GetBlock(x,y,z);
}
