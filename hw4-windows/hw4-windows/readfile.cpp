/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

/*****************************************************************************/
// This file is readfile.cpp.  It includes helper functions for matrix 
// transformations for a stack (matransform) and to rightmultiply the 
// top of a stack.  These functions are given to aid in setting up the 
// transformations properly, and to use glm functions in the right way.  
// Their use is optional in your program.  


// The functions readvals and readfile do basic parsing.  You can of course 
// rewrite the parser as you wish, but we think this basic form might be 
// useful to you.  It is a very simple parser.

// Please fill in parts that say YOUR CODE FOR HW 2 HERE. 
// Read the other parts to get a context of what is going on. 

/*****************************************************************************/

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glew.h>
//#include <GL/glut.h>
#include "Transform.h" 

using namespace std;
#include "variables.h" 
#include "readfile.h"

// You may not need to use the following two functions, but it is provided
// here for convenience

// The function below applies the appropriate transform to a 4-vector
void matransform(stack<mat4> &transfstack, float* values)
{
	mat4 transform = transfstack.top();
	vec4 valvec = vec4(values[0], values[1], values[2], values[3]);
	vec4 newval = transform * valvec;
	for (int i = 0; i < 4; i++) values[i] = newval[i];
}

void rightmultiply(const mat4 & M, stack<mat4> &transfstack)
{
	mat4 &T = transfstack.top();
	T = T * M;
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(stringstream &s, const int numvals, float* values)
{
	for (int i = 0; i < numvals; i++) {
		s >> values[i];
		if (s.fail()) {
			cout << "Failed reading value " << i << " will skip\n";
			return false;
		}
	}
	return true;
}

void readfile(const char* filename)
{
	string str, cmd;
	ifstream in;
	in.open(filename);
	if (in.is_open()) {

		// I need to implement a matrix stack to store transforms.  
		// This is done using standard STL Templates 
		stack <mat4> transfstack;
		transfstack.push(mat4(1.0));  // identity
		getline(in, str);
		while (in) {
			if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
				// Ruled out comment and blank lines 

				stringstream s(str);
				s >> cmd;
				int i;
				GLfloat values[10]; // Position and color for light, colors for others
									// Up to 10 params for cameras.  
				bool validinput; // Validity of input 

								 // Process the light, add it to database.
								 // Lighting Command
				if (cmd == "directional") {
					validinput = readvals(s, 6, values); // Position/color for lts.
					if (validinput) {
						dirposn.push_back(vec3(values[0], values[1], values[2]));
						dircolor.push_back(vec3(values[3], values[4], values[5]));
					}
				}
				else if (cmd == "point") {
					validinput = readvals(s, 6, values); // Position/color for lts.
					if (validinput) {
						pointposn.push_back(vec3(values[0], values[1], values[2]));
						pointcolor.push_back(vec3(values[3], values[4], values[5]));
					}
				}

				// Material Commands 
				// Ambient, diffuse, specular, shininess properties for each object.
				// Filling this in is pretty straightforward, so I've left it in 
				// the skeleton, also as a hint of how to do the more complex ones.
				// Note that no transforms/stacks are applied to the colors. 
				else if (cmd == "attenuation") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						constant = values[0];
						linear = values[1];
						quadratic = values[2];
					}
				}
				else if (cmd == "ambient") {
					validinput = readvals(s, 3, values); // colors 
					if (validinput) {
						for (i = 0; i < 3; i++) {
							ambient[i] = values[i];
						}
					}
				}
				else if (cmd == "diffuse") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						for (i = 0; i < 3; i++) {
							diffuse[i] = values[i];
						}
					}
				}
				else if (cmd == "specular") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						for (i = 0; i < 3; i++) {
							specular[i] = values[i];
						}
					}
				}
				else if (cmd == "emission") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						for (i = 0; i < 3; i++) {
							emission[i] = values[i];
						}
					}
				}
				else if (cmd == "shininess") {
					validinput = readvals(s, 1, values);
					if (validinput) {
						shininess = values[0];
					}
				}
				else if (cmd == "size") {
					validinput = readvals(s, 2, values);
					if (validinput) {
						width = values[0]; height = values[1];
					}
				}
				else if (cmd == "maxdepth") {
					validinput = readvals(s, 1, values);
					if (validinput) {
						depth = (int)values[0];
					}
				}
				else if (cmd == "output") {
					s >> outputfile;
					if (s.fail())
						cout << "Failed reading output filename will skip\n";
				}
				else if (cmd == "camera") {
					validinput = readvals(s, 10, values); // 10 values eye cen up fov
					if (validinput) {
						// Use all of values[0...9]
						// You may need to use the upvector fn in Transform.cpp
						// to set up correctly. 
						// Set eyeinit upinit center fovy in variables.h 
						eyeinit = vec3(values[0], values[1], values[2]);
						center = vec3(values[3], values[4], values[5]);
						vec3 up = vec3(values[6], values[7], values[8]);
						upinit = Transform::upvector(up, eyeinit);
						fovy = values[9];
					}
				}
				else if (cmd == "maxverts"){
					validinput = readvals(s, 1, values);
					if (validinput) {
						maxverts = (int)values[0];
					}
				}

				else if (cmd == "vertex") {
					if(maxverts <= vertex.size())
						cerr << "Reached Maximum Number of Vertex " << maxverts << " Will ignore further Vertex\n";
					else {
						validinput = readvals(s, 3, values);
						if (validinput) {
							vertex.push_back(vec3(values[0], values[1], values[2]));
						}
					}
				}

				// I've left the code for loading objects in the skeleton, so 
				// you can get a sense of how this works.  
				// Also look at demo.txt to get a sense of why things are done this way.
				else if (cmd == "sphere") {
					validinput = readvals(s, 4, values);
					if (validinput) {
						object * obj = new object;

						// Set the object's light properties
						for (i = 0; i < 3; i++) {
							(obj->ambient)[i] = ambient[i];
							(obj->diffuse)[i] = diffuse[i];
							(obj->specular)[i] = specular[i];
							(obj->emission)[i] = emission[i];
						}
						obj->shininess = shininess;

						obj->pos = vec3(values[0], values[1], values[2]);
						obj->radius = values[3];
						// Set the object's transform
						obj->transform = transfstack.top();
						// Set the object's type
						obj->type = sphere;
						objects.push_back(*obj);
						delete obj;
					}
				}

				else if (cmd == "tri") {

					validinput = readvals(s, 3, values);
					if (validinput) {
						object * obj = new object;

						// Set the object's light properties
						for (i = 0; i < 3; i++) {
							(obj->ambient)[i] = ambient[i];
							(obj->diffuse)[i] = diffuse[i];
							(obj->specular)[i] = specular[i];
							(obj->emission)[i] = emission[i];
						}
						obj->shininess = shininess;

						obj->vertex[0] = (int)values[0];
						obj->vertex[1] = (int)values[1];
						obj->vertex[2] = (int)values[2];
						// Set the object's transform
						obj->transform = transfstack.top();
						// Set the object's type
						obj->type = tri;
						objects.push_back(*obj);
						delete obj;
					}
				}

				else if (cmd == "translate") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						mat4 t = Transform::translate(values[0], values[1], values[2]);
						rightmultiply(t, transfstack);
						// Think about how the transformation stack is affected
						// You might want to use helper functions on top of file. 
						// Also keep in mind what order your matrix is!

					}
				}
				else if (cmd == "scale") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						mat4 s = Transform::scale(values[0], values[1], values[2]);
						rightmultiply(s, transfstack);
						// YOUR CODE FOR HW 2 HERE.  
						// Think about how the transformation stack is affected
						// You might want to use helper functions on top of file.  
						// Also keep in mind what order your matrix is!

					}
				}
				else if (cmd == "rotate") {
					validinput = readvals(s, 4, values);
					if (validinput) {
						mat3 r3 = Transform::rotate(values[3], vec3(values[0], values[1], values[2]));
						mat4 r4 = mat4(r3);
						rightmultiply(r4, transfstack);
						// YOUR CODE FOR HW 2 HERE. 
						// values[0..2] are the axis, values[3] is the angle.  
						// You may want to normalize the axis (or in Transform::rotate)
						// See how the stack is affected, as above.  
						// Note that rotate returns a mat3. 
						// Also keep in mind what order your matrix is!
					}
				}

				// I include the basic push/pop code for matrix stacks
				else if (cmd == "pushTransform") {
					transfstack.push(transfstack.top());
				}
				else if (cmd == "popTransform") {
					if (transfstack.size() <= 1) {
						cerr << "Stack has no elements.  Cannot Pop\n";
					}
					else {
						transfstack.pop();
					}
				}

				else {
					cerr << "Unknown Command: " << cmd << " Skipping \n";
				}
			}
			getline(in, str);
		}

		// Set up initial position for eye, up and amount
		// As well as booleans 

		eye = eyeinit;
		up = upinit;
		amount = 5;
		sx = sy = 1.0;  // keyboard controlled scales in x and y 
		tx = ty = 0.0;  // keyboard controllled translation in x and y  

		//glEnable(GL_DEPTH_TEST);
	}
	else {
		cerr << "Unable to Open Input Data File " << filename << "\n";
		throw 2;
	}
}
