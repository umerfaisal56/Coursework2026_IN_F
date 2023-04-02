#ifndef __ENEMYSPACESHIP_H__
#define __ENEMYSPACESHIP_H__

#include "GameUtil.h"
#include "GameObject.h"
#include "Shape.h"
#include "IEnemyListener.h"

class EnemySpaceship : public GameObject
{
	shared_ptr<GameObject> target;
public:
	EnemySpaceship();
	EnemySpaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r);
	EnemySpaceship(const EnemySpaceship& s);
	virtual ~EnemySpaceship(void);

	virtual void Update(int t);
	virtual void Render(void);

	virtual void Thrust(float t);
	virtual void Rotate(float r);
	virtual void Shoot(void);

	void SetSpaceshipShape(shared_ptr<Shape> spaceship_shape) { mSpaceshipShape = spaceship_shape; }
	void SetThrusterShape(shared_ptr<Shape> thruster_shape) { mThrusterShape = thruster_shape; }
	void SetBulletShape(shared_ptr<Shape> bullet_shape) { mBulletShape = bullet_shape; }

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList &objects);
	void setTarget(shared_ptr<GameObject> target);

	void FireEnemyKilled();
	void AddListener(shared_ptr<IEnemyListener> listener);
	void setLives(int l) {
		mLives = l;
	}

private:
	int mLives = 4;
	float shootTime = 0;
	float rotateTime = 0;
	float thrustTime = 0;

	float rotateResetTime = 1.f;
	float thrustTimeReset = 1.f;
	
	float mThrust;

	shared_ptr<Shape> mSpaceshipShape;
	shared_ptr<Shape> mThrusterShape;
	shared_ptr<Shape> mBulletShape;

	typedef std::list< shared_ptr<IEnemyListener> > EnemyListenerList;

	EnemyListenerList mListeners;
};

#endif