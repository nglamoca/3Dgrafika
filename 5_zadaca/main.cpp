#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <limits>
#include "glad/glad.h"
#include "GLFW/include/glfw3.h"
#include "glm/glm.hpp"

#include "CircleRenderer.h"
#include "LineRenderer.h"

using namespace std;
using namespace glm;

int Factorial(int x) {
	if (x > 1) return x * Factorial(x - 1);
	else return 1;
}

int BinomialCoefficiant(int n, int k) {
	return Factorial(n) / (Factorial(k) * Factorial(n - k));
}

struct BezierCurve {
  vec2 P0, P1, P2, P3;
  vector<vec2> points;
  vec2* press = nullptr;

  BezierCurve(vec2 P0, vec2 P1, vec2 P2, vec2 P3): P0(P0), P1(P1), P2(P2), P3(P3) {}


  vector<vec2>& getCurve() {
    return points;
  }
  void curve_create(float point) {
    for(float t = 0; t <= 1; t += point){
      points.push_back((1-t)*(1-t)*(1-t)*P0 + 3*(1-t)*(1-t)*t*P1 + 3*(1-t)*t*t*P2 + t*t*t*P3);}}

  void ClearPoints() {
    points.clear();
  }
};

BezierCurve b(vec2(20,10), vec2(100, 200), vec2(500, 400), vec2(600, 600));
vec2* press = nullptr;

//window-that recieved the event,button -pressed/released, action -GLFWPRESS/RELEASE,
//mods-Modifier keys held down
void mouse(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if(GLFW_PRESS == action){
      double x;
      double y;
      glfwGetCursorPos(window, &x, &y);
      if(sqrt((x - b.P0.x)*(x - b.P0.x) + (y - b.P0.y)*(y - b.P0.y)) < 4){
            press = &b.P0;}
      else if(sqrt((x - b.P1.x)*(x - b.P1.x) + (y - b.P1.y)*(y - b.P1.y)) < 4){
            press = &b.P1;}
      else if(sqrt((x - b.P2.x)*(x - b.P2.x) + (y - b.P2.y)*(y - b.P2.y)) < 4){
            press = &b.P2;}
      else if(sqrt((x - b.P3.x)*(x - b.P3.x) + (y - b.P3.y)*(y - b.P3.y)) < 4){
            press = &b.P3;}
    }
    else if(GLFW_RELEASE == action) {
      press = nullptr;
    }
  }
}


void cursor(GLFWwindow* window, double x, double y) {
  if(press != nullptr){
    press->x = x;
    press->y = y;
    b.ClearPoints();
    b.curve_create(0.02);
  }
}

// minGW: g++ main.cpp glad\glad.c glfw\lib-mingw-w64\libglfw3.a LineRenderer.cpp CircleRenderer.cpp -l gdi32 -l opengl32 -std=c++17
int main () {
	GLFWwindow* window;

	if (!glfwInit()) cout << "Error : could not initilize GLFW";

	int width = 1000;
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow(width, width * 9/16, "Hello World", NULL, NULL);
	if (!window) {
		cout << "Error : could not create window";
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) cout << "Error : could not initilize Glad";

	glfwSwapInterval(1);

	InitCircleRendering(32);
	InitLineRendering();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   b.curve_create(0.02);

  glfwSetMouseButtonCallback(window, mouse);
  glfwSetCursorPosCallback(window, cursor);

  while (!glfwWindowShouldClose(window)) {
	   glClear(GL_COLOR_BUFFER_BIT);

    RenderCircle(b.P0, 4);
    RenderCircle(b.P1, 4);
    RenderCircle(b.P2, 4);
    RenderCircle(b.P3, 4);

    RenderLine(b.getCurve());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

	return 0;
}
