// OpenGL_Billiards.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <GL/glut.h>
#include <GL/GL.h>
#include <GL/glu.h>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

#define PI 3.1416
using namespace std;
GLfloat SuDelta, JukDelta; //속도
GLfloat SuX, SuY; //수구의 좌표
GLfloat JukX, JukY; //적구의 좌표
GLfloat SuXdir, SuYdir; //수구의 진행 방향
GLfloat JukXdir, JukYdir; //적구의 진행 방향
GLint SuCushion, JukCushion = 0;
GLfloat RADIUS = 0.1;
bool IS_HEAT = false; //수구가 적구를 쳤는지 여부 저장

void MoveAndDraw();
double getRadian(int n) { 
	return n * (PI / 180);
}

bool isBump() { //두 공이 부딪혔는지 판단하는 함수
	float distance = sqrt(pow(SuX - JukX, 2) + pow(SuY - JukY, 2));
	if (distance <= 2 * RADIUS) {
		return true;
	}
	else { return false; }
}

void isHeat() {//처음으로 수구가 적구를 치면 충돌 시간이 지난 후에 첫 충돌 완료 선언
	while (isBump()) { MoveAndDraw(); }
	IS_HEAT = true;
}

void DrawSuCircle(float cx, float cy, float r)
{
	float x1, y1, x2, y2;
	float angle;
	double radius = r;

	x1 = cx, y1 = cy;
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 0.01f; angle <= 2 * 3.14f + 0.2; angle += 0.1) {
		x2 = x1 - sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();
}

void DrawJukCircle(float cx, float cy, float r) {
	float x1, y1, x2, y2;
	float angle;
	double radius = r;

	x1 = cx, y1 = cy;
	glColor3f(8.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1); //2차원에서 점 두개 찍음

	for (angle = 0.01f; angle <= 2 * 3.14f + 0.2; angle += 0.1) {
		x2 = x1 - sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();
}

//수구의 다음 진행방향 구하는 함수
void MoveSuCircle() {
	if (SuCushion >= 2) { //쿠션2회이상 충돌시
		if (SuDelta > 0.0) { //속도 점점 줄어들다
			SuDelta -= 0.0008;
		}
		else { return; } //멈춤
	}
	if (IS_HEAT && isBump()) { return; } //수구와 적구 둘다 움직이다 부딫히면 멈춤

	SuX += SuXdir * SuDelta; //수구의 다음 좌표 계산
	SuY += SuYdir * SuDelta;

	if ((SuXdir < 0 && SuX < -0.9) || (SuXdir > 0 && SuX > 0.9)) {//당구대의 좌우에 쿠션
		SuXdir = -SuXdir; //입사각=반사각 방향전환
		SuCushion++;
	}
	if ((SuYdir < 0 && SuY < -1.9) || (SuYdir > 0 && SuY > 1.9)) { //당구대의 상하에 쿠션
		SuYdir = -SuYdir;//입사각=반사각 방향전환
		SuCushion++;
	}
}
//적구의 다음 진행방향 함수
void MoveJukCircle() {
	if (JukCushion >= 3) { //쿠션 3회이상 충돌시
		if (JukDelta > 0.0) { //속도 줄어들다
			JukDelta -= 0.0008;
		}
		else { return; } //멈춤
	}
	if(IS_HEAT && isBump()) { return; } //수구와 적구 둘다 움직이다 부딫히면 멈춤

	JukX += JukXdir * JukDelta; //적구의 다음 좌표 계산
	JukY += JukYdir * JukDelta;

	if ((JukXdir < 0 && JukX < -0.9) || (JukXdir > 0 && JukX > 0.9)) {//당구대의 좌우에 쿠션
		JukXdir = -JukXdir;//입사각=반사각 방향전환
		JukCushion++;
	}
	if ((JukYdir < 0 && JukY < -1.9) || (JukYdir > 0 && JukY > 1.9)){//당구대의 상하에 쿠션
		JukYdir = -JukYdir;//입사각=반사각 방향전환
		JukCushion++;
	}
}

void MoveAndDraw() {
	MoveSuCircle();
	MoveJukCircle();
	glClear(GL_COLOR_BUFFER_BIT);
	DrawSuCircle(SuX, SuY, RADIUS);
	DrawJukCircle(JukX, JukY, RADIUS);
	glutSwapBuffers();
}

void MyDisplay() {
	if (!IS_HEAT && isBump()) { //첫 충돌일때
		JukXdir = cos(getRadian(30)) *SuXdir - sin(getRadian(30))*SuYdir; //당구 분리각 90도의 법칙 만족하게 방향 전환
		JukYdir = sin(getRadian(30)) * SuXdir + cos(getRadian(30)) * SuYdir;
		SuXdir = cos(getRadian(-90)) * JukXdir - sin(getRadian(-90)) * JukYdir;
		SuYdir = sin(getRadian(-90)) * JukXdir + cos(getRadian(-90)) * JukYdir;
		isHeat();
	}
	MoveAndDraw();
}

void MyTimer(int Value) {
	glutPostRedisplay();
	glutTimerFunc(40, MyTimer, 1);
}

int main(int argc, char** argv) {

	SuDelta = 0.05;
	JukDelta = 0.05;
	
	//시작위치 랜덤 생성
	srand((unsigned int)time(NULL));
	SuX = 0.1 * (rand() % 9 + 1);
	SuY = 0.1 * (rand() % 19 + 1);
	JukX = 0.1 * (rand() % 9 + 1);
	JukY = -0.1 * (rand() % 19 + 1);

	SuXdir = JukX-SuX; //수구는 적구를 향해 진행
	SuYdir = JukY-SuY;
	JukXdir, JukYdir = 0.0; //처음 적구는 정지상태
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(300, 600);
	glutInitWindowPosition(700,0);
	glutCreateWindow("OpenGL Drawing Example");
	glClearColor(0.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -2.0, 2.0, 1.0, -1.0);
	glutDisplayFunc(MyDisplay);
	glutTimerFunc(40, MyTimer, 1);
	glutMainLoop();
	return 0;
}