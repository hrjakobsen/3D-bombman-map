using namespace std;
#include "controls.h"

void Draw2DCircle(float Radius) {
	glBegin(GL_TRIANGLE_FAN);
	for (float i = 0; i < 6.28; i += 0.2) {
		glVertex2f(sinf(i) * Radius, cosf(i) * Radius);
	}
	glEnd();
}

void DrawMiniMap() {
	float Scale = 10.0f;
	glTranslatef(0.0105 * Scale, -0.0105 * Scale, 0);
	for (int x = 0; x < WorldSize; x++) {
		for (int z = 0; z < WorldSize; z++) {
			if (!SolidBlock(World[z][x])) {
				glColor3f(0.3, 0.3, 0.3);
			} else if (World[z][x] == BLOCK_WALL) {
				glColor3f(0.15, 0.15, 0.15);
			} else if (World[z][x] == BLOCK_CRATE) {
				glColor3f(0.6, 0.3, 0.0);
			}
			glTranslatef(-0.0014 * z * Scale, 0.0014 * x * Scale, 0);
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(0.0007 * Scale, 0.0007 * Scale);
			glVertex2f(0.0007 * Scale, -0.0007 * Scale);
			glVertex2f(-0.0007 * Scale, -0.0007 * Scale);
			glVertex2f(-0.0007 * Scale, 0.0007 * Scale);
			glEnd();
			if (BombWorld[z][x].armed && World[z][x] == BLOCK_AIR) {
				glColor3f(0, 0, 0);
				Draw2DCircle(0.0005 * Scale);
			}
			if (World[z][x] == BLOCK_BONUS_LIFE || World[z][x] == BLOCK_BONUS_MOREBOMBS || World[z][x] == BLOCK_BONUS_POWER || World[z][x] == BLOCK_BONUS_SPEED) {
				glBegin(GL_TRIANGLE_FAN);
				glColor3f(1, 0, 0);
				glVertex2f(0.00035 * Scale, 0.00035 * Scale);
				glColor3f(0, 1, 0);
				glVertex2f(0.00035 * Scale, -0.00035 * Scale);
				glColor3f(0, 0, 1);
				glVertex2f(-0.00035 * Scale, -0.00035 * Scale);
				glColor3f(1, 0, 1);
				glVertex2f(-0.00035 * Scale, 0.00035 * Scale);
				glEnd();
			}
			if (World[z][x] == BLOCK_XFIRE) {
				glBegin(GL_TRIANGLE_FAN);
				glColor3f(1, 0, 0);
				glVertex2f(0.0007 * Scale, 0.00035 * Scale);
				glVertex2f(0.0007 * Scale, -0.00035 * Scale);
				glVertex2f(-0.0007 * Scale, -0.00035 * Scale);
				glVertex2f(-0.0007 * Scale, 0.00035 * Scale);
				glEnd();
			} else if (World[z][x] == BLOCK_ZFIRE) {
				glBegin(GL_TRIANGLE_FAN);
				glColor3f(1, 0, 0);
				glVertex2f(0.00035 * Scale, 0.0007 * Scale);
				glVertex2f(0.00035 * Scale, -0.0007 * Scale);
				glVertex2f(-0.00035 * Scale, -0.0007 * Scale);
				glVertex2f(-0.00035 * Scale, 0.0007 * Scale);
				glEnd();
			} else if (World[z][x] == BLOCK_CFIRE) {
				glBegin(GL_TRIANGLE_FAN);
				glColor3f(1, 0, 0);
				glVertex2f(0.00035 * Scale, 0.0007 * Scale);
				glVertex2f(0.00035 * Scale, -0.0007 * Scale);
				glVertex2f(-0.00035 * Scale, -0.0007 * Scale);
				glVertex2f(-0.00035 * Scale, 0.0007 * Scale);
				glEnd();
				glBegin(GL_TRIANGLE_FAN);
				glVertex2f(0.0007 * Scale, 0.00035 * Scale);
				glVertex2f(0.0007 * Scale, -0.00035 * Scale);
				glVertex2f(-0.0007 * Scale, -0.00035 * Scale);
				glVertex2f(-0.0007 * Scale, 0.00035 * Scale);
				glEnd();
			}
			glTranslatef(0.0014 * z * Scale, -0.0014 * x * Scale, 0);
		}
	}
	for (int i = 0; i < 4; i++) {
		if (IsOnline[i] && OtherLifes[i] > 0) {
			if (i == 0) {
				glColor3f(0, 1, 0);
			} else if (i == 1) {
				glColor3f(0, 0, 1);
			} else if (i == 2) {
				glColor3f(1, 1, 0);
			} else if (i == 3) {
				glColor3f(1, 0, 1);
			}
			glTranslatef(0.0014 * -OtherPos[i].x * Scale, 0.0014 * OtherPos[i].z * Scale, 0);
			glTranslatef(0.0007 * Scale, -0.0007 * Scale, 0);
			Draw2DCircle(0.0005 * Scale);
			glTranslatef(-0.0007 * Scale, 0.0007 * Scale, 0);
			glTranslatef(0.0014 * OtherPos[i].x * Scale, 0.0014 * -OtherPos[i].z * Scale, 0);
		}
	}
	glColor3f(1, 1, 1);
	glTranslatef(-0.0105 * Scale, 0.0105 * Scale, 0);
}

void display(void) {
	//Reset
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, -.01);
	DrawMiniMap();
	glTranslatef(0, 0, .01);
	glutSwapBuffers();
}

void reshape(int w, int h){
	// Prevent a divide by zero, when window is too short
	// (you can't make a window of zero width).
	if (h == 0)
		h = 1;
	ScreenHeight = h;
	ScreenWidth = w;
	float ratio = (float)w / h;
	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset Matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the correct perspective.
	gluPerspective(170, ratio, 0.01, 100);
	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
	display();
}