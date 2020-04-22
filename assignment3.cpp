/***
 Assignment-3: Geometric Modeling of a Scene

 Name: Wong, Alex (Please write your name in Last Name, First Name format)

 Collaborators: Doe, John; Doe, Jane
 ** Note: although the assignment should be completed individually
 you may speak with classmates on high level algorithmic concepts. Please
 list their names in this section

 Project Summary: A short paragraph (3-4 sentences) describing the work you
 did for the project.
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

//Given multiples 
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

	// Define a 50 degree field of view, 1:1 aspect ratio, near and far planes at 3 and 7
	gluPerspective(50.0, 1.0, 3.0, 80.0);
	// Position camera at (2, 6, 5), attention at (0, 0, 0), up at (0, 1, 0)
	//gluLookAt(20.0, 6.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    gluLookAt(20.0, 6.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}


vector<GLfloat> build_table() {
    vector<GLfloat> tabletopCenter = mat_mult(translation_matrix(0, 1.75, 0), mat_mult(scaling_matrix(4, 0.5, 4), build_cube()));
    vector<GLfloat> tabletopRim1 = mat_mult(translation_matrix(2.25, 1.75, 0), mat_mult(scaling_matrix(0.5, 0.5, 5), build_cube()));
    vector<GLfloat> tabletopRim2 = mat_mult(translation_matrix(-2.25, 1.75, 0), mat_mult(scaling_matrix(0.5, 0.5, 5), build_cube()));
    vector<GLfloat> tabletopRim3 = mat_mult(translation_matrix(0, 1.75, 2.25), mat_mult(scaling_matrix(4, 0.5, 0.5), build_cube()));
    vector<GLfloat> tabletopRim4 = mat_mult(translation_matrix(0, 1.75, -2.25), mat_mult(scaling_matrix(4, 0.5, 0.5), build_cube()));
    vector<GLfloat> tabletop = composeItems({ tabletopCenter , tabletopRim1 , tabletopRim2 ,tabletopRim3 ,tabletopRim4 });
    vector<GLfloat> tableg0 = mat_mult(scaling_matrix(0.5, 2, 0.5), build_cube());
    vector<GLfloat> tableg1 = mat_mult(translation_matrix(2, 0.5, 2), tableg0);
    vector<GLfloat> tableg2 = mat_mult(translation_matrix(-2, 0.5, 2), tableg0);
    vector<GLfloat> tableg3 = mat_mult(translation_matrix(2, 0.5, -2), tableg0);
    vector<GLfloat> tableg4 = mat_mult(translation_matrix(-2, 0.5, -2), tableg0);
    return composeItems({ tabletop, tableg1, tableg2, tableg3, tableg4 });
}

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
    vector<GLfloat> couchSection1 = mat_mult({ translation_matrix(0, 0, -7.6) }, couchSection0);
    vector<GLfloat> couchSection2 = mat_mult({ translation_matrix(-7.6, 0, 0), rotation_matrix_y(90), scaling_matrix(-1,1,1) }, couchSection0);
    //Corner Section
    vector<GLfloat> cornerBottom = mat_mult(scaling_matrix(5, 1, 5), build_cube());
    vector<GLfloat> cornerCushion = cushions0;
    vector<GLfloat> cornerBackCushion = mat_mult(rotation_matrix_y(90), backCushion);
    vector<GLfloat> couchCorner = composeItems({ cornerBottom, cornerCushion, cornerBackCushion });
    vector<GLfloat> couchSection3 = mat_mult({ translation_matrix(-7.5, 0, -7.5) }, couchCorner);
    return composeItems({ couchSection1, couchSection2, couchSection3 });
}

vector<GLfloat> build_lamp() {
    return composeItems({});
}

vector<GLfloat> build_candle() {
    return composeItems({ });
}

vector<GLfloat> build_piano() {
    vector<GLfloat> back = mat_mult({ translation_matrix(8, 0, 6), scaling_matrix(10,3,1.5) }, build_cube());
    vector<GLfloat> keyBottom = mat_mult({ translation_matrix(8, 0, 7.75), scaling_matrix(10,1,2) }, build_cube());
    vector<GLfloat> white_key = mat_mult({ translation_matrix(4.25, 0.75, 7.5), scaling_matrix(0.17,0.17,1.2) }, build_cube());

    vector<vector<GLfloat>> keys;
    for (int i = 0; i < 52; i++) {
        keys.push_back(mat_mult(translation_matrix(i * (0.18), 0, 0), white_key));
    }

    vector<GLfloat> keybed = composeItems(keys);

    return composeItems({ back, keyBottom, keybed });
}

vector<GLfloat> build_pillow() {
    return composeItems({ });
}

vector<GLfloat> build_ottoman() {
    return composeItems({ });
}


// Construct the scene using objects built from cubes/prisms
vector<GLfloat> init_scene() {
	vector<GLfloat> scene;
    vector<GLfloat> basicTable =  build_table();
    vector<GLfloat> basicCouch = build_couch();
    //vector<GLfloat> basicOttoman = build_ottoman();
    //vector<GLfloat> basicPillow = build_pillow();
    //vector<GLfloat> basicCandle = build_candle();
    //vector<GLfloat> basicLamp = build_lamp();
    //vector<GLfloat> basicPiano = build_piano();
    //vector<GLfloat> floorPlane = mat_mult({ scaling_matrix(40, 40, 40), rotation_matrix_x(90) }, init_plane());

    scene = to_cartesian_coord(composeItems({ basicCouch, basicTable }));
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
	THETA = THETA + 0.06;
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