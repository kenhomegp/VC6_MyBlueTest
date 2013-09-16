// Navigator.h: interface for the CNavigator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NAVIGATOR_H__88DA6377_8825_4264_9426_524AB91BA9C1__INCLUDED_)
#define AFX_NAVIGATOR_H__88DA6377_8825_4264_9426_524AB91BA9C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;

template <class T>
class CNavigator  
{
public:
	CNavigator();
	virtual ~CNavigator();

	void AddStep(T t);
	T StepBack();
	T StepForward();
	UINT BackCount();
	UINT ForwardCount();
	void ClearHistory();

private:
	vector<T>	m_vHistory;			// history of navigation
	UINT		m_unNavigationPos;	// current navigation position. It can be less than
									// m_vHistory.size() if back syeps were done
	UINT		m_unNavMax;			// m_unNavigationPos + #of undone steps
};

template <class T>
CNavigator<T>::CNavigator()
{
	m_unNavigationPos	= 0;
	m_unNavMax			= 0;
}

template <class T>
CNavigator<T>::~CNavigator()
{
}

template <class T>
void CNavigator<T>::AddStep(T t)
{
	m_vHistory.erase( m_vHistory.begin() + m_unNavigationPos, m_vHistory.end() );
	m_vHistory.push_back(t);
	m_unNavigationPos ++;
	m_unNavMax = m_unNavigationPos;
}

template <class T>
T CNavigator<T>::StepBack()
{
	// The member function returns a reference to the element of the 
	// controlled sequence at position pos. If that position is invalid, 
	// the behavior is undefined (comment to vector::operator [])
	m_unNavigationPos --;
	return m_vHistory[m_unNavigationPos - 1];
}

template <class T>
T CNavigator<T>::StepForward()
{
	// The member function returns a reference to the element of the 
	// controlled sequence at position pos. If that position is invalid, 
	// the behavior is undefined (comment to vector::operator [])
	m_unNavigationPos ++;
	return m_vHistory[m_unNavigationPos - 1];
}

template <class T>
UINT CNavigator<T>::BackCount()
{
	return m_vHistory.empty() ? 0 : m_unNavigationPos - 1;
}

template <class T>
UINT CNavigator<T>::ForwardCount()
{
	return m_unNavMax - m_unNavigationPos;
}

template <class T>
void CNavigator<T>::ClearHistory()
{
	m_vHistory.clear();
	m_unNavigationPos	= 0;
	m_unNavMax			= 0;
}

#endif // !defined(AFX_NAVIGATOR_H__88DA6377_8825_4264_9426_524AB91BA9C1__INCLUDED_)
