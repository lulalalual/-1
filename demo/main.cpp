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

void test_json()
{
	ifstream file("test.json"); // 打开名为 "test.json" 的文件用于读取。
	if (!file.good())           // 检查文件是否成功打开。
	{
		cout << "无法打开文件" << endl; // 如果文件没有成功打开，则输出 "无法打开文件" 到控制台。
		return;                  // 并立即返回，结束此函数。
	}

	stringstream str_stream;    // 创建一个 stringstream 对象用于存储整个文件的内容。
	str_stream << file.rdbuf(); // 读取文件的所有内容到 stringstream 中。
	file.close();               // 关闭文件。

	cJSON* json_root = cJSON_Parse(str_stream.str().c_str()); // 将 stringstream 中的内容转换为 C 风格的字符串，并解析为 cJSON 结构。
	cJSON* json_name = cJSON_GetObjectItem(json_root, "name"); // 从解析后的 JSON 对象中获取 "name" 键对应的值。

	cout << json_name->string << ": " << json_name->valuestring << endl; // 输出 "name" 键及其对应的值。
}
//json解析

void test_csv()
{
	ifstream file("test.csv"); // 打开名为 "test.csv" 的文件用于读取。
	if (!file.good())          // 检查文件是否成功打开。
	{
		cout << "失败" << endl; // 如果文件没有成功打开，则输出 "失败" 到控制台。
		return;                 // 并立即返回，结束此函数。
	}
	string str_line;           // 定义一个字符串变量 str_line 用于保存文件的一行内容。
	while (getline(file, str_line)) // 使用 getline 函数读取文件的一行内容到 str_line 变量中。
	{
		string str_grid;               // 定义一个字符串变量 str_grid 用于保存一行中的单个字段。
		stringstream str_stream(str_line); // 将 str_line 字符串转换为 stringstream 对象，以便更方便地处理。
		while (getline(str_stream, str_grid, ',')) // 使用 getline 函数读取 str_stream 中的一个字段到 str_grid 变量中，
		{                                                 // 字段间由逗号（','）分隔。
			cout << str_grid << " "; // 输出字段到控制台，并以空格分隔。
		}

		cout << endl; // 每读取完一行后换行。
	}
	file.close(); // 关闭文件。
}
//csv解析

int main()
{
	test_json();
	cout <<"=================="<<endl;
	test_csv();

	SDL_Init(SDL_INIT_EVERYTHING);//SDL的初始化所有

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);//初始化图片库

	Mix_Init(MIX_INIT_MP3);//初始化音频库，还要打开声道
	TTF_Init();//初始化字体库
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);//一套比较通用的设置
	// 四个参数，第一个是音频采样率，44100是CD采样率
	// 第二个是解码的音频格式,和系统的字节序有关，format更通用
	// 第三个是声道数
	// 第四个是音频缓冲区大小,小缓冲区可以减小音频的播放延迟，但会增加处理负担

	SDL_Window* window = SDL_CreateWindow(u8"你好世界", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	//创建一个窗口
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//第三个是硬件加速的
	//这是个渲染器

	SDL_Surface* suf_img = IMG_Load("avatar.jpg");	//内存结构体，先将图片加载到内存中
	SDL_Texture* tex_img = SDL_CreateTextureFromSurface(renderer, suf_img);//把内存中的数据变成纹理中的数据,再从内存中通过renderer渲染器把纹理渲染出来
	//渲染图片必备的两步，先加载后渲染

	TTF_Font* font = TTF_OpenFont("ipix.ttf", 32);//设置多大像素的字体，一般设置为32、
	//这是加载字体
	//接下来是渲染字体
	SDL_Color color = { 255,255,255,255 };//纯白不透明
	SDL_Surface* suf_text = TTF_RenderUTF8_Blended(font, u8"黑神话悟空", color);//使用color颜色，用font的字体，渲染你好世界（UTF-8）
	//surface创建完后就要创建纹理
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);

	Mix_Music* music = Mix_LoadMUS("music.mp3");
	Mix_FadeInMusic(music, -1, 1500);//淡入播放music，第二个参数是循环数，-1代表无限循环，第三个是淡入时间的毫秒数。

	int fps = 60;//内存占用过高，需要使用动态延时。

	bool is_quit = false;
	SDL_Event event;//事件对象
	SDL_Point pos_cursor = { 0,0 };//记录鼠标的位置
	SDL_Rect rect_img, rect_text;//矩形,表示图片和文字各自的矩形

	Uint64 last_counter = SDL_GetPerformanceCounter();//获得循环前高性能计时器的计数
	Uint64 conter_freq = SDL_GetPerformanceFrequency();//获得频率-每一秒这个计数器奇数多少次
	//二者一比，得到的就是时间

	rect_img.w = suf_img->w;
	rect_img.h = suf_img->h;
	//在主循环外对这个矩形的宽度和高度赋值
	rect_text.w = suf_text->w;
	rect_text.h = suf_text->h;

	while (!is_quit)
	{
		while (SDL_PollEvent(&event))//主动去拉取事件,游戏主循环
		{
			if (event.type == SDL_QUIT)//如果主动关闭窗口
			{
				is_quit = true;
			}
			else if (event.type == SDL_MOUSEMOTION)//鼠标移动事件
			{
				pos_cursor.x = event.motion.x;
				pos_cursor.y = event.motion.y;
			}//主循环内对这个矩形的位置随鼠标移动改变
		}

		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta = (double)(current_counter - last_counter) / conter_freq;//这就得到了一个循环的时间，以秒为单位
		last_counter = current_counter;//然后周而复始
		if (delta * 1000 < 1000.0 / 60)//秒转换成毫秒数后比较，如果每一帧的秒数小于期望的话
		{
			SDL_Delay((Uint32)(1000.0 / 60 - delta * 1000));
		}//为了减少CPU占用率

		//第二步，处理数据
		rect_img.x = pos_cursor.x;
		rect_img.y = pos_cursor.y;

		rect_text.x = pos_cursor.x;
		rect_text.y = pos_cursor.y;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//设置绘图染色，三个0分别表示RGB，255表示透明度
		SDL_RenderClear(renderer);//然后用这个绘图颜色清空整个渲染器窗口

		//第三步，渲染绘图
		SDL_RenderCopy(renderer, tex_img, nullptr, &rect_img);//第三个参数是从原图片中取一个矩形，粘贴到第四个参数表示的矩形中
		filledCircleRGBA(renderer, pos_cursor.x, pos_cursor.y, 50, 255, 0, 0, 125);//用RGBA渲染一个红圈，50代表半径

		SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);//位置越靠下，渲染的越在上层
		SDL_RenderPresent(renderer);//将渲染器绘制的内容更新到窗口上。
		//（并未执行清屏操作，所以会有残影，所以每次绘图前都要对上一帧的内容进行清空）
	}


	return 0;
}