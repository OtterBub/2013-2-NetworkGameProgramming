#ifndef FMS_H
#define FMS_H

#include "State.h"

template <class type>
class FMS{
private:
	State<type>* m_pGlobalState;
	State<type>* m_pCurrentState;
	State<type>* m_pPreviousState;

	type* m_obj;

public:
	/*FMS() :m_CurrentState(NULL),
		m_GlobalState(NULL),
		m_PreviousState(NULL)
		m_obj(NULL)
	{}*/

	FMS(type* _obj) : 
		m_pCurrentState(NULL),
		m_pGlobalState(NULL),
		m_pPreviousState(NULL),
		m_obj(_obj)
	{
	}

	virtual ~FMS() {}

	void ChangeState(State<type> *_State) {
		m_pCurrentState->Exit(m_obj);
		m_pPreviousState = m_pCurrentState;
		m_pCurrentState = _State;
		m_pCurrentState->Enter();
	}

	void Execute() {
		if((m_pGlobalState == NULL) &&
			(m_pCurrentState == NULL) )
			return;

		if(m_GlobalState == NULL)
			m_pCurrentState->Execute(m_obj);
		else 
			m_pGlobalState->Execute(m_obj);
	}

	void BackToPrevState() {
		State<type> *temp = m_pCurrentState;
		m_pCurrentState = m_pPreviousState;
		m_pPreviousState = temp;
	}

};

#endif // FMS_H