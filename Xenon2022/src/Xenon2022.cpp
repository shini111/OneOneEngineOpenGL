#include "Engine.h"

#include "InfiniteScrollerLevel.h"
#include "backgroundAssets.h"
#include "shiftingBackground.h"
#include "ToolBox.h"
#include "UIScore.h"
#include "PlayerLife.h"
#include "PlayerHealth.h"
#include "powerUpCompanion.h"
#include "powerUpHeal.h"
#include "powerUpMissile.h"
#include "Enemy.h"
#include "Explosion.h"
#include "Missile.h"
#include "rusher.h"
#include "enemyProjectile.h"
#include "loner.h"
#include "metalAsteroid.h"
#include "stoneAsteroid.h"
#include "drone.h"
#include "dronePack.h"
#include "metalAsteroidSpawner.h"
#include "stoneAsteroidSpawner.h"
#include "ally.h"
#include "companion.h"
#include "spaceship.h"
#include "droneSpawner.h"
#include "rusherSpawner.h"
#include "lonerSpawner.h"

#undef main

GameEngine::Engine engine;

float globalRotation = 0.0f;

int main()
{
	GameWindow gameWindow;
	gameWindow.windowName = "Xenon 2000";
	gameWindow.windowWidth = 640;
	gameWindow.windowHeight = 480;

	InfiniteScrollerLevel* level = new InfiniteScrollerLevel(10);
	engine.setLevel(level);

	LevelBackground* baseBackground = new LevelBackground("resources/graphics/galaxy2.bmp", 2.f, 2.f, 0.f, 0.f);
	baseBackground->SetSortingLayer(0);

	// Create a tiled background layer
	std::vector<int> tileIDs = { 400,401,402,403,404,416,417,418,419,420,432,433,434,435,436,448,449,450,451,452,464,465,466,467,468,480,481,482,483,484,496,497,498,499,500 };
	shiftingBackground* backgroundAsset = new shiftingBackground("resources/graphics/Blocks.bmp", 0.15f, 0.15f, -150.f, 500.f, true, 64, 16, 5, 7, tileIDs);
	backgroundAsset->SetSortingLayer(1);

	std::vector<int> tileIDs2 = { 768,769,770,771,772,773,784,785,786,787,788,789,800,801,802,803,804,805 };
	shiftingBackground* backgroundAsset2 = new shiftingBackground("resources/graphics/Blocks.bmp", 0.15f, 0.15f, -300.f, 750.f, true, 64, 16, 6, 3, tileIDs2);
	backgroundAsset2->SetSortingLayer(1);
	
	std::vector<int> tileIDs3 = { 591, 636,637,591,651,1652,653,654,667,668,669,670,683,684,685,686,699,700,701,702,591,716,717,591,591,732,733,591 };
	shiftingBackground* backgroundAsset3 = new shiftingBackground("resources/graphics/Blocks.bmp", 0.15f, 0.15f, 340.f, 600.f, true, 64, 16, 4, 7, tileIDs3);
	backgroundAsset3->SetSortingLayer(1);
	
	shiftingBackground* backgroundAsset6 = new shiftingBackground("resources/graphics/Blocks.bmp", 0.15f, 0.15f, 100.f, 1100.f, true, 64, 16, 4, 7, tileIDs3);
	backgroundAsset3->SetSortingLayer(1);

	std::vector<int> tileIDs4 = { 304,305,306,307,308,309,310,311,312,313,314,320,321,322,323,324,325,326,327,328,329,330 };
	shiftingBackground* backgroundAsset4 = new shiftingBackground("resources/graphics/Blocks.bmp", 0.15f, 0.15f, 200.f, 850.f, true, 64, 16, 11, 2, tileIDs4);
	backgroundAsset3->SetSortingLayer(2);
	
	shiftingBackground* backgroundAsset5 = new shiftingBackground("resources/graphics/Blocks.bmp", 0.15f, 0.15f, -200.f, 1000.f, true, 64, 16, 11, 2, tileIDs4);
	backgroundAsset3->SetSortingLayer(2);
	
	std::vector<int> tileIDs0 = { 816,817,818,819,820,821,822,823,824,816,816,832,833,834,835,836,837,838,839,840,832,832,848,849,850,851,852,853,854,855,856,848,848,864,865,866,867,868,869,870,871,872,873,874,880,881,882,883,884,885,886,887,888,889,890,896,897,898,899,900,901,902,903,904,905,906,912,913,914,915,916,917,918,919,920,921,922,928,929,930,931,932,933,934,935,936,937,938,944,945,946,947,948,949,950,951,952,954,954,960,961,962,963,964,965,966,967,969,969,969,976,976,976,976,980,981,982,976,976,976,976,992,992,992,992,996,997,998,992,992,992,992 };
	backgroundAssets* backgroundAsset0 = new backgroundAssets("resources/graphics/Blocks.bmp", 0.2f, 0.2f, 0.f, 2500.f, true, 64, 16, 11, 12, tileIDs0);
	backgroundAsset0->SetSortingLayer(0);
	backgroundAsset0->moveSpeed = 10.f;


	level->addBackground(baseBackground);
	level->addBackground(backgroundAsset);
	level->addBackground(backgroundAsset2);
	level->addBackground(backgroundAsset3);
	level->addBackground(backgroundAsset4);
	level->addBackground(backgroundAsset5);
	level->addBackground(backgroundAsset6);
	level->addBackground(backgroundAsset0);

	Font* myFont = new Font("resources/graphics/font16x16.bmp", 8, 12);
	UIScore* myUIScore = new UIScore(myFont, "", -0.9f, 0.85f, 0.05f, 0.05f);


	myUIScore->SetSortingLayer(9);

	level->addUIText(myUIScore);

	spaceship* ship = new spaceship();
	ship->SetSortingLayer(4);
	level->addObject(ship);
	
	rusherSpawner* spawner = new rusherSpawner();
	level->addObject(spawner);

	lonerSpawner* spawner2 = new lonerSpawner();
	level->addObject(spawner2);

	metalAsteroidSpawner* spawner3 = new metalAsteroidSpawner();
	level->addObject(spawner3);
	
	stoneAsteroidSpawner* spawner4 = new stoneAsteroidSpawner();
	level->addObject(spawner4);
	
	droneSpawner* spawner5 = new droneSpawner();
	level->addObject(spawner5);
	
	powerUpCompanion* p1 = new powerUpCompanion();
	p1->position.x = 0.f;
	p1->position.y = 250;
	level->addObject(p1);

	engine.Initialize(gameWindow);

}
