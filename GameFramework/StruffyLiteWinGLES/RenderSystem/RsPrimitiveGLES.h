///////////////////////////////////////////////////////////////////////////////
// RsPrimitiveWin32GL.h

#ifndef __RsPrimitiveWin32GL_h__
#define __RsPrimitiveWin32GL_h__

// Includes
#include "BtTypes.h"
#include "RsPrimitive.h"
#include "RsGLES.h"

struct RsPrimitiveWinGL : public RsPrimitive
{
	GLenum			 					m_primitiveType;
	BtU32								m_numVertex;
	BtU32								m_nStartVertex;
};

struct RsIndexedPrimitiveWinGL : public RsIndexedPrimitive
{
	GLenum			 					m_primitiveType;
    BtU32 								m_baseVertexIndex;
    BtU32 								m_minIndex;
    BtU32 								m_numVertices;
    BtU32 								m_startIndex;
    BtU32								m_primitives;
	BtU32								m_numIndices;
};

#endif // __RsPrimitiveWin32GL_h__
