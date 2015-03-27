#ifndef __WORLD3D_H__
#define __WORLD3D_H__

#include <iostream>
#include <vector>
#include <algorithm>


#include <gl\glut.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <cstdlib>

#include <ctime>
#include "Object3D.h"

#define CLIENT

class World3D {
protected:
	
	std::vector<Object3D*> m_obj;
	bool m_update;

	World3D(){ m_update = true; }	
	~World3D(){ m_obj.clear(); }
	World3D(const World3D&);
	World3D& operator=(const World3D&);
	
public:
	static World3D* Instance();

	void Draw();
	void Update();
	Object3D* AddObject(Object3D* object);
	Object3D* GetPointIndex(int index){ return m_obj[index]; }
	void AddFunction();
	void UpdateStop() { 
		if(m_update)
			m_update = false;
		else
			m_update = true;
	}
	void DeleteObject(Object3D* obj);
};


#endif //__WORLD3D_H__