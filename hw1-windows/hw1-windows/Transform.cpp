// Transform.cpp: implementation of the Transform class.

#include "Transform.h"
#include <stdio.h>

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
  // YOUR CODE FOR HW1 HERE
	float theta = degrees * pi / 180.0;
	mat3 m1 = mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);
	mat3 m2 = mat3(axis[0], axis[1], axis[2], 0, 0, 0, 0, 0, 0);
	mat3 m3 = mat3(0, axis[2], -axis[1], -axis[2], 0, axis[0], axis[1], -axis[0], 0);
	m1 = m1 * cosf(theta);
	m2 = m2 * glm::transpose(m2) *(1 - cosf(theta));
	m3 = m3 * sinf(theta);
  // You will change this return call
  return m1 + m2 + m3;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE
  up = glm::normalize(up);
  eye = rotate(degrees, up) * eye;
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE
  vec3 h = glm::cross(eye, up);
  h = glm::normalize(h);
  eye = rotate(degrees, h) * eye;
  up = rotate(degrees, h) *up;
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
  // YOUR CODE FOR HW1 HERE
	vec3 w = glm::normalize(eye);
	vec3 u = glm::normalize(glm::cross(up, w));
	vec3 v = glm::cross(w, u);
	mat3 rotation = mat3(u.x, v.x, w.x, u.y, v.y, w.y, u.z, v.z, w.z);
	vec3 translation = rotation * (-eye);
	mat4 final = mat4(u.x, v.x, w.x, 0, u.y, v.y, w.y, 0, u.z, v.z, w.z, 0, translation.x, translation.y, translation.z, 1);
	return final;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}