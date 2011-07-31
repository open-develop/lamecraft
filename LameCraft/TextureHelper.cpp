#include "TextureHelper.h"
#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Utils/pgeZip.h>

TextureHelper::TextureHelper()
{
	defaulPatch = "Assets/Lamecraft/";
	texturePatch = "Assets/Lamecraft/Lamecraft.zip";
}

TextureHelper::~TextureHelper()
{

}

int TextureHelper::GetTextureFromZip(const char* name)
{
	if(!TextureManager::Instance()->TextureExist(name))
	{
		pgeZip* theZip = pgeZipOpen(texturePatch.c_str());
		pgeZipFile* fileInZip = pgeZipFileRead(theZip, name, NULL);
		pgeZipClose(theZip);

		TextureManager::Instance()->LoadTexture(name,fileInZip->data,fileInZip->size);

		pgeZipFileFree(fileInZip);
	}

	return TextureManager::Instance()->GetTextureNumber(name);
}

int TextureHelper::GetTexture(Textures texture)
{
	switch(texture)
	{
		case Genesis:
		{
			return GetTextureFromZip("genesis.png");
		}
		break;
		case PspTeam:
		{
			return GetTextureFromZip("pspteam.png");
		}
		break;
		case Logo:
		{
			return GetTextureFromZip("logo.png");
		}
		break;
		case Utils:
		{
			return GetTextureFromZip("utils.png");
		}
		break;
		case Dirt:
		{
			return GetTextureFromZip("dirt.png");
		}
		break;
		case Glass:
		{
			return GetTextureFromZip("glass.png");
		}
		break;
		case Terrain:
		{
			return GetTextureFromZip("terrain_medium.png");
		}
		break;
		case Blue:
		{
			return GetTextureFromZip("blue.png");
		}
		break;
		case Sky:
		{
			return GetTextureFromZip("sky.png");
		}
		break;
		case Sun:
		{
			return GetTextureFromZip("sun.png");
		}
		break;
		case Moon:
		{
			return GetTextureFromZip("moon.png");
		}
		break;
	}

	return 0;
}

void TextureHelper::SetTextureZipName(std::string name)
{
	defaultZip = name;
	texturePatch = defaulPatch + defaultZip;
}

TextureHelper TextureHelper::m_TextureHelper;

TextureHelper* TextureHelper::Instance()
{
	return &m_TextureHelper;
}
