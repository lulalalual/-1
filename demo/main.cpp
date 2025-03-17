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

void test_json()
{
	ifstream file("test.json"); // ����Ϊ "test.json" ���ļ����ڶ�ȡ��
	if (!file.good())           // ����ļ��Ƿ�ɹ��򿪡�
	{
		cout << "�޷����ļ�" << endl; // ����ļ�û�гɹ��򿪣������ "�޷����ļ�" ������̨��
		return;                  // ���������أ������˺�����
	}

	stringstream str_stream;    // ����һ�� stringstream �������ڴ洢�����ļ������ݡ�
	str_stream << file.rdbuf(); // ��ȡ�ļ����������ݵ� stringstream �С�
	file.close();               // �ر��ļ���

	cJSON* json_root = cJSON_Parse(str_stream.str().c_str()); // �� stringstream �е�����ת��Ϊ C �����ַ�����������Ϊ cJSON �ṹ��
	cJSON* json_name = cJSON_GetObjectItem(json_root, "name"); // �ӽ������ JSON �����л�ȡ "name" ����Ӧ��ֵ��

	cout << json_name->string << ": " << json_name->valuestring << endl; // ��� "name" �������Ӧ��ֵ��
}
//json����

void test_csv()
{
	ifstream file("test.csv"); // ����Ϊ "test.csv" ���ļ����ڶ�ȡ��
	if (!file.good())          // ����ļ��Ƿ�ɹ��򿪡�
	{
		cout << "ʧ��" << endl; // ����ļ�û�гɹ��򿪣������ "ʧ��" ������̨��
		return;                 // ���������أ������˺�����
	}
	string str_line;           // ����һ���ַ������� str_line ���ڱ����ļ���һ�����ݡ�
	while (getline(file, str_line)) // ʹ�� getline ������ȡ�ļ���һ�����ݵ� str_line �����С�
	{
		string str_grid;               // ����һ���ַ������� str_grid ���ڱ���һ���еĵ����ֶΡ�
		stringstream str_stream(str_line); // �� str_line �ַ���ת��Ϊ stringstream �����Ա������ش���
		while (getline(str_stream, str_grid, ',')) // ʹ�� getline ������ȡ str_stream �е�һ���ֶε� str_grid �����У�
		{                                                 // �ֶμ��ɶ��ţ�','���ָ���
			cout << str_grid << " "; // ����ֶε�����̨�����Կո�ָ���
		}

		cout << endl; // ÿ��ȡ��һ�к��С�
	}
	file.close(); // �ر��ļ���
}
//csv����

int main()
{
	test_json();
	cout <<"=================="<<endl;
	test_csv();

	SDL_Init(SDL_INIT_EVERYTHING);//SDL�ĳ�ʼ������

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);//��ʼ��ͼƬ��

	Mix_Init(MIX_INIT_MP3);//��ʼ����Ƶ�⣬��Ҫ������
	TTF_Init();//��ʼ�������
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);//һ�ױȽ�ͨ�õ�����
	// �ĸ���������һ������Ƶ�����ʣ�44100��CD������
	// �ڶ����ǽ������Ƶ��ʽ,��ϵͳ���ֽ����йأ�format��ͨ��
	// ��������������
	// ���ĸ�����Ƶ��������С,С���������Լ�С��Ƶ�Ĳ����ӳ٣��������Ӵ�����

	SDL_Window* window = SDL_CreateWindow(u8"�������", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	//����һ������
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//��������Ӳ�����ٵ�
	//���Ǹ���Ⱦ��

	SDL_Surface* suf_img = IMG_Load("avatar.jpg");	//�ڴ�ṹ�壬�Ƚ�ͼƬ���ص��ڴ���
	SDL_Texture* tex_img = SDL_CreateTextureFromSurface(renderer, suf_img);//���ڴ��е����ݱ�������е�����,�ٴ��ڴ���ͨ��renderer��Ⱦ����������Ⱦ����
	//��ȾͼƬ�ر����������ȼ��غ���Ⱦ

	TTF_Font* font = TTF_OpenFont("ipix.ttf", 32);//���ö�����ص����壬һ������Ϊ32��
	//���Ǽ�������
	//����������Ⱦ����
	SDL_Color color = { 255,255,255,255 };//���ײ�͸��
	SDL_Surface* suf_text = TTF_RenderUTF8_Blended(font, u8"�������", color);//ʹ��color��ɫ����font�����壬��Ⱦ������磨UTF-8��
	//surface��������Ҫ��������
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);

	Mix_Music* music = Mix_LoadMUS("music.mp3");
	Mix_FadeInMusic(music, -1, 1500);//���벥��music���ڶ���������ѭ������-1��������ѭ�����������ǵ���ʱ��ĺ�������

	int fps = 60;//�ڴ�ռ�ù��ߣ���Ҫʹ�ö�̬��ʱ��

	bool is_quit = false;
	SDL_Event event;//�¼�����
	SDL_Point pos_cursor = { 0,0 };//��¼����λ��
	SDL_Rect rect_img, rect_text;//����,��ʾͼƬ�����ָ��Եľ���

	Uint64 last_counter = SDL_GetPerformanceCounter();//���ѭ��ǰ�����ܼ�ʱ���ļ���
	Uint64 conter_freq = SDL_GetPerformanceFrequency();//���Ƶ��-ÿһ������������������ٴ�
	//����һ�ȣ��õ��ľ���ʱ��

	rect_img.w = suf_img->w;
	rect_img.h = suf_img->h;
	//����ѭ�����������εĿ�Ⱥ͸߶ȸ�ֵ
	rect_text.w = suf_text->w;
	rect_text.h = suf_text->h;

	while (!is_quit)
	{
		while (SDL_PollEvent(&event))//����ȥ��ȡ�¼�,��Ϸ��ѭ��
		{
			if (event.type == SDL_QUIT)//��������رմ���
			{
				is_quit = true;
			}
			else if (event.type == SDL_MOUSEMOTION)//����ƶ��¼�
			{
				pos_cursor.x = event.motion.x;
				pos_cursor.y = event.motion.y;
			}//��ѭ���ڶ�������ε�λ��������ƶ��ı�
		}

		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta = (double)(current_counter - last_counter) / conter_freq;//��͵õ���һ��ѭ����ʱ�䣬����Ϊ��λ
		last_counter = current_counter;//Ȼ���ܶ���ʼ
		if (delta * 1000 < 1000.0 / 60)//��ת���ɺ�������Ƚϣ����ÿһ֡������С�������Ļ�
		{
			SDL_Delay((Uint32)(1000.0 / 60 - delta * 1000));
		}//Ϊ�˼���CPUռ����

		//�ڶ�������������
		rect_img.x = pos_cursor.x;
		rect_img.y = pos_cursor.y;

		rect_text.x = pos_cursor.x;
		rect_text.y = pos_cursor.y;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//���û�ͼȾɫ������0�ֱ��ʾRGB��255��ʾ͸����
		SDL_RenderClear(renderer);//Ȼ���������ͼ��ɫ���������Ⱦ������

		//����������Ⱦ��ͼ
		SDL_RenderCopy(renderer, tex_img, nullptr, &rect_img);//�����������Ǵ�ԭͼƬ��ȡһ�����Σ�ճ�������ĸ�������ʾ�ľ�����
		filledCircleRGBA(renderer, pos_cursor.x, pos_cursor.y, 50, 255, 0, 0, 125);//��RGBA��Ⱦһ����Ȧ��50����뾶

		SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);//λ��Խ���£���Ⱦ��Խ���ϲ�
		SDL_RenderPresent(renderer);//����Ⱦ�����Ƶ����ݸ��µ������ϡ�
		//����δִ���������������Ի��в�Ӱ������ÿ�λ�ͼǰ��Ҫ����һ֡�����ݽ�����գ�
	}


	return 0;
}