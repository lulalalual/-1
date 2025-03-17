#define SDL_MAIN_HANDLED
#include"iostream"
#include"sstream"
#include"fstream"
#include"string"

#include"cJSON.h"
#include"SDL.h"
#include"SDL_image.h"//图片库
#include"SDL_mixer.h"//音频库
#include"SDL_ttf.h"//字体库
#include"SDL2_gfxPrimitives.h"//图形效果和功能库

using namespace std;
#include"game_manager.h"


int main(int argc,char** argv)
{
	return GameManager::instance()->run(argc, argv);
	//获取游戏实例后，调用主循环，然后把返回值返回出去就行
	
}