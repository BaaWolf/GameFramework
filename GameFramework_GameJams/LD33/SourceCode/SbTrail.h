////////////////////////////////////////////////////////////////////////////////
// SbTrail.h

// Include guard
#pragma once

#include "BtArray.h"
#include "BtTypes.h"
#include "BaArchive.h"
#include "BtCollection.h"
#include "ShTouch.h"
#include "RsVertex.h"
#include "RsColour.h"

const BtU32 MaxTrail = 1024;

class RsMaterial;

struct MuTrailBit
{
	BtU32								m_id;
	MtVector3							m_v3Position;
	MtVector3							m_v3Cross;
	BtU32								m_tick;
	MtVector3							m_v3Left;
	MtVector3							m_v3Right;
	BtFloat								m_alpha;
	RsColour							m_colour;
};

// Class definition
class SbTrail
{
public:

	// Public functions
	void								Setup(RsMaterial *pMaterial);
	void								Add(BtU32 colourIndex, const MtVector3 &v3Position, const MtVector3 &v3Cross);
	void								Terminate();
	void								Reset();
	void								Update();
	void								Render();
	BtU32								GetNumPoints();

private:

	// Private functions

	// Private members

	// Trail
	BtCollection<MuTrailBit, MaxTrail>	m_trail;
	RsMaterial						   *m_pMaterial;
	RsVertex3							m_vertex[MaxTrail + 2];

	BtArray< MuTrailBit, MaxTrail >		m_points;
	MtVector3							m_v3Last;
};
