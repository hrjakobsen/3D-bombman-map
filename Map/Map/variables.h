#include <GL\glut.h>
#include <iostream>
#include <Windows.h>
#include "vectors.h"

#include "math.h"
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <deque>

#include <thread>
#include <time.h>
#include <chrono>
#include "blocks.h"
#include "bomb.h"

//GAME SETUP
float FramesPerSecond = 60;
//VARS

float DEC2RAD = 3.14159 / 180;

short GameState = 1;
/*
1 = Playing
2 = Paused
*/
bool FirstPersonView = true;

//WORLD
int WorldWidth = 100;
int WorldHeight = 3;
int WorldDepth = 100;
const int WorldSize = 15;
short World[WorldSize][WorldSize] = {
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 2, 0, 2, 0, 2, 0, 2, 0, 0, 0, 1 },
	{ 1, 0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0, 1 },
	{ 1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1 },
	{ 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1 },
	{ 1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1 },
	{ 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1 },
	{ 1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1 },
	{ 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1 },
	{ 1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1 },
	{ 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1 },
	{ 1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1 },
	{ 1, 0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0, 1 },
	{ 1, 0, 0, 0, 2, 0, 2, 0, 2, 0, 2, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	};
BombJB BombWorld[WorldSize][WorldSize];
int BombFireWorld[WorldSize][WorldSize];

//WINDOW
int ScreenWidth;
int ScreenHeight;

//BODY
vectorJB BodyPosition = { 1.5, 0, 1.5 }; // Center of feet
float BodyHeight = 0.75; //Camera is at 95% of height
float BodyWidth = .75; //Shoulderwidth
float BodyRadius = 0.2; // For colision detection if set to 0.2 a minimum of 0.4 space is required to pass through

//Propeties
float MovementSpeed = 2.5; // m/s
int PlayerPower = 2;
int MaksBombs = 2;
int LifesBack = 3;
float LostLifeTimer = 0;
float LostLifeTimerMaks = 2.5;
bool InFire = false;

//CAMERA
vectorJB CameraAngle = { -45, 0, 0 };
float CameraSensitivity = 40;

//CONTROLS
bool KEYS[256];
bool ELast = false;

//BOMBS
float BombExplodeAge = 3;
float FireTime = 2;

//SERVER DATA
vectorJB OtherPos[4] = { { 1.5, 0, 1.5 }, { 13.5, 0, 13.5 }, { 1.5, 0, 13.5 }, { 13.5, 0, 1.5 }};
int OtherLifes[4] = { 3, 3, 3, 3};
bool IsOnline[4] = { false, false, false, false };
bool PlaceNextTime = false;
bool LoseLifeNextTime = false;
bool IsAlive = true;
int BoxOpps[250];
int BoxOppsCounter = 0;
bool OpNext;
int NextOpPosX;
int NextOpPosY;



void UpdateBombs(float Diff) {
	int BombID = 0;
	for (int i = 0; i < WorldSize; i++) {
		for (int ii = 0; ii < WorldSize; ii++) {
			BombID ++;
			if (BombWorld[i][ii].armed) {
				BombWorld[i][ii].age += Diff;
				if (BombWorld[i][ii].age > BombExplodeAge && !BombWorld[i][ii].FireDrawen) {
					BombWorld[i][ii].FireDrawen = true;
					World[i][ii] = BLOCK_CFIRE;
					BombFireWorld[i][ii] = BombID;
					for (int f = 1; f <= BombWorld[i][ii].power; f++) {
						int Ni = i + f;
						int Nii = ii;
						if ( World[Ni][Nii] == BLOCK_WALL) {
							break;
						}
						int OldBlock = World[Ni][Nii];
						World[Ni][Nii] = BLOCK_XFIRE;
						if (BombWorld[Ni][Nii].armed) {
							BombWorld[Ni][Nii].age = BombExplodeAge;
						}
						BombFireWorld[Ni][Nii] = BombID;
						if (OldBlock == BLOCK_CRATE && BoxOpps[BoxOppsCounter] != 0) {
							BombFireWorld[Ni][Nii] = 0;
							World[Ni][Nii] = BoxOpps[BoxOppsCounter++] + 100;
							if (World[Ni][Nii] == 100) {
								World[Ni][Nii] = BLOCK_AIR;
							}
							break;
						}
						else if (OldBlock == BLOCK_CRATE) {
							BoxOppsCounter++;
							break;
						}
					}
					for (int f = 1; f <= BombWorld[i][ii].power; f++) {
						int Ni = i - f;
						int Nii = ii;
						if (World[Ni][Nii] == BLOCK_WALL) {
							break;
						}
						int OldBlock = World[Ni][Nii];
						World[Ni][Nii] = BLOCK_XFIRE;
						if (BombWorld[Ni][Nii].armed) {
							BombWorld[Ni][Nii].age = BombExplodeAge;
						}
						BombFireWorld[Ni][Nii] = BombID;
						if (OldBlock == BLOCK_CRATE && BoxOpps[BoxOppsCounter] != 0) {
							BombFireWorld[Ni][Nii] = 0;
							World[Ni][Nii] = BoxOpps[BoxOppsCounter++] + 100;
							if (World[Ni][Nii] == 100) {
								World[Ni][Nii] = BLOCK_AIR;
							}
							break;
						}
						else if (OldBlock == BLOCK_CRATE) {
							BoxOppsCounter++;
							break;
						}
					}
					for (int f = 1; f <= BombWorld[i][ii].power; f++) {
						int Ni = i;
						int Nii = ii + f;
						if (World[Ni][Nii] == BLOCK_WALL) {
							break;
						}
						int OldBlock = World[Ni][Nii];
						World[Ni][Nii] = BLOCK_ZFIRE;
						if (BombWorld[Ni][Nii].armed) {
							BombWorld[Ni][Nii].age = BombExplodeAge;
						}
						BombFireWorld[Ni][Nii] = BombID;
						if (OldBlock == BLOCK_CRATE && BoxOpps[BoxOppsCounter] != 0) {
							BombFireWorld[Ni][Nii] = 0;
							World[Ni][Nii] = BoxOpps[BoxOppsCounter++] + 100;
							if (World[Ni][Nii] == 100) {
								World[Ni][Nii] = BLOCK_AIR;
							}
							break;
						}
						else if (OldBlock == BLOCK_CRATE) {
							BoxOppsCounter++;
							break;
						}
					}
					for (int f = 1; f <= BombWorld[i][ii].power; f++) {
						int Ni = i;
						int Nii = ii - f;
						if (World[Ni][Nii] == BLOCK_WALL) {
							break;
						}
						int OldBlock = World[Ni][Nii];
						World[Ni][Nii] = BLOCK_ZFIRE;
						if (BombWorld[Ni][Nii].armed) {
							BombWorld[Ni][Nii].age = BombExplodeAge;
						}
						BombFireWorld[Ni][Nii] = BombID;
						if (OldBlock == BLOCK_CRATE && BoxOpps[BoxOppsCounter] != 0) {
							BombFireWorld[Ni][Nii] = 0;
							World[Ni][Nii] = BoxOpps[BoxOppsCounter++] + 100;
							if (World[Ni][Nii] == 100) {
								World[Ni][Nii] = BLOCK_AIR;
							}
							break;
						} else if (OldBlock == BLOCK_CRATE) {
							BoxOppsCounter++;
							break;
						}
					}
				} else if (BombWorld[i][ii].age > BombExplodeAge + FireTime) {
					BombWorld[i][ii].armed = false;
					BombWorld[i][ii].age = 0;
					BombWorld[i][ii].FireDrawen = false;
					BombWorld[i][ii].power = 0;
					if (BombWorld[i][ii].WasItMeWhoPlaced) {
						MaksBombs++;
					}
					BombWorld[i][ii].WasItMeWhoPlaced = false;;
					for (int i = 0; i < WorldSize; i++) {
						for (int ii = 0; ii < WorldSize; ii++) {
							if (BombFireWorld[i][ii] == BombID) {
								World[i][ii] = BLOCK_AIR;
								BombFireWorld[i][ii] = 0;
							}
						}
					}
				}
			}
		}
	}
}
