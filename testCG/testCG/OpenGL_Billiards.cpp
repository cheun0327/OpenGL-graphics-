// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.

#include <GL/glut.h>
#include <GL/GL.h>
#include <GL/glu.h>
#include <math.h>
#include <cstdlib>
#include <ctime>

GLfloat SuDelta, JukDelta; //속도
GLfloat SuX, SuY; //수구의 좌표
GLfloat JukX, JukY; //적구의 좌표
GLfloat SuXdir, SuYdir; //수구의 진행 방향
GLfloat JukXdir, JukYdir; //적구의 진행 방향
GLint SuCushion, JukCushion = 0;
GLfloat RADIUS = 0.1;

void DrawSuCircle(float cx, float cy, float r)
{
	float x1, y1, x2, y2;
	float angle;
	double radius = r;
	
	x1 = cx, y1 = cy;
	glColor3f(1.0, 1.0, 1.0);
	
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 0.01f; angle <= 2*3.14f + 0.2; angle += 0.1) {
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
	glVertex2f(x1, y1); 

	for (angle = 0.01f; angle <= 2 * 3.14f + 0.2; angle += 0.1) {
		x2 = x1 - sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();
}

bool isBump() { //두 공이 부딪혔는지 판단하는 함수
	float distance = sqrt(pow(SuX - JukX, 2) + pow(SuY - JukY, 2));
	if (distance <= 2 * RADIUS) { return true; }
	else { return false; }
}

//수구의 움직임 함수
void MoveSuCircle() {
	if (SuCushion >= 2) { //쿠션2회이상 충돌시
		if (SuDelta > 0.0) { //속도 점점 줄어들다
			SuDelta -= 0.0008;
		}
		else { return;  } //멈춤
	}

	if (isBump()) { return; } //수구와 적구가 부딪히면 멈춤

	SuX += SuXdir * SuDelta;
	SuY += SuYdir * SuDelta;

	if ((SuXdir < 0 && SuX < -0.9) || (SuXdir > 0 && SuX > 0.9)) {
		SuXdir = -SuXdir;
		SuCushion++;
	}
	if ((SuYdir < 0 && SuY < -0.9) || (SuYdir > 0 && SuY > 0.9)) {
		SuYdir = -SuYdir;
		SuCushion++;
	}
}
//적구의 움직임 함수
void MoveJukCircle() {
	if (JukCushion >= 3) { //쿠션 3회이상 충돌시
		if (JukDelta > 0.0) { //속도 줄어들다
			JukDelta -= 0.0008;
		}
		else { return; } //멈춤
	}

	if (isBump()) { return; } //수구와 적구가 부딪히면 멈춤

	JukX += JukXdir * JukDelta;
	JukY += JukYdir * JukDelta;

	if ((JukXdir < 0 && JukX < -0.9) || (JukXdir > 0 && JukX > 0.9)) {
		JukXdir = -JukXdir;
		JukCushion++;
	}
	if ((JukYdir < 0 && JukY < -0.9) || (JukYdir > 0 && JukY > 0.9)) {
		JukYdir = -JukYdir;
		JukCushion++;
	}
}

void MyDisplay() {
	MoveSuCircle();
	MoveJukCircle();
	glClear(GL_COLOR_BUFFER_BIT);
	DrawSuCircle(SuX, SuY, RADIUS);
	DrawJukCircle(JukX, JukY, RADIUS);

	glutSwapBuffers();
}

void MyTimer(int Value) {
	glutPostRedisplay();
	glutTimerFunc(40, MyTimer, 1);
}

int main(int argc, char** argv) {
	
	SuDelta = 0.03;
	JukDelta = 0.03;
	
	//시작위치 랜덤 생성
	srand((unsigned int)time(NULL));
	SuX = 0.1 * (rand() % 9 + 1);
	SuY = 0.1 * (rand() % 9 + 1);
	JukX = 0.1 * (rand() % 9 + 1);
	JukY = -0.1 * (rand() % 9 + 1);

	SuXdir = 1.0;
	SuYdir = 1.0;
	JukXdir = 1.0;
	JukYdir = 1.0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(300, 600);
	glutInitWindowPosition(200, 200);
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
