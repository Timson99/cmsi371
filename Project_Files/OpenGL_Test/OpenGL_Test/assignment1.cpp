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
#include <GL/glew.h>
#include <GL/freeglut.h>
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

///*
vector<Vertex> generate_points(vector<Vertex> control_points) {
	if(control_points.size() <= 1) {
		return control_points;
	}
	vector<Vertex> midpoints;
	vector<Vertex> bezier_points;
	for (int j = 0; j < control_points.size() - 1; j++) {
			Vertex midpoint{0.5f * (control_points[j].get_x() + control_points[j+1].get_x()),
					 0.5f * (control_points[j].get_y() + control_points[j+1].get_y()) };
			midpoints.push_back(midpoint);
	}
	bezier_points.insert(bezier_points.begin(), { control_points[0], control_points[control_points.size() - 1]});
	vector<Vertex> recursion_points = generate_points(midpoints);
	bezier_points.insert(bezier_points.begin() + 1, recursion_points.begin(), recursion_points.end());
	return bezier_points;
}
//*/
/*
vector<Vertex> generate_points(vector<Vertex> control_points) {
	vector<Vertex> points{ control_points };
	vector<Vertex> midpoints;
	vector<Vertex> bezier_points;
	while (points.size() > 1) {
		bezier_points.insert(bezier_points.begin() + bezier_points.size()/2, {points[0], points[points.size() - 1]});

		for (int j = 0; j < points.size() - 1; j++) {
			Vertex midpoint{0.5f * (points[j].get_x() + points[j+1].get_x()),
				     0.5f * (points[j].get_y() + points[j+1].get_y()) };
			midpoints.push_back(midpoint);
			//Q.insert(Q.begin() + j + i + 1, m);
			//cout << "Midpoint: " << m.get_x() << "," << m.get_y() << endl;
		}
		points = midpoints;
		//Q.insert(Q.end() - 1, M.begin(), M.end());
		midpoints.clear();
		//cout << "Ran Outer: i=" << " P.size() = "<< P.size() << endl;
	}
	bezier_points.insert(bezier_points.begin() + bezier_points.size() / 2, points[0]);
	return bezier_points;
}
//*/
void draw_curve(vector<Vertex> control_points, int n_iter) {

	vector<Vertex> current_points = control_points;
	for (int i = 0; i < n_iter; i++) {
		current_points = generate_points(current_points);
		//for (auto& point : current_points) {
			//cout << " (" << point.get_x() << "," << point.get_y() << ")";

		//}
		//cout << endl;
		//cout << "Iterate" << endl;
	}

	//Invisible Bexier Points
	glPointSize(1.0f);
	glBegin(GL_POINTS);
	for (auto& v : current_points) {
		//cout << v.get_x() << " " << v.get_y() << endl;
		glVertex2f(v.get_x(), v.get_y());
	}
	glEnd();

	/*

	//Original Points
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for (auto& v : control_points) {
		glVertex2f(v.get_x(), v.get_y());
	}
	glEnd();

	//Lines Between Original Points
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < control_points.size() - 1; i++) {
		glVertex2f(control_points[i].get_x(), control_points[i].get_y());
		glVertex2f(control_points[i + 1].get_x(), control_points[i + 1].get_y());
	}
	glEnd();

	//*/

	//Lines Between Bezier Points
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < current_points.size() - 1; i++) {
		glVertex2f(current_points[i].get_x(), current_points[i].get_y());
		glVertex2f(current_points[i+1].get_x(), current_points[i+1].get_y());
	}
	glEnd();

}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set our color to black (R, G, B)
	glColor3f(0.0f, 0.0f, 0.0f);

	int n_iterations = 5;

	vector<vector<Vertex>> curve_list = { 
		//Right Shoulder
		{ Vertex{-0.72f, -1.0f},  Vertex{-0.6f, -0.93f}, Vertex{-0.78f, -0.5f}, 
		  Vertex{-0.28f, -0.53f}, Vertex{-0.23f, -0.39f}},
		//Left Shoulder
		{ Vertex{0.99f, -1.0f}, Vertex{1.0f, -0.48f}, Vertex{0.48f, -0.56f}, 
		  Vertex{0.32f, -0.23f} },
		//Beard
		{ Vertex{-0.21f, -0.06f}, Vertex{-0.33f, -1.1f}, Vertex{0.05f, -1.1f},
		  Vertex{0.37f, -0.07f} },
		//Left Side
		{ Vertex{0.37f, -0.07f}, Vertex{0.39f, 0.12f}, Vertex{0.33f, 0.18f}, Vertex{0.41f, 0.22f}, Vertex{0.41f, 0.3f}, },
		//Left Head
		{ Vertex{0.41f, 0.3f}, Vertex{0.49f, 0.99f}, Vertex{0.0f, 0.931f}, },
		//Right Head
		{ Vertex{0.0f, 0.931f}, Vertex{-0.41f, 0.93f}, Vertex{-0.21f, 0.355f}, Vertex{-0.21f, -0.06f}, },
		//Coat Left
		{ Vertex{0.32f, -0.23f}, Vertex{0.19f, -0.61f}, Vertex{-0.09f, -1.0f}, },
		//Coat Right
		{ Vertex{-0.225f, -0.452f}, Vertex{-0.368f, -0.682f}, Vertex{-0.411f, -1.0f}, }
	};

	for (auto& control_points : curve_list) {
		draw_curve(control_points, n_iterations);
	}
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

