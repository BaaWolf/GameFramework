////////////////////////////////////////////////////////////////////////////////
/// DyWorld.cpp

#include "BtBase.h"
#include "DyImpl.h"
#include "DyWorld.h"
#include "BtTime.h"
#include "ApConfig.h"

#include "btCollisionAlgorithm.h"
#include "btDefaultCollisionConfiguration.h"
#include "btCollisionDispatcher.h"
#include "btSimpleBroadphase.h"
#include "btAxisSweep3.h"
#include "btSequentialImpulseConstraintSolver.h"
#include "btDiscreteDynamicsWorld.h"
#include "btGImpactCollisionAlgorithm.h"
#include "btSoftRigidDynamicsWorld.h"
#include "btSoftBody.h"

struct YourOwnFilterCallback : public btOverlapFilterCallback
{
	// return true when pairs need collision
	virtual bool needBroadphaseCollision(btBroadphaseProxy* proxy0,btBroadphaseProxy* proxy1) const
	{
		return false;
	}
};

////////////////////////////////////////////////////////////////////////////////
// Create

static btVector3 gWorldAabbMin( -100.0f, -100.0f, -100.0f );
static btVector3 gWorldAabbMax(  100.0f,  100.0f,  100.0f );
static unsigned int gMaxHandles = 32700;

//static
void DyWorld::Create()
{
	// Setup default memory usage.
	btDefaultCollisionConstructionInfo DefaultCollisionConstructionInfo;
	DefaultCollisionConstructionInfo.m_stackAlloc = NULL;
	DefaultCollisionConstructionInfo.m_persistentManifoldPool = (0);
	DefaultCollisionConstructionInfo.m_collisionAlgorithmPool = (0);
	DefaultCollisionConstructionInfo.m_defaultMaxPersistentManifoldPoolSize = 1000;
	DefaultCollisionConstructionInfo.m_defaultMaxCollisionAlgorithmPoolSize = 1000;
	DefaultCollisionConstructionInfo.m_defaultStackAllocatorSize = 1000;

		// Create the default collision configuration
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration( DefaultCollisionConstructionInfo );

	// Default collision dispatcher
	m_pCollisionDispatcher = new btCollisionDispatcher( m_pCollisionConfiguration );

	// Register GIMPACT
	btGImpactCollisionAlgorithm::registerAlgorithm( m_pCollisionDispatcher );

	// Create the broad phase interface
	m_pBroadphaseInterface = new btAxisSweep3( gWorldAabbMin, gWorldAabbMax, gMaxHandles );

	// Create the constraint solver
	m_pConstraintSolver = new btSequentialImpulseConstraintSolver();

	// Create the dynamics world
	// m_pDynamicsWorld = new btDiscreteDynamicsWorld( m_pCollisionDispatcher, m_pBroadphaseInterface, m_pConstraintSolver, m_pCollisionConfiguration );
	m_pDynamicsWorld = new btSoftRigidDynamicsWorld(m_pCollisionDispatcher, m_pBroadphaseInterface, m_pConstraintSolver, m_pCollisionConfiguration);


	Gravity( MtVector3( 0, 0, 0) );

    if( ApConfig::GetDevice() == ApDevice_WIN )
    {
        m_tick = 1.0f / 60.0f;
    }
    else
    {
        m_tick = 1.0f / 30.0f;
    }
    
	m_speed = 1.0f;

	m_subSteps = 3.0f;
}

////////////////////////////////////////////////////////////////////////////////
// Update

//static
void DyWorld::Update()
{
	BtBool m_paused = ApConfig::IsPaused();// && DyImpl::IsPaused();

	BtAssert( m_pDynamicsWorld != NULL );

	BtFloat tick = m_tick * m_speed;

	if( m_paused == BtFalse )
	{
		m_pDynamicsWorld->stepSimulation( tick, m_subSteps, tick / (BtFloat) m_subSteps );
	}
	else
	{
		m_pDynamicsWorld->stepSimulation( 0, 2.0f );
	}
}

////////////////////////////////////////////////////////////////////////////////
// RegisterBroadphaseCollision

void DyWorld::RegisterBroadphaseCollision()
{
	btOverlapFilterCallback * filterCallback = new YourOwnFilterCallback();
	m_pDynamicsWorld->getPairCache()->setOverlapFilterCallback(filterCallback);
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

//static
void DyWorld::Destroy()
{
	delete m_pDynamicsWorld;
	delete m_pConstraintSolver;
	delete m_pBroadphaseInterface;
	delete m_pCollisionDispatcher;
	delete m_pCollisionConfiguration;

	m_pDynamicsWorld = BtNull;
	m_pConstraintSolver = BtNull;
	m_pBroadphaseInterface = BtNull;
	m_pCollisionDispatcher = BtNull;
	m_pCollisionConfiguration = BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// AddRigidBody

void DyWorld::AddRigidBody( btRigidBody* pRigidBody )
{
	BtAssert( m_pDynamicsWorld != BtNull );

	m_pDynamicsWorld->addRigidBody( pRigidBody );
}

////////////////////////////////////////////////////////////////////////////////
// RemoveSoftBody

void DyWorld::RemoveSoftBody(btSoftBody* pSoftBody)
{
	btSoftRigidDynamicsWorld *pSoftWorld = (btSoftRigidDynamicsWorld*)m_pDynamicsWorld;
	pSoftWorld->removeSoftBody(pSoftBody);
}

////////////////////////////////////////////////////////////////////////////////
// RemoveRigidBody

void DyWorld::RemoveRigidBody( btRigidBody* pRigidBody )
{
	m_pDynamicsWorld->removeRigidBody( pRigidBody );
}

////////////////////////////////////////////////////////////////////////////////
// RayCast

const btRigidBody* DyWorld::RayCast( const MtVector3& v3From,
									 const MtVector3& v3To,
									 DyCollision& collision
							 )
{
	btVector3 from = btVector3( v3From.x, v3From.y, v3From.z );
	btVector3 to   = btVector3( v3To.x,   v3To.y,   v3To.z );

	btCollisionWorld::ClosestRayResultCallback rayCallback( from, to );

	m_pDynamicsWorld->rayTest( from, to, rayCallback );

	if( rayCallback.hasHit() )
	{
		const btRigidBody* pBody = btRigidBody::upcast(rayCallback.m_collisionObject);

		if( pBody )
		{
			collision.m_hitPointInWorld = MtVector3( rayCallback.m_hitPointWorld.getX(),
													 rayCallback.m_hitPointWorld.getY(),
													 rayCallback.m_hitPointWorld.getZ() );

			collision.m_hitNormalInWorld = MtVector3( rayCallback.m_hitNormalWorld.getX(),
													  rayCallback.m_hitNormalWorld.getY(),
													  rayCallback.m_hitNormalWorld.getZ() );

			collision.m_hitNormalInWorld.Normalise();

			collision.m_distFraction = rayCallback.m_closestHitFraction;

			collision.m_pUserPointer = (void*)pBody->getUserPointer();

			return pBody;
		}
		else
		{
			collision.m_pUserPointer = BtNull;
		}
	}
	else
	{
		collision.m_pUserPointer = BtNull;
	}
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// GetUserPointerFromRayCast

const void*	DyWorld::GetUserPointerFromRayCast( const MtVector3& from, const MtVector3& to, DyCollision& collision )
{
	const btRigidBody *pRigidBody = RayCast( from, to, collision );
	if( pRigidBody )
	{
		return (void*)pRigidBody->getUserPointer();
	}
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// SetSubsteps

void DyWorld::SetSubsteps( BtU32 subSteps )
{
	m_subSteps = subSteps;
}

////////////////////////////////////////////////////////////////////////////////
// MetersPerSecondToKph

BtFloat DyWorld::MetersPerSecondToKph( BtFloat SpeedInMetersPerSecond )
{
	return SpeedInMetersPerSecond * 3.6f;
}

////////////////////////////////////////////////////////////////////////////////
// KnotsToMetersPerSecond

BtFloat DyWorld::KnotsToMetersPerSecond(BtFloat SpeedInMetersPerSecond)
{
	return 1.0f / SpeedInMetersPerSecond * 1.944f;
}

////////////////////////////////////////////////////////////////////////////////
// MetersPerSecondToKnots

BtFloat DyWorld::MetersPerSecondToKnots( BtFloat SpeedInMetersPerSecond )
{
	return SpeedInMetersPerSecond * 1.944f;
}

////////////////////////////////////////////////////////////////////////////////
// MetersPerSecondToMph

BtFloat DyWorld::MetersPerSecondToMph( BtFloat SpeedInMetersPerSecond )
{
	return SpeedInMetersPerSecond * 2.23694f;
}

////////////////////////////////////////////////////////////////////////////////
// Gravity

MtVector3 DyWorld::GetGravity() const
{
	btVector3 gravity = m_pDynamicsWorld->getGravity();
	return MtVector3( gravity.getX(), gravity.getY(), gravity.getZ() );
}

////////////////////////////////////////////////////////////////////////////////
// Gravity

void DyWorld::Gravity( const MtVector3& v3Gravity )
{
	btVector3 gravity( v3Gravity.x, v3Gravity.y, v3Gravity.z );
	m_pDynamicsWorld->setGravity( gravity );
}