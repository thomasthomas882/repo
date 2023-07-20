#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <sstream>
#include <fstream>
#include "Square.h"

using namespace std;
using namespace sf;



class Grid
{
public:

	int wdth, hgt, theminesnumber;
	vector<vector<Square *>> ttt;
	int directions[8][2] = { { 0,1 } ,{ 0,-1 },{ 1,0 } ,{ -1,0 } ,{ 1,1 } ,{ 1,-1 } ,{ -1,1 } ,{ -1,-1 } };
	bool lose;
	bool win;

	vector<int> axis, ayis;

	Grid()
	{
		wdth = 25;
		lose = false;
		theminesnumber = 50;
		win = false;
		hgt = 16;
		Grid(hgt, wdth, theminesnumber);
	}


	Grid(int h, int w, int  m)
	{
		hgt = h;
        theminesnumber = m;
        wdth = w;
        vector<Square*> thug;
        for (int i = 0; i < hgt; i++)
        {
            thug.clear();
            for (int j = 0; j < wdth; j++)
            {
                thug.push_back(new Square());
                thug.at(j)->y = j;
                thug.at(j)->x = i;
            
            }
            ttt.push_back(thug);
        }
        win = false;
        lose = false;
        for (int i = 0; i < hgt; i++)
        {
            for (int j = 0; j < wdth; j++)
            {
                for (int x = 0; x < 8; x++)
                {
                    if (i + directions[x][0] < hgt && i + directions[x][0] >= 0 && j + directions[x][1] < wdth && j + directions[x][1] >= 0)
                    {
                        
                        ttt[i][j]->adjT.push_back(ttt[i + directions[x][0]][j + directions[x][1]]);
                    }
                }
                    
            }
        }

        randomMines(theminesnumber);

	}

	void randomMines(int nom)
	{
		int tmp = 0;
		srand(time(NULL));

		while (tmp != nom)
		{
			int y = rand() % (wdth - 1);
			int x = rand() % (hgt - 1);
			Square *p = ttt[x][y];
			if (p->value["mmmine"] == false)
			{
				p->value["mmmine"] = true;
				p->value["hidden"] = false;
				tmp++;
			}
		}
	}


	int cMines(Square *t)
	{
		int tmpnumber = 0;
		for (int x = 0; x < 8; x++)
		{
			if ((t->x + directions[x][0] < hgt && t->x + directions[x][0] >= 0) && (t->y + directions[x][1] < wdth && t->y + directions[x][1] >= 0))
			{
				if (ttt[t->x + directions[x][0]][t->y + directions[x][1]]->value["mmmine"] == true)
				{
					tmpnumber++;
				}
			}
		}
		if (tmpnumber > 0)
		{
			t->value["rev"] = false;
			t->adjMines = tmpnumber;
			t->value["hidden"] = false;
			stringstream o;
			o << tmpnumber;
			string s;
			o >> s;
			t->value[s] = true;
		}

		return tmpnumber;

	}

	bool setmove(Square* t)
	{

		if (t->value["mmmine"] == true)
		{
			lose = true;
			for (int i = 0; i < hgt; i++)
			{
				for (int j = 0; j < wdth; j++)
				{
					if (ttt[i][j]->value["hidden"] == true)
						setmove(ttt[i][j]);
				}
			}
			return true;
		}

		if (t->value["hidden"] == true)
		{
			int x = cMines(t);
			if (x > 0)
			{
			
				if (winChecker() && lose == false)
				{
					win = true;
					return false;
				}
				return true;
			}
			else
			{
				t->value["hidden"] = false;
				t->value["rev"] = true;
				for (int i = 0; i < t->adjT.size(); i++)
				{
					Square *p = t->adjT[i];
					setmove(p);
				}
				if (winChecker() && lose == false)
				{
					win = true;
					return false;
				}
				return true;
			}
		}

		return false;

	}

	bool flagged(Square* t)
	{
		if (t->value["mmmine"] == true && t->value["ffflag"] == false)
		{
			
			t->value["ffflag"] = true;
			t->value["mmmine"] = false;
			axis.push_back(t->x);
			ayis.push_back(t->y);
			theminesnumber--;
			return true;
		}
		bool isMine = false;

		for (int i = 0; i < axis.size(); i++)
		{
			if (axis[i] == t->x && ayis[i] == t->y)
			{
				isMine = true;
				if (t->value["mmmine"] == false && t->value["ffflag"] == true)
				{
					
					t->value["ffflag"] = false;
					t->value["mmmine"] = true;
					axis[i] = -1;
					ayis[i] = -1;
					theminesnumber++;
					return true;
				}


			}
		}


		if (t->value["ffflag"] == false && t->value["hidden"] == true)
		{
			theminesnumber--;
		
			t->value["hidden"] = false;
			t->value["ffflag"] = true;
		
			return true;
		}
		if (t->value["ffflag"] == true && t->value["hidden"] == false)
		{
			theminesnumber++;
			
			t->value["hidden"] = true;
			t->value["ffflag"] = false;
			
			return true;
		}
		return false;
	}
	void loadTextures(Texture &t, map<string, Texture> &materials)
	{

		t.loadFromFile("images/face_lose.png");
		materials["lll"] = t;
		t.loadFromFile("images/face_win.png");
		materials["wwww"] = t;
		t.loadFromFile("images/debug.png");
		materials["ddda"] = t;
		t.loadFromFile("images/test_1.png");
		materials["Tffff"] = t;
		t.loadFromFile("images/test_2.png");
		materials["Ttttt"] = t;
		t.loadFromFile("images/test_3.png");
		materials["threee"] = t;
		t.loadFromFile("images/tile_hidden.png");
		materials["Titlee"] = t;
		t.loadFromFile("images/tile_revealed.png");
		materials["Reeveal"] = t;
		t.loadFromFile("images/flag.png");
		materials["ffflag"] = t;
		t.loadFromFile("images/mine.png");
		materials["mmmine"] = t;
		t.loadFromFile("images/face_happy.png");
		materials["hhfa"] = t;

	}

	void getGridFromFile(string filename)
	{
		string line;
		ifstream infile(filename);
		if (!infile)
			cout << "can't find file" << endl;
		
		vector<vector<int>> theMines;

	
		getline(infile, line);
		theMines.resize(line.length());


		int w=0, h=0, m=0;

		theMines[h].resize(line.length());
		for (int i = 0; i < line.length(); i++)
		{
			if (line[i] == '1')
			{
				theMines[h][i] = 1;
				m++;
			}
			else
				theMines[h][i] = 0;
		}
		h++;


		while (getline(infile, line))
		{

			istringstream iss(line);
			theMines[h].resize(line.length());

			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] == '1')
				{
					theMines[h][i] = 1;
					m++;
				}
				else
					theMines[h][i] = 0;
			}
			h++;

		}

		this->wdth = line.length();
		this->hgt = h;
		this->theminesnumber = m;

		ttt.resize(hgt);
		lose = false;

		for (int i = 0; i < hgt; i++)
		{
			ttt[i].resize(wdth);
			for (int j = 0; j < wdth; j++)
			{
				ttt[i][j] = new Square();
				ttt[i][j]->x = i;
				ttt[i][j]->y = j;
				if (theMines[i][j] == 1)
				{
					ttt[i][j]->value["hidden"] = false;
					ttt[i][j]->value["mmmine"] = true;
				}
			}
		}

		for (int i = 0; i < hgt; i++)
		{
			for (int j = 0; j < wdth; j++)
			{
				for (int x = 0; x < 8; x++)
					if (i + directions[x][0] < hgt && i + directions[x][0] >= 0 && j + directions[x][1] < wdth && j + directions[x][1] >= 0)
					{
						ttt[i][j]->adjT.push_back(ttt[i + directions[x][0]][j + directions[x][1]]);
					}
			}
		}

	}
	void rollBack()
	{
		ttt.clear();
		wdth = 25;
		hgt = 16;
		theminesnumber = 50;
		ttt.resize(hgt);
		lose = false;
		win = false;
		for (int i = 0; i < hgt; i++)
		{
			ttt[i].resize(wdth);
			for (int j = 0; j < wdth; j++)
			{
				ttt[i][j] = new Square();
				ttt[i][j]->x = i;
				ttt[i][j]->y = j;
			}
		}

		for (int i = 0; i < hgt; i++)
		{
			for (int j = 0; j < wdth; j++)
			{
				for (int x = 0; x < 8; x++)
					if (i + directions[x][0] < hgt && i + directions[x][0] >= 0 && j + directions[x][1] < wdth && j + directions[x][1] >= 0)
					{
						ttt[i][j]->adjT.push_back(ttt[i + directions[x][0]][j + directions[x][1]]);
					}
			}
		}

		randomMines(theminesnumber);
	}

	bool winChecker()
	{
		if (lose == false)
			for (int i = 0; i < hgt; i++)
			{
				for (int j = 0; j < wdth; j++)
				{
					if (ttt[i][j]->value["hidden"] == true)
					{
						return false;
					}
				}
			}
		return true;
	}


	
};

int main()

{	

	ifstream infile("boards/config.cfg");
	std::string line;
	int intiArray[3] = { -1,-1,-1 };
	int i = 0;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		int a;
		if (!(iss >> a)) { break; } 
		intiArray[i] = a;
		i++;
	}
	 
	RenderWindow GameMineSwepper(VideoMode(intiArray[0] * 32, (intiArray[1] * 32) + 88), "Minesweeper Game" , sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
	
	Grid gameGrid(intiArray[1], intiArray[0], intiArray[2]);
	map<string, Texture> materials;
	Texture texture ;
	gameGrid.loadTextures(texture,materials);
	Square *p;
	map<string, IntRect> count;


	for (int nof = 1; nof <= 8; nof++)
	{
		stringstream o;
		o << nof;
		string s;
		o >> s;
		texture.loadFromFile("images/number_"+s+".png");
		materials[s] = texture;
	}

	bool ddd = false;
	int fHeight = gameGrid.hgt*32;
	

	RectangleShape theGame(Vector2f(64.0f, 64.0f));
	RectangleShape debugstyle(Vector2f(64.0f, 64.0f));

	debugstyle.setTexture(&materials["ddda"]);


	
	RectangleShape tttt1(Vector2f(64.0f, 64.0f));
	RectangleShape tttt2(Vector2f(64.0f, 64.0f));
	RectangleShape tttt3(Vector2f(64.0f, 64.0f));

	tttt1.setTexture(&materials["Tffff"]);
	tttt2.setTexture(&materials["Ttttt"]);
	tttt3.setTexture(&materials["threee"]);

	
	RectangleShape a[4];
	
	a[0].setSize(Vector2f(21.0f, 32.0f));
	a[1].setSize(Vector2f(21.0f, 32.0f));
	a[2].setSize(Vector2f(21.0f, 32.0f));
	a[3].setSize(Vector2f(21.0f, 32.0f));

	texture.loadFromFile("images/digits.png");
	
	Vector2u theSize = texture.getSize();
	theSize.x /= 11;
	count["-"] = IntRect( theSize.x * 10, 0, theSize.x, theSize.y);

	a[0].setTexture(&texture);
	a[1].setTexture(&texture);
	a[2].setTexture(&texture);
	a[3].setTexture(&texture);
	

	for (int p = 0; p <= 9; p++)
	{
		stringstream o;
		o << p;
		string s;
		o >> s;
		count[s] = IntRect(theSize.x * p, 0, theSize.x, theSize.y);
	}
	


	while (GameMineSwepper.isOpen())
	{
		Vector2i position = Mouse::getPosition(GameMineSwepper);
		int x = position.x /32;
		int y = position.y / 32;

		Event e;
		while (GameMineSwepper.pollEvent(e))
		{
			if (e.type == Event::Closed)
				GameMineSwepper.close(); 

			if (e.type == Event::MouseButtonPressed)
				if (e.mouseButton.button == Mouse::Left)
				{
			
					if (y < gameGrid.hgt && y >= 0 && x < gameGrid.wdth && x >= 0) {
						p = gameGrid.ttt[y][x];
						gameGrid.setmove(p);
					} 
					if (theGame.getGlobalBounds().contains((Vector2f)position))
					{
						gameGrid.rollBack();
					}

					if (debugstyle.getGlobalBounds().contains((Vector2f)position))
					{
						if (ddd)
							ddd = false;
						else
							ddd = true;
					}

					if (tttt1.getGlobalBounds().contains((Vector2f)position))
					{
						gameGrid.getGridFromFile("boards/testboard1.brd");
					}
					if (tttt2.getGlobalBounds().contains((Vector2f)position))
					{
						gameGrid.getGridFromFile("boards/testboard2.brd");
					}
					if (tttt3.getGlobalBounds().contains((Vector2f)position))
					{
						gameGrid.getGridFromFile("boards/testboard3.brd");
					}
					
				}

				else if (e.mouseButton.button == Mouse::Right)
				{
				
					if (y < gameGrid.hgt && y >= 0 && x < gameGrid.wdth && x >= 0) {
						p = gameGrid.ttt[y][x];
						gameGrid.flagged(p);
					}
				}
		}
		sf::Color color(192, 192, 192);
		GameMineSwepper.clear(sf::Color::White);

		
		for (int i = 0; i < gameGrid.hgt; i++)
		{
			for (int j = 0; j < gameGrid.wdth; j++)
			{
				RectangleShape box(Vector2f(32.0f, 32.0f));
				box.setPosition(j * 32 , i * 32);
				if (gameGrid.ttt[i][j]->value["hidden"] == true){
					box.setTexture(&materials["Titlee"]);
					GameMineSwepper.draw(box);
				}
					

				if (gameGrid.ttt[i][j]->value["rev"] == true){
					box.setTexture(&materials["Reeveal"]);
					GameMineSwepper.draw(box);
				}
					

				if (gameGrid.ttt[i][j]->value["ffflag"] == true)
				{
					box.setTexture(&materials["Titlee"]);
					GameMineSwepper.draw(box);
					box.setTexture(&materials["ffflag"]);
					GameMineSwepper.draw(box);
				}

				if (gameGrid.ttt[i][j]->value["mmmine"] == true && gameGrid.lose == false){
					box.setTexture(&materials["Titlee"]);
					GameMineSwepper.draw(box);
				}
					

				if ((gameGrid.ttt[i][j]->value["mmmine"] == true && (gameGrid.lose == true || gameGrid.win==true)
					|| (gameGrid.ttt[i][j]->value["mmmine"] == true && ddd))){
						box.setTexture(&materials["Reeveal"]);
						GameMineSwepper.draw(box);
						box.setTexture(&materials["mmmine"]);
						GameMineSwepper.draw(box);
					}
					

				for (int nof = 1; nof <= 8; nof++)
				{
					stringstream o;
					o << nof;
					string s;
					o >> s;
					if (gameGrid.ttt[i][j]->value[s] == true){
						box.setTexture(&materials["Reeveal"]);
						GameMineSwepper.draw(box);
						box.setTexture(&materials[s]);
						GameMineSwepper.draw(box);
					}
						
				}

			}
		
			theGame.setPosition(300, fHeight);
			debugstyle.setPosition(300 + 64 *2, fHeight);

			
			tttt1.setPosition(300 + 64 * 4, fHeight);
			tttt2.setPosition(300 + 64 * 5, fHeight);
			tttt3.setPosition(300 + 64 * 6, fHeight);

			if(gameGrid.lose==false)
			theGame.setTexture(&materials["hhfa"]);
			else
			theGame.setTexture(&materials["lll"]);
			if (gameGrid.win == true)
			{
				theGame.setTexture(&materials["wwww"]);
				for (int y = 0; y <=0; y++)
				{
					stringstream oo;
					oo << 0;
					string ss;
					oo >> ss;
					if (y <= ss.length())
					{
						stringstream w;
						w << ss[y];
						w >> ss;
						a[y].setTextureRect(count[ss]);
					}
					else
					{
						a[y].setFillColor(Color::Black);
					}

				}

			}
			a[0].setPosition(0 + 21 * 0, fHeight);
			a[1].setPosition(0 + 21 * 1, fHeight);
			a[2].setPosition(0 + 21 * 2, fHeight);
			a[3].setPosition(0 + 21 * 3, fHeight);

		
			if(gameGrid.win == false)
			for (int y = 0; y < 4; y++)
			{
				stringstream oo;
				oo << gameGrid.theminesnumber;
				string ss;
				oo >> ss;
				if (y <= ss.length())
				{
					stringstream w;
					w << ss[y];
					w >> ss;
				
					a[y].setTextureRect(count[ss]);
				}
				else
				{
					a[y].setFillColor(Color::Black);
				}
			
			}
			

			GameMineSwepper.draw(theGame);
			GameMineSwepper.draw(tttt1);
			GameMineSwepper.draw(tttt2);
			GameMineSwepper.draw(tttt3);
			GameMineSwepper.draw(debugstyle);
			GameMineSwepper.draw(a[0]);
			GameMineSwepper.draw(a[1]);
			GameMineSwepper.draw(a[2]);
			GameMineSwepper.draw(a[3]);

		}
		


		GameMineSwepper.display();
	}


	return 0;
}

