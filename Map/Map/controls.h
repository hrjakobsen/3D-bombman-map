#include "variables.h"

bool FirstTime = true;
int lastX, lastY;

void MouseMotion(int x, int y) {
	if (IsAlive) {
		if (FirstTime) {
			FirstTime = false;
			lastY = y;
			lastX = x;
		} else {
			FirstTime = true;
			CameraAngle.x += (x - lastX) / CameraSensitivity;
			CameraAngle.y += (y - lastY) / CameraSensitivity;
			SetCursorPos(ScreenWidth / 2, ScreenHeight / 2);
			lastY = ScreenHeight / 2;
			lastX = ScreenWidth / 2;
		}
		if (CameraAngle.y > 45) {
			CameraAngle.y = 45;
		}
		if (CameraAngle.y < -45) {
			CameraAngle.y = -45;
		}
	}
}

void KeyBoardCallBackUp(unsigned char key, int x, int y) {
	KEYS[key] = false;
	if (key == 'W' || key == 'A' || key == 'S' || key == 'D' || key == 'E') {
		KEYS[key + 32] = false;
	}
}

void KeyBoardCallBackDown(unsigned char key, int x, int y) {
	KEYS[key] = true;
	if (key == 'W' || key == 'A' || key == 'S' || key == 'D' || key == 'E') {
		KEYS[key] = false;
		KEYS[key + 32] = true;
	}
}

void KeyBoardUpdate(float Diff) {
	if (KEYS[27]) { // ESC
		glutDestroyWindow(glutGetWindow());
		exit(0);
	}
	if (IsAlive) {
		vectorJB OldPos = BodyPosition;
		if (KEYS['w']) {
			vectorJB movement = { sinf(CameraAngle.x * DEC2RAD), 0, -1 * cosf(CameraAngle.x * DEC2RAD) };
			movement = Times(movement, MovementSpeed);
			movement = Times(movement, (Diff));
			BodyPosition = Subtract(BodyPosition, movement);
		}
		if (KEYS['s']) {
			vectorJB movement = { sinf(CameraAngle.x * DEC2RAD), 0, -1 * cosf(CameraAngle.x * DEC2RAD) };
			movement = Times(movement, MovementSpeed);
			movement = Times(movement, (Diff));
			BodyPosition = Add(BodyPosition, movement);
		}
		if (KEYS['a']) {
			vectorJB movement = { cosf(CameraAngle.x * DEC2RAD), 0, sinf(CameraAngle.x * DEC2RAD) };
			movement = Times(movement, MovementSpeed);
			movement = Times(movement, (Diff));
			BodyPosition = Add(BodyPosition, movement);
		}
		if (KEYS['d']) {
			vectorJB movement = { cosf(CameraAngle.x * DEC2RAD), 0, sinf(CameraAngle.x * DEC2RAD) };
			movement = Times(movement, MovementSpeed);
			movement = Times(movement, (Diff));
			BodyPosition = Subtract(BodyPosition, movement);
		}
		if (KEYS['e'] && !ELast) {
			int CAX = (CameraAngle.x + 45) / 90;
			CAX += 2;
			CameraAngle.x = CAX * 90;
			ELast = true;
		} else if (!KEYS['e']) {
			ELast = false;
		}
		if (SolidBlock(World[(int)OldPos.x][(int)BodyPosition.z])) {
			BodyPosition.z = OldPos.z;
		}
		if (SolidBlock(World[(int)BodyPosition.x][(int)OldPos.z])) {
			BodyPosition.x = OldPos.x;
		}
		if (SolidBlock(World[(int)(OldPos.x + BodyRadius)][(int)BodyPosition.z])) {
			BodyPosition.z = OldPos.z;
		}
		if (SolidBlock(World[(int)(BodyPosition.x + BodyRadius)][(int)OldPos.z])) {
			BodyPosition.x = OldPos.x;
		}
		if (SolidBlock(World[(int)OldPos.x][(int)(BodyPosition.z + BodyRadius)])) {
			BodyPosition.z = OldPos.z;
		}
		if (SolidBlock(World[(int)BodyPosition.x][(int)(OldPos.z + BodyRadius)])) {
			BodyPosition.x = OldPos.x;
		}
		if (SolidBlock(World[(int)(OldPos.x - BodyRadius)][(int)BodyPosition.z])) {
			BodyPosition.z = OldPos.z;
		}
		if (SolidBlock(World[(int)(BodyPosition.x - BodyRadius)][(int)OldPos.z])) {
			BodyPosition.x = OldPos.x;
		}
		if (SolidBlock(World[(int)OldPos.x][(int)(BodyPosition.z - BodyRadius)])) {
			BodyPosition.z = OldPos.z;
		}
		if (SolidBlock(World[(int)BodyPosition.x][(int)(OldPos.z - BodyRadius)])) {
			BodyPosition.x = OldPos.x;
		}
		if (FireBlock(World[(int)BodyPosition.x][(int)(BodyPosition.z)])) {
			if (!InFire) {
				InFire = true;
				LoseLifeNextTime = true;
			}
		} else {
			InFire = false;
		}
		if (World[(int)BodyPosition.x][(int)(BodyPosition.z)] == BLOCK_BONUS_LIFE) {
			OpNext = true;
			NextOpPosX = (int)BodyPosition.x;
			NextOpPosY = (int)BodyPosition.z;
		}
		if (World[(int)BodyPosition.x][(int)(BodyPosition.z)] == BLOCK_BONUS_POWER) {
			OpNext = true;
			NextOpPosX = (int)BodyPosition.x;
			NextOpPosY = (int)BodyPosition.z;
		}
		if (World[(int)BodyPosition.x][(int)(BodyPosition.z)] == BLOCK_BONUS_SPEED) {
			OpNext = true;
			NextOpPosX = (int)BodyPosition.x;
			NextOpPosY = (int)BodyPosition.z;
		}
		if (World[(int)BodyPosition.x][(int)(BodyPosition.z)] == BLOCK_BONUS_MOREBOMBS) {
			OpNext = true;
			NextOpPosX = (int)BodyPosition.x;
			NextOpPosY = (int)BodyPosition.z;
		}
		if (KEYS[' ']) {
			PlaceNextTime = true;
		}
	}
}
