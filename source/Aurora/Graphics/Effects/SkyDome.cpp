#include <Aurora/Graphics/Effects/SkyDome.h>
#include <Aurora/Graphics/TextureManager.h>

#define PI 3.1415926535897f
#define DEG_TO_RAD (PI / 180.0f)


namespace Aurora
{
	namespace Graphics
	{

		SkyDome::SkyDome()
		{
			dtheta = 5;
			dphi = 5;

			numVerts = (int)((360 / dtheta) * (90 / dphi) * 2);
			domeVertices = (CraftPSPVertex*)malloc( sizeof(CraftPSPVertex) * numVerts);

			timeOfDay = 0.0f;
		}

		SkyDome::~SkyDome()
		{
			free(domeVertices);
		}

		void SkyDome::CreateSkyDomeMesh()
		{
			int i = 0;
			int radius = 500;
			float textureScale = 1.0f / (90 / dphi);
			for (int phi = 0; phi < 90; phi += dphi)
			{
				for (int theta = 0; theta < 360 - dtheta; theta += dtheta)
				{
					/* vertex (phi + dphi, theta) */
					domeVertices[i].color = GU_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
					domeVertices[i].x = 320.0f + radius * cosf((phi + dphi) * DEG_TO_RAD) *cosf(theta * DEG_TO_RAD);
					domeVertices[i].y = radius * sinf((phi + dphi) * DEG_TO_RAD);
					domeVertices[i].z = 320.0f + radius * cosf((phi + dphi) * DEG_TO_RAD) * sinf(theta * DEG_TO_RAD);
					domeVertices[i].u = 0;
					domeVertices[i].v = (1.0f / 64) * domeVertices[i].y * textureScale;
					i++;

					/* vertex (phi, theta) */
					domeVertices[i].color = GU_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
					domeVertices[i].x = 320.0f + radius * cosf(phi * DEG_TO_RAD) * cosf(theta * DEG_TO_RAD);
					domeVertices[i].y = radius * sinf(phi * DEG_TO_RAD);
					domeVertices[i].z = 320.0f + radius * cosf(phi * DEG_TO_RAD) * sinf(theta * DEG_TO_RAD);
					domeVertices[i].u = 0;
					domeVertices[i].v = (1.0f / 64) * domeVertices[i].y * textureScale;
					i++;
				}
			}

			sceKernelDcacheWritebackAll();
		}

		void SkyDome::SetTexture(int texture)
		{
			textureNumber = texture;
		}

		void SkyDome::Render()
		{
			sceGuColor(0xFFFFFFFF);
			sceGuEnable(GU_TEXTURE_2D);

			TextureManager::Instance()->SetTextureModeulate(textureNumber);

			//sceGuTexMode( GU_PSM_8888, 0, 0, texture.Swizzled() );
			sceGuTexFunc( GU_TFX_MODULATE, GU_TCC_RGB );
			sceGuTexFilter( GU_LINEAR, GU_LINEAR );
			sceGuTexOffset(timeOfDay, 0.0f);
			sceGuTexWrap(GU_REPEAT, GU_CLAMP);
			//sceGuTexImage( 0, texture.Width(), texture.Height(), texture.Width(), texture.Image() );


			sceGumDrawArray(GU_TRIANGLE_STRIP, GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D, numVerts, 0, domeVertices);


			sceGuDisable(GU_TEXTURE_2D);
		}

	}
}

