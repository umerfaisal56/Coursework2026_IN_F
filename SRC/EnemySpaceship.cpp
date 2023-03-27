#include "GameUtil.h"
#include "GameWorld.h"
#include "Bullet.h"
#include "EnemySpaceship.h"
#include "BoundingSphere.h"

using namespace std;

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/**  Default constructor. */
EnemySpaceship::EnemySpaceship()
	: GameObject("EnemySpaceship"), mThrust(0)
{
}

/** Construct a spaceship with given position, velocity, acceleration, angle, and rotation. */
EnemySpaceship::EnemySpaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r)
	: GameObject("EnemySpaceship", p, v, a, h, r), mThrust(0)
{
}

/** Copy constructor. */
EnemySpaceship::EnemySpaceship(const EnemySpaceship& s)
	: GameObject(s), mThrust(0)
{
}

/** Destructor. */
EnemySpaceship::~EnemySpaceship(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update this spaceship. */
void EnemySpaceship::Update(int t)
{
	// Call parent update function
	GameObject::Update(t);
	float dt = t / 1000.0f;

	shootTime += dt;
	if (shootTime >= 1.f) {
		shootTime = 0;
		Shoot();
	}

	rotateTime += dt;
	if (rotateTime >= rotateResetTime) {
		rotateTime = 0;
		rotateResetTime = rand() % 3;
		if (rand() % 5 == 1) {
			Rotate(-90);

		}
		else {
			Rotate(90);
		}
	}

	thrustTime += dt;
	if (thrustTime >= 1.0f) {
		thrustTime = 0;
		//thrustTimeReset = rand() % 3;
		Thrust(10);
	}

}

/** Render this spaceship. */
void EnemySpaceship::Render(void)
{
	if (mSpaceshipShape.get() != NULL) mSpaceshipShape->Render();

	// If ship is thrusting
	if ((mThrust > 0) && (mThrusterShape.get() != NULL)) {
		mThrusterShape->Render();
	}

	GameObject::Render();
}

/** Fire the rockets. */
void EnemySpaceship::Thrust(float t)
{
	mThrust = t;
	// Increase acceleration in the direction of ship
	mAcceleration.x = mThrust*cos(DEG2RAD*mAngle);
	mAcceleration.y = mThrust*sin(DEG2RAD*mAngle);
}

/** Set the rotation. */
void EnemySpaceship::Rotate(float r)
{
	mRotation = r;
}

/** Shoot a bullet. */
void EnemySpaceship::Shoot(void)
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
	shared_ptr<GameObject> bullet
		(new Bullet("EnemyBullet", bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000));
	bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
	bullet->SetShape(mBulletShape);
	// Add the new bullet to the game world
	mWorld->AddObject(bullet);

}

bool EnemySpaceship::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() == GameObjectType("Asteroid")) return false;
	if (o->GetType() == GameObjectType("EnemyBullet")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void EnemySpaceship::OnCollision(const GameObjectList &objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
	FireEnemyKilled();
}

void EnemySpaceship::setTarget(shared_ptr<GameObject> target)
{
	this->target = target;
}

void EnemySpaceship::FireEnemyKilled()
{
	// Send message to all listeners
	for (EnemyListenerList::iterator lit = mListeners.begin();
		lit != mListeners.end(); ++lit) {
		(*lit)->OnEnemyKilled(0);
	}
}

void EnemySpaceship::AddListener(shared_ptr<IEnemyListener> listener)
{
	mListeners.push_back(listener);
}
