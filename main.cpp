#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <time.h>
#include <iostream>
#include "lods.hpp"
#pragma comment(lib, "glu32.lib")

using namespace sf;

float angleX, angleY;

class Player
{
public:
	float x, y, z;
	float dx, dy, dz;
	float w, h, d; // width, height, depth
	bool onGround;
	float speed;
	bool ultraMode;
	int HealthPoints;

	Player(float x0, float y0, float z0)
	{
		x = x0; dx = 0;
		y = y0; dy = 0;
		z = z0; dz = 0;
		w = 5; h = 20, d = 5;
		onGround = false;
		speed = 5;
		HealthPoints = 100;
		ultraMode = false;
	}

	void keyboard()
	{
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			if (ultraMode)
			{
				if (onGround)
				{
					onGround = false;
					dy = 24;
				}
			}
			else
			{
				if (onGround) 
				{
					onGround = false;
					dy = 12;
				}
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::W))	
		{
			dx = -sin(angleX / 180 * PI) * speed; 
			dz = -cos(angleX / 180 * PI) * speed;
		}

		if (Keyboard::isKeyPressed(Keyboard::S))	
		{
			dx =  sin(angleX / 180 * PI) * speed;
			dz =  cos(angleX / 180 * PI) * speed;
		}

		if (Keyboard::isKeyPressed(Keyboard::D))		
		{
			dx =  sin((angleX + 90) / 180 * PI) * speed;	
			dz =  cos((angleX + 90) / 180 * PI) * speed;
		}

		if (Keyboard::isKeyPressed(Keyboard::A))	
		{
			dx = sin((angleX - 90) / 180 * PI) * speed;	
			dz = cos((angleX - 90) / 180 * PI) * speed;
		}

		if (Keyboard::isKeyPressed(Keyboard::Z))
		{
			speed = 15;
			ultraMode = true;
		}

		if (Keyboard::isKeyPressed(Keyboard::X))
		{
			speed = 5;
			ultraMode = false;
		}
	}

	void collision(float Dx,float Dy,float Dz)
	{
		for (int X = (x - w) / size; X < (x + w) / size; X++)
			for (int Y = (y - h) / size; Y < (y + h) / size; Y++)
				for (int Z = (z - d) / size; Z < (z + d) / size; Z++)
					if (check(X, Y, Z)) 
					{
						if (Dx > 0)  x = X * size - w; 
						if (Dx < 0)  x = X * size + size + w; 
						if (Dy > 0)  y = Y * size - h; 
						if (Dy < 0) {y = Y * size + size + h; onGround = true; dy = 0;} 
						if (Dz > 0)  z = Z * size - d; 
						if (Dz < 0)  z = Z * size + size + d; 
					}
	}

	void update(float time)
	{
		if (!onGround) dy -= 1.5 * time;
		onGround = 0;
	 
		x += dx * time;
		collision(dx, 0, 0);	     
		y += dy * time;        
		collision(0, dy, 0);
		z += dz * time;
		collision(0, 0, dz);
 
		dx = dz = 0; 
	}
};

int main()
{
	int mode, map_ch;
	std::cout << "Choose mode:\n1-Parkour mode\n2-Normal mode\n";
	std::cin >> mode;
	if (mode == 1)
	{
		srand(time(0));
		for (int x = 0; x < 20; x++)
			{
				for (int y = 0; y < 20; y++)
				{
					for (int z = 0; z < 20; z++)
					{
						if ((y == 0) || (rand() % 100 == 1))
						{
							mass[x][y][z] = true;
						}
					}
				}
			}
	}
	else
	{
		Image map;  
		std::cout << "\nChoose map:\n1, 2, 3 or 4\n";
		std::cin >> map_ch;
		if (map_ch == 1)
			map.loadFromFile("resources/heightmap.png");
		if (map_ch == 2)
			map.loadFromFile("resources/heightmap1.png");
		if (map_ch == 3)
			map.loadFromFile("resources/heightmap2.png");
		if (map_ch == 4)
			map.loadFromFile("resources/heightmap3.png");
		for (int x = 0; x < 256; x++)
			for (int z = 0; z < 256; z++)
			{
				int c = map.getPixel(x, z).r / 15;
				for (int y = 0; y < c; y++)
					if (y > c - 3) 
						mass[x][y][z] = 1;
			}
	}

	RenderWindow window(VideoMode(1920, 1080), "Undercraft");
	
	//Some opengl magic
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.f, 1.f, 1.f, 2000.f);
	glEnable(GL_TEXTURE_2D);
	ShowCursor(false);
	////////////////////

	GLuint grass[6];
	grass[0] = LoadTexture("resources/grass/side.jpg");
	grass[1] = LoadTexture("resources/grass/side.jpg");
	grass[2] = LoadTexture("resources/grass/side.jpg");
	grass[3] = LoadTexture("resources/grass/side.jpg");
	grass[4] = LoadTexture("resources/grass/bottom.jpg");
	grass[5] = LoadTexture("resources/grass/top.jpg");

	GLuint skybox[6];
	skybox[0] = LoadTexture("resources/skybox3/skybox_front.bmp");
	skybox[1] = LoadTexture("resources/skybox3/skybox_back.bmp");
	skybox[2] = LoadTexture("resources/skybox3/skybox_left.bmp");
	skybox[3] = LoadTexture("resources/skybox3/skybox_right.bmp");
	skybox[4] = LoadTexture("resources/skybox3/skybox_bottom.bmp");
	skybox[5] = LoadTexture("resources/skybox3/skybox_top.bmp");

	//Custom cursor
	Texture t; 
	t.loadFromFile("resources/cursor.png");
	Sprite s(t); 
	s.setOrigin(8,8); 
	s.setPosition(960, 540);
	////////////////

	Clock clock;
	bool mLeft = false, mRight = false; //Mouse buttons
	Player eliot(100, 200, 100);

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMilliseconds();
		clock.restart();
		time /= 50;
		if (time > 3)
			time = 3;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == event.Closed)
			{
				window.close();
			}

			if ((event.type == Event::KeyPressed) && (event.key.code == Keyboard::Escape))
				window.close();

			if (event.type == Event::MouseButtonPressed)
			{
				if (event.key.code == Mouse::Right)
					mRight = true;
				if (event.key.code == Mouse::Left)
					mLeft = true;
			}
		}

		//Clear the depth buffer
		glClear(GL_DEPTH_BUFFER_BIT);
		
		eliot.keyboard();
		eliot.update(time);

		/////Mouse movements//////////////////
		POINT mousexy;
		GetCursorPos(&mousexy);
		int xt = window.getPosition().x + 960; 
		int yt = window.getPosition().y + 540;

		angleX += (xt - mousexy.x) / 4;// 4 - sensivity
		angleY += (yt - mousexy.y) / 4;

		if (angleY < -89.0){angleY = -89.0;}
		if (angleY > 89.0){angleY = 89.0;}

		SetCursorPos(xt,yt);   
		//////////////////////////////////////

		if (mRight || mLeft)
		{
			float x = eliot.x;
			float y = eliot.y + eliot.h / 2;
            float z = eliot.z;

			int X, Y, Z, oldX, oldY, oldZ;
			int dist = 0;
		    while(dist < 120) // Distanse of building
		    {
				dist++;

				x += -sin(angleX / 180 * PI); X = x / size;
				y +=  tan(angleY / 180 * PI); Y = y / size;
				z += -cos(angleX / 180 * PI); Z = z / size;
			
				if (check(X, Y, Z))
					if (mLeft) {mass[X][Y][Z]=0; break;}
					else {mass[oldX][oldY][oldZ] = 1; break;}
			   
				oldX = X; oldY = Y; oldZ = Z;
			}
		}
  
		mLeft = mRight = false;

		//Apply some transformations
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eliot.x, eliot.y + eliot.h / 2, eliot.z, eliot.x - sin(angleX / 180 * PI), eliot.y + eliot.h / 2 + (tan(angleY / 180 * PI)), eliot.z - cos(angleX / 180 * PI), 0, 1, 0); 

		//lol, Drawing the qubes
		int R = 30;  
		
		int X = eliot.x / size;
		int Y = eliot.y / size;
		int Z = eliot.z / size;
	
		for (int x = X - R; x < X + R; x++)
		{
			for (int y = 0; y < 25; y++)
			{
				for (int z = Z - R; z < Z + R; z++)
				{
					if (!check(x,y,z))
						continue;
					glTranslatef( size * x + size / 2,  size * y + size / 2,  size * z + size / 2);
					createBlock(grass, size / 2);
					glTranslatef(-size * x - size / 2, -size * y - size / 2, -size * z - size / 2);
				}
			}
		}

		//Drawing the skybox
		glTranslatef( eliot.x,  eliot.y,  eliot.z);
		createBlock(skybox, 1000);
		glTranslatef(-eliot.x, -eliot.y, -eliot.z);
	   
		//Drawing the cursor
		window.pushGLStates();
		window.draw(s);
		window.popGLStates();
		
		//Display, please
		window.display();
		}
    return 0;
}