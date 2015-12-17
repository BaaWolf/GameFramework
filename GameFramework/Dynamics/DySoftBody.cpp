////////////////////////////////////////////////////////////////////////////////
/// DySoftBody.cpp

#include "DySoftBody.h"
#include "btRigidBody.h"
#include "btMotionState.h"
#include "btDefaultMotionState.h"
#include "btCompoundShape.h"
#include "btCylinderShape.h"
#include "btBoxShape.h"
#include "btSphereShape.h"
#include "btTriangleIndexVertexArray.h"
#include "btGImpactShape.h"
#include "btBvhTriangleMeshShape.h"
#include "btRigidBody.h"
#include "btSoftBody.h"
#include "btSphereShape.h"
#include "btDynamicsWorld.h"
#include "btConvexHullShape.h"
#include "DyImpl.h"
#include "SgNode.h"
#include "SgCollision.h"
#include "SgRigidBody.h"
#include "BtString.h"
#include "HlDraw.h"
#include "btSoftBodyHelpers.h"
#include "btSoftRigidDynamicsWorld.h"
#include "btSoftBodyRigidBodyCollisionConfiguration.h"
#include "btDbvtBroadphase.h"
#include "DyBody.h"

btSoftBodyWorldInfo worldInfo;

// http://bulletphysics.org/Bullet/phpBB3/viewtopic.php?t=10412

////////////////////////////////////////////////////////////////////////////////
// Constructor

DySoftBody::DySoftBody()
{
	m_pSoftBody = BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// CreatePatch

void DySoftBody::CreatePatch(MtVector3 v3A, MtVector3 v3B, BtFloat width, BtU32 segments, BtBool anchorA, BtBool anchorB, BtFloat mass)
{
	DyWorld *pDynamicsWorld = DyImpl::pWorld();

	worldInfo.air_density = 1.0f;
	worldInfo.m_gravity = pDynamicsWorld->GetDynamicsWorld()->getGravity();
	worldInfo.m_broadphase = pDynamicsWorld->m_pBroadphaseInterface;
	worldInfo.m_dispatcher = pDynamicsWorld->m_pCollisionDispatcher;
	worldInfo.m_sparsesdf.Initialize();
	worldInfo.m_sparsesdf.Reset();

	// http://bulletphysics.org/Bullet/phpBB3/viewtopic.php?t=10412
	btVector3 p1(v3A.x - width, v3A.y, v3A.z);
	btVector3 p2(v3B.x - width, v3B.y, v3B.z);
	btVector3 p3(v3A.x + width, v3A.y, v3A.z);
	btVector3 p4(v3A.x + width, v3A.y, v3A.z);

	BtU32 mask = 0;
	if(anchorA)
	{
		mask = mask | 1;
	}
	if(anchorB)
	{
		mask = mask | 2;
	}

	m_pSoftBody = btSoftBodyHelpers::CreatePatch(worldInfo, p1, p2, p3, p4, 2, segments, anchorA | anchorB, BtFalse );
	m_pSoftBody->m_cfg.piterations = 7;
	m_pSoftBody->m_materials[0]->m_kLST = 0.9f;
	m_pSoftBody->setTotalMass(mass);
	m_pSoftBody->addForce(btVector3(0, 0, -20));

	btSoftRigidDynamicsWorld* world = (btSoftRigidDynamicsWorld*)pDynamicsWorld->GetDynamicsWorld();
	world->addSoftBody(m_pSoftBody);

	m_pSoftBody->setFriction(0.2f);
	m_pSoftBody->setRestitution(0);
	m_pSoftBody->setActivationState(DISABLE_DEACTIVATION);
	m_pSoftBody->setUserPointer(this);
}

////////////////////////////////////////////////////////////////////////////////
// CreateRope

void DySoftBody::CreateRope( MtVector3 v3A, MtVector3 v3B, BtU32 segments, BtBool anchorA, BtBool anchorB, BtFloat mass )
{
	DyWorld *pDynamicsWorld = DyImpl::pWorld();

	worldInfo.air_density = 1.0f;

	if( mass == 0 )
	{
		mass = 0.01f;
		worldInfo.m_gravity = btVector3( 0, 0, 0 );
	}
	else
	{
		worldInfo.m_gravity = pDynamicsWorld->GetDynamicsWorld()->getGravity();
	}
	
	worldInfo.m_broadphase = pDynamicsWorld->m_pBroadphaseInterface;
	worldInfo.m_dispatcher = pDynamicsWorld->m_pCollisionDispatcher;
	worldInfo.m_sparsesdf.Initialize();
	worldInfo.m_sparsesdf.Reset();

	// http://bulletphysics.org/Bullet/phpBB3/viewtopic.php?t=10412
	btVector3 p1( v3A.x, v3A.y, v3A.z );
	btVector3 p2( v3B.x, v3B.y, v3B.z );
	 
	BtU32 mask = 0;
	if( anchorA )
	{
		mask = mask | 1;
	}
	if( anchorB )
	{
		mask = mask | 2;
	}

	m_pSoftBody = btSoftBodyHelpers::CreateRope( worldInfo, p1, p2, segments, mask );
	m_pSoftBody->m_cfg.piterations = 8;
	m_pSoftBody->m_materials[0]->m_kLST = 0.9f;
	m_pSoftBody->m_cfg.kDP = 0.1f;
	//m_pSoftBody->m_materials[0]->m_kLST = 0.1f;
	m_pSoftBody->setTotalMass(mass);
	
	btSoftRigidDynamicsWorld* world = (btSoftRigidDynamicsWorld*)pDynamicsWorld->GetDynamicsWorld();
	world->addSoftBody(m_pSoftBody);

	m_pSoftBody->setFriction(0.2f);
	m_pSoftBody->setRestitution(0);
	m_pSoftBody->setActivationState(DISABLE_DEACTIVATION);
	m_pSoftBody->setUserPointer(this);
}

////////////////////////////////////////////////////////////////////////////////
// Detach

void DySoftBody::Detach()
{
	if( m_pSoftBody )
	{
		m_pSoftBody->m_anchors.clear();
	}
}

////////////////////////////////////////////////////////////////////////////////
// Move

void DySoftBody::Move( MtVector3 v3Pos )
{
	if( m_pSoftBody )
	{
		m_pSoftBody->m_nodes[0].m_x = btVector3( v3Pos.x, v3Pos.y, v3Pos.z );
	}
}

////////////////////////////////////////////////////////////////////////////////
// AttachFirst

void DySoftBody::AttachFirst( DyBody *pBody )
{
	if(m_pSoftBody)
	{
		BtU32 count = 0;
		m_pSoftBody->appendAnchor( count, pBody->m_pRigidBody );
	}
}

////////////////////////////////////////////////////////////////////////////////
// AttachLast

void DySoftBody::AttachLast(DyBody *pBody)
{
	BtU32 count = m_pSoftBody->m_nodes.size() - 1;
	m_pSoftBody->appendAnchor(count, pBody->m_pRigidBody);
}

////////////////////////////////////////////////////////////////////////////////
// Update

void DySoftBody::Update()
{
}

////////////////////////////////////////////////////////////////////////////////
// SetWorldTransform

void DySoftBody::SetWorldTransform(const MtMatrix4& m4WorldTransform)
{
	BtAssert( m_pSoftBody != BtNull);

	btTransform transform;
	transform.setFromOpenGLMatrix(&m4WorldTransform[0][0]);
	m_pSoftBody->setWorldTransform(transform);
}

////////////////////////////////////////////////////////////////////////////////
// GetWorldTransform

MtMatrix4 DySoftBody::GetWorldTransform() const
{
	MtMatrix4 m4Transform;
	btTransform transform;
	transform = m_pSoftBody->getWorldTransform();
	transform.getOpenGLMatrix(&m4Transform[0][0]);
	return m4Transform;
}

////////////////////////////////////////////////////////////////////////////////
// GetPosition

MtVector3 DySoftBody::GetPosition() const
{
	BtU32 count = m_pSoftBody->m_links.size();
	for(BtU32 i = 0; i < count; i++ )
	{
		const btSoftBody::Link&	l = m_pSoftBody->m_links[i];
		btVector3 b3P1 = l.m_n[0]->m_x;
		MtVector3 v3A(b3P1.x(), b3P1.y(), b3P1.z());
		return v3A;
	}
	return MtVector3( 0, 0, 0 );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void DySoftBody::Render()
{
	if( m_pSoftBody )
	{
		BtU32 numFaces = m_pSoftBody->m_faces.size();
		for( BtU32 i = 0; i < numFaces; ++i)
		{
			int a=0;
			a++;
		}

		BtU32 count = m_pSoftBody->m_links.size();
		for( BtU32 i = 0; i < count; ++i)
		{
			const btSoftBody::Link&	l = m_pSoftBody->m_links[i];

			btVector3 b3P1 = l.m_n[0]->m_x;
			btVector3 b3P2 = l.m_n[1]->m_x;
			MtVector3 v3A( b3P1.x(), b3P1.y(), b3P1.z() );
			MtVector3 v3B( b3P2.x(), b3P2.y(), b3P2.z() );
			HlDraw::RenderLine( v3A, v3B, RsColour( 1.0f, 0.6f, 0, 1.0f ), MaxSortOrders-1 );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void DySoftBody::Destroy()
{
	if( m_pSoftBody )
	{
		// Clear the user pointer
		m_pSoftBody->setUserPointer(BtNull);

		// Remove the soft body
		DyImpl::RemoveSoftBody(m_pSoftBody);

		m_pSoftBody = BtNull;
	}
}
