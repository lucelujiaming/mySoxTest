#ifndef VEC3_H
#define VEC3_H

#include<math.h>
#include<stdlib.h>
#include<iostream>

class CVec3
{
public:
	CVec3(){}
	CVec3(float e0, float e1, float e2)
	{
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
	}

	inline float x() const { 
		return e[0]; 
	}
	inline float y() const {
		return e[1]; 
	}
	inline float z()const {
		return e[2];
	}
	inline float r()const {
		return e[0];
	}
	inline float g()const { 
		return e[1]; 
	}
	inline float b()const {
		return e[2]; 
	}
	
	inline const CVec3 & operator+() const{
		return*this; 
	}
	inline CVec3 operator-() const{ 
		return CVec3(-e[0], -e[1], -e[2]);
	}
	inline float operator[] (int i) const{
		return e[i]; 
	}
	inline float&operator[] (int i) {
		return e[i]; 
	};
	inline CVec3 & operator+=(const CVec3 & v2);
	inline CVec3 & operator-=(const CVec3 & v2);
	inline CVec3 & operator*=(const CVec3 & v2);
	inline CVec3 & operator/=(const CVec3 & v2);
	inline CVec3 & operator*=(const float t); 
	inline CVec3 & operator/=(const float t);
	
	inline float length() const{
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}
	inline float squared_length() const{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	inline void make_unit_vector();
	
	float e[3];
};


inline std::istream&  operator>>(std::istream&is, CVec3 & t) {
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}
inline std::ostream& operator<<(std::ostream&os, const CVec3 & t) {
	os << t.e[0] << " " << t.e[1] << " " << t.e[2];
	return os;
}
inline void CVec3::make_unit_vector() {
	float k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	e[0] *= k; e[1] *= k; e[2] *= k;
}

inline CVec3 operator+(const CVec3 & vl, const CVec3 & v2) {
	return CVec3(vl.e[0] + v2.e[0], vl.e[1] + v2.e[1], vl.e[2] + v2.e[2]);
}

inline CVec3 operator-(const CVec3 & vl, const CVec3 & v2) {
	return CVec3(vl.e[0] - v2.e[0], vl.e[1] - v2.e[1], vl.e[2] - v2.e[2]);
}

inline CVec3 operator*(const CVec3 & vl, const CVec3 & v2) {
	return CVec3(vl.e[0] * v2.e[0], vl.e[1] * v2.e[1], vl.e[2] * v2.e[2]);
}

inline CVec3 operator/(const CVec3 & vl, const CVec3 & v2) {
	return CVec3(vl.e[0] / v2.e[0], vl.e[1] / v2.e[1], vl.e[2] / v2.e[2]);
}

inline CVec3 operator*(float t, const CVec3 & v) {
	return CVec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline CVec3 operator/(float t, const CVec3 & v) {
	return CVec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline CVec3 operator*(const CVec3 & v, float t) {
	return CVec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline CVec3 operator/(const CVec3 & v, float t) {
	return CVec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline float dot(const CVec3 & v1, const CVec3 & v2){
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline CVec3 cross(const CVec3 & v1, const CVec3 & v2) {
	return CVec3((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
		(-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
		(v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

inline CVec3 & CVec3::operator += (const CVec3 & v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline CVec3 & CVec3::operator *= (const CVec3 & v) {
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline CVec3 & CVec3::operator /= (const CVec3 & v) {
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline CVec3 & CVec3::operator -= (const CVec3 & v) {
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline CVec3 & CVec3::operator *= (const float t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline CVec3 & CVec3::operator /= (const float t) {
	float k = 1.0 / t;
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}
inline CVec3 unit_vector(CVec3 v){
	return v / v.length();
}

#endif