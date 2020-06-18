#include <iostream>
#include <unistd.h>
#include <time.h>
#include <math.h>

// Apple Specific Compatibility Issues
#ifdef __APPLE__
	#include "GLUT/glut.h"
#else
	#include "GL\glut.h"
#endif

// Define the value of PI, upto 12 decimal places
#define PI 3.141592653589

// ID to detect which scene the story is in.
int SCENE_ID;

// Variables for Animator Faders
GLfloat title_fade,
	intro_next_text_appear, summary_next_text_appear,
	summary_para1_fade, summary_para2_fade, summary_para3_fade, summary_para4_fade,
	kg_chap_fade, kg_title_fade,
	kg_a_fade, kg_apple_fade, kg_b_fade, kg_ball_fade, kg_soon_fade, kg_subtitle_1_appear,
	ps_chap_fade, ps_title_fade,
	ps_subtitle_1_appear, ps_subtitle_2_appear,
	hs_chap_fade, hs_title_fade,
	hs_subtitle_1_appear,
	puc_chap_fade, puc_title_fade,
	puc_subtitle_1_fade,
	eng_chap_fade, eng_title_fade,
	ec_subtitle_1_fade,
	wp_chap_fade, wp_title_fade,
	wp_subtitle_1_fade,
	epilogue_para1_fade, epilogue_para2_fade, epilogue_para3_fade, epilogue_para4_fade, epilogue_para5_fade, epilogue_para6_fade,
	moral_para1_fade, moral_para2_fade, moral_para3_fade, moral_para4_fade;

// Variables for Translation Animators
GLfloat trans_x_chap1, trans_x_title1,
	trans_x_chap2, trans_y_title2,
	trans_x_kid1, trans_x_kid2, trans_x_kid3, trans_x_ball,
	trans_subtitle_1_done, trans_subtitle_2_done,
	trans_x_chap3, trans_x_title3,
	trans_x_chap4, trans_x_title4,
	sun_move_left,
	schoolboy_x = 1150, schoolboy_y = 5220,
	trans_x_sb1 = -630, trans_y_sb1 = -50, trans_x_sb2 = -1630, trans_y_sb2 = -50,
	trans_x_chap5, trans_x_title5,
	trans_x_ec_sb1 = 850, trans_x_ec_sb2 = -1000, trans_y_ec_sb = 40, trans_x_ec_gb = -180, trans_y_ec_gb = 1040,
	trans_x_chap6, trans_x_title6;

// Variables for Color Morphers
GLfloat window_top_r = 59, window_top_g = 91, window_top_b = 132,
	window_bottom_r = 97, window_bottom_g = 131, window_bottom_b = 159,
	sky_r = 12, sky_g = 172, sky_b = 232,
	grass_r = 82, grass_g =  163, grass_b = 42,
	sun_r = 251, sun_g = 255, sun_b = 163;

// Variables for Turn-based Switching
bool sun_moved_half, sun_has_set, stars_are_made,
	chapter_1_done, chapter_2_done, chapter_3_done, chapter_4_done, chapter_5_done, tuition_done,
	puc_begin_anim, puc_end_anim,
	ec_graduation_done;

// Variables for Random Star Generation
int star_alpha, no_of_stars, stars_array[40][2];

// Function to Create Delay
void delay(float secs) {
	float end = clock()/CLOCKS_PER_SEC + secs;
	while((clock()/CLOCKS_PER_SEC) < end);
}

// Function to Print Text
void print(char *string,
	GLfloat r, GLfloat g, GLfloat b, GLfloat a,
	GLint x, GLint y,
	GLfloat w, GLfloat h,
	GLfloat strokeSize) {
	glPushMatrix();

	glColor4f(r, g, b, a);
	glTranslatef(x, y, 0);
	glScalef(w, h, 0);

	glPointSize(strokeSize);
	glLineWidth(strokeSize);

	while (*string)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *string++);

	glPopMatrix();

	glLineWidth(1);
	glPointSize(1);
}

// Function to Draw Circle
void drawCircle(GLfloat x, GLfloat y,
	GLfloat r, GLfloat g, GLfloat b,
	GLfloat sx, GLfloat sy,
	GLfloat radius) {
		glPushMatrix();

		glTranslatef(x, y, 0);
		glScalef(sx, sy, 0);

    glBegin(GL_POLYGON);
		glColor3ub(r, g, b);
    for (GLfloat i = 0; i < 360; i += 5)
        glVertex2f(radius * sin(i * PI / 180), radius * cos(i * PI / 180));
    glEnd();

		glPopMatrix();
}

// Function to Draw Circle
void drawSemiCircle(GLfloat tx, GLfloat ty,
	GLfloat sx, GLfloat sy,
	GLfloat r, GLfloat g, GLfloat b,
	GLfloat radius,
	GLfloat start_angle, GLfloat end_angle) {
		glPushMatrix();

		glTranslatef(tx, ty, 0);
		glScalef(sx, sy, 0);

    glBegin(GL_POLYGON);
		glColor3ub(r, g, b);
    for (GLfloat i = start_angle; i < end_angle; i += 5)
        glVertex2f(radius * sin(i * PI / 180), radius * cos(i * PI / 180));
    glEnd();

		glPopMatrix();
}

// Function to Draw Arc
void drawArc(GLfloat tx, GLfloat ty,
	GLfloat sx, GLfloat sy,
	GLfloat r, GLfloat g, GLfloat b) {
	glPushMatrix();

	glTranslatef(tx, ty, 0);
	glScalef(sx, sy, 0);

	glPointSize(2);
	glBegin(GL_POINTS);
	glColor3ub(r, g, b);
  for ( GLfloat i = 90; i < 270; i++)
    glVertex2f(15 * sin(i * PI / 180), 15 * cos(i * PI / 180));
  glEnd();
	glPointSize(1);

	glPopMatrix();
}

// Function to Draw a Woman
void drawWoman(GLfloat tx, GLfloat ty, GLfloat sx, GLfloat sy,
	GLfloat hair_r, GLfloat hair_g, GLfloat hair_b,
	GLfloat top_r, GLfloat top_g, GLfloat top_b,
	GLfloat ribbon_r, GLfloat ribbon_g, GLfloat ribbon_b,
	GLfloat skirt_r, GLfloat skirt_g, GLfloat skirt_b) {
	glPushMatrix();

	glTranslatef(tx, ty, 0);
	glScalef(sx, sy, 0);

	// Neck
	glLineWidth(10);
	glBegin(GL_LINES);
	glColor3ub(203, 166, 108); // Darker Skin
	glVertex2f(507, 380);
	glVertex2f(507, 360);
	glVertex2f(513, 380);
	glVertex2f(513, 360);
	glEnd();
	glLineWidth(1);

	// Face
	drawCircle(510, 407,
		232, 190, 123, // Lighter Skin
		1, 1,
		30);

	// Hair
	drawSemiCircle(510, 420, 1, .6,
		hair_r, hair_g, hair_b,
		35, -90, 90);

	drawSemiCircle(485, 400, .4, 1,
		hair_r, hair_g, hair_b,
		35, -180, 0);

	// Left Eye
	drawCircle(505, 410,
		250, 250, 250,
		1, 1,
		5);

	drawCircle(508, 408,
		10, 10, 10,
		1, 1,
		2.5);

	// Right Eye
	drawCircle(530, 410,
		250, 250, 250,
		1, 1,
		5);

	drawCircle(532, 408,
		10, 10, 10,
		1, 1,
		2.5);

	// Smile
	drawArc(514, 388, .5, .3,
		20, 20, 20);

	// Nose
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(518, 405);
	glVertex2f(522, 398);
	glVertex2f(522, 398);
	glVertex2f(518, 395);
	glEnd();
	glLineWidth(1);

	// Arms
	glLineWidth(10);
	glBegin(GL_LINES);
	glColor3ub(232, 190, 123); // Lighter Skin
	glVertex2f(470, 340);
	glVertex2f(470, 240);
	glVertex2f(475, 340);
	glVertex2f(475, 240);
	glVertex2f(480, 340);
	glVertex2f(480, 240);

	glVertex2f(540, 340);
	glVertex2f(540, 240);
	glVertex2f(545, 340);
	glVertex2f(545, 240);
	glVertex2f(550, 340);
	glVertex2f(550, 240);
	glEnd();
	glLineWidth(1);

	// Top
	glBegin(GL_POLYGON);
	glColor3ub(top_r, top_g, top_b);
	glVertex2f(460, 370);
	glVertex2f(560, 370);
	glVertex2f(540, 300);
	glVertex2f(480, 300);
	glEnd();

	// Skirt
	glBegin(GL_POLYGON);
	glColor3ub(skirt_r, skirt_g, skirt_b);
	glVertex2f(480, 300);
	glVertex2f(540, 300);
	glVertex2f(560, 180);
	glVertex2f(460, 180);
	glEnd();

	// Ribbon
	glLineWidth(10);
	glBegin(GL_LINES);
	glColor3ub(ribbon_r, ribbon_g, ribbon_b);
	glVertex2f(480, 300);
	glVertex2f(540, 300);
	glEnd();
	glLineWidth(1);

	// Legs
	glLineWidth(10);
	glBegin(GL_LINES);
	glColor3ub(232, 190, 123); // Lighter Skin
	glVertex2f(490, 180);
	glVertex2f(490, 150);
	glVertex2f(495, 180);
	glVertex2f(495, 150);

	glVertex2f(525, 180);
	glVertex2f(525, 150);
	glVertex2f(530, 180);
	glVertex2f(530, 150);
	glEnd();
	glLineWidth(1);

	glPopMatrix();
}

void drawKidsShirtAndTrousers(GLfloat tx, GLfloat ty,
	GLfloat sx, GLfloat sy,
	GLfloat shirt_r, GLfloat shirt_g, GLfloat shirt_b) {
	glPushMatrix();

	glTranslatef(tx, ty, 0);
	glScalef(sx, sy, 0);

	// Bottom Shirt
	glColor3ub(shirt_r, shirt_g, shirt_b);
	glBegin(GL_POLYGON);
	glVertex2f(0, 20);
	glVertex2f(100, 20);
	glVertex2f(100, 100);
	glVertex2f(0, 100);
	glEnd();

	// Top Shirt
	glBegin(GL_POLYGON);
	glVertex2f(100, 100);
	glVertex2f(135, 105);
	glVertex2f(120, 175);
	glVertex2f(100, 200);
	glVertex2f(50, 180);
	glVertex2f(0, 200);
	glVertex2f(-20, 175);
	glVertex2f(-35, 105);
	glVertex2f(0, 100);
	glEnd();

	// Left Trouser
	glColor3ub(37, 107, 202);
	glBegin(GL_POLYGON);
	glVertex2f(0, 25);
	glVertex2f(0, -100);
	glVertex2f(35, -100);
	glVertex2f(65, 25);
	glEnd();

	// Right Trouser
	glBegin(GL_POLYGON);
	glVertex2f(35, 25);
	glVertex2f(65, -100);
	glVertex2f(100, -100);
	glVertex2f(100, 25);
	glEnd();

	glPopMatrix();
}

void drawKid(GLfloat tx, GLfloat ty,
	GLfloat sx, GLfloat sy,
	GLfloat shirt_r, GLfloat shirt_g, GLfloat shirt_b) {
	glPushMatrix();

	glTranslatef(tx, ty, 0);
	glScalef(sx, sy, 0);

	// Neck
	drawSemiCircle(-1, -35, .6, 1,
		203, 166, 108,
		20, -90, 90);

	// Face
  drawCircle(0, 0,
		232, 190, 123,
		1, 1,
		24);

	// Left Eye
	drawCircle(-8, 0,
		250, 250, 250,
		1, 1,
		4);
	drawCircle(-6, 0,
		10, 10, 10,
		1, 1,
		2);

	// Right Eye
	drawCircle(8, 0,
		250, 250, 250,
		1, 1,
		4);
	drawCircle(10, 0,
		10, 10, 10,
		1, 1,
		2);

	// Cap
	drawSemiCircle(0, 10, 1, 1,
		37, 107, 202,
		24, -90, 90);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2f(20, 14);
	glVertex2f(40, 16);
	glEnd();
	glLineWidth(1);

	// Hands
	drawCircle(-42, -82,
		232, 190, 123,
		1, 1,
		10);
	drawCircle(38, -82,
		232, 190, 123,
		1, 1,
		10);

	// Shirt and Trousers
	drawKidsShirtAndTrousers(-32, -125,
		.6, .5,
		shirt_r, shirt_g, shirt_b);

	// Left Shoe
	drawSemiCircle(-21, -178,
		1.2, 1,
		20, 20, 20,
		10,
		-90, 90);

	// Right Shoe
	drawSemiCircle(18, -178,
		1.2, 1,
		20, 20, 20,
		10,
		-90, 90);

	glPopMatrix();
}

void drawTree(GLfloat tx, GLfloat ty,
	GLfloat sx, GLfloat sy) {
	glPushMatrix();

	glTranslatef(tx, ty, 0);
	glScalef(sx, sy, 0);

	// Bark
	glBegin(GL_POLYGON);
	glColor3ub(86, 46, 11);
	glVertex2f(0, 0);
	glVertex2f(40, 0);
	glColor3ub(71, 36, 6);
	glVertex2f(35, 200);
	glVertex2f(5, 200);
	glEnd();

	// Tree
	drawCircle(20, 200, 5, 80, 10, 1, 1, 80);

	// Apples
	drawCircle(27, 194, 255, 0, 0, 1, 1.2, 5);
	drawCircle(-15, 170, 255, 0, 0, 1, 1.2, 5);
	drawCircle(47, 155, 255, 0, 0, 1, 1.2, 5);
	drawCircle(-2, 228, 255, 0, 0, 1, 1.2, 5);
	drawCircle(72, 216, 255, 0, 0, 1, 1.2, 5);

	glPopMatrix();
}

void highSchoolTitleScreen() {
	// Background
	glClearColor(0.05, 0.05, 0.05, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Story Title
	print("CHAPTER 3",
		1, 1, 1, hs_chap_fade, 300 + trans_x_chap3, 400, .2, .2, 2);
	print("High School",
		1, 1, 1, hs_title_fade, 400 - trans_x_title3, 350, .3, .3, 2);
}

void HS_drawBackground() {
	// Background
	glBegin(GL_POLYGON);
	glColor3ub(sky_r, sky_g, sky_b); // Sky Blue
	glVertex2f(0, 800);
	glVertex2f(1400, 800);
	glVertex2f(1400, 400);
	glVertex2f(0, 400);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3ub(sky_r, sky_g, sky_b); // Sky Blue
	glVertex2f(0, 400);
	glVertex2f(1400, 400);
	glColor3ub(grass_r, grass_g, grass_b); // Green Grass
	glVertex2f(1400, 175);
	glVertex2f(0, 175);
	glEnd();
}

void HS_drawSun() {
	// Sun
	drawCircle(1250 - sun_move_left, 700,
		sun_r, sun_g, sun_b,
		1, 1,
		50);
}

void HS_drawRoad() {
	// Road
	glBegin(GL_POLYGON);
	glColor3ub(40, 40, 40);
	glVertex2f(0, 175);
	glVertex2f(1400, 175);
	glVertex2f(1400, 75);
	glVertex2f(0, 75);
	glEnd();

	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3ub(255, 255, 255);
	glVertex2f(0, 125);
	glVertex2f(200, 125);

	glVertex2f(300, 125);
	glVertex2f(500, 125);

	glVertex2f(600, 125);
	glVertex2f(800, 125);

	glVertex2f(900, 125);
	glVertex2f(1100, 125);

	glVertex2f(1200, 125);
	glVertex2f(1400, 125);
	glEnd();
	glLineWidth(1);
}

void HS_drawHome() {
	// Home Bottom
	glBegin(GL_POLYGON);
	glColor3ub(169, 117, 83);
	glVertex2f(1375, 175);
	glColor3ub(181, 145, 113);
	glVertex2f(1375, 300);
	glColor3ub(181, 145, 113);
	glVertex2f(1100, 300);
	glColor3ub(169, 117, 83);
	glVertex2f(1100, 175);
	glEnd();

	// Home Bottom Border
	glBegin(GL_LINE_LOOP);
	glColor3ub(20, 20, 20);
	glVertex2f(1375, 175);
	glVertex2f(1375, 300);
	glVertex2f(1100, 300);
	glVertex2f(1100, 175);
	glEnd();

	// Home Top Left
	glBegin(GL_POLYGON);
	glColor3ub(181, 145, 113);
	glVertex2f(1100, 300);
	glVertex2f(1220, 300);
	glColor3ub(169, 117, 83);
	glVertex2f(1220, 400);
	glVertex2f(1100, 400);
	glEnd();

	// Home Top Left Border
	glBegin(GL_LINE_LOOP);
	glColor3ub(20, 20, 20);
	glVertex2f(1100, 300);
	glVertex2f(1220, 300);
	glVertex2f(1220, 400);
	glVertex2f(1100, 400);
	glEnd();

	// Triangle Rooftop
	glBegin(GL_TRIANGLES);
	glColor3ub(69, 39, 23);
	glVertex2f(1090, 400);
	glVertex2f(1230, 400);
	glVertex2f(1160, 450);
	glEnd();

	// Triangle Rooftop Border
	glBegin(GL_LINE_LOOP);
	glColor3ub(20, 20, 20);
	glVertex2f(1090, 400);
	glVertex2f(1230, 400);
	glVertex2f(1160, 450);
	glEnd();

	// Home Top Right
	glBegin(GL_POLYGON);
	glColor3ub(69, 39, 23);
	glVertex2f(1220, 375);
	glVertex2f(1375, 375);
	glVertex2f(1375, 300);
	glVertex2f(1220, 300);
	glEnd();

	glBegin(GL_LINES);
	glColor3ub(99, 56, 33);
	for (int temp_x = 1220; temp_x <= 1375; temp_x += 5) {
		glVertex2f(temp_x, 375);
		glVertex2f(temp_x, 300);
	}
	for (int temp_y = 300; temp_y <= 375; temp_y += 5) {
		glVertex2f(1220, temp_y);
		glVertex2f(1375, temp_y);
	}
	glEnd();

	// Home Bottom-Top Divider
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3ub(20, 20, 20);
	glVertex2f(1090, 300);
	glVertex2f(1385, 300);
	glEnd();
	glLineWidth(1);

	// Door
	glBegin(GL_POLYGON);
	glColor3ub(31, 47, 53);
	glVertex2f(1130, 175);
	glVertex2f(1130, 280);
	glVertex2f(1190, 280);
	glVertex2f(1190, 175);
	glEnd();

	// Door Border
	glBegin(GL_LINE_LOOP);
	glColor3ub(20, 20, 20);
	glVertex2f(1130, 175);
	glVertex2f(1130, 280);
	glVertex2f(1190, 280);
	glVertex2f(1190, 175);
	glEnd();

	// Top Window
	glBegin(GL_POLYGON);
	glColor3ub(window_top_r, window_top_g, window_top_b);
	glVertex2f(1130, 330);
	glVertex2f(1130, 370);
	glVertex2f(1190, 370);
	glColor3ub(window_bottom_r, window_bottom_g, window_bottom_b);
	glVertex2f(1190, 330);
	glEnd();

	// Top Window Border
	glBegin(GL_LINE_LOOP);
	glColor3ub(20, 20, 20);
	glVertex2f(1130, 330);
	glVertex2f(1130, 370);
	glVertex2f(1190, 370);
	glVertex2f(1190, 330);
	glEnd();

	// Rectangular Window
	glBegin(GL_POLYGON);
	glColor3ub(59, 91, 132);
	glVertex2f(1240, 280);
	glVertex2f(1240, 240);
	glVertex2f(1355, 240);
	glColor3ub(97, 131, 159);
	glVertex2f(1355, 280);
	glEnd();

	// Rectangular Window Border
	glBegin(GL_LINE_LOOP);
	glColor3ub(20, 20, 20);
	glVertex2f(1240, 280);
	glVertex2f(1240, 240);
	glVertex2f(1355, 240);
	glVertex2f(1355, 280);
	glEnd();

	// Rectangular Window Panes
	glBegin(GL_LINES);
	glColor3ub(20, 20, 20);
	glVertex2f(1297, 280);
	glVertex2f(1297, 240);
	glVertex2f(1355, 260);
	glVertex2f(1240, 260);
	glEnd();

	// Top Circle
	drawCircle(1160, 420,
		255, 255, 255,
		1, 1,
		10);
}

void HS_drawSchool() {
	// School Building
	glBegin(GL_POLYGON);
  glColor3ub(184, 88, 68);
  glVertex2f(400, 175);
	glVertex2f(1000, 175);
	glVertex2f(1000, 500);
	glColor3ub(241, 130, 94);
	glVertex2f(400, 500);
	glEnd();

	// School Building Border
	glBegin(GL_LINE_LOOP);
	glColor3ub(20, 20, 20);
	glVertex2f(400, 175);
	glVertex2f(1000, 175);
	glVertex2f(1000, 500);
	glVertex2f(400, 500);
	glEnd();

	// School Board
	glBegin(GL_POLYGON);
	glColor3ub(183, 184, 188);
	glVertex2f(570, 530);
	glVertex2f(830, 530);
	glVertex2f(830, 470);
	glVertex2f(570, 470);
	glEnd();

	// School Board Border
	glBegin(GL_LINE_LOOP);
	glColor3ub(20, 20, 20);
	glVertex2f(570, 530);
	glVertex2f(830, 530);
	glVertex2f(830, 470);
	glVertex2f(570, 470);
	glEnd();

	print("SCHOOL", 0, 0, 0, 1, 610, 485, .3, .3, 1.5);

  // School Door
	glBegin(GL_POLYGON);
	glColor3ub(183, 184, 188);
	glVertex2f(600, 175);
	glVertex2f(600, 280);
	glVertex2f(800, 280);
	glVertex2f(800, 175);
	glEnd();

	// School Door Border
	glBegin(GL_LINE_LOOP);
	glColor3ub(20, 20, 20);
	glVertex2f(600, 175);
	glVertex2f(600, 280);
	glVertex2f(800, 280);
	glVertex2f(800, 175);
	glEnd();

	// School Door Divider
	glBegin(GL_LINES);
	glColor3ub(20, 20, 20);
	glVertex2f(700, 280);
	glVertex2f(700, 175);
	glEnd();

	// School Windows
	for (int i = 0; i <= 500; i+=100) {
		for (int j = 0; j <= 100; j+=80) {
			glBegin(GL_POLYGON);
			glColor3ub(59, 91, 132);
			glVertex2f(425 + i, 450 - j);
			glVertex2f(475 + i, 450 - j);
			glVertex2f(475 + i, 400 - j);
			glColor3ub(97, 131, 159);
			glVertex2f(425 + i, 400 - j);
			glEnd();

			glBegin(GL_LINE_LOOP);
			glColor3ub(20, 20, 20);
			glVertex2f(425 + i, 450 - j);
			glVertex2f(475 + i, 450 - j);
			glVertex2f(475 + i, 400 - j);
			glVertex2f(425 + i, 400 - j);
			glEnd();

			glLineWidth(4);
			glBegin(GL_LINES);
			glColor3ub(140, 75, 55);
			glVertex2f(425 + i, 400 - j);
			glVertex2f(475 + i, 400 - j);
			glEnd();
			glLineWidth(1);
		}
	}

	glBegin(GL_POLYGON);
	glColor3ub(59, 91, 132);
	glVertex2f(425, 280);
	glVertex2f(475, 280);
	glVertex2f(475, 200);
	glColor3ub(97, 131, 159);
	glVertex2f(425, 200);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3ub(20, 20, 20);
	glVertex2f(425, 280);
	glVertex2f(475, 280);
	glVertex2f(475, 200);
	glVertex2f(425, 200);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3ub(59, 91, 132);
	glVertex2f(525, 280);
	glVertex2f(575, 280);
	glVertex2f(575, 200);
	glColor3ub(97, 131, 159);
	glVertex2f(525, 200);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3ub(20, 20, 20);
	glVertex2f(525, 280);
	glVertex2f(575, 280);
	glVertex2f(575, 200);
	glVertex2f(525, 200);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3ub(59, 91, 132);
	glVertex2f(825, 280);
	glVertex2f(875, 280);
	glVertex2f(875, 200);
	glColor3ub(97, 131, 159);
	glVertex2f(825, 200);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3ub(20, 20, 20);
	glVertex2f(825, 280);
	glVertex2f(875, 280);
	glVertex2f(875, 200);
	glVertex2f(825, 200);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3ub(59, 91, 132);
	glVertex2f(925, 280);
	glVertex2f(975, 280);
	glVertex2f(975, 200);
	glColor3ub(97, 131, 159);
	glVertex2f(925, 200);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3ub(20, 20, 20);
	glVertex2f(925, 280);
	glVertex2f(975, 280);
	glVertex2f(975, 200);
	glVertex2f(925, 200);
	glEnd();
}

void HS_drawTuition() {
	// Tuition Building
	glBegin(GL_POLYGON);
	glColor3ub(151, 188, 207);
	glVertex2f(25, 175);
	glVertex2f(300, 175);
	glColor3ub(144, 180, 200);
	glVertex2f(300, 350);
	glVertex2f(25, 350);
	glEnd();

	// Tuition Building Border
	glBegin(GL_LINE_LOOP);
	glColor3ub(20, 20, 20);
	glVertex2f(25, 175);
	glVertex2f(300, 175);
	glVertex2f(300, 350);
	glVertex2f(25, 350);
	glEnd();

	// Tuition Board
	glBegin(GL_POLYGON);
	glColor3ub(255, 218, 154);
	glVertex2f(50, 375);
	glVertex2f(180, 375);
	glVertex2f(180, 325);
	glVertex2f(50, 325);
	glEnd();

	// Tuition Board Border
	glBegin(GL_LINE_LOOP);
	glColor3ub(20, 20, 20);
	glVertex2f(50, 375);
	glVertex2f(180, 375);
	glVertex2f(180, 325);
	glVertex2f(50, 325);
	glEnd();

	print("TUITION", 0, 0, 0, 1, 58, 342, .15, .15, 1.3);

	// Tuition Door
	glBegin(GL_POLYGON);
	glColor3ub(70, 39, 21);
	glVertex2f(55, 175);
	glVertex2f(55, 280);
	glVertex2f(115, 280);
	glVertex2f(115, 175);
	glEnd();

	// Tuition Door Border
	glBegin(GL_LINE_LOOP);
	glColor3ub(20, 20, 20);
	glVertex2f(55, 175);
	glVertex2f(55, 280);
	glVertex2f(115, 280);
	glVertex2f(115, 175);
	glEnd();

	// Tuition Rectangular Window
	glBegin(GL_POLYGON);
	glColor3ub(59, 91, 132);
	glVertex2f(155, 280);
	glVertex2f(155, 240);
	glVertex2f(270, 240);
	glColor3ub(97, 131, 159);
	glVertex2f(270, 280);
	glEnd();

	// Tuition Rectangular Window Border
	glBegin(GL_LINE_LOOP);
	glColor3ub(20, 20, 20);
	glVertex2f(155, 280);
	glVertex2f(155, 240);
	glVertex2f(270, 240);
	glVertex2f(270, 280);
	glEnd();

	// Tuition Rectangular Window Panes
	glBegin(GL_LINES);
	glColor3ub(20, 20, 20);
	glVertex2f(213, 280);
	glVertex2f(213, 240);
	glVertex2f(155, 260);
	glVertex2f(270, 260);
	glEnd();
}

void HS_drawLights() {
	// Left Light Pole
	glLineWidth(4);
	glBegin(GL_LINES);
	glColor3ub(200, 200, 200);
	glVertex2f(350, 175);
	glVertex2f(350, 300);
	glEnd();
	glLineWidth(1);

	// Right Light Pole
	glLineWidth(4);
	glBegin(GL_LINES);
	glColor3ub(200, 200, 200);
	glVertex2f(1050, 175);
	glVertex2f(1050, 300);
	glEnd();
	glLineWidth(1);

	// Left Bulb
	drawCircle(350,300,
		window_top_r, window_top_g, window_top_b,
		1, 1,
  	15);

	// Right Bulb
	drawCircle(1050,300,
		window_top_r, window_top_g, window_top_b,
		1, 1,
  	15);
}

void HS_drawStars() {
	if (stars_are_made == false) {
		for (int i = 0, j = 0; i < 40; i++) {
			stars_array[i][j] = 0 + (rand() % (int)(1400 - 0 + 1));
			stars_array[i][j+1] = 530 + (rand() % (int)(800 - 530 + 1));
			no_of_stars++;
		}
		stars_are_made = true;
	} else {
		glPointSize(2);
		glBegin(GL_POINTS);
		glColor4ub(255, 255, 255, star_alpha);
		for (int i = 0, j = 0; i < no_of_stars; i++)
			glVertex2f(stars_array[i][j], stars_array[i][j+1]);
		glEnd();
		glPointSize(1);
	}
}

void drawSchoolBoy(GLfloat tx, GLfloat ty,
	GLfloat sx, GLfloat sy,
	GLfloat shirt_r, GLfloat shirt_g, GLfloat shirt_b) {
	glPushMatrix();

	glScalef(sx, sy, 0);
	glTranslatef(tx, ty, 0);

	// Shoes
	drawSemiCircle(1160, 120,
		1, 1,
		255, 255, 255,
		10,
		-90, 91);

	// Shoe Extension
	glBegin(GL_POLYGON);
	glColor3ub(255, 255, 255);
	glVertex2f(1160, 120);
	glVertex2f(1170, 120);
	glVertex2f(1170, 135);
	glVertex2f(1160, 135);
	glEnd();

	// Pants
	glBegin(GL_POLYGON);
	glColor3ub(0, 0, 200);
	glVertex2f(1158, 135);
	glVertex2f(1172, 135);
	glVertex2f(1170, 190);
	glVertex2f(1160, 190);
	glEnd();

	// Shirt
	glBegin(GL_POLYGON);
	glColor3ub(shirt_r, shirt_g, shirt_b);
	glVertex2f(1155, 190);
	glVertex2f(1170, 190);
	glVertex2f(1170, 260);
	glVertex2f(1160, 260);
	glEnd();

	// Bag
	glBegin(GL_POLYGON);
	glColor3ub(156, 86, 47);
	glVertex2f(1170, 250);
	glVertex2f(1180, 245);
	glVertex2f(1185, 200);
	glVertex2f(1170, 195);
	glEnd();

	// Head
	drawCircle(1164, 273,
		232, 190, 123,
		1, 1.4,
		12);

	// Hair
	drawSemiCircle(1167, 277,
		1, 1,
		0, 0, 0,
	  14,
		-80, 180);

	// Nose
	glBegin(GL_TRIANGLES);
	glColor3ub(232, 190, 123);
	glVertex2f(1155, 270);
	glVertex2f(1152, 260);
	glVertex2f(1157, 262);
	glEnd();

	// Eye
	glPointSize(2);
	glBegin(GL_POINTS);
	glColor3ub(0, 0, 0);
	glVertex2f(1156, 270);
	glEnd();
	glPointSize(1);

	// Lips
	glPointSize(1);
	glBegin(GL_POINTS);
	glColor3ub(0, 0, 0);
	glVertex2f(1158, 256);
	glVertex2f(1159, 257);
	glVertex2f(1160, 258);
	glEnd();
	glPointSize(1);

	glPopMatrix();
}

void highSchool() {
	// Background
	glClearColor(0.05, 0.05, 0.05, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	HS_drawBackground();
	HS_drawSun();
	HS_drawRoad();

	HS_drawHome();
	HS_drawSchool();
	HS_drawTuition();

	HS_drawLights();
	HS_drawStars();

	drawSchoolBoy(schoolboy_x, schoolboy_y,
		.5, .5,
		255, 0, 0);

	print("Home -> School -> Tuition -> Home -> School -> Tuition -> Home -> School -> Tuition",
		1, 1, 1, hs_subtitle_1_appear, 80, 30, .14, .14, 1);
}
void renderScene() {
	// Switch to know which scene is playing
	
	
	
		highSchool();
	
	
	

	// glFush sends it to the Display Buffer
	glFlush();
}
// Function to Handle Mouse Clicks
void mouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		std::cout << x << "\t" << 800 - y << "\n";
}

// Animator Updation Function
void update(int) {
	// Introduction
	if (SCENE_ID == 0) {
		if (title_fade < 1)
			title_fade += .003;
		else
			intro_next_text_appear = 1;
	}

	// Summary
	if (SCENE_ID == 1) {
		if (summary_para1_fade < 1)
			summary_para1_fade += .02;
		else
			if (summary_para2_fade < 1)
				summary_para2_fade += .02;
			else
				if (summary_para3_fade < 1)
					summary_para3_fade += .02;
				else
					if (summary_para4_fade < 1)
						summary_para4_fade += .02;
					else
						summary_next_text_appear = 1;
	}

	// Kindergarten Title Screen
	if (SCENE_ID == 2) {
		if (trans_x_chap1 < 100)
			trans_x_chap1 += 1;

		if (trans_x_title1 < 100)
			trans_x_title1 += 1;

		if (kg_chap_fade < 1)
			kg_chap_fade += .025;

		if (kg_title_fade < 1)
			kg_title_fade += .025;
	}

	// Kindergarten
	if (SCENE_ID == 3) {
		if (kg_a_fade < 1)
			kg_a_fade += .025;
			else
				if (kg_apple_fade < 1)
					kg_apple_fade += .025;
				else
					if (kg_b_fade < 1)
						kg_b_fade += .025;
					else
						if (kg_ball_fade < 1)
							kg_ball_fade += .025;
						else
							if (kg_soon_fade < 1)
								kg_soon_fade += .025;
							else
								kg_subtitle_1_appear = 1;

	}

	// Primary School Title Screen
	if (SCENE_ID == 4) {
		if (trans_x_chap2 < 100)
			trans_x_chap2 += 1;

		if (trans_y_title2 < 100)
			trans_y_title2 += 1;

		if (ps_chap_fade < 1)
			ps_chap_fade += .01;

		if (ps_title_fade < 1)
			ps_title_fade += .01;
	}

	// Primary School
	if (SCENE_ID == 5) {
		if (trans_x_kid1 < 40)
			trans_x_kid1 += .4;

		if (trans_x_kid2 < wp_tb_big)
			trans_x_kid2 += .3;

		if (trans_x_kid3 < 40)
			trans_x_kid3 += .3;

		if (trans_x_ball < 50)
			trans_x_ball += .4;

		if (ps_subtitle_1_appear < 1)
			ps_subtitle_1_appear += .005;
		else {
			if (ps_subtitle_2_appear < 1) {
				trans_subtitle_1_done = 200;
				ps_subtitle_2_appear += .005;
			}
		}
	}

	// High School Title Screen
	if (SCENE_ID == 6) {
		if (trans_x_chap3 < 100)
			trans_x_chap3 += 1;

		if (trans_x_title3 < 100)
			trans_x_title3 += 1;

		if (hs_chap_fade < 1)
			hs_chap_fade += .01;

		if (hs_title_fade < 1)
			hs_title_fade += .01;
	}

	// High School
	if (SCENE_ID == 7) {
		if (!chapter_1_done) {
			delay(1);
			schoolboy_y -= 5000;
			chapter_1_done = true;
		}

		if (chapter_1_done && !chapter_2_done) {
			if (schoolboy_x >= 300) {
				schoolboy_x -= 5;
			} else {
				schoolboy_y += 5000;
				chapter_2_done = true;
			}
		}

		if (chapter_1_done && chapter_2_done && !chapter_3_done && !sun_has_set) {
			if (sun_r <= 255)
				sun_r += .25;
			if (sun_g <= 255)
				sun_g += .25;
			if (sun_b <= 255)
				sun_b += .25;

			if (star_alpha <= 200)
				star_alpha += 1;

			if (grass_r <= 255)
				grass_r += .25;
			if (grass_g <= 220)
				grass_g += .25;
			if (grass_b >= 65)
				grass_b += .25;

			if (window_top_r <= 255)
				window_top_r += .25;
			if (window_top_g <= 220)
				window_top_g += .25;
			if (window_top_b >= 65)
				window_top_b -= .25;

			if (window_bottom_r <= 255)
				window_bottom_r += .25;
			if (window_bottom_g <= 220)
				window_bottom_g += .25;
			if (window_bottom_b >= 65)
				window_bottom_b -= .25;

			if (sky_r <= 0)
				sky_r += .25;
			if (sky_g >= 0)
				sky_g -= .25;
			if (sky_b >= 0)
				sky_b -= .25;

			if (sun_move_left < 1100)
				sun_move_left += 1.5;
			else
				sun_has_set = true;

			if (sun_move_left > 500)
				sun_moved_half = true;
		}

		if (sun_moved_half && !chapter_4_done) {
			if (schoolboy_y != 220)
				schoolboy_y -= 5000;

			if (schoolboy_x >= -1000)
				schoolboy_x -= 5;
			else {
				schoolboy_y += 5000;
				chapter_4_done = true;
			}
		}

		if (chapter_4_done) {
			if (schoolboy_y != 5220 && !tuition_done) {
				schoolboy_y += 5000;
				tuition_done = true;
			}
		}

		if (sun_has_set) {
			if (schoolboy_y != 220)
				schoolboy_y -= 5000;
			chapter_5_done= true;
		}

		if (chapter_5_done) {
			if (schoolboy_x <= 1150)
				schoolboy_x += 5;
			else {
				if (schoolboy_y != 5220)
					schoolboy_y += 5000;
			}

			if (hs_subtitle_1_appear < 1) {
				hs_subtitle_1_appear += .005;
			}
		}
	}

	// Pre University College Title Screen
	if (SCENE_ID == 8) {
		if (trans_x_chap4 < 100)
			trans_x_chap4 += 1;

		if (trans_x_title4 < 100)
			trans_x_title4 += 1;

		if (puc_chap_fade < 1)
			puc_chap_fade += .01;

		if (puc_title_fade < 1)
			puc_title_fade += .01;
	}

	// Pre University College
	if (SCENE_ID == 9) {
		if (puc_subtitle_1_fade < 1)
			puc_subtitle_1_fade += 0.005;

		if (!puc_begin_anim) {
			delay(1);
			puc_begin_anim = true;
		}

		if (!puc_end_anim) {
			if (trans_x_sb1 >= -930)
				trans_x_sb1 -= 1;
			if (trans_y_sb1 <= -20)
				trans_y_sb1 += .1;

			if (trans_x_sb2 >= -1930)
				trans_x_sb2 -= 1;
			if (trans_y_sb2 <= -20)
				trans_y_sb2 += .1;

			if (trans_y_sb1 >= -20 && trans_y_sb2 >= -20)
				puc_end_anim = true;
		}

		if (puc_end_anim) {
			delay(1);

			if (trans_y_sb1 <= 5000)
				trans_y_sb1 += 5000;

			if (trans_y_sb2 <= 5000)
				trans_y_sb2 += 5000;
		}
	}

	// Engineering College Title Screen
	if (SCENE_ID == 10) {
		if (trans_x_chap5 < 100)
			trans_x_chap5 += 1;

		if (trans_x_title5 < 100)
			trans_x_title5 += 1;

		if (eng_chap_fade < 1)
			eng_chap_fade += .025;

		if (eng_title_fade < 1)
			eng_title_fade += .025;
	}

	if (SCENE_ID == 11) {
		if (trans_x_ec_sb1 >= -180)
			trans_x_ec_sb1 -= 4;
		if (trans_x_ec_sb2 >= -2050)
			trans_x_ec_sb2 -= 4;

		if (trans_x_ec_sb1 <= -180 && trans_x_ec_sb2 <= -2050 && !ec_graduation_done) {
			trans_y_ec_sb += 1000;
			ec_graduation_done = true;
		}

		if (ec_graduation_done) {
			if (trans_y_ec_gb != 40) {
				trans_y_ec_gb -= 1000;
			}
			if (trans_x_ec_gb >= -1200)
				trans_x_ec_gb -= 2;
		}

		if (ec_subtitle_1_fade <= 1)
			ec_subtitle_1_fade += 0.005;
	}

	if (SCENE_ID == 12) {
		if (trans_x_chap6 < 100)
			trans_x_chap6 += 1;

		if (trans_x_title6 < 100)
			trans_x_title6 += 1;

		if (wp_chap_fade < 1)
			wp_chap_fade += .025;

		if (wp_title_fade < 1)
			wp_title_fade += .025;
	}

	if (SCENE_ID == 13) {
		if (server_on) {
			server_on = false;
		} else {
			server_on = true;
		}

		if (!wp_tb_s1_done) {
			if (wp_tb_s1 <= 10)
				wp_tb_s1 += .25;
			else
				wp_tb_s1_done = true;
		}

		if (light_a) {
			light_a = false;
			light_b = true;
			light_c = false;
			light_d = false;
		} else if (light_b) {
			light_a = false;
			light_b = false;
			light_c = true;
			light_d = false;
		} else if (light_c) {
			light_a = false;
			light_b = false;
			light_c = false;
			light_d = true;
		} else {
			light_a = true;
			light_b = false;
			light_c = false;
			light_d = false;
		}

		if (wp_tb_s1_done && !wp_tb_s2_done) {
			if (wp_tb_s2 <= 12)
				wp_tb_s2 += .25;
			else
				wp_tb_s2_done = true;
		}

		if (wp_tb_s1_done && wp_tb_s2_done && !wp_tb_s3_done) {
			if (wp_tb_s3 <= 14)
				wp_tb_s3 += .25;
			else
				wp_tb_s3_done = true;
		}

		if (wp_tb_s1_done && wp_tb_s2_done && wp_tb_s3_done && !wp_tb_big_done) {
			if (wp_tb_big <= 30)
				wp_tb_big += .5;
			else
				wp_tb_big_done = true;
		}

		if (wp_tb_s1_done && wp_tb_s2_done && wp_tb_s3_done && wp_tb_big_done) {
			if (wp_tb_text <= 1)
				wp_tb_text += .005;

			if (wp_subtitle_1_fade <= 1)
				wp_subtitle_1_fade += .005;
		}
	}

	// Epilogue
	if (SCENE_ID == 14) {
		if (epilogue_para1_fade < 1)
			epilogue_para1_fade += .02;
		else
			if (epilogue_para2_fade < 1)
				epilogue_para2_fade += .02;
			else
				if (epilogue_para3_fade < 1)
					epilogue_para3_fade += .02;
				else
					if (epilogue_para4_fade < 1)
						epilogue_para4_fade += .02;
					else
						if (epilogue_para5_fade < 1)
							epilogue_para5_fade += .02;
						else
							if (epilogue_para6_fade < 1)
								epilogue_para6_fade += .02;
	}

	// Morals
	if (SCENE_ID == 15) {
		if (moral_para1_fade < 1)
			moral_para1_fade += .02;
		else
			if (moral_para2_fade < 1)
				moral_para2_fade += .02;
			else
				if (moral_para3_fade < 1)
					moral_para3_fade += .02;
				else
					if (moral_para4_fade < 1)
						moral_para4_fade += .02;
	}

	// Recalls the Display Function
	glutPostRedisplay();

	// Creates a Timer of 25ms
	glutTimerFunc(25, update, 0);
}
void keyPress(unsigned char key, int x, int y) {
	switch (key) {
	// Go to Previous Scene
	case 'b':
	case 'B':
		if (SCENE_ID == 0)
			break;
		SCENE_ID--;
		break;
	// Go to Next Scene
	case 'n':
	case 'N':
		if (SCENE_ID == 15)
			break;
		SCENE_ID++;
		break;
	// Quit Story
	case 'q':
	case 'Q':
		exit(0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

// Function to Initialize Screen
void initializeScreen() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1400, 0, 800);
}

// Main Function
int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1400, 800);
	glutCreateWindow("The Epilogue of Education");

	// Enables Transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// Enable Smoothening
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	// Display Function
	glutDisplayFunc(renderScene);

	// Input Functions
	glutKeyboardFunc(keyPress);
	glutMouseFunc(mouseClick);

	initializeScreen();

	// Timer Function
	// every 25 milliseconds, update function is called
	glutTimerFunc(25, update, 0);

	glutMainLoop();

	return 0;
}
