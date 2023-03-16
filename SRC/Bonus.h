#ifndef __BONUS_H__
#define __BONUS_H__

#include "GameObject.h"
#include "BoundingShape.h"
#include "ILiveBonusListener.h"

class Bonus : public GameObject {

	typedef list< ILiveBonusListener* > BonusListenerList;
	BonusListenerList mListeners;

public:
	Bonus(void) : GameObject("Bonus")
	{
		mAngle = rand() % 360;
		mRotation = 0; // rand() % 90;
		mPosition.x = rand() / 2;
		mPosition.y = rand() / 2;
		mPosition.z = 0.0;
		mVelocity.x = 5 * cos(DEG2RAD * mAngle);
		mVelocity.y = 5 * sin(DEG2RAD * mAngle);
		mVelocity.z = 0.0;
	}
	~Bonus(void)
	{
	}

	void AddListener(ILiveBonusListener* listener)
	{
		mListeners.push_back(listener);
	}

	void FireBonusEated()
	{
		for (BonusListenerList::iterator lit = mListeners.begin();
			lit != mListeners.end(); ++lit) {
			(*lit)->onEatBonus();
		}
	}

	bool CollisionTest(shared_ptr<GameObject> o) {
		if (GetType() == o->GetType()) return false;
		if (o->GetType() == GameObjectType("Asteroid")) return false;
		if (o->GetType() == GameObjectType("Bullet")) return false;
		if (mBoundingShape.get() == NULL) return false;
		if (o->GetBoundingShape().get() == NULL) return false;
		return mBoundingShape->CollisionTest(o->GetBoundingShape());
	}
	void OnCollision(const GameObjectList& objects) {
		mWorld->FlagForRemoval(GetThisPtr());
		FireBonusEated();
	}
};

#endif