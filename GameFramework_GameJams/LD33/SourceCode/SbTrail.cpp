////////////////////////////////////////////////////////////////////////////////
// SbTrail.cpp

// Includes
#include <stdio.h>
#include "BtTime.h"
#include "SbTrail.h"
#include "RsMaterial.h"
#include "RsUtil.h"
#include "SbWorld.h"

//const BtFloat TrailDuration = 1.0f;
const BtFloat TrailDuration = 4.0f;

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbTrail::Setup(RsMaterial *pMaterial)
{
	m_pMaterial = pMaterial;
	m_pMaterial->SetTechniqueName("RsShaderLC");
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void SbTrail::Reset()
{
	m_trail.Empty();
	m_points.Empty();
}

////////////////////////////////////////////////////////////////////////////////
// Add

void SbTrail::Add(BtU32 colourIndex, const MtVector3 &v3Position, const MtVector3 &v3Cross)
{
	RsColour colour(RsColour::WhiteColour());

	if( v3Position.y > SeaLevel )
	{
		colour = RsColour( 0.4f, 0.4f, 0.4f, 1.0f );
	}
	else
	{
		colour = RsColour(0.4f, 0.4f, 1.0f, 1.0f);
	}

	MtVector2 v2Delta = v3Position - m_v3Last;

	if((v2Delta.GetLength() > 0.01f))
	{
		MuTrailBit *pTrailBit = m_trail.AddTail();

		pTrailBit->m_v3Position = v3Position;
		pTrailBit->m_v3Cross = v3Cross;
		pTrailBit->m_tick = 0;
		pTrailBit->m_id = 1;
		pTrailBit->m_colour = colour;

		m_v3Last = v3Position;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Terminate

void SbTrail::Terminate()
{
	if(m_trail.GetNumElements() > 0)
	{
		MuTrailBit *pTrailBit = m_trail.GetTail();

		if(pTrailBit)
		{
			pTrailBit->m_id = 0;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbTrail::Update()
{
	const BtFloat tick = BtTime::GetTick();

	BtCollectionIterator< MuTrailBit, MaxTrail > iterator(m_trail);

	for(MuTrailBit* pTrail = iterator.GetFirstElement(); pTrail; pTrail = iterator.GetNextElement())
	{
		pTrail->m_tick++;

		if(((BtFloat)pTrail->m_tick) * tick > TrailDuration)
		{
			iterator.DeleteElement();
		}
	}

	m_points.Empty();

	MuTrailBit *pTrail = iterator.GetTailElement();

	if(pTrail != BtNull)
	{
		// Cache the last point
		MtVector3 v3Last = pTrail->m_v3Position;

		const BtFloat width = 0.1f;

		// Extract the points to an array
		for(; pTrail; pTrail = iterator.GetPreviousElement())
		{
			MtVector3 v3Position = pTrail->m_v3Position;

			BtFloat alpha = 1.0f - (((BtFloat)pTrail->m_tick) * BtTime::GetTick() / TrailDuration);

			MuTrailBit trail;
			trail.m_alpha = alpha;

			MtVector3 v3In(0, 0, 1);

			MtVector3 v3Result = pTrail->m_v3Cross.GetNormalise();

			v3Result = v3Result * width;

			trail.m_v3Left = v3Position - v3Result;
			trail.m_v3Right = v3Position + v3Result;
			trail.m_id = pTrail->m_id;
			trail.m_colour = pTrail->m_colour;
			trail.m_colour.Alpha(alpha * 0.5f);

			m_points.Add(trail);

			v3Last = v3Position;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbTrail::Render()
{
	BtU32 numItems = m_points.GetNumItems();

	if(numItems < 2)
	{
		return;
	}

	numItems = m_points.GetNumItems();

	BtU32 vertex = 0;
	BtU32 i = 0;

	while(i < numItems)
	{
		if(m_points[i].m_id == 0)
		{
			if(vertex > 3)
			{
				m_pMaterial->Render(RsPT_TriangleStrip, m_vertex, vertex, MaxSortOrders-1 );
			}
			vertex = 0;
		}
		else
		{
			m_vertex[vertex].m_v3Position = m_points[i].m_v3Left;
			m_vertex[vertex].m_colour = m_points[i].m_colour.asWord();
			++vertex;

			m_vertex[vertex].m_v3Position = m_points[i].m_v3Right;
			m_vertex[vertex].m_colour = m_points[i].m_colour.asWord();
			++vertex;
		}

		++i;
	}

	if(vertex > 3)
	{
		m_pMaterial->Render(RsPT_TriangleStrip, m_vertex, vertex, ParticleFXSortOrder );
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetNumTrails

BtU32 SbTrail::GetNumPoints()
{
	return m_trail.GetNumElements();
}