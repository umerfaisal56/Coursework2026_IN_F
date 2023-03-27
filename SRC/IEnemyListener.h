#ifndef __IENEMYLISTENER_H__
#define __IENEMYLISTENER_H__

class IEnemyListener
{
public:
	virtual void OnEnemyKilled(int lives_left) = 0;
};

#endif
