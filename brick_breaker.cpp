
//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>

bool quit = false;
bool begin_game = false;
void destroy();


SDL_Event event;
	  
//background variables
int bkw = 800;
int bkh = 450;
int bkw_min = 0;
int bkh_min = 0;

double ball_x = bkw / 2 + 16;
double ball_y = bkh - 55;
double ball_vel_x = 0.2;
double ball_vel_y = 0.2;
SDL_Rect ball_rect;


//brick variables
int brick_w=60;
int brick_h=25;
SDL_Surface *brick;
SDL_Surface* ball;
SDL_Surface* background;
SDL_Surface* bat;
SDL_Texture* brick_txt;
SDL_Texture* background_txt;
SDL_Texture* ball_txt;
SDL_Texture* bat_txt;

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Rect brick_rect[3][7];

int destroy_brick_count = 0;
int brick_num = sizeof(brick_rect) / sizeof(brick_rect[0][0]);



void brick_init()
{
	brick_rect[0][0] = { 50, 50, brick_w, brick_h };
	brick_rect[0][1] = { 150, 50, brick_w, brick_h };
	brick_rect[0][2] = { 250, 50, brick_w, brick_h };
	brick_rect[0][3] = { 350, 50, brick_w, brick_h };
	brick_rect[0][4] = { 450, 50, brick_w, brick_h };
	brick_rect[0][5] = { 550, 50, brick_w, brick_h };
	brick_rect[0][6] = { 650, 50, brick_w, brick_h };
	brick_rect[1][0] = { 50, 100, brick_w, brick_h };
	brick_rect[1][1] = { 150, 100, brick_w, brick_h };
	brick_rect[1][2] = { 250, 100, brick_w, brick_h };
	brick_rect[1][3] = { 350, 100, brick_w, brick_h };
	brick_rect[1][4] = { 450, 100, brick_w, brick_h };
	brick_rect[1][5] = { 550, 100, brick_w, brick_h };
	brick_rect[1][6] = { 650, 100, brick_w, brick_h };
	brick_rect[2][0] = { 50, 150, brick_w, brick_h };
	brick_rect[2][1] = { 150, 150, brick_w, brick_h };
	brick_rect[2][2] = { 250, 150, brick_w, brick_h };
	brick_rect[2][3] = { 350, 150, brick_w, brick_h };
	brick_rect[2][4] = { 450, 150, brick_w, brick_h };
	brick_rect[2][5] = { 550, 150, brick_w, brick_h };
	brick_rect[2][6] = { 650, 150, brick_w, brick_h };

}



//bat variables
double bat_x = bkw/2;
double bat_y = bkh-30;


//handles all events
void event_handler()
{	
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT)
	{
		quit = true;
	}
	else if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_SPACE && ball_x == bkw / 2 + 16 && ball_y == bkh - 55)
		{
			begin_game = true;
		}
		if (event.key.keysym.sym == SDLK_LEFT && bat_x > 0)
		{
			bat_x = bat_x - 0.5;
		}
		if (event.key.keysym.sym == SDLK_RIGHT && bat_x < bkw - 60)
		{
			bat_x = bat_x + 0.5;
		}
	}
	
}

void game_over()
{

	SDL_Surface* go = SDL_LoadBMP("go.bmp");
	SDL_Texture* go_txt = SDL_CreateTextureFromSurface(renderer, go);
	SDL_Rect go_rect = { 0, 0, bkw, bkh };
	SDL_RenderCopy(renderer, go_txt, NULL, &go_rect);
	SDL_RenderPresent(renderer);
	SDL_Delay(4000);
	destroy();
	SDL_Quit();
}
	
void move_ball()
{
	ball_x = ball_x + ball_vel_x;
	ball_y = ball_y + ball_vel_y;
}

void ball_collision()
{
	//"30" is a width of the ball
	if (ball_x < bkw_min|| ball_x>bkw-30)
	{
		ball_vel_x = -ball_vel_x;
	}

	if (ball_y < bkh_min)
	{
		ball_vel_y = -ball_vel_y;
	}
	
	if (ball_y > bkh + 60)
	{
		game_over();
	}

	int ball_scaling = 20;
	if(ball_y+ball_scaling >= bat_y && ball_y+ball_scaling <= bat_y + 30 && ball_x+ball_scaling >= bat_x && ball_x+ball_scaling <= bat_x+60)
	{
		ball_vel_y = -ball_vel_y;
	}
}

bool ball_brick_col_detect(SDL_Rect rect_brick, SDL_Rect rect_ball)
{
	if (rect_brick.x > rect_ball.x + rect_ball.w)
	{
		return false;
	}
	if(rect_brick.x+rect_brick.w<rect_ball.x)
	{
		return false;
	}
	if (rect_brick.y > rect_ball.y + rect_ball.h)
	{
		return false;
	}
	if (rect_brick.y + rect_brick.h < rect_ball.y)
	{
		return false;
	}

	return true;
}

void ball_brick_col()
{
	bool a;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			a = ball_brick_col_detect(brick_rect[i][j], ball_rect);
			if (a == true) 
			{
				brick_rect[i][j].x = 30000;
				ball_vel_y = -ball_vel_y;
				destroy_brick_count++;
			}
			a = false;
		}
	}
}

void destroy()
{
	SDL_DestroyTexture(bat_txt);
	SDL_DestroyTexture(brick_txt);
	SDL_DestroyTexture(ball_txt);
	SDL_DestroyTexture(background_txt);

	SDL_FreeSurface(bat);
	SDL_FreeSurface(brick);
	SDL_FreeSurface(ball);
	SDL_FreeSurface(background);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}
void win()
{

	SDL_Surface* win_pic = SDL_LoadBMP("win.bmp");
	SDL_Texture* win_txt = SDL_CreateTextureFromSurface(renderer, win_pic);
	SDL_Rect win_rect = { 0, 0, 800, 450 };
	SDL_RenderCopy(renderer, win_txt, NULL, &win_rect);
	SDL_RenderPresent(renderer);
	SDL_Delay(4000);
	destroy();
	SDL_Quit();
}


int main(int argc, char** argv)
{

	
	SDL_Init(SDL_INIT_VIDEO);


	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,800, 450, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);

	
	SDL_Rect background_rect = {0, 0, 800, 450};

	ball = SDL_LoadBMP("ball.bmp");
	background = SDL_LoadBMP("bk.bmp");
	bat = SDL_LoadBMP("bat.bmp");
	brick = SDL_LoadBMP("brick.bmp");

	background_txt = SDL_CreateTextureFromSurface(renderer, background);
	ball_txt = SDL_CreateTextureFromSurface(renderer, ball);
	bat_txt = SDL_CreateTextureFromSurface(renderer, bat);
	brick_txt = SDL_CreateTextureFromSurface(renderer, brick);

	brick_init();

	//main game loop
	while (!quit)
	{
		event_handler();
		
		ball_rect = {(int) ball_x,(int) ball_y, 32, 32 };
		SDL_Rect bat_rect = { bat_x, bat_y, 60, 30 };

		if (begin_game == true)
		{
			move_ball();
		}

		ball_collision();
		ball_brick_col();

		if (destroy_brick_count == brick_num)
		{
			win();
		}
	
		SDL_RenderCopy(renderer, background_txt, NULL, &background_rect);
		SDL_RenderCopy(renderer, ball_txt, NULL, &ball_rect);
		SDL_RenderCopy(renderer, bat_txt, NULL, &bat_rect);

		for (int i = 0; i <= 2; i++)
		{
			for (int j = 0; j <= 6; j++)
			{
				SDL_RenderCopy(renderer, brick_txt, NULL, &brick_rect[i][j]);
			}
		};
		  
		SDL_RenderPresent(renderer);

		//clears renderer after each frame
		SDL_RenderClear(renderer);
	}



	
	destroy();
	SDL_Quit();
	
	return 0;

}