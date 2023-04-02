#include "GameUtil.h"
#include "GameWorld.h"
#include "Bullet.h"
#include "Spaceship.h"
#include "BoundingSphere.h"

using namespace std;

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/**  Default constructor. */
Spaceship::Spaceship()
	: GameObject("Spaceship"), mThrust(0)
{
}

/** Construct a spaceship with given position, velocity, acceleration, angle, and rotation. */
Spaceship::Spaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r)
	: GameObject("Spaceship", p, v, a, h, r), mThrust(0)
{
}

/** Copy constructor. */
Spaceship::Spaceship(const Spaceship& s)
	: GameObject(s), mThrust(0)
{
}

/** Destructor. */
Spaceship::~Spaceship(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update this spaceship. */
void Spaceship::Update(int t)
{
	// Call parent update function
	GameObject::Update(t);

	float dt = t / 1000.0f;
	this->dt = dt;

	if (mNoCollideTime > 0) {
		mNoCollideTime -= dt;
	}

	if (bounceBulletTime > 0) {
		bounceBulletTime -= dt;
	}

}



/** Render this spaceship. */
void Spaceship::Render(void)
{
	if (mNoCollideTime > 0) {
		flashTime += dt;
		if (flashTime >= 0.001) {
			flashTime = 0;
			if (mSpaceshipShape.get() != NULL) mSpaceshipShape->Render();

			// If ship is thrusting
			if ((mThrust > 0) && (mThrusterShape.get() != NULL)) {
				mThrusterShape->Render();
			}
			GameObject::Render();
		}

	}
	else {
		if (mSpaceshipShape.get() != NULL) mSpaceshipShape->Render();

		// If ship is thrusting
		if ((mThrust > 0) && (mThrusterShape.get() != NULL)) {
			mThrusterShape->Render();
		}

		GameObject::Render();
	}
}

/** Fire the rockets. */
void Spaceship::Thrust(float t)
{
	mThrust = t;
	// Increase acceleration in the direction of ship
	mAcceleration.x = mThrust*cos(DEG2RAD*mAngle);
	mAcceleration.y = mThrust*sin(DEG2RAD*mAngle);
}

/** Set the rotation. */
void Spaceship::Rotate(float r)
{
	mRotation = r;
}

/** Shoot a bullet. */
void Spaceship::Shoot(void)
{
	// Check the world exists
	if (!mWorld) return;
	// Construct a unit length vector in the direction the spaceship is headed
	GLVector3f spaceship_heading(cos(DEG2RAD*mAngle), sin(DEG2RAD*mAngle), 0);
	spaceship_heading.normalize();
	// Calculate the point at the node of the spaceship from position and heading
	GLVector3f bullet_position = mPosition + (spaceship_heading * 4);
	// Calculate how fast the bullet should travel
	float bullet_speed = 30;
	// Construct a vector for the bullet's velocity
	GLVector3f bullet_velocity = mVelocity + spaceship_heading * bullet_speed;
	// Construct a new bullet

	if (bounceBulletTime > 0) {
		shared_ptr<GameObject> bullet
		(new Bullet(bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000,true));
		bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
		mBulletShape->setRGBColour(1.0, 1.0, 0.1);
		bullet->SetShape(mBulletShape);
		// Add the new bullet to the game world
		mWorld->AddObject(bullet);
	}
	else {
		shared_ptr<GameObject> bullet
		(new Bullet(bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000));
		bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
		mBulletShape->setRGBColour(0.1, 1.0, 0.8);
		bullet->SetShape(mBulletShape);
		// Add the new bullet to the game world
		mWorld->AddObject(bullet);
	}

	

}

void Spaceship::setNoCollideTime(float t)
{
	mNoCollideTime = t;
}


bool Spaceship::CollisionTest(shared_ptr<GameObject> o)
{
	if (getNoCollideTime() > 0) {
		return false;
	}

	if (o->GetType() == GameObjectType("Spaceship")) return false;
	if (o->GetType() == GameObjectType("Bullet")) return false;
	if (o->GetType() == GameObjectType("bonusLive")) return false;
	if (o->GetType() == GameObjectType("bonusBounceBullet")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Spaceship::OnCollision(const GameObjectList &objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
}

void Spaceship::onEatBonus()
{
	bounceBulletTime = 10;
}
