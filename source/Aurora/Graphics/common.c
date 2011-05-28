#include <Aurora/Graphics/common.h>

// Exit callback
int exit_callback(int arg1, int arg2, void *common)
{
	sceKernelExitGame();
	return 0;
}

// Callback thread 
int CallbackThread(SceSize args, void *argp)
{
	int cbid;
 
	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
 
	sceKernelSleepThreadCB();
 
	return 0;
}

// Sets up the callback thread and returns its thread id
int SetupCallbacks(void)
{
	int thid = 0;
 
	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}
 
	return thid;
}

void memCopy(void* inDest, void* inSrc, u32 inSize)
{
     if(!inDest || !inSrc || !inSize)
          return;

     inSize += (u32)inSrc;
     if(((u32)inDest & 3) || ((u32)inSrc & 3) || ((u32)inSize & 3)) {
          if(((u32)inDest & 1) || ((u32)inSrc & 1) || ((u32)inSize & 1)) {
               u8* tempDest1 = (u8*)inDest;
               u8* tempSrc1 = (u8*)inSrc;
               while((u32)tempSrc1 < inSize)
                    *(tempDest1++) = *(tempSrc1++);
          } else {
               u16* tempDest2 = (u16*)inDest;
               u16* tempSrc2 = (u16*)inSrc;
               while((u32)tempSrc2 < inSize)
                    *(tempDest2++) = *(tempSrc2++);
          }
     } else {
          u32* tempDest4 = (u32*)inDest;
          u32* tempSrc4 = (u32*)inSrc;
          while((u32)tempSrc4 < inSize)
               *(tempDest4++) = *(tempSrc4++);
     }
}



