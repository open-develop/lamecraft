TARGET = Tutorial1
OBJS = source/Aurora/Math/MathLib.o \
source/Aurora/Math/Matrix3.o \
source/Aurora/Math/Matrix4.o \
source/Aurora/Math/Quaternion.o \
source/Aurora/Math/BoundingBox.o \
source/Aurora/Math/BoundingSphere.o \
source/Aurora/Math/Plane.o \
source/Aurora/Math/Ray.o \
source/Aurora/Math/Frustum.o \
source/Aurora/Math/Polygon.o \
source/Aurora/Graphics/common.o \
source/Aurora/Graphics/vfram.o \
source/Aurora/Graphics/vram.o \
source/Aurora/Graphics/RenderManager.o \
source/Aurora/Graphics/TextureManager.o \
source/Aurora/Graphics/Camera.o \
source/Aurora/Graphics/Sprite.o \
source/Aurora/Graphics/Models/ObjModel.o \
source/Aurora/Graphics/Effects/SkyLight.o \
source/Aurora/Graphics/Effects/SkyDome.o \
source/Aurora/Graphics/Models/ObjModel.o \
source/Aurora/Graphics/intraFont/libccc.o \
source/Aurora/Graphics/intraFont/intraFont.o \
source/Aurora/Utils/Logger.o \
source/Aurora/Utils/Timer.o \
source/Aurora/Utils/StateManager.o \
source/Aurora/System/SystemManager.o \
source/Aurora/System/NetworkManager.o \
LameCraft/BaseBlock.o \
LameCraft/Blocks.o \
LameCraft/SimpleMeshChunk2.o \
LameCraft/CraftWorld2.o \
LameCraft/StateMenu.o \
LameCraft/StateOptions.o \
LameCraft/StatePlay.o \
LameCraft/LoadingScreen.o \
LameCraft/SplashScreen.o \
LameCraft/InputHelper.o \
LameCraft/SoundManager.o \
LameCraft/sound_utils/vfsFile.o \
LameCraft/sound_utils/VirtualFile.o \
LameCraft/sound_utils/audio.o \
LameCraft/sound_utils/bgm.o \
LameCraft/sound_utils/media.o \
LameCraft/sound_utils/mod.o \
Lamecraft.o

INCDIR = $(PSPPATH)/include
INCDIR += ./include
INCDIR += ./source
CFLAGS = -O2 -G0 -Wall -mpreferred-stack-boundary=4 #-enable-newlib-hw-fp -ffast-math
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PSP_FW_VERSION=371
#PSP_LARGE_MEMORY = 1

LIBDIR = 
LDFLAGS =
LIBS=  -lNoisepp -lpspaudiolib -lpspaudio -lmikmod -lmmio -lpspaudiocodec -lpng -lz -lpspgum -lpspgu -lstdc++ -lpsprtc -lpspctrl -lm -lpspvfpu -lpspsdk -lpsppower -lpsphttp #-lpsp_req -lcurl -lTiny

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = LameCraft
PSP_EBOOT_ICON = ICON0.png
PSP_EBOOT_ICON1 = ICON1.PMF
PSP_EBOOT_PIC1 = PIC1.png

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
