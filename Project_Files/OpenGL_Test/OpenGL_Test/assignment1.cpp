/***
 assignment1.cpp
 Assignment-1: Cartoonify

 Name: Herrmann, Tim

 Collaborators: Luis Garcia

 Project Summary: In this project, I used Bezier's Algorithm to sketch out
 a headshot Rutherford B. Hayes, the coolest man to ever be president (I mean,
 just look at that beard!). I initially programmed the algorithm using the iterative approach, 
 but I later refactored the code into a more concise recursive definition. The implementations
 of both approaches appear below, with the recursive definition as the default.
 Luis Garcia, a former student of this class, helped me conceptually understand 
 the algorithm better while I was working through the implementation.

 This code defaults to 5 iterations of bezier's algorithm.
 This code does not run using gcc. Please use g++ instead.
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


/***********************
 * RECURSIVE DEFINITION
 ***********************/

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

/*************************
 * ITERATIVE DEFINITION
 *************************/
/*
vector<Vertex> generate_points(vector<Vertex> control_points) {
	vector<Vertex> points{ control_points };
	vector<Vertex> midpoints;
	vector<Vertex> bezier_pts;

	while (points.size() > 1) {
		bezier_pts.insert(bezier_pts.begin() + bezier_pts.size()/2, {points.front(), points.back()});
		for (int j = 0; j < points.size() - 1; j++) {
			Vertex midpoint{  0.5f * (points[j].get_x() + points[j+1].get_x()), 
				              0.5f * (points[j].get_y() + points[j+1].get_y()) };
			midpoints.push_back(midpoint);
		}
		points = midpoints;
		midpoints.clear();
	}
	bezier_pts.insert(bezier_pts.begin() + bezier_pts.size() / 2, points.front());
	return bezier_pts;
}
//*/
void draw_curve(vector<Vertex> control_points, int n_iter) {

	vector<Vertex> current_points = control_points;
	for (int i = 0; i < n_iter; i++) {
		current_points = generate_points(current_points);
	}


	//Original Points
	/*
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for (auto& v : control_points) {
		glVertex2f(v.get_x(), v.get_y());
	}
	glEnd();
	//*/

	/*
	//Lines Between Original Points
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < control_points.size() - 1; i++) {
		glVertex2f(control_points[i].get_x(), control_points[i].get_y());
		glVertex2f(control_points[i + 1].get_x(), control_points[i + 1].get_y());
	}
	glEnd();
	//*/
	/*
	//Bezier Points
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for (auto& v : current_points) {
		glVertex2f(v.get_x(), v.get_y());
	}
	glEnd();
	//*/

	//Lines Between Bezier Points

	int count = 1;

	glLineWidth(1.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < current_points.size() - 1; i++) {
		glVertex2f(current_points[i].get_x(), current_points[i].get_y());
		glVertex2f(current_points[i+1].get_x(), current_points[i+1].get_y());
		count++;
	}
	cout << count << endl;

	glEnd();

}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set our color to black (R, G, B)
	glColor3f(0.0f, 0.0f, 0.0f);

	int n_iterations = 6;

	vector<vector<Vertex>> curve_list = { 
		//Right Shoulder
		{ Vertex{-0.72f, -1.0f},  Vertex{-0.6f, -0.93f}, Vertex{-0.78f, -0.5f}, 
		  Vertex{-0.28f, -0.53f}, Vertex{-0.225f, -0.39f}},
		//Left Shoulder
		{ Vertex{0.99f, -1.0f}, Vertex{1.0f, -0.48f}, Vertex{0.48f, -0.56f}, 
		  Vertex{0.315f, -0.23f} },
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
		{ Vertex{-0.22f, -0.452f}, Vertex{-0.368f, -0.682f}, Vertex{-0.411f, -1.0f}, },
		//Lapel Left Pt1
		{ Vertex{0.362f, -1.0f}, Vertex{0.442f, -0.839f}, },
		//Lapel Left Pt2
		{ Vertex{0.442f, -0.839f}, Vertex{0.231f, -0.776f}, },
		//Lapel Left Pt3
		{ Vertex{0.231f, -0.776f}, Vertex{0.308f, -0.74f}, Vertex{0.379f, -0.761f}, },
		//Lapel Left Pt4
		{ Vertex{0.379f, -0.761f}, Vertex{0.355f, -0.547f}, Vertex{0.41f, -0.348f}, },
		//Lapel Right Pt1
		{ Vertex{-0.6, -1.0f}, Vertex{-0.547f, -0.732f}, },
		//Lapel Right Pt2
		{ Vertex{-0.547f, -0.732f}, Vertex{-0.45f, -0.716f}, },
		//Lapel Right Pt3
		{ Vertex{-0.45f, -0.716f}, Vertex{-0.265f, -0.43f}, },
		//Vest Left
		{ Vertex{-0.194f, -1.0f}, Vertex{-0.104f, -0.83f}, Vertex{0.135f, -0.63f}, },
		//Vest Right
		{ Vertex{-0.323f, -1.0f}, Vertex{-0.33f, -0.824f}, Vertex{-0.243f, -0.493f}, },
		//Beard Line
		{ Vertex{-0.205f, 0.177f}, Vertex{-0.197f, -0.933f}, Vertex{-0.01, 1.588f}, Vertex{0.205f, -0.93f}, Vertex{0.34f, 0.158f}},
		//Hairline
		{ Vertex{-0.205f, 0.177f}, Vertex{-0.35f, 1.2f}, Vertex{0.4f, 1.2f}, Vertex{0.34f, 0.158f}},
		//Nose
		{ Vertex{-0.004f, 0.339f}, Vertex{0.048f, 0.24f}, Vertex{-0.182f, 0.1f}, Vertex{-0.006f, 0.035f}, },
		//Moustache Top
		{ Vertex{-0.16f, -0.192f}, Vertex{-0.115f, -0.027f}, Vertex{0.004f, 0.15f}, Vertex{0.01f, 0.15f}, Vertex{0.13f, -0.017f}, Vertex{0.172f, -0.196f}, },
		//Moustache Bottom
		{ Vertex{-0.16f, -0.192f}, Vertex{-0.02f, -0.096f}, Vertex{0.004f, 0.03f}, Vertex{0.01f, 0.03f}, Vertex{0.02f, -0.09f}, Vertex{0.172f, -0.196f}, },
		//Top Left Eyebrow
		{ Vertex{0.036f, 0.383f}, Vertex{0.181f, 0.42f}, Vertex{0.196f, 0.324f}, },
		//Bottom Left Eyebrow
		{ Vertex{0.035f, 0.343f}, Vertex{0.181f, 0.42f}, Vertex{0.196f, 0.324f}, },
		//Connect Left Eyebrow
		{ Vertex{0.035f, 0.343f}, Vertex{0.0f, 0.353f}, Vertex{0.036f, 0.383f}, },
		//Top Right Eyebrow
		{ Vertex{-0.176f, 0.308f}, Vertex{-0.156f, 0.41f}, Vertex{-0.025f, 0.383f}, },
		//Bottom Right Eyebrow
		{ Vertex{-0.176f, 0.308f}, Vertex{-0.156f, 0.41f}, Vertex{-0.029f, 0.338f}, },
		//Connect Right Eyebrow
		{ Vertex{-0.029f, 0.338f}, Vertex{0.02f, 0.354f}, Vertex{-0.025f, 0.383f}, },
		//Right Eye Bottom
		{ Vertex{-0.012f, 0.2804f}, Vertex{-0.049f, 0.317f}, Vertex{-0.155f, 0.209f}, Vertex{-0.139f, 0.315f},},
		//Left  Eye Bottom
		{ Vertex{0.052f, 0.279f},  Vertex{0.108f, 0.31f},  Vertex{0.151f, 0.2255f}, Vertex{0.175f, 0.314f},},
		//Right Eye Top
		{ Vertex{-0.012f, 0.2804f}, Vertex{-0.065f, 0.388f}, Vertex{-0.139f, 0.315f}, },
		//Left Eye Top
		{ Vertex{0.052f, 0.279f},  Vertex{0.137f, 0.4f}, Vertex{0.175f, 0.314f}, },
	};

	for (auto& control_points : curve_list) {
		draw_curve(control_points, n_iterations);
	}

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

