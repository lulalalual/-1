#define SDL_MAIN_HANDLED
#include"iostream"
#include"sstream"
#include"fstream"
#include"string"

#include"cJSON.h"
#include"SDL.h"
#include"SDL_image.h"//ͼƬ��
#include"SDL_mixer.h"//��Ƶ��
#include"SDL_ttf.h"//�����
#include"SDL2_gfxPrimitives.h"//ͼ��Ч���͹��ܿ�

using namespace std;
#include"game_manager.h"


int main(int argc,char** argv)
{
	return GameManager::instance()->run(argc, argv);
	//��ȡ��Ϸʵ���󣬵�����ѭ����Ȼ��ѷ���ֵ���س�ȥ����
	
}