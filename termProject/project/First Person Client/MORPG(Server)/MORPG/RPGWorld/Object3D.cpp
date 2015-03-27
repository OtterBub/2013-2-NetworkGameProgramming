#include "Object3D.h"

void Object3D::Draw(){

	glTranslatef(point.x, point.y, point.z);
	glRotatef(angle, 0, 1, 0);
}

void Object3D::ObjectControlPosition(float add_x, float add_y, float add_z){
	point.x += add_x;
	point.y += add_y;
	point.z += add_z;
}

void Object3D::SetObjectPosition(float _x, float _y, float _z){
	point.x = _x;
	point.y = _y;
	point.z = _z;
}

void Object3D::ObjectControlAngle(float add_angle){
	angle += add_angle;
}

void Object3D::ObjectSeek(_point pos, float move) 
{
	_point dist = GetDist(pos);

	float scalar = sqrt(pow(dist.x,2) + pow(dist.y,2) + pow(dist.z,2));

	_point UnitVector;
	UnitVector.x = dist.x / scalar;
	UnitVector.y = dist.y / scalar;
	UnitVector.z = dist.z / scalar;

	UnitVector.x *= move;
	UnitVector.y *= move;
	UnitVector.z *= move;

	ObjectControlPosition(UnitVector.x, UnitVector.y, UnitVector.z);
}

void Object3D::ObjectFlee(_point pos, float move) 
{
	_point dist = GetDist(pos);

	float scalar = sqrt(pow(dist.x,2) + pow(dist.y,2) + pow(dist.z,2));

	_point UnitVector;
	UnitVector.x = dist.x / scalar;
	UnitVector.y = dist.y / scalar;
	UnitVector.z = dist.z / scalar;

	UnitVector.x *= move;
	UnitVector.y *= move;
	UnitVector.z *= move;

	ObjectControlPosition(-UnitVector.x, -UnitVector.y, -UnitVector.z);
}

_point Object3D::GetDist(_point pos)
{
	_point dist;
	dist.x = pos.x - point.x;
	dist.y = pos.y - point.y;
	dist.z = pos.z - point.z;

	return dist;
}