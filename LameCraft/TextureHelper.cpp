#include "TextureHelper.h"
#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Utils/Logger.h>
#include <dirent.h>
#include <fcntl.h>

TextureHelper::TextureHelper()
{
	defaulPatch = "Assets/Lamecraft/";
	texturePatch = "Assets/Lamecraft/Lamecraft.zip";
}

TextureHelper::~TextureHelper()
{

}

const char* TextureHelper::GetTextureFromZip(const char* name)
{
	if(!TextureManager::Instance()->TextureExist(name))
	{
		if(theZip == NULL)
		{
			theZip = pgeZipOpen(texturePatch.c_str());
		}

		pgeZipFile* fileInZip = pgeZipFileRead(theZip, name, NULL);

		TextureManager::Instance()->LoadTexture(name,fileInZip->data,fileInZip->size);

		pgeZipFileFree(fileInZip);
	}

	return name;
}

const char* TextureHelper::GetTexture(Textures texture)
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

	return "";
}

void TextureHelper::SetTextureZipName(std::string name)
{
	defaultZip = name;
	texturePatch = defaulPatch + defaultZip;

	if(theZip != NULL)
	{
		pgeZipClose(theZip);
	}
	theZip = pgeZipOpen(texturePatch.c_str());

	//clean old textures
	CleanAllPackImages();
}

void TextureHelper::CleanAllPackImages()
{
	RemoveTexture("logo.png");
	RemoveTexture("utils.png");
	RemoveTexture("dirt.png");
	RemoveTexture("glass.png");
	RemoveTexture("terrain_medium.png");
	RemoveTexture("blue.png");
	RemoveTexture("sky.png");
	RemoveTexture("sun.png");
	RemoveTexture("moon.png");

	GetTextureFromZip("logo.png");
	GetTextureFromZip("utils.png");
	GetTextureFromZip("dirt.png");
	GetTextureFromZip("glass.png");
	GetTextureFromZip("terrain_medium.png");
	GetTextureFromZip("blue.png");
	GetTextureFromZip("sky.png");
	GetTextureFromZip("sun.png");
	GetTextureFromZip("moon.png");
}

void TextureHelper::RemoveTexture(const char* name)
{
	if(TextureManager::Instance()->TextureExist(name))
	{
		TextureManager::Instance()->RemoveTexture(name);
	}
}

void TextureHelper::ScanForTexturePacks()
{
	packFiles.clear();

	DIR *Dir;
	struct dirent *DirEntry;
	Dir = opendir(defaulPatch.c_str());

	while((DirEntry = readdir(Dir)) != NULL)
	{
	   if ( DirEntry->d_stat.st_attr & FIO_SO_IFREG)
	   {
			std::string plik = DirEntry->d_name;
			size_t found = plik.find(".zip");
			size_t found2 = plik.find(".ZIP");
			if(found != std::string::npos || found2 != std::string::npos)//not found
			{
				packFiles.push_back(plik);
			}
	   }
	}

	closedir(Dir);
}

TextureHelper TextureHelper::m_TextureHelper;

TextureHelper* TextureHelper::Instance()
{
	return &m_TextureHelper;
}
