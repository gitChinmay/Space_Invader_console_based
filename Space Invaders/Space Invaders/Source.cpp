#include <iostream>
#include <ctime>
#include <Windows.h>
#include <cstdlib>
#include <string>
#include <algorithm>

//player class which holds player properties
class Player{
	float playerHealth;				//player health
	const int yPos = 18;			//players constant y position
	int xPos;						//x position
public:
	Player();
	void setPosition(int);			//sets position
	int getXPosition();
	int getYPosition() const;
	void decreaseHealth();			//decrease health by 33
	float getHealth();				//returns health
	void setHealth();				//sets health to 100
};

Player::Player(){
	playerHealth = 100;
	xPos = 9;
}

void Player::setPosition(int x){
	xPos = x;
}

int Player::getXPosition(){
	return xPos;
}

int Player::getYPosition() const{
	return yPos;
}

void Player::decreaseHealth(){
	playerHealth -= 33.33;
}

float Player::getHealth(){
	return playerHealth;
}

void Player::setHealth(){
	playerHealth = 100;
}

//for future purpose
class Enemy{
	float enemyHealth;
public:
	Enemy();
};

Enemy::Enemy(){
	enemyHealth = 100;
}



//this class manages all the gameobjects and contains gameloop
class GameManager{
	int gameWidth;					//width of playable area
	int gameHeight;					//height of playable area
	char **gameArray;				//array which holds gameobjects in the form of chars

	Player p;						//player is contained in a game

	bool gameOver;					//tells wether game is over

	int score;						//score


public :
	GameManager(int, int);
	void createEnvironment();		//creates initial environment
	void displayEnvironment();		//displays current environment
	int gameLoop();					//main loop
	void displayGameOver();			
	void displayGameWon();
	void restartGame();				//resets game parameters
};

GameManager::GameManager(int height,int width){
	gameHeight = height;
	gameWidth = width;
	gameOver = false;
	score = 0;
}


void GameManager:: createEnvironment(){
	gameArray = new char*[gameHeight];
	for (int i = 0; i < gameHeight; i++){
		gameArray[i] = new char[gameWidth];
	}

	for (int i = 0; i <gameHeight ; i++){
		for (int j = 0; j < gameWidth; j++){
			if (i == 0 || i == gameHeight-1){
				gameArray[i][j] = '#';
			}
			else if (j == 0 || j == gameWidth-1){
				gameArray[i][j] = '#';
			}
			else{
				if (i>2 && i < 8 && j>=4 && j<36){
					if ((i % 2 != 0 && j % 2 != 0) || (i % 2 == 0 && j % 2 == 0)){
						gameArray[i][j] = '@';
					}
					else{
						gameArray[i][j] = ' ';
					}
				}
				else{
					gameArray[i][j] = ' ';
				}
				
			}
		}
		std::cout << "\n";
	}
	gameArray[p.getYPosition()][p.getXPosition()] = 'W';
}

void GameManager:: displayEnvironment(){
	std::cout << "\nScore: " << score<<"\t\tHealth: "<<p.getHealth()<<"\n";
	for (int i = 0; i < gameHeight; i++){
		for (int j = 0; j < gameWidth; j++){
			std::cout << gameArray[i][j];
		}
		std::cout << "\n";
	}
}

int GameManager::gameLoop(){
	restartGame();
	while (!gameOver){
		int randomNum, hold_i;
		
		system("cls");
		srand(time(0));
		displayEnvironment();
		for (int i = 0; i < gameHeight; i++){
			for (int j = 0; j < gameWidth; j++){
				bool enemyClearToShoot = true;
				switch (gameArray[i][j])
				{
					case 'W':
						if (GetAsyncKeyState(VK_RIGHT) != 0){
							int newXPos = j + 1;
							switch (gameArray[i][newXPos])
							{
								case ' ':
									gameArray[i][j] = ' ';
									j++;
									gameArray[i][newXPos] = 'W';
									break;
							}
						}
						else if (GetAsyncKeyState(VK_LEFT) != 0){
							int newXPos = j - 1;
							switch (gameArray[i][newXPos])
							{
								case ' ':
									gameArray[i][j] = ' ';
									j--;
									gameArray[i][newXPos] = 'W';
									break;
							}
						}
						else if (GetAsyncKeyState(VK_SPACE) != 0){
							--i;
							gameArray[i][j] = '^';
						}
						break;

					case '^':
						gameArray[i][j] = ' ';
						--i;
						if (gameArray[i][j] !='#' && gameArray[i][j]!='@'){
							gameArray[i][j] = '^';
						}
						else if (gameArray[i][j] == '@'){
							gameArray[i][j] = ' ';
							score += 1;
							if (score == 80){
								gameOver = true;
							}
						}
						break;
						
					case '@':
						hold_i = i;
						while (++hold_i < 8){
							if (gameArray[hold_i][j] == '@'){
								enemyClearToShoot = false;
								break;
							}

						}
						if (enemyClearToShoot){
							randomNum = rand() % 50 + 1;
							if (randomNum == 1){
								i++;
								gameArray[i][j] = '*';
							}
						}
						break;
						
					case '*':
						gameArray[i][j] = ' ';
						i++;
						switch (gameArray[i][j])
						{
							case ' ':
								gameArray[i][j] = '*';
								break;

							case 'W':
								if (p.getHealth() > 35){
									p.decreaseHealth();
								}
								else{
									gameOver = true;
								}
								break;
						}
						break;
						
				}
			}
		}
		Sleep(100);
	}
	if (score == 80){
		return 1;
	}
	else{
		return 0;
	}

}

void GameManager::restartGame(){
	score = 0;
	gameOver = false;
	p.setHealth();
}

void GameManager::displayGameOver(){
	system("cls");
	std::cout << "\n\n\n\n\n\n\n\t\tG A M E   O V E R\n\n\t\t    Continue? \n\t\t      Y  N\n\n\n\n\n\n\n\n\n";
}

void GameManager::displayGameWon(){
	system("cls");
	std::cout << "\n\n\n\n\n\n\n\t\tG A M E   W O N\n\n\t\t    Continue? \n\t\t      Y  N\n\n\n\n\n\n\n\n\n";
}
int main(){
	char ch;
	GameManager gameObj(20,40);
	do{
		gameObj.createEnvironment();
		gameObj.displayEnvironment();
		int status = gameObj.gameLoop();
		status ? (gameObj.displayGameWon()) : (gameObj.displayGameOver());
		std::cin >> ch;
	} while (ch == 'y');

	return 0;
}