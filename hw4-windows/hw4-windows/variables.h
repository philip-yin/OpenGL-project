/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This is the basic include file for the global variables in the program.  
// Since all files need access to it, we define EXTERN as either blank or 
// extern, depending on if included in the main program or not.  

#include <glm/glm.hpp>
#include <vector>
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

EXTERN int amount; // The amount of rotation for each arrow press
EXTERN vec3 eye; // The (regularly updated) vector coordinates of the eye 
EXTERN vec3 up;  // The (regularly updated) vector coordinates of the up 
EXTERN vec3 raynow;

#ifdef MAINPROGRAM 
vec3 eyeinit(0.0, 0.0, 5.0); // Initial eye position, also for resets
vec3 upinit(0.0, 1.0, 0.0); // Initial up position, also for resets
vec3 center(0.0, 0.0, 0.0); // Center look at point 
int amountinit = 5;
float width = 500, height = 500; // width and height 
float fovy = 90.0; // For field of view
#else 
EXTERN vec3 eyeinit;
EXTERN vec3 upinit;
EXTERN vec3 center;
EXTERN int amountinit;
EXTERN float width, height;
EXTERN float fovy;
#endif 

EXTERN float constant, linear, quadratic;
EXTERN int depth;
EXTERN int maxverts;
EXTERN string outputfile;
EXTERN unsigned int vertexshader, fragmentshader, shaderprogram; // shaders
static enum { view, translate, scale } transop; // which operation to transform 
enum shape { cube, sphere, teapot, tri};
EXTERN float sx, sy; // the scale in x and y 
EXTERN float tx, ty; // the translation in x and y

EXTERN vector<vec3> pointposn; // Light Positions
EXTERN vector<vec3> pointcolor; // Light Colors
EXTERN vector<vec3> pointtransf; // Lights transformed by modelview
EXTERN vector<vec3> dirposn;
EXTERN vector<vec3> dircolor;
EXTERN vector<vec3> dirtransf;
EXTERN vector<vec3> vertex;

EXTERN vec3 norm;
EXTERN vec3 ambient;
EXTERN vec3 diffuse;
EXTERN vec3 specular;
EXTERN vec3 emission;
EXTERN float shininess;

// For multiple objects, read from a file.  
EXTERN struct object {
	shape type;
	int vertex[3];
	float radius;
	vec3 pos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emission;
	float shininess;
	mat4 transform;
};

EXTERN vector<object> objects;
