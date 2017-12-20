// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.
#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
	vec3 norm = glm::normalize(axis);
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

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1.  
	up = glm::normalize(up);
	eye = rotate(degrees, up) * eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE 
    // Likely the same as in HW 1.  
	vec3 h = glm::cross(eye, up);
	h = glm::normalize(h);
	eye = rotate(degrees, h) * eye;
	up = rotate(degrees, h) *up;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
	vec3 w = glm::normalize(eye);
	vec3 u = glm::normalize(glm::cross(up, w));
	vec3 v = glm::cross(w, u);
	mat3 rotation = mat3(u.x, v.x, w.x, u.y, v.y, w.y, u.z, v.z, w.z);
	vec3 translation = rotation * (-eye);
	mat4 final = mat4(u.x, v.x, w.x, 0, u.y, v.y, w.y, 0, u.z, v.z, w.z, 0, translation.x, translation.y, translation.z, 1);
	return final;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
	float theta = glm::radians(fovy) / 2;
	float d = 1/tan(theta);
	float A = -(zFar + zNear) / (zFar - zNear);
	float B = -2 * zFar* zNear / (zFar - zNear);
    mat4 ret = mat4(d/aspect, 0, 0, 0, 0, d, 0, 0, 0, 0, A, -1, 0, 0, B, 0);
    // YOUR CODE FOR HW2 HERE
    // New, to implement the perspective transform as well.  
    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
	mat4 ret = mat4(sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1);
    // YOUR CODE FOR HW2 HERE
    // Implement scaling 
    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
	mat4 ret = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, tx, ty, tz, 1);
    // YOUR CODE FOR HW2 HERE
    // Implement translation 
    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
