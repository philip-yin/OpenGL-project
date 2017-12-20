#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "Transform.h"
#include <FreeImage.h>
#include <algorithm>

using namespace std;

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h" // prototypes for readfile.cpp  

vec3 rayThruPixel(const vec3 eyeinit, const vec3 center, const vec3 upinit, const float fovy, float i, float j) {
	vec3 a = eyeinit - center;
	vec3 b = upinit;
	vec3 w = glm::normalize(a);
	vec3 u = glm::normalize(glm::cross(b, w));
	vec3 v = glm::cross(w, u);
	float fovx = 2 * atanf(width/height*tanf(glm::radians(fovy/2)));
	float alpha = tanf(fovx / 2) * (j - width / 2) / (width / 2);
	float beta = tanf(glm::radians(fovy) / 2) * (i - height / 2) / (height / 2);
	vec3 raydir = glm::normalize(alpha * u + beta * v - w);
	return raydir;
}

float intersect(const vec3 pos, const vec3 ray, const object obj) {
	if (obj.type == sphere) {
		float a = glm::dot(ray, ray);
		float b = 2 * glm::dot(ray, pos - obj.pos);
		float c = glm::dot(pos - obj.pos, pos - obj.pos) - obj.radius * obj.radius;
		float det = b * b - 4 * a * c;
		//case when no intersect
		if (det < 0)
			return -1.0F;
		float small = (-b - sqrt(det)) / 2 / a;
		float big = (-b + sqrt(det)) / 2 / a;
		if (small < 0)
			return big;
		else
			return small;
	}
	else if (obj.type == tri) {
		vec3 A = vertex[obj.vertex[0]];
		vec3 B = vertex[obj.vertex[1]];
		vec3 C = vertex[obj.vertex[2]];
		vec3 normal = glm::normalize(glm::cross(B-A, C-A));
		float t = glm::dot(A, normal) - glm::dot(pos, normal);
		float d = glm::dot(ray, normal);
		if (d == 0.0F)
			return -1.0F;
		t = t / d;
		vec3 P = pos + ray * t;
		vec3 AB = B - A;
		vec3 BC = C - B;
		vec3 CA = A - C;
		vec3 AP = P - A;
		vec3 BP = P - B;
		vec3 CP = P - C;
		vec3 prod1 = glm::cross(AP, AB);
		vec3 prod2 = glm::cross(BP, BC);
		vec3 prod3 = glm::cross(CP, CA);
		float test1 = glm::dot(prod1, prod2);
		float test2 = glm::dot(prod2, prod3);
		float test3 = glm::dot(prod3, prod1);
		//inside tri
		if (test1 > 0.0F && test2 > 0.0F && test3 > 0.0F || test1==0.0F && test2==0.0F && test3==0.0F || test1 == 0.0F && test2 == 0.0F && test3 > 0.0F || test1 == 0.0F && test2 > 0.0F && test3 == 0.0F || test1 > 0.0F && test2 == 0.0F && test3 == 0.0F)
			return t;
		//not inside
		else
			return -1.0F;
	}
	return -1.0F;
}

void findColor(vec3 ray, unsigned char* pixels, int index, int target) {
	if (target == -1) {
		pixels[index] = 0;
		pixels[index + 1] = 0;
		pixels[index + 2] = 0;
		return;
	}
	object obj = objects[target];
	vec3 finalcolor = obj.ambient + obj.emission;
	//point lights
	for (int i = 0; i < pointposn.size(); i++) {
		int uselight = 1;
		vec3 posn = pointposn[i];
		vec3 color = pointcolor[i];
		float dist = glm::length(posn - ray);
		vec3 shadowdir = glm::normalize(posn - ray);
		vec3 shadowpos = ray + shadowdir / 1000.0F;
		for (int j = 0; j < objects.size(); j++) {
			float hit = intersect(shadowpos, shadowdir, objects[j]);
			if (hit >= 0) {
				uselight = 0;
				break;
			}
		}
		//if shadow ray hits object, then skip this light
		if (uselight == 0)
			continue;
		//shadow ray doesn't hit object
		vec3 eyedir = glm::normalize(eye - ray);
		vec3 halfvec = glm::normalize(shadowdir + eyedir);
		float nDotL = glm::dot(norm, shadowdir);
		vec3 lambert = obj.diffuse * pointcolor[i] * max(nDotL, 0.0F);
		float nDotH = glm::dot(norm, halfvec);
		vec3 phong = obj.specular * pointcolor[i] * pow(max(nDotH, 0.0F), obj.shininess);
		vec3 dAnds = (lambert + phong)/(constant + linear * dist + quadratic * dist * dist);
		finalcolor += dAnds;
	}
	//directional lights
	for (int i = 0; i < dirposn.size(); i++) {
		int uselight = 1;
		vec3 posn = dirposn[i];
		vec3 color = dircolor[i];
		vec3 shadowdir = glm::normalize(posn);
		vec3 shadowpos = ray + shadowdir / 1000.0F;
		for (int j = 0; j < objects.size(); j++) {
			float hit = intersect(shadowpos, shadowdir, objects[j]);
			if (hit >= 0) {
				uselight = 0;
				break;
			}
		}
		//if shadow ray hits object, then skip this light
		if (uselight == 0)
			continue;
		//shadow ray doesn't hit object
		vec3 eyedir = glm::normalize(eye - ray);
		vec3 halfvec = glm::normalize(shadowdir + eyedir);
		float nDotL = glm::dot(norm, shadowdir);
		vec3 lambert = obj.diffuse * pointcolor[i] * max(nDotL, 0.0F);
		float nDotH = glm::dot(norm, halfvec);
		vec3 phong = obj.specular * pointcolor[i] * pow(max(nDotH, 0.0F), obj.shininess);
		vec3 dAnds = lambert + phong;
		finalcolor += dAnds;
	}
	//recursive ray tracing
	//FreeImage On Windows OS use BGR as color order.
	pixels[index] = finalcolor[2] * 255;
	pixels[index + 1] = finalcolor[1] * 255;
	pixels[index + 2] = finalcolor[0] * 255;
}


int main(int argc, char* argv[]) {
	//initialize constant to 1
	constant = 1.0F;
	if (argc < 2) {
		cerr << "Usage: transforms scenefile";
		exit(-1);
	}
	readfile(argv[1]);

	int n = 3 * (int)width * (int)height;
	unsigned char* pixels = new unsigned char[n];
	
	for (int i = 0; i < (int)height; i++)
	{
		for (int j = 0; j < (int)width; j++)
		{
			int index = 3 * (i * (int)width + j);
			//ray trace in middle of each grid
			vec3 raydir = rayThruPixel(eyeinit, center, upinit, fovy, i + 0.5, j + 0.5);
			int k = 0;
			//target and targetpos have the final intersect object and pos
			int target = -1;
			float targetpos = FLT_MAX;
			//iterate through all objects
			while (k < objects.size()){
				//apply transform algorithm if needed
				if (objects[k].transform != mat4(1.0F)) {
					mat4 inverse = glm::inverse(objects[k].transform);
					eye= inverse * vec4(eyeinit, 1);
					raynow = inverse * vec4(raydir, 0);
				}
				else{
					eye = eyeinit;
					raynow = raydir;
				}
				float hit = intersect(eye, raynow, objects[k]);
				if (hit >= 0){
					if (targetpos > hit) {
						targetpos = hit;
						target = k;
					}
				}
				k++;
			}
			//find intersect in object coordinate
			vec3 ray = eye + raynow * targetpos;
			if (target != -1) {
				//find normal of object in object coordinate
				object obj = objects[target];
				if (obj.type == tri) {
					vec3 A = vertex[obj.vertex[0]];
					vec3 B = vertex[obj.vertex[1]];
					vec3 C = vertex[obj.vertex[2]];
					norm = glm::normalize(glm::cross(B - A, C - A));
				}
				else if (obj.type == sphere) {
					norm = glm::normalize(ray - obj.pos);
					//update normal to world coordinate
					norm = (glm::transpose(glm::inverse(obj.transform))) * vec4(norm, 0.0F);
					norm = glm::normalize(norm);
				}
				//update ray to world coordinate
				ray = obj.transform * vec4(ray, 1);
			}
			findColor(ray, pixels, index, target);
		}
	}
	FreeImage_Initialise();
	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, width, height, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	string fname = outputfile;
	FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
	FreeImage_DeInitialise();

	delete [] pixels;
	return 0;
}