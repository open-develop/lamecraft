#include "StatePlay.h"

StatePlay::StatePlay()
{
	mRender = NULL;
	mSystemMgr = NULL;
	fppCam = NULL;

	//utils
	freeMemory = 0;
	freeMemoryTimer = 0.0f;

	showCube = true;

	//zmienne do poruszania
	GRAVITY = -9.8f;
	JUMPVELOCITY = 4.0f;
	CLIMBVELOCITY = 2.5f;

	walkingOnGround = false;
	jumping = false;
	headInWater = false;
	footInWater = false;

	selectedCube = 0;
	selectedCubeSet = 0;
	selectedCubeStart = 80;

	ram1 = 0;
	ram2 = 0;
	dt = 0.0f;
	sunTime = 32400;
	sunTimeTick = 0.0f;

	cameraSpeed = 2.0f / 60.0f;
	cameraMoveSpeed = 4.0f/ 60.0f;

	//stete to game
	menuState = 0;
	selectPos = 0;

	//flying
	canFly = false;
	devMode = false;
	makeScreen = false;
}

StatePlay::~StatePlay()
{

}

void StatePlay::InitCamera()
{
	fppCam = new Camera();
	fppCam->PositionCamera(playerPosition.x,playerPosition.y,playerPosition.z, playerPosition.x,playerPosition.y,playerPosition.z-5.0f, 0.0f,1.0f,0.0f);
	mRender->SetActiveCamera(fppCam);
}

void StatePlay::Init()
{
	//set render manager instance
	mRender = RenderManager::InstancePtr();
	mSystemMgr = SystemManager::Instance();
	mSoundMgr = SoundManager::Instance();
	mIhelper = InputHelper::Instance();

	//
	playerPosition = newPlayerPos = oldPlayerPos = Vector3(64.0f,90.0f,64.0f);

	//then create our perfect world
	mWorld = new CraftWorld();

	mWorld->initRandompMap(128,16);
	mWorld->setTextureSize(128,16);
	mWorld->initWorldBlocksLight();
	mWorld->SetWolrdTime(9);
	mWorld->UpdatePlayerZoneBB(playerPosition);
	mWorld->buildMap();
	mWorld->buildblocksVerts();

	dt = mTimer.GetDeltaTime();

	Logger::Instance()->LogMessage("created chunks: %d\n",mWorld->createdChunksCount);

	//texture
	TextureManager::Instance()->LoadTexture("Assets/Lamecraft/terrain_medium.png");
	texture = TextureManager::Instance()->GetTextureNumber("Assets/Lamecraft/terrain_medium.png");

	//water filter
	TextureManager::Instance()->LoadTexture("Assets/Lamecraft/blue.png");
	blue  = TextureManager::Instance()->GetTextureNumber("Assets/Lamecraft/blue.png");

	//bar image
	barSprite = new Sprite("Assets/Lamecraft/utils.png",0,0,182,22);
	barSprite->SetPosition(240,253);
	barSprite->Scale(1.75f,1.75f);

	selectSprite = new Sprite("Assets/Lamecraft/utils.png",1,23,22,22);
	selectSprite->SetPosition(100,253);
	selectSprite->Scale(1.75f,1.75f);

	crossSprite = new Sprite("Assets/Lamecraft/utils.png",201,13,9,9);
	crossSprite->SetPosition(240,136);
	crossSprite->Scale(2.0f,2.0f);

	buttonSprite = new Sprite("Assets/Lamecraft/utils.png",24,22,200,20);
	buttonSprite->SetPosition(240,150);

	sbuttonSprite = new Sprite("Assets/Lamecraft/utils.png",24,42,200,20);
	sbuttonSprite->SetPosition(240,150);

	//ram2 = mSystemMgr->ramAvailable();

	cubeModel = new ObjModel();
	cubeModel->LoadObj("Assets/Lamecraft/textureCube.obj");
	cubeModel->Optimize();

	//sky dome
	skyDome = new SkyDome();
	skyDome->CreateSkyDomeMesh();

	TextureManager::Instance()->LoadTexture("Assets/Lamecraft/sky.png");
	skyDome->SetTexture(TextureManager::Instance()->GetTextureNumber("Assets/Lamecraft/sky.png"));
	skyDome->timeOfDay = 0.1f;

	//sky light
	//8 am = 28800 seconds
	// 10 am = 36000 seconds
	// 12 pm = 43200 seconds
	//1 second = 72
	//1 hour = 3600
	skyLight = new SkyLight();
	TextureManager::Instance()->LoadTexture("Assets/Lamecraft/sun.png");
	skyLight->SetTexture(TextureManager::Instance()->GetTextureNumber("Assets/Lamecraft/sun.png"));

	menuOptions = false;
	optionsMenuPos = 0;

	analogLeft = analogRight = analogUp = analogDown = false;

	walkSoundAccu = 0.0f;
	isWalking = false;
}

void StatePlay::InitParametric(int terrainType,bool makeFlat,bool makeTrees,bool makeWater,bool makeCaves)
{
	//set render manager instance
	mRender = RenderManager::InstancePtr();
	mSystemMgr = SystemManager::Instance();
	mSoundMgr = SoundManager::Instance();
	mIhelper = InputHelper::Instance();

	//
	playerPosition = newPlayerPos = oldPlayerPos = Vector3(64.0f,90.0f,64.0f);

	//then create our perfect world
	mWorld = new CraftWorld();

	mWorld->initRandompMap(128,16,terrainType,makeFlat,makeTrees,makeWater,makeCaves);
	mWorld->setTextureSize(128,16);
	mWorld->initWorldBlocksLight();
	mWorld->SetWolrdTime(9);
	mWorld->UpdatePlayerZoneBB(playerPosition);
	mWorld->buildMap();
	mWorld->buildblocksVerts();

	dt = mTimer.GetDeltaTime();

	Logger::Instance()->LogMessage("created chunks: %d\n",mWorld->createdChunksCount);

	//texture
	TextureManager::Instance()->LoadTexture("Assets/Lamecraft/terrain_medium.png");
	texture = TextureManager::Instance()->GetTextureNumber("Assets/Lamecraft/terrain_medium.png");

	//water filter
	TextureManager::Instance()->LoadTexture("Assets/Lamecraft/blue.png");
	blue  = TextureManager::Instance()->GetTextureNumber("Assets/Lamecraft/blue.png");

	//bar image
	barSprite = new Sprite("Assets/Lamecraft/utils.png",0,0,182,22);
	barSprite->SetPosition(240,253);
	barSprite->Scale(1.75f,1.75f);

	selectSprite = new Sprite("Assets/Lamecraft/utils.png",1,23,22,22);
	selectSprite->SetPosition(100,253);
	selectSprite->Scale(1.75f,1.75f);

	crossSprite = new Sprite("Assets/Lamecraft/utils.png",201,13,9,9);
	crossSprite->SetPosition(240,136);
	crossSprite->Scale(2.0f,2.0f);

	buttonSprite = new Sprite("Assets/Lamecraft/utils.png",24,22,200,20);
	buttonSprite->SetPosition(240,150);

	sbuttonSprite = new Sprite("Assets/Lamecraft/utils.png",24,42,200,20);
	sbuttonSprite->SetPosition(240,150);

	//ram2 = mSystemMgr->ramAvailable();

	cubeModel = new ObjModel();
	cubeModel->LoadObj("Assets/Lamecraft/textureCube.obj");
	cubeModel->Optimize();

	//sky dome
	skyDome = new SkyDome();
	skyDome->CreateSkyDomeMesh();

	TextureManager::Instance()->LoadTexture("Assets/Lamecraft/sky.png");
	skyDome->SetTexture(TextureManager::Instance()->GetTextureNumber("Assets/Lamecraft/sky.png"));
	skyDome->timeOfDay = 0.1f;

	//sky light
	//8 am = 28800 seconds
	// 10 am = 36000 seconds
	// 12 pm = 43200 seconds
	skyLight = new SkyLight();
	TextureManager::Instance()->LoadTexture("Assets/Lamecraft/sun.png");
	skyLight->SetTexture(TextureManager::Instance()->GetTextureNumber("Assets/Lamecraft/sun.png"));



	menuOptions = false;
	optionsMenuPos = 0;

	analogLeft = analogRight = analogUp = analogDown = false;

	walkSoundAccu = 0.0f;
	isWalking = false;
}

void StatePlay::SetWorldAndSaveName(std::string worldName,std::string fileName)
{
	if(mWorld != NULL)
	{
		sprintf(mWorld->worldName,"%s",worldName.c_str());
	}
	saveFileName = fileName;
}

void StatePlay::LoadMap(std::string fileName,bool compressed)
{
	//set render manager instance
	mRender = RenderManager::InstancePtr();
	mSystemMgr = SystemManager::Instance();
	mSoundMgr = SoundManager::Instance();
	mIhelper = InputHelper::Instance();

	//save name
	saveFileName = fileName;
	//
	playerPosition = newPlayerPos = oldPlayerPos = Vector3(64.0f,90.0f,64.0f);

	//then create our perfect world
	mWorld = new CraftWorld();

	if(compressed)
	{
		mWorld->LoadCompressedWorld(saveFileName);
		//set player pos from map
		playerPosition = mWorld->GetPlayerPos();
		newPlayerPos  = mWorld->GetPlayerPos();
		oldPlayerPos = mWorld->GetPlayerPos();
	}
	else
		mWorld->LoadWorld(saveFileName.c_str());

	mWorld->SetWolrdTime(12);

	mWorld->setTextureSize(128,16);

	mWorld->UpdatePlayerZoneBB(playerPosition);
	mWorld->buildMap();
	mWorld->buildblocksVerts();

	dt = mTimer.GetDeltaTime();

	Logger::Instance()->LogMessage("created chunks: %d\n",mWorld->createdChunksCount);


	//texture
	TextureManager::Instance()->LoadTexture("Assets/Lamecraft/terrain_medium.png");
	texture = TextureManager::Instance()->GetTextureNumber("Assets/Lamecraft/terrain_medium.png");

	//water filter
	TextureManager::Instance()->LoadTexture("Assets/Lamecraft/blue.png");
	blue  = TextureManager::Instance()->GetTextureNumber("Assets/Lamecraft/blue.png");

	//bar image
	barSprite = new Sprite("Assets/Lamecraft/utils.png",0,0,182,22);
	barSprite->SetPosition(240,253);
	barSprite->Scale(1.75f,1.75f);

	selectSprite = new Sprite("Assets/Lamecraft/utils.png",1,23,22,22);
	selectSprite->SetPosition(100,253);
	selectSprite->Scale(1.75f,1.75f);

	crossSprite = new Sprite("Assets/Lamecraft/utils.png",201,13,9,9);
	crossSprite->SetPosition(240,136);
	crossSprite->Scale(2.0f,2.0f);

	buttonSprite = new Sprite("Assets/Lamecraft/utils.png",24,22,200,20);
	buttonSprite->SetPosition(240,150);

	sbuttonSprite = new Sprite("Assets/Lamecraft/utils.png",24,42,200,20);
	sbuttonSprite->SetPosition(240,150);

	//ram2 = mSystemMgr->ramAvailable();

	cubeModel = new ObjModel();
	cubeModel->LoadObj("Assets/Lamecraft/textureCube.obj");
	cubeModel->Optimize();

	//sky dome
	skyDome = new SkyDome();
	skyDome->CreateSkyDomeMesh();

	TextureManager::Instance()->LoadTexture("Assets/Lamecraft/sky.png");
	skyDome->SetTexture(TextureManager::Instance()->GetTextureNumber("Assets/Lamecraft/sky.png"));
	skyDome->timeOfDay = 0.1f;

	//sky light
	//8 am = 28800 seconds
	// 10 am = 36000 seconds
	// 12 pm = 43200 seconds
	skyLight = new SkyLight();
	TextureManager::Instance()->LoadTexture("Assets/Lamecraft/sun.png");
	skyLight->SetTexture(TextureManager::Instance()->GetTextureNumber("Assets/Lamecraft/sun.png"));


	menuOptions = false;
	optionsMenuPos = 0;

	analogLeft = analogRight = analogUp = analogDown = false;

	walkSoundAccu = 0.0f;
	isWalking = false;
}

void StatePlay::Enter()
{
	RenderManager::InstancePtr()->SetPerspective(55.0f, 480.0f / 272.0f, 0.18f, 1000.f);
}

void StatePlay::CleanUp()
{
	delete mRender->mCam;
	mRender->mCam = NULL;

	delete buttonSprite;
	delete sbuttonSprite;
	delete barSprite;
	delete selectSprite;
	delete crossSprite;
	delete cubeModel;
	delete skyDome;
	//delete fppCam;
	delete mWorld;
}

void StatePlay::Pause()
{

}

void StatePlay::Resume()
{

}

void StatePlay::HandleEvents(StateManager* sManager)
{
	//update delta time
	dt = mTimer.GetDeltaTime();

	cameraSpeed = 2.0f * dt;
	cameraMoveSpeed = 4.0f * dt;

	//free memory counter
	if(devMode)
	{
		freeMemoryTimer += dt;
		if(freeMemoryTimer > 1.0f)
		{
			freeMemory = mSystemMgr->ramAvailable();
			freeMemoryTimer = 0.0f;
		}
	}

	//update input
	mSystemMgr->InputUpdate();

	if(menuState == 0)//game state
	{
		//menu
		if(keyPressed(InputHelper::Instance()->getButtonToAction(15)))
		{
			menuState = 1;
			menuOptions = false;
			optionsMenuPos = 0;
		}

		//camera input
		//rotate down
		if(keyHold(InputHelper::Instance()->getButtonToAction(5)))
		{
			fppCam->PitchView(-cameraSpeed);
		}
		//rotate up
		if(keyHold(InputHelper::Instance()->getButtonToAction(4)))
		{
			fppCam->PitchView(cameraSpeed);
		}
		//rotate right
		if(keyHold(InputHelper::Instance()->getButtonToAction(7)))
		{
			fppCam->RotateView(-cameraSpeed,0,1,0);
		}
		//rotate left
		if(keyHold(InputHelper::Instance()->getButtonToAction(6)))
		{
			fppCam->RotateView(cameraSpeed,0,1,0);
		}

		//move right
		if(keyHold(InputHelper::Instance()->getButtonToAction(3)))
		{
			fppCam->StrafePhysic(cameraMoveSpeed);
		}
		//move left
		if(keyHold(InputHelper::Instance()->getButtonToAction(2)))
		{
			fppCam->StrafePhysic(-cameraMoveSpeed);
		}

		//move back
		if(keyHold(InputHelper::Instance()->getButtonToAction(1)))
		{
			if(canFly)
				fppCam->MovePhysic(-cameraMoveSpeed);
			else
				fppCam->MovePhysicNoY(-cameraMoveSpeed);
		}

		//move forward
		if(keyHold(InputHelper::Instance()->getButtonToAction(0)))
		{
			if(canFly)
				fppCam->MovePhysic(cameraMoveSpeed);
			else
				fppCam->MovePhysicNoY(cameraMoveSpeed);
		}

		//cubes management
		//switch right
		if(keyPressed(InputHelper::Instance()->getButtonToAction(9)))
		{
			selectedCube++;
			if(selectedCube > 8)
				selectedCube = 0;

			selectSprite->SetPosition(100 + (selectedCube * 35),253);
		}
		//switch right
		if(keyPressed(InputHelper::Instance()->getButtonToAction(8)))
		{
			selectedCube--;
			if(selectedCube < 0)
				selectedCube = 8;

			selectSprite->SetPosition(100 + (selectedCube * 35),253);
		}
		//switch down
		if(keyPressed(InputHelper::Instance()->getButtonToAction(11)))
		{
			selectedCubeSet-=9;
			if(selectedCubeSet < 0)
				selectedCubeSet = 36;
		}
		//switch up
		if(keyPressed(InputHelper::Instance()->getButtonToAction(10)))
		{
			selectedCubeSet+=9;
			if(selectedCubeSet > 36)
				selectedCubeSet = 0;
		}

		//add cube
		if(keyPressed(InputHelper::Instance()->getButtonToAction(14)))
		{
			//add cube
			Vector3 rayDir = fppCam->m_vView - fppCam->m_vPosition;
			rayDir.normalize();

			//pobieramy pozycje gdzie patrzymy jak i nasz¹ pozycje
			Vector3 testPos;

			//jedziemy co kawa³ek a¿ do celu np. co 0.5
			for(float i = 0;i < 5.25f;i+=0.25f)
			{
				testPos = fppCam->m_vPosition + (rayDir * i);

				//sprawdzamy czy tykamy coœ
				if(mWorld->BlockEditable(testPos.x,testPos.y,testPos.z))
				{
					BoundingBox testBox = BoundingBox(Vector3(cubePos.x - 0.5f,cubePos.y - 0.5f,cubePos.z - 0.5f),Vector3(cubePos.x + 0.5f,cubePos.y + 0.5f,cubePos.z + 0.5f));
					Ray tesRay = Ray(fppCam->m_vPosition,rayDir);

					float distance = -1.0f;
					tesRay.hasIntersected(testBox,distance);

					if(distance != -1.0f)
					{
						Vector3 hitPos = fppCam->m_vPosition + (rayDir * distance);
						Vector3 normal = (hitPos - cubePos);
						normal.normalize();

						Vector3 testPos2;

						if(normal.x < -0.66f)
							testPos2.x = -1.0f;
						else if(normal.x > 0.66f)
							testPos2.x = 1.0f;
						else
							testPos2.x = 0.0f;

						if(normal.y < -0.66f)
							testPos2.y = -1.0f;
						else if(normal.y > 0.66f)
							testPos2.y = 1.0f;
						else
							testPos2.y = 0.0f;

						if(normal.z < -0.66f)
							testPos2.z = -1.0f;
						else if(normal.z > 0.66f)
							testPos2.z = 1.0f;
						else
							testPos2.z = 0.0f;


						testPos2 += testPos;

						//check if this block is empty
						if(mWorld->GetBlock(testPos2.x,testPos2.y,testPos2.z) == 0)
						{
							//now check if player is not there ;) we don't want to place cubes where we stand
							int boxX = (int)testPos2.x;
							int boxY = (int)testPos2.y;
							int boxZ = (int)testPos2.z;

							BoundingBox blockBox = BoundingBox(Vector3(boxX,boxY,boxZ),Vector3(boxX + 1,boxY + 1,boxZ + 1));
							BoundingBox playerBox = BoundingBox(Vector3(playerPosition.x - 0.15f,playerPosition.y - 1.5f,playerPosition.z - 0.15f),Vector3(playerPosition.x + 0.15f,playerPosition.y + 0.2f,playerPosition.z + 0.15f));

							if(!blockBox.intersect(playerBox))
							{
								mWorld->GetBlock(testPos2.x,testPos2.y,testPos2.z) = selectedCubeSet + selectedCube+1;//10;//set block type

								int chunkTarget = mWorld->getChunkId(testPos2);

								if(chunkTarget != -1)
								{
									mSoundMgr->PlayPlopSound();
									//rebuild
									//Logger::Instance()->LogMessage("chunk: %d\n",chunkTarget);

									mWorld->rebuildChunk(chunkTarget);
									mWorld->rebuildTransparentChunk(chunkTarget);
									mWorld->rebuildNearestChunks(chunkTarget,testPos2);

								}

								fppCam->needUpdate = true;
							}
						}
					}

					break;
				}
			}

		}

		//remove cube
		if(keyPressed(InputHelper::Instance()->getButtonToAction(13)))
		{
			//remove cube
			Vector3 rayDir = fppCam->m_vView - fppCam->m_vPosition;
			rayDir.normalize();

			//pobieramy pozycje gdzie patrzymy jak i nasz¹ pozycje
			Vector3 testPos;

			//jedziemy co kawa³ek a¿ do celu np. co 0.5
			for(float i = 0;i < 5.25f;i+=0.25f)
			{
				testPos = fppCam->m_vPosition + (rayDir * i);

				//sprawdzamy czy tykamy coœ
				if(mWorld->BlockEditable(testPos.x,testPos.y,testPos.z))
				{

					mSoundMgr->PlayWalkSound(mWorld->BlockSoundAtPos(testPos));
					mWorld->GetBlock(testPos.x,testPos.y,testPos.z) = 0;

					int	chunkTarget = mWorld->getChunkId(testPos);

					if(chunkTarget != -1)
					{
						//rebuild
						mWorld->rebuildChunk(chunkTarget);
						mWorld->rebuildTransparentChunk(chunkTarget);
						mWorld->rebuildNearestChunks(chunkTarget,testPos);
					}

					fppCam->needUpdate = true;
					break;
				}
			}

		}

		//jump
		if(keyHold(InputHelper::Instance()->getButtonToAction(12)))
		{
			if(walkingOnGround || headInWater)
			{
				jumping = true;
			}
		}
	}else if(menuState == 1)//menu state
	{
		//turn off menu with the same key
		if(keyPressed(InputHelper::Instance()->getButtonToAction(15)))
		{
			menuState = 0;
			menuOptions = false;
			optionsMenuPos = 0;
			selectPos = 0;
		}

		//menu keys
		if(menuOptions)
		{
			//up, down
			if(mSystemMgr->KeyPressed(PSP_CTRL_UP))
			{
				optionsMenuPos--;
				if(optionsMenuPos < 0)
					optionsMenuPos = 5;

				mSoundMgr->PlayMenuSound();
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
			{
				optionsMenuPos++;
				if(optionsMenuPos > 5)
					optionsMenuPos = 0;

				mSoundMgr->PlayMenuSound();
			}

			//back
			if(mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
			{
				selectPos = 0;
				menuOptions = false;
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
			{
				//fly
				if(optionsMenuPos == 0)
					canFly = !canFly;

				//devmode
				if(optionsMenuPos == 1)
					devMode = !devMode;

				//take screenshot
				if(optionsMenuPos == 2)
					makeScreen = true;

				//rename
				if(optionsMenuPos == 3)
				{
					unsigned short test[128];
					unsigned short opis[10] = {'W','o','r','l','d',' ','n','a','m','e'};
					if(mSystemMgr->ShowOSK(opis,test,128) != -1)
					{
						std::string newWorldName = "";
						for(int j = 0; test[j]; j++)
						{
							unsigned c = test[j];

							if(32 <= c && c <= 127) // print ascii only
								newWorldName += c;
						}

						sprintf(mWorld->worldName,"%s",newWorldName.c_str());
					}
				}

				if(optionsMenuPos == 4)
					mSoundMgr->playerSounds = !mSoundMgr->playerSounds;

				//back to ingame menu
				if(optionsMenuPos == 5)
				{
					selectPos = 0;
					menuOptions = false;
				}
			}
		}else
		{
			//up, down
			if(mSystemMgr->KeyPressed(PSP_CTRL_UP))
			{
				selectPos--;
				if(selectPos < 0)
					selectPos = 4;

				mSoundMgr->PlayMenuSound();
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
			{
				selectPos++;
				if(selectPos > 4)
					selectPos = 0;

				mSoundMgr->PlayMenuSound();
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
			{
				menuState = 0;
				selectPos = 0;
				optionsMenuPos = 0;
				menuOptions = false;
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
			{
				if(selectPos == 0)//resume
				{
					menuState = 0;
				}
				if(selectPos == 1)//options
				{
					optionsMenuPos = 0;
					menuOptions = true;
				}
				if(selectPos == 2)//save
				{
					//mWorld->SaveWorld(saveFileName.c_str());
					mWorld->SaveCompressedWorld(saveFileName.c_str());
					menuState = 0;
				}
				if(selectPos == 3)//save end exit
				{
					//save
					//mWorld->SaveWorld(saveFileName.c_str());
					mWorld->SaveCompressedWorld(saveFileName.c_str());
					//exit
					sManager->PopState();
				}
				if(selectPos == 4)//exit
				{
					//exit
					sManager->PopState();
				}
			}
		}
	}
}

void StatePlay::Update(StateManager* sManager)
{
	if(menuState == 0)//game state
	{
		if(fppCam->needUpdate)
		{
			showCube = false;

			//show pick cube
			Vector3 rayDir = fppCam->m_vView - fppCam->m_vPosition;
			rayDir.normalize();

			//pobieramy pozycje gdzie patrzymy jak i nasz¹ pozycje
			Vector3 testPos ;

			//jedziemy co kawa³ek a¿ do celu np. co 0.5
			for(float i = 0;i < 5.25f;i+=0.25f)
			{
				testPos = fppCam->m_vPosition + (rayDir * i);

				//sprawdzamy czy tykamy coœ
				if(mWorld->BlockEditable(testPos.x,testPos.y,testPos.z))
				{
					//i pokazujemy pickniêty cube
					cubePos.x = (int)testPos.x + 0.5f;
					cubePos.y = (int)testPos.y + 0.5f;
					cubePos.z = (int)testPos.z + 0.5f;

					showCube = true;
					break;
				}
			}

		}

		int soundBlockType = -1;

		//update player position
		if(dt < 0.1f)
		{
			Vector3 delta = fppCam->m_vView - fppCam->m_vPosition;
			playerPosition = fppCam->m_vPosition;

			//gravity
			Vector3 footPosition = playerPosition + Vector3(0.0f, -1.5f, 0.0f);
			Vector3 legsPosition = playerPosition + Vector3(0.0f, -1.0f, 0.0f);
			Vector3 headPosition = playerPosition + Vector3(0.0f, 0.1f, 0.0f);

			//foot in water
			if(mWorld->PlayerInWater(legsPosition))
				footInWater = true;
			else
				footInWater = false;

			//check if head is is in the water
			if(mWorld->PlayerInWater(headPosition))
			{
				//change gravity
				playerVelocity.y += (GRAVITY/8.0f) * dt;
				headInWater = true;
			}else
			{
				headInWater = false;
				playerVelocity.y += GRAVITY * dt;
			}



			//check if we are standing on ground
			if(mWorld->SolidAtPointForPlayer(footPosition) || mWorld->SolidAtPointForPlayer(headPosition))
			{
				walkingOnGround = true;
				soundBlockType = mWorld->BlockSoundAtPos(footPosition);

				// If the player has their head stuck in a block, push them down.
				if (mWorld->SolidAtPointForPlayer(headPosition))
				{
					int blockIn = (int)(headPosition.y);
					playerPosition.y = (float)(blockIn - 0.15f);
				}

				// If the player is stuck in the ground, bring them out.
				// This happens because we're standing on a block at -1.5, but stuck in it at -1.4, so -1.45 is the sweet spot.
				if (mWorld->SolidAtPointForPlayer(footPosition))
				{
					int blockOn = (int)(footPosition.y);
					playerPosition.y = (float)(blockOn + 1 + 1.45f);
				}

				playerVelocity.y = 0.0f;

				//dodatkowa logika podczas stania na klocku
				//jump
				if(jumping)
				{
					playerVelocity.y = 1.2f * JUMPVELOCITY;
					jumping = false;
					walkingOnGround = false;
				}

			}else
			{
				walkingOnGround = false;
			}

			//update position
			if(!canFly)
				playerPosition += playerVelocity * dt;
			else
				playerVelocity.y = 0.0f;


			//teraz kolizje ze œciankami
			isWalking = false;
			Vector3 moveVector = fppCam->m_vVelocity;

			if(moveVector.x != 0.0f || moveVector.z != 0.0f)
			{
				if(TryToMove(moveVector,dt))
				{
					isWalking = true;
				}else if(TryToMove(Vector3(0,0,moveVector.z),dt))
				{
					isWalking = true;
				}else if(TryToMove(Vector3(moveVector.x,0,0),dt))
				{
					isWalking = true;
				}
			}


			//update camera
			fppCam->m_vPosition = playerPosition;
			fppCam->m_vView = fppCam->m_vPosition + delta;
			fppCam->m_vVelocity = Vector3(0,0,0);
			fppCam->needUpdate = true;
		}

		if(fppCam->needUpdate)
		{
			mWorld->UpdatePlayerZoneBB(fppCam->m_vPosition);
		}

		//walking sound
		if(isWalking && walkingOnGround && !footInWater)
		{
			if(walkSoundAccu > 0.5f || walkSoundAccu == 0.0f)
			{
				mSoundMgr->PlayWalkSound(soundBlockType);
				walkSoundAccu = 0.0f;
			}
			walkSoundAccu += dt;
		}else
		{
			walkSoundAccu = 0.0f;
		}
	}

	mWorld->UpdateWorldTime(dt);

	//update skydome - every hour
	skyDome->timeOfDay = mWorld->worldDayTime * 0.041666f;

	//update sky and sun light time
	sunTimeTick += dt;
	if(sunTimeTick > 1.0f)
	{
		sunTime += 72.0f;
		sunTimeTick = 0.0f;
	}

	if(sunTime >= 86400)
		sunTime = 0;

}

void StatePlay::Draw(StateManager* sManager)
{
	//start rendering
	bool needUpdate = fppCam->needUpdate;
	mRender->StartFrame();

	//draw skydome
	skyDome->Render();

	//draw sun/moon
	skyLight->UpdateLightSource(skyLight->TimeToAngle(sunTime));
	skyLight->Render();

	TextureManager::Instance()->SetTextureModeulate(texture);

	//draw level

	sceGuFog( 0.0f, 100.75f,GU_COLOR(mWorld->fogColor.x,mWorld->fogColor.y,mWorld->fogColor.z,1.0f));	// Fog parameters
	//sceGuAmbientColor(0xff282828);
	//sceGuFog( 0.0f, 128.75f, 0x00CCCCff );	// Fog parameters
	sceGuEnable(GU_FOG );	// Enable fog

	sceGumPushMatrix();
	mWorld->drawWorld(fppCam->mFrustum,needUpdate);
	sceGumPopMatrix();

	sceGuDisable(GU_FOG );	// disable fog




	if(makeScreen)
	{
		//end frame now to update frame buffer
		mRender->EndFrame();
		//make screenshot
		mRender->TakeNextScreenshot();
		makeScreen = false;
	}

	//render pick cube
	if(showCube)
	{
		//cubePos = fppCam->m_vPosition;
		sceGumPushMatrix();

		ScePspFVector3 move = {cubePos.x,cubePos.y,cubePos.z};
		sceGumTranslate(&move);

		sceGuEnable(GU_BLEND);
		sceGuColor(0xFFFFFFFF);

		//GU_COLOR()
		mRender->Draw(cubeModel);
		sceGuDisable(GU_BLEND);

		sceGumPopMatrix();
	}

	//render cube in right hand
	{
		TextureManager::Instance()->SetTextureModeulate(texture);
		sceGumPushMatrix();

		//set view matrix to identity
		sceGumMatrixMode(GU_VIEW);
		sceGumLoadIdentity();

		//translate
		ScePspFVector3 move = {0.47f,-0.32f,-0.7f};
		sceGumTranslate(&move);
		//rotate
		sceGumRotateX(0.2792f);
		sceGumRotateY(0.7853f);
		//scale
		ScePspFVector3 scale = {0.25f,0.25f,0.25f};
		sceGumScale(&scale);

		//draw cube
		sceGuDisable(GU_DEPTH_TEST);
		sceGuDepthMask(1);
		mWorld->drawCubes((selectedCubeSet + selectedCube+1));
		sceGuEnable(GU_DEPTH_TEST);
		sceGuDepthMask(0);

		sceGumMatrixMode(GU_MODEL);
		sceGumPopMatrix();
	}



	//gui
	mRender->SetOrtho(0,0,0,0,0,0);

	if(headInWater)
	{
		sceGuDisable(GU_DEPTH_TEST);
		sceGuDepthMask(1);
		sceGuEnable(GU_BLEND);
		sceGuEnable(GU_TEXTURE_2D);
		sceGuColor(GU_COLOR(1,1,1,0.7f));
		TextureManager::Instance()->SetTextureModeulate(blue);
		advancedBlit(0,0,SCR_WIDTH,SCR_HEIGHT,0,0,32);
		sceGuDisable(GU_BLEND);
		sceGuDisable(GU_TEXTURE_2D);
		sceGuEnable(GU_DEPTH_TEST);
		sceGuDepthMask(0);
	}

	//gui
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthMask(1);
	sceGuEnable(GU_BLEND);
	sceGuColor(GU_COLOR(1,1,1,1.0f));

	barSprite->Draw();
	selectSprite->Draw();
	crossSprite->Draw();

	sceGuDisable(GU_BLEND);


	//draw 3d cube on 2d panel
	TextureManager::Instance()->SetTextureModeulate(texture);
	int bloStartPos = 100;
	for(int blo = 1 + selectedCubeSet;blo < 10 + selectedCubeSet;blo++)
	{
		sceGumPushMatrix();

		ScePspFVector3 loc = {bloStartPos,253,0.0f};
		sceGumTranslate(&loc);

		ScePspFVector3 sca = {17,17,17.0f};
		sceGumScale(&sca);

		//rotate
		sceGumRotateX(2.61f);
		sceGumRotateY(0.78f);

		sceGuFrontFace(GU_CW);
		mWorld->drawCubes(blo);
		sceGuFrontFace(GU_CCW);

		sceGumPopMatrix();

		bloStartPos+=35;
	}

	sceGuEnable(GU_DEPTH_TEST);
	sceGuDepthMask(0);



	//menu buttons
	if(menuState == 1)
	{
		if(menuOptions)
		{
			sceGuDisable(GU_DEPTH_TEST);
			sceGuEnable(GU_BLEND);
			sceGuColor(GU_COLOR(1,1,1,1.0f));

			//fly
			buttonSprite->SetPosition(240,70);
			buttonSprite->Draw();

			//dev menu
			buttonSprite->SetPosition(240,100);
			buttonSprite->Draw();

			//screenshot
			buttonSprite->SetPosition(240,130);
			buttonSprite->Draw();

			//rename map
			buttonSprite->SetPosition(240,160);
			buttonSprite->Draw();

			//player sounds
			buttonSprite->SetPosition(240,190);
			buttonSprite->Draw();

			//menu
			buttonSprite->SetPosition(240,220);
			buttonSprite->Draw();

			//selected button
			sbuttonSprite->SetPosition(240,(optionsMenuPos * 30) + 70);
			sbuttonSprite->Draw();

			sceGuDisable(GU_BLEND);
			sceGuEnable(GU_DEPTH_TEST);

			//draw subtitles on buttons
			canFly == true ? mRender->DebugPrint(240,75,"Fly : ON"): mRender->DebugPrint(240,75,"Fly : OFF");
			devMode == true ? mRender->DebugPrint(240,105,"DevMode : ON"): mRender->DebugPrint(240,105,"DevMode : OFF");
			mRender->DebugPrint(240,135,"Take Screenshot");
			mRender->DebugPrint(240,165,"Change map name");
			mSoundMgr->playerSounds == true ? mRender->DebugPrint(240,195,"Player sounds: ON"):mRender->DebugPrint(240,195,"Player sounds: OFF");
			mRender->DebugPrint(240,225,"Back");
		}else
		{
			sceGuDisable(GU_DEPTH_TEST);
			sceGuEnable(GU_BLEND);
			sceGuColor(GU_COLOR(1,1,1,1.0f));

			//resume
			buttonSprite->SetPosition(240,100);
			buttonSprite->Draw();

			//options
			buttonSprite->SetPosition(240,130);
			buttonSprite->Draw();

			//save
			buttonSprite->SetPosition(240,160);
			buttonSprite->Draw();

			//quit
			buttonSprite->SetPosition(240,190);
			buttonSprite->Draw();

			//quit and save
			buttonSprite->SetPosition(240,220);
			buttonSprite->Draw();

			//selected button
			sbuttonSprite->SetPosition(240,(selectPos * 30) + 100);
			sbuttonSprite->Draw();

			sceGuDisable(GU_BLEND);
			sceGuEnable(GU_DEPTH_TEST);

			//draw subtitles on buttons
			mRender->DebugPrint(240,105,"Resume");
			mRender->DebugPrint(240,135,"Options");
			mRender->DebugPrint(240,165,"Save");
			mRender->DebugPrint(240,195,"Save and Exit");
			mRender->DebugPrint(240,225,"Exit");
		}
	}

	//debug info
	if(devMode)
	{
		mRender->SetFontStyle(0.5f,0xFFFFFFFF,0xFF000000,0x00000000);

		if(dt > 0.0f)
			mRender->DebugPrint(20,20,"fps: %4.2f",(1.0f/dt));
		mRender->DebugPrint(20,30,"cpu: %d%%",mRender->GetCpuUsage());
		mRender->DebugPrint(20,40,"gpu: %d%%",mRender->GetGpuUsage());
		mRender->DebugPrint(20,50,"memory: %d",freeMemory);
		mRender->DebugPrint(20,60,"poly: %d",(mWorld->GetDrawntTrianglesCount() / 3));
		mRender->DebugPrint(20,70,"verts: %d",mWorld->GetDrawntTrianglesCount());
		mRender->DebugPrint(20,80,"day time: %f",mWorld->worldDayTime);
		mRender->DebugPrint(20,90,"sky time: %f",skyDome->timeOfDay);

		mRender->SetFontStyle(0.5f,0xFFFFFFFF,0xFF000000,0x00000200);
	}

	//end frame
	mRender->EndFrame();
}

//additional functions
void StatePlay::advancedBlit(int sx, int sy, int sw, int sh, int dx, int dy, int slice)
{
	int start, end;

	// blit maximizing the use of the texture-cache

	for (start = sx, end = sx+sw; start < end; start += slice, dx += slice)
	{
		TexturesPSPVertex16* vertices = (TexturesPSPVertex16*)sceGuGetMemory(2 * sizeof(TexturesPSPVertex16));
		int width = (start + slice) < end ? slice : end-start;

		vertices[0].u = start; vertices[0].v = sy;
		vertices[0].x = dx; vertices[0].y = dy; vertices[0].z = 0;

		vertices[1].u = start + width; vertices[1].v = sy + sh;
		vertices[1].x = dx + width; vertices[1].y = dy + sh; vertices[1].z = 0;

		sceGuDrawArray(GU_SPRITES,GU_TEXTURE_16BIT|GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vertices);
	}
}

bool StatePlay::TryToMove(Vector3 moveVector,float dt)
{
	// Build a "test vector" that is a little longer than the move vector.
	float moveLength = moveVector.magnitude();
	Vector3 testVector = moveVector;
	testVector.normalize();
	testVector = testVector * (moveLength + 0.2f);

	// Apply this test vector.
	Vector3 movePosition = playerPosition + testVector;
	Vector3 midBodyPoint = movePosition + Vector3(0, -0.7f, 0);
	Vector3 lowerBodyPoint = movePosition + Vector3(0, -1.4f, 0);

	if (!mWorld->SolidAtPointForPlayer(movePosition) && !mWorld->SolidAtPointForPlayer(lowerBodyPoint) && !mWorld->SolidAtPointForPlayer(midBodyPoint))
	{
		playerPosition = playerPosition + moveVector;
		return true;
	}

	//teraz mo¿na robiæ ciekawe rzeczy

	testVector = moveVector;
	testVector.normalize();
	testVector = testVector * (moveLength + 0.35f);
	movePosition = playerPosition + testVector;
	midBodyPoint = movePosition + Vector3(0, -0.7f, 0);
	lowerBodyPoint = movePosition + Vector3(0, -1.4f, 0);

	unsigned char lowerBlock = mWorld->BlockAtPoint(lowerBodyPoint);
	unsigned char midBlock = mWorld->BlockAtPoint(midBodyPoint);
	unsigned char upperBlock = mWorld->BlockAtPoint(movePosition);

	//kolizja z drabin¹ :>
	if (upperBlock == 6 || lowerBlock == 6 || midBlock == 6)
	{
		playerVelocity.y = CLIMBVELOCITY;
		Vector3 footPosition = playerPosition + Vector3(0.0f, -1.5f, 0.0f);
		if (mWorld->SolidAtPointForPlayer(footPosition))
			playerPosition.y += 0.1f;
		return true;
	}



	return false;
}

bool StatePlay::keyPressed(int currentKey)
{
	//analog reset
	if(analogUp)
	{
		if(mSystemMgr->GetAnalogY() < InputHelper::Instance()->analogYup)
			analogUp = false;
	}
	if(analogDown)
	{
		if(mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYdown)
			analogDown = false;
	}
	if(analogLeft)
	{
		if(mSystemMgr->GetAnalogX() > InputHelper::Instance()->analogXleft)
			analogLeft = false;
	}
	if(analogRight)
	{
		if(mSystemMgr->GetAnalogX() < InputHelper::Instance()->analogXright)
			analogRight = false;
	}

	//keys
	if(currentKey == 0)//cross
		return mSystemMgr->KeyPressed(PSP_CTRL_UP);
	if(currentKey == 1)//cross
		return mSystemMgr->KeyPressed(PSP_CTRL_DOWN);
	if(currentKey == 2)//cross
		return mSystemMgr->KeyPressed(PSP_CTRL_LEFT);
	if(currentKey == 3)//cross
		return mSystemMgr->KeyPressed(PSP_CTRL_RIGHT);

	if(currentKey == 4)//cross
		return mSystemMgr->KeyPressed(PSP_CTRL_TRIANGLE);
	if(currentKey == 5)//cross
		return mSystemMgr->KeyPressed(PSP_CTRL_CROSS);
	if(currentKey == 6)//cross
		return mSystemMgr->KeyPressed(PSP_CTRL_SQUARE);
	if(currentKey == 7)//cross
		return mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE);

	if(currentKey == 8)//cross
		return mSystemMgr->KeyPressed(PSP_CTRL_LTRIGGER);
	if(currentKey == 9)//cross
		return mSystemMgr->KeyPressed(PSP_CTRL_RTRIGGER);
	if(currentKey == 10)//cross
		return mSystemMgr->KeyPressed(PSP_CTRL_SELECT);
	if(currentKey == 15)//cross
		return mSystemMgr->KeyPressed(PSP_CTRL_START);

	//analog stick....
	if(currentKey == 12)//cross
	{	if(!analogUp)
		{
			if(mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
			{
				analogUp = true;
				return true;//analog up
			}
		}
	}
	if(currentKey == 11)//cross
	{
		if(!analogDown)
		{
			if(mSystemMgr->GetAnalogY() < InputHelper::Instance()->analogYdown)
			{
				analogDown = true;
				return true;//analog down
			}
		}
	}
	if(currentKey == 13)//cross
	{
		if(!analogLeft)
		{
			if(mSystemMgr->GetAnalogX() < InputHelper::Instance()->analogXleft)
			{
				analogLeft = true;
				return true;//analog left
			}
		}
	}
	if(currentKey == 14)//cross
	{
		if(!analogRight)
		{
			if(mSystemMgr->GetAnalogX() > InputHelper::Instance()->analogXright)
			{
				analogRight = true;
				return true;//analog right
			}
		}
	}

	return false;
}

bool StatePlay::keyHold(int currentKey)
{
	if(currentKey == 0)//cross
		return mSystemMgr->KeyHold(PSP_CTRL_UP);
	if(currentKey == 1)//cross
		return mSystemMgr->KeyHold(PSP_CTRL_DOWN);
	if(currentKey == 2)//cross
		return mSystemMgr->KeyHold(PSP_CTRL_LEFT);
	if(currentKey == 3)//cross
		return mSystemMgr->KeyHold(PSP_CTRL_RIGHT);

	if(currentKey == 4)//cross
		return mSystemMgr->KeyHold(PSP_CTRL_TRIANGLE);
	if(currentKey == 5)//cross
		return mSystemMgr->KeyHold(PSP_CTRL_CROSS);
	if(currentKey == 6)//cross
		return mSystemMgr->KeyHold(PSP_CTRL_SQUARE);
	if(currentKey == 7)//cross
		return mSystemMgr->KeyHold(PSP_CTRL_CIRCLE);

	if(currentKey == 8)//cross
		return mSystemMgr->KeyHold(PSP_CTRL_LTRIGGER);
	if(currentKey == 9)//cross
		return mSystemMgr->KeyHold(PSP_CTRL_RTRIGGER);
	if(currentKey == 10)//cross
		return mSystemMgr->KeyHold(PSP_CTRL_SELECT);
	if(currentKey == 15)//cross
		return mSystemMgr->KeyHold(PSP_CTRL_START);

	//analog stick....
	if(currentKey == 12)//cross
		return (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup);//analog up
	if(currentKey == 11)//cross
		return (mSystemMgr->GetAnalogY() < InputHelper::Instance()->analogYdown);//analog down
	if(currentKey == 13)//cross
		return (mSystemMgr->GetAnalogX() < InputHelper::Instance()->analogXleft);//analog left
	if(currentKey == 14)//cross
		return (mSystemMgr->GetAnalogX() > InputHelper::Instance()->analogXright);//analog right

	return false;
}
