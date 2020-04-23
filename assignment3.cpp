/***
 Assignment-3: Geometric Modeling of a Scene

 Name: Herrmann, Timothy
 Collaborators: None

 Project Summary: 
 ***/


#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

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

#define _USE_MATH_DEFINES

#pragma GCC diagnostic pop

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
using namespace std;

// If a float is < EPSILON or > -EPILSON then it should be 0
float EPSILON = 0.000001;
// theta is the angle to rotate the scene
float THETA = 0.0;
// Vector placeholders for the scene and color array
vector<GLfloat> SCENE;
vector<GLfloat> COLOR;

/**************************************************
 *  Rectangular Prisms via Hierarchical Modeling  *
 *                                                *
 *  using planes as building blocks, build a unit *
 *  cube with transformations that will serve as  *
 *  a primitive for modeling objects in the scene *
 *                                                *
 *************************************************/

// Converts degrees to radians for rotation
float deg2rad(float d) {
	return (d * M_PI) / 180.0;
}

// Converts a vector to an array
GLfloat* vector2array(vector<GLfloat> vec) {
	GLfloat* arr = new GLfloat[vec.size()];
	for (int i = 0; i < vec.size(); i++) {
		arr[i] = vec[i];
	}
	return arr;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_homogeneous_coord(vector<GLfloat> cartesian_coords) {
	vector<GLfloat> result;
	for (int i = 0; i < cartesian_coords.size(); i++) {
		result.push_back(cartesian_coords[i]);
		if ((i + 1) % 3 == 0) {
			result.push_back(1.0);
		}
	}
	return result;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_cartesian_coord(vector<GLfloat> homogeneous_coords) {
	vector<GLfloat> result;
	for (int i = 0; i < homogeneous_coords.size(); i++) {
		if ((i + 1) % 4 == 0) {
			continue;
		}
		else {
			result.push_back(homogeneous_coords[i]);
		}
	}
	return result;
}


// Definition of a translation matrix
vector<GLfloat> translation_matrix(float dx, float dy, float dz) {
	vector<GLfloat> translate_mat = {
		 1, 0, 0, dx,
		 0, 1, 0, dy,
		 0, 0, 1, dz,
		 0, 0, 0, 1
	};
	return translate_mat;
}

// Definition of a scaling matrix
vector<GLfloat> scaling_matrix(float sx, float sy, float sz) {
	vector<GLfloat> scale_mat = {
		 sx, 0, 0, 0,
		 0, sy, 0, 0,
		 0, 0, sz, 0,
		 0, 0, 0, 1
	};
	return scale_mat;
}

// Definition of a rotation matrix about the x-axis theta degrees
vector<GLfloat> rotation_matrix_x(float theta) {
	vector<GLfloat> rotate_mat_x = {
		 1, 0, 0, 0,
		 0, cos(deg2rad(theta)), -sin(deg2rad(theta)), 0,
		 0, sin(deg2rad(theta)),  cos(deg2rad(theta)), 0,
		 0, 0, 0, 1
	};
	return rotate_mat_x;
}


// Definition of a rotation matrix about the y-axis by theta degrees
vector<GLfloat> rotation_matrix_y(float theta) {
	vector<GLfloat> rotate_mat_y = {
		cos(deg2rad(theta)),  0, sin(deg2rad(theta)), 0,
		0, 1, 0, 0,
		-sin(deg2rad(theta)), 0, cos(deg2rad(theta)), 0,
		0, 0, 0, 1
	};

	return rotate_mat_y;
}


// Definition of a rotation matrix about the z-axis by theta degrees
vector<GLfloat> rotation_matrix_z(float theta) {
	vector<GLfloat> rotate_mat_z = {
		cos(deg2rad(theta)), -sin(deg2rad(theta)), 0, 0,
		sin(deg2rad(theta)), cos(deg2rad(theta)),  0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	return rotate_mat_z;
}

// Perform matrix multiplication for A B
vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B) {
	vector<GLfloat> result;
	for (int i = 0; i < B.size(); i += 4) {
		for (int j = 0; j < A.size(); j += 4) {
			GLfloat dot_product = 0.0;
			for (int k = 0; k < 4; k++) {
				dot_product += A[j + k] * B[i + k];
			}
			result.push_back(dot_product);
		}
	}
	return result;
}

//Takes components and concatenates them into a new component
vector<GLfloat> composeItems(vector<vector<GLfloat>> items) {
    vector<GLfloat> result;
    for (auto& item : items) {
        result.insert(result.end(), item.begin(), item.end());
    }
    return result;
}

//Accepts multiple transformations in an array for matirx multiplication 
vector<GLfloat> mat_mult(vector<vector<GLfloat>> matrices, vector<GLfloat> points) {
    vector<GLfloat> result = points;
    for (int i = matrices.size()-1; i >= 0; i--) {
        result = mat_mult(matrices[i], result);
    }
    return result;
}

// Initializes a square plane of unit lengths
vector<GLfloat> init_plane() {
    vector<GLfloat> vertices = {
        +0.5,   +0.5,   +0.0,
        -0.5,   +0.5,   +0.0,
        -0.5,   -0.5,   +0.0,
        +0.5,   -0.5,   +0.0
    };
    return to_homogeneous_coord(vertices);
}


// Builds a unit cube centered at the origin
vector<GLfloat> build_cube() {
    vector<GLfloat> frontFace = mat_mult(translation_matrix(0, 0, 0.5), init_plane() );
    vector<GLfloat> backFace =   mat_mult( { translation_matrix(0, 0, -0.5), rotation_matrix_y(180) }, init_plane());
    vector<GLfloat> rightFace =  mat_mult( { translation_matrix(0.5, 0, 0),  rotation_matrix_y(90)  }, init_plane());
    vector<GLfloat> leftFace =   mat_mult( { translation_matrix(-0.5, 0, 0), rotation_matrix_y(-90) }, init_plane());
    vector<GLfloat> topFace =    mat_mult( { translation_matrix(0, 0.5, 0),  rotation_matrix_x(-90) }, init_plane());
    vector<GLfloat> bottomFace = mat_mult( { translation_matrix(0, -0.5, 0), rotation_matrix_x(90)  }, init_plane());
	return composeItems({ frontFace , backFace , rightFace , leftFace  , topFace  , bottomFace });
}

/**************************************************
 *            Camera and World Modeling           *
 *                                                *
 *  create a scene by applying transformations to *
 *  the objects built from planes and position    *
 *  the camera to view the scene by setting       *
 *  the projection/viewing matrices               *
 *                                                *
 *************************************************/

void setup() {
	// Enable the vertex array functionality
	glEnableClientState(GL_VERTEX_ARRAY);
	// Enable the color array functionality (so we can specify a color for each vertex)
	glEnableClientState(GL_COLOR_ARRAY);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Set up some default base color
	glColor3f(0.5, 0.5, 0.5);
	// Set up white background
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void init_camera() {
	// Camera parameters
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Define a 50 degree field of view, 1:1 aspect ratio, near and far planes at 3 and 80
	gluPerspective(50.0, 1.0, 3.0, 80.0);
	// Position camera at (20, 13, 20), attention at (0, 0, 0), up at (0, 1, 0)
    gluLookAt(20.0, 13.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}

//Builds a basic table
vector<GLfloat> build_table() {
    //TableTop
    vector<GLfloat> tabletopCenter = mat_mult(translation_matrix(0, 1.75, 0), mat_mult(scaling_matrix(4, 0.5, 4), build_cube()));
    vector<GLfloat> tabletopRim1 = mat_mult(translation_matrix(2.25, 1.75, 0), mat_mult(scaling_matrix(0.5, 0.5, 4), build_cube()));
    vector<GLfloat> tabletopRim2 = mat_mult(translation_matrix(-2.25, 1.75, 0), mat_mult(scaling_matrix(0.5, 0.5, 4), build_cube()));
    vector<GLfloat> tabletopRim3 = mat_mult(translation_matrix(0, 1.75, 2.25), mat_mult(scaling_matrix(5, 0.5, 0.5), build_cube()));
    vector<GLfloat> tabletopRim4 = mat_mult(translation_matrix(0, 1.75, -2.25), mat_mult(scaling_matrix(5, 0.5, 0.5), build_cube()));
    vector<GLfloat> tabletop = composeItems({ tabletopCenter , tabletopRim1 , tabletopRim2 ,tabletopRim3 ,tabletopRim4 });
    //Legs
    vector<GLfloat> tableg0 = mat_mult(scaling_matrix(0.5, 2, 0.5), build_cube());
    vector<GLfloat> tableg1 = mat_mult(translation_matrix(2, 0.5, 2), tableg0);
    vector<GLfloat> tableg2 = mat_mult(translation_matrix(-2, 0.5, 2), tableg0);
    vector<GLfloat> tableg3 = mat_mult(translation_matrix(2, 0.5, -2), tableg0);
    vector<GLfloat> tableg4 = mat_mult(translation_matrix(-2, 0.5, -2), tableg0);
    return composeItems({ tabletop, tableg1, tableg2, tableg3, tableg4 });
}

//Builds a basic right angled couch
vector<GLfloat> build_couch() {
    //Structure
    vector<GLfloat> bottom = mat_mult(scaling_matrix(10.5, 1, 5), build_cube());
    vector<GLfloat> side = mat_mult({ translation_matrix(5.75, 1, -0.25), scaling_matrix(1, 3, 5.5) }, build_cube());
    vector<GLfloat> back = mat_mult({ translation_matrix(-2.5, 2, -2.75), scaling_matrix(15.5, 5, 0.5) }, build_cube());
    //Cushions
    vector<GLfloat> bottomCushion = mat_mult({ translation_matrix(0, 1, 0), scaling_matrix(5, 1, 5) }, build_cube());
    vector<GLfloat> backCushion = mat_mult({ translation_matrix(0, 3.25, -2), rotation_matrix_x(-5) ,scaling_matrix(5, 3.5, 0.75) }, build_cube());
    vector<GLfloat> cushions0 = composeItems({ bottomCushion, backCushion });
    vector<GLfloat> cushions1 = mat_mult({ translation_matrix(2.6, 0, 0) }, cushions0);
    vector<GLfloat> cushions2 = mat_mult({ translation_matrix(-2.6, 0, 0) }, cushions0);
    //Assemble Main Sections
    vector<GLfloat> couchSection0 = composeItems({ bottom, side, back, cushions1, cushions2 });
    vector<GLfloat> couchSection1 = mat_mult({ translation_matrix(0, 0, -7.5) }, couchSection0);
    vector<GLfloat> couchSection2 = mat_mult({ translation_matrix(-7.5, 0, 0), rotation_matrix_y(90), scaling_matrix(-1,1,1) }, couchSection0);
    //Corner Section
    vector<GLfloat> cornerBottom = mat_mult(scaling_matrix(5, 1, 5), build_cube());
    vector<GLfloat> cornerCushion = mat_mult(scaling_matrix(0.93, 1, 0.93), cushions0);
    vector<GLfloat> cornerBackCushion = mat_mult({translation_matrix(-0.1,0,0.25), rotation_matrix_y(90), scaling_matrix(0.78, 1, 0.93) }, backCushion);
    vector<GLfloat> couchCorner = composeItems({ cornerBottom, cornerCushion, cornerBackCushion });
    vector<GLfloat> couchSection3 = mat_mult({ translation_matrix(-7.5, 0, -7.5) }, couchCorner);
    return composeItems({ couchSection1, couchSection2, couchSection3 });
}

//Builds a basic lamp
vector<GLfloat> build_lamp() {
    vector<GLfloat> base = mat_mult({ scaling_matrix(0.75,3,0.75) }, build_cube());
    vector<GLfloat> shadeBase = mat_mult({ translation_matrix(0,1,0), scaling_matrix(2,0.25,2) }, build_cube());
    vector<vector<GLfloat>> lampShadeSections;
    float iterations = 10.0f;
    for (int i = 0; i < iterations; i++) {
        lampShadeSections.push_back(mat_mult({ translation_matrix(0, 0.25 * (i + 1), 0), 
                                              scaling_matrix(0.8 - 0.8 * (i / (iterations*2)), 1, 
                                                             0.8 - 0.8 * (i/(iterations*2))) }, shadeBase));
    }
    vector<GLfloat> lampShade = composeItems(lampShadeSections);
    return composeItems({ base, lampShade});
}

//Builds a basic candle
vector<GLfloat> build_candle() {
    vector<GLfloat> holder = mat_mult({ translation_matrix(0, 0, 0), scaling_matrix(1.5,0.15,1.5) }, build_cube());
    vector<GLfloat> candle = mat_mult({ translation_matrix(0, 0.65, 0), scaling_matrix(0.5,1,0.5) }, build_cube());
    return composeItems({ holder , candle});
}

//Builds a basic pillow
vector<GLfloat> build_pillow() {
    vector<GLfloat> base = mat_mult({ translation_matrix(0, 0, 0), scaling_matrix(1,1,0.03) }, build_cube());
    vector<vector<GLfloat>> frontSections;
    int iterations = 9;
    for (int i = 0; i < iterations; i++) {
        frontSections.push_back(mat_mult({ translation_matrix(0, 0, 0.03*(i+1)), 
                                           scaling_matrix(0.9 - 0.9 *(pow(i,2) /pow(iterations,2)),
                                                          0.9 - 0.9 *(pow(i,2) /pow(iterations,2)), 1) }, base));
    }
    vector<GLfloat> front = composeItems(frontSections);
    vector<GLfloat> back = mat_mult(rotation_matrix_y(180), front);
    return composeItems({ base, front, back});
}

//Builds a basic ottoman
vector<GLfloat> build_ottoman() {
    vector<GLfloat> bottomSection = mat_mult({ translation_matrix(0, 0, 0), scaling_matrix(5,1,3) }, build_cube());
    vector<GLfloat> topSection = mat_mult({ translation_matrix(0, 1, 0) }, bottomSection);
    return composeItems({ bottomSection , topSection });
}


// Construct the scene using objects built from cubes/prisms
vector<GLfloat> init_scene() {
    vector<GLfloat> scene;
    //Build Basic Furniture Building Bocks
    vector<GLfloat> basicTable = build_table();
    vector<GLfloat> basicCouch = build_couch();
    vector<GLfloat> basicOttoman = build_ottoman();
    vector<GLfloat> basicPillow = build_pillow();
    vector<GLfloat> basicCandle = build_candle();
    vector<GLfloat> basicLamp = build_lamp();
    //Set Furniture in Scene
    vector<GLfloat> coffeeTable = mat_mult({ translation_matrix(1,0,-1.3), scaling_matrix(1.6, 1, 0.82) }, basicTable);
    vector<GLfloat> readingTable = mat_mult({ translation_matrix(-8,0, 9), scaling_matrix(1, 1, 1) }, basicTable);
    vector<GLfloat> lamp = mat_mult(translation_matrix(-8, 3, 9), basicLamp);
    vector<GLfloat> candle = mat_mult({ translation_matrix(1,2.5,-1.3), scaling_matrix(1, 1, 1) }, basicCandle);
    vector<GLfloat> ottoman = mat_mult({ translation_matrix(1,0,4.3), scaling_matrix(1.15, 1.15, 1.15) }, basicOttoman);
    vector<GLfloat> pillow1 = mat_mult({ translation_matrix(4,3,-8), rotation_matrix_y(-25), rotation_matrix_x(-10), scaling_matrix(2.7, 2.7, 2.7) }, basicPillow);
    vector<GLfloat> pillow2 = mat_mult({ translation_matrix(-8,3,4), rotation_matrix_y(115), rotation_matrix_x(-10), scaling_matrix(2.7, 2.7, 2.7) }, basicPillow);
    vector<GLfloat> pillow3 = mat_mult({ translation_matrix(-8,3,-8), rotation_matrix_y(230), rotation_matrix_x(10), scaling_matrix(2.7, 2.7, 2.7) }, basicPillow);
    //vector<GLfloat> floorPlane = mat_mult({ translation_matrix(0,-0.5, 0), scaling_matrix(100, 100, 100), rotation_matrix_x(90) }, init_plane());
    scene = to_cartesian_coord(mat_mult(translation_matrix(3, 0, 0), composeItems({ basicCouch, coffeeTable, readingTable, ottoman,  pillow1, pillow2, pillow3, lamp, candle })));
	return scene;
}

// Construct the color mapping of the scene
vector<GLfloat> init_color(vector<GLfloat> scene) {
	vector<GLfloat> colors;
	for (int i = 0; i < scene.size(); i++) {
		colors.push_back(static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
	}
	return colors;
}

void display_func() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SCENE = to_cartesian_coord(mat_mult(rotation_matrix_y(THETA), to_homogeneous_coord(init_scene())));

	GLfloat* scene_vertices = vector2array(SCENE);
	GLfloat* color_vertices = vector2array(COLOR);
	// Pass the scene vertex pointer
	glVertexPointer(3,                // 3 components (x, y, z)
		GL_FLOAT,         // Vertex type is GL_FLOAT
		0,                // Start position in referenced memory
		scene_vertices);  // Pointer to memory location to read from

// Pass the color vertex pointer
	glColorPointer(3,                   // 3 components (r, g, b)
		GL_FLOAT,            // Vertex type is GL_FLOAT
		0,                   // Start position in referenced memory
		color_vertices);     // Pointer to memory location to read from

// Draw quad point planes: each 4 vertices

	glDrawArrays(GL_QUADS, 0, 4 * (SCENE.size() / 12));

	glFlush();			//Finish rendering
	glutSwapBuffers();
	delete[] scene_vertices;
	delete[] color_vertices;
}

void idle_func() {
    //THETA = THETA + 0.03;
	THETA = THETA + 0.4;
	display_func();
}

int main(int argc, char** argv) {

	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	// Create a window with rendering context and everything else we need
	glutCreateWindow("Assignment 3");

	setup();
	init_camera();
	// Setting global variables SCENE and COLOR with actual values
	SCENE = init_scene();
	COLOR = init_color(SCENE);

	// Set up our display function
	glutDisplayFunc(display_func);
	glutIdleFunc(idle_func);
	// Render our world
	glutMainLoop();
	return 0;
}