#ifndef TEXTUREHELPER_H_
#define TEXTUREHELPER_H_

#include <string>

class TextureHelper
{
private:

	std::string defaulPatch;
	std::string defaultZip;
	std::string texturePatch;

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

	int GetTexture(Textures texture);
	int GetTextureFromZip(const char* name);
	void SetTextureZipName(std::string name);
};

#endif
