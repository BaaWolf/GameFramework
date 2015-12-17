////////////////////////////////////////////////////////////////////////////////
// MtLine.h

// Include guard
#ifndef __MtLine_h__
#define __MtLine_h__

// Forward declarations
#include "MtVector2.h"
#include "MtVector3.h"
#include "MtTriangle.h"
#include "MtPlane.h"

// Class definition
struct MtLine
{
       MtLine()
       {
          point1 = point2 = MtVector3(0, 0, 0);
       }

       MtLine(MtVector3 a, MtVector3 b)
       {
          point1 = a;
          point2 = b;
       }
		
	   // Public functions
	   void				FindLineRadius();
	   BtBool			Collide( const MtTriangle &triangle, MtVector3 &intersection );
	   BtBool			CollideWithPlane( const MtPlane &plane, MtVector3 &intersection );
	   static BtBool	Intersects( MtVector2 l1p1, MtVector2 l1p2, MtVector2 l2p1, MtVector2 l2p2 );

	   static BtBool	IntersectLineCircle( const MtVector2& c,	// center
										     BtFloat r,             // radius
										     MtLine& line );	    // segment end

	   // Member variables
       MtVector3		point1;
       MtVector3		point2;
       MtVector3		center;
       BtFloat			m_fRadius;
};

#endif // __MtLine_h__