/***
 assignment1.cpp
 Assignment-1: Cartoonify

 Name: Wong, Alex (Please write your name in Last Name, First Name format)

 Collaborators: Doe, John; Doe, Jane
 ** Note: although the assignment should be completed individually
 you may speak with classmates on high level algorithmic concepts. Please
 list their names in this section

 Project Summary: A short paragraph (3-4 sentences) describing the work you
 did for the project: e.g. did you use the Chaikin's or Bezier's algorithm?
 Did you take an iterative or recursive approach?
 ***/


#ifdef _WIN32
#include "GL/glew.h"
#include "GL/freeglut.h"
#elif __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <vector>
#include <iostream>
using namespace std;

class Vertex {
	GLfloat x, y;
public:
	Vertex(GLfloat, GLfloat);
	GLfloat get_y() { return y; };
	GLfloat get_x() { return x; };
};

Vertex::Vertex(GLfloat X, GLfloat Y) {
	x = X;
	y = Y;
}

void setup() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

vector<Vertex> generate_points(vector<Vertex> control_points) {
	vector<Vertex> P;
	vector<Vertex> M;
	vector<Vertex> Q;
	P = control_points;

	// TODO:
	// Generate points for a given Chaikin or Bezier curve iteration

	Q.push_back(P[0]);
	Q.push_back(P[P.size() - 1]);

	while (P.size() > 1) {

		

		for (int j = 0; j < P.size() - 1; j++) {
			Vertex m{0.5f * (P[j].get_x() + P[j+1].get_x()), 
				     0.5f * (P[j].get_y() + P[j+1].get_y()) };
			M.push_back(m);
			//Q.insert(Q.begin() + j + i + 1, m);
			cout << "Midpoint: " << m.get_x() << "," << m.get_y() << endl;
		}

		
		P = M;
		Q.insert(Q.begin() + Q.size()/2, P[0]);
		Q.insert(Q.end() - Q.size()/2, P[P.size() - 1]);
		//Q.insert(Q.end() - 1, M.begin(), M.end());
		M.clear();
		cout << "Ran Outer: i=" << " P.size() = "<< P.size() << endl;
	}


	//

	return Q;
}

void draw_curve(vector<Vertex> control_points, int n_iter) {

	vector<Vertex> current_points = control_points;
	for (int i = 0; i < n_iter; i++) {
		current_points = generate_points(current_points);
		for (auto& point : current_points) {
			cout << " (" << point.get_x() << "," << point.get_y() << ")";

		}
		cout << endl;
		cout << "Iterate" << endl;
	}

	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for (auto& v : current_points) {
		cout << v.get_x() << " " << v.get_y() << endl;
		glVertex2f(v.get_x(), v.get_y());
	}

	//glVertex2f(-0.25f,0);
	//glVertex2f(0.25,0);

	glEnd();

}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set our color to black (R, G, B)
	glColor3f(0.0f, 0.0f, 0.0f);

	

	//
	vector<Vertex> control_points{ Vertex{-0.5f, -0.5f}, Vertex{0.0f, 0.5},
								   Vertex{0.5f, -0.5f} };
	draw_curve(control_points, 6);
	// TODO:
	// Draw cartoon

	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(800, 600); // Set your own window size
	glutCreateWindow("Assignment 1");
	setup();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}

