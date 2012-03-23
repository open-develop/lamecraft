#ifndef TEXTUREHELPER_H_
#define TEXTUREHELPER_H_

#include <string>
#include <vector>

#include <Aurora/Utils/pgeZip.h>

class TextureHelper
{
private:

	std::string defaulPatch;
	std::string defaultZip;
	std::string texturePatch;

	pgeZip* theZip;

public:

	enum Textures
	{
		Genesis,
		PspTeam,
		Logo,
		Utils,
		Dirt,
		Glass,
		Terrain,
		Blue,
		Sky,
		Sun,
		Moon
	};

	static TextureHelper m_TextureHelper;
	static TextureHelper* Instance();


	TextureHelper();
	~TextureHelper();

	const char* GetTexture(Textures texture);
	const char* GetTextureFromZip(const char* name);
	void SetTextureZipName(std::string name);

	void CleanAllPackImages();
	void RemoveTexture(const char* name);

	void ScanForTexturePacks();

	std::vector<std::string> packFiles;
};

#endif
