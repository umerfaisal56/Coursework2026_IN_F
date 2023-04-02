#include <stdlib.h>
#include "GameUtil.h"
#include "Asteroid.h"
#include "BoundingShape.h"
#include "Spaceship.h"

Asteroid::Asteroid(void) : GameObject("Asteroid")
{
	mAngle = rand() % 360;
	mRotation = 0; // rand() % 90;
	mPosition.x = rand() / 2;
	mPosition.y = rand() / 2;
	mPosition.z = 0.0;
	mVelocity.x = 5 * cos(DEG2RAD*mAngle);
	mVelocity.y = 5 * sin(DEG2RAD*mAngle);
	mVelocity.z = 0.0;
}

Asteroid::~Asteroid(void)
{
}

bool Asteroid::CollisionTest(shared_ptr<GameObject> o)
{
	if (GetType() == o->GetType()) return false;
	if (o->GetType() == GameObjectType("Bonus")) return false;
	if (o->GetType() == GameObjectType("EnemySpaceship")) return false;
	if (o->GetType() == GameObjectType("EnemyBullet")) return false;
	if (o->GetType() == GameObjectType("bonusLive")) return false;
	if (o->GetType() == GameObjectType("bonusBounceBullet")) return false;
	if (o->GetType() == GameObjectType("Spaceship")) {
		shared_ptr<Spaceship> ship = std::dynamic_pointer_cast<Spaceship>(o);
		if (ship.get()->getNoCollideTime() > 0) {
			return false;
		}
	}
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Asteroid::OnCollision(const GameObjectList& objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
}

bool Asteroid::canSplit()
{
	return split;
}

void Asteroid::setSplit(bool split)
{
	this->split = split;
}
