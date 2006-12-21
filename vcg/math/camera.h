/****************************************************************************
* VCGLib                                                            o o     *
* Visual and Computer Graphics Library                            o     o   *
*                                                                _   O  _   *
* Copyright(C) 2004                                                \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *   
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/
/****************************************************************************
  History
$Log: not supported by cvs2svn $
Revision 1.27  2006/12/18 16:02:55  matteodelle
minor eroor correction on variable names

Revision 1.26  2006/12/18 09:46:38  callieri
camera+shot revamp: changed field names to something with more sense, cleaning of various functions, correction of minor bugs/incongruences, removal of the infamous reference in shot.

Revision 1.25  2005/12/12 16:52:55  callieri
Added Unproject, from 2D local space + Zdepth to 3D camera space. Added ViewportToLocal, inverse of LocalToViewport

Revision 1.24  2005/12/01 01:03:37  cignoni
Removed excess ';' from end of template functions, for gcc compiling

Revision 1.23  2005/10/12 16:43:32  ponchio
Added IsOrtho...

Revision 1.22  2005/07/11 13:12:34  cignoni
small gcc-related compiling issues (typenames,ending cr, initialization order)

Revision 1.21  2005/07/01 10:55:42  cignoni
Removed default values from the implementation of SetCavalieri and SetIsometric

Revision 1.20  2005/06/29 14:59:03  spinelli
aggiunto:
- l' enum dei tipi  PERSPECTIVE,  ORTHO, ISOMETRIC,  CAVALIERI
- inline void SetCavalieri(...)
- inline void SetIsometric(...)

- modificato
- void SetOrtho( .. )

Revision 1.19  2005/02/22 10:57:58  tommyfranken
Corrected declaration and some syntax errors in GetFrustum

Revision 1.18  2005/02/21 18:11:07  ganovelli
GetFrustum moved from gl/camera to math/camera.h

Revision 1.17  2005/02/15 14:55:52  tommyfranken
added principal point

Revision 1.16  2005/01/18 16:40:50  ricciodimare
*** empty log message ***

Revision 1.15  2005/01/18 15:14:22  ponchio
Far and end are reserved.

Revision 1.14  2005/01/14 15:28:33  ponchio
vcg/Point.h -> vcg/point.h   (again!)

Revision 1.13  2005/01/05 13:25:29  ganovelli
aggiunte conversione di coordinate

Revision 1.12  2004/12/16 11:22:30  ricciodimare
*** empty log message ***

Revision 1.11  2004/12/16 11:21:03  ricciodimare
*** empty log message ***

Revision 1.10  2004/12/15 18:45:50  tommyfranken
*** empty log message ***

<<<<<<< camera.h
=======
Revision 1.8  2004/11/23 10:15:38  cignoni
removed comment in comment gcc warning

Revision 1.7  2004/11/03 09:40:53  ganovelli
Point?.h to point?.h

Revision 1.6  2004/11/03 09:32:50  ganovelli
SetPerspective and SetFrustum added (same parameters as in opengl)

>>>>>>> 1.8
Revision 1.4  2004/10/07 14:39:57  fasano
Remove glew.h include

Revision 1.3  2004/10/07 14:22:38  ganovelli
y axis reverse in projecting (!)

Revision 1.2  2004/10/05 19:04:25  ganovelli
version 5-10-2004 in progress

Revision 1.1  2004/09/15 22:58:05  ganovelli
re-creation

Revision 1.2  2004/09/06 21:41:30  ganovelli
*** empty log message ***

Revision 1.1  2004/09/03 13:01:51  ganovelli
creation

****************************************************************************/


#ifndef __VCGLIB_CAMERA
#define __VCGLIB_CAMERA

// VCG
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>
#include <vcg/math/similarity.h>

namespace vcg{

	enum {
		PERSPECTIVE =	0,
		ORTHO       =	1,
		ISOMETRIC	=   2,
		CAVALIERI	=   3
	};


template<class S>
class Camera
{
public:
	typedef S ScalarType;

	Camera():
		FocalMm(0.f),
		ViewportPx(vcg::Point2<int>(0,0)),
		PixelSizeMm(vcg::Point2<S>(0.0,0.0)),
		CenterPx(vcg::Point2<S>(0.0,0.0)),
		DistorCenterPx(vcg::Point2<S>(0.0,0.0)),
		cameraType(0)
		{}

	//------ camera intrinsics
	ScalarType	FocalMm;			/// Focal Distance: the distance between focal center and image plane. Expressed in mm
	Point2<int>	ViewportPx;			/// Dimension of the Image Plane (in pixels)
	Point2<S>	PixelSizeMm;		/// Dimension in mm of a single pixel
	Point2<S>	CenterPx;			/// Position of the projection of the focal center on the image plane. Expressed in pixels

	Point2<S>	DistorCenterPx;		/// Position of the radial distortion center on the image plane in pixels
	S			k[4];				/// 1st & 2nd order radial lens distortion coefficient (only the first 2 terms are used)
	//------------------------

	int cameraType;					/// Type of camera: PERSPECTIVE,ORTHO,ISOMETRIC,CAVALIERI

	void SetOrtho(S dist)
	{ 
		cameraType = ORTHO;
		ViewportPx = ( ((ViewportPx[0] * PixelSizeMm[0]) * (ViewportPx[1] * PixelSizeMm[1])) / FocalMm ) * dist;
	};

	bool IsOrtho() const
	{ 
		return ( cameraType == ORTHO ); 
	}

	//--- Set-up methods

	/// set the camera specifying the perspecive view
	inline void SetPerspective(S AngleDeg, S AspectRatio, S Focal, vcg::Point2<int> Viewport);

	/// set the camera specifying the cavalieri view
	inline void SetCavalieri(S sx, S dx, S bt, S tp, S Focal, vcg::Point2<int> Viewport);

	/// set the camera specifying the isometric view
	inline void SetIsometric(S sx, S dx, S bt, S tp, S Focal, vcg::Point2<int> Viewport);

	/// set the camera specifying the frustum view
	inline void SetFrustum(S dx, S sx, S bt, S tp, S Focal, vcg::Point2<int> Viewport);
	//------------------

	/// returns the frustum
	inline void GetFrustum(S & sx, S & dx, S & bt, S & tp, S & nr);

	//--- Space transformation methods 
	
	/// project a point from 3d CAMERA space to the camera local plane
	inline vcg::Point2<S> Project(const vcg::Point3<S> & p) const;

	/// unproject a point from the camera local plane (plus depth) to 3d CAMERA space 
	inline vcg::Point3<S> UnProject(const vcg::Point2<S> & p, const S & d) const;

	/// transforms local plane coords to vieport (pixel) coords	
	inline vcg::Point2<S> LocalToViewportPx(const vcg::Point2<S> & p) const;
	
	/// transforms vieport (pixel) coords to local plane coords 	
	inline vcg::Point2<S> ViewportPxToLocal(const vcg::Point2<S> & p) const;

	/// transforms local plane coords to [0 1] coords
	inline vcg::Point2<S> LocalTo_0_1(const vcg::Point2<S> & p) const;

	/// transforms local plane coords to [-1 1] coords
	inline vcg::Point2<S> LocalTo_neg1_1(const vcg::Point2<S> & p) const;
	//--------------------------------
};



/// project a point from 3d CAMERA space to the camera's plane]
template<class S>
vcg::Point2<S> Camera<S>::Project(const vcg::Point3<S> & p) const
{
	vcg::Point2<S> q =  Point2<S>(p[0],p[1]);

	if(!IsOrtho())
	{
		q[0] *= FocalMm/p.Z();
		q[1] *= FocalMm/p.Z();
	}

	return q;
}

/// unproject a point from the camera 2d plane [-1,-1]x[1,1] (plus depth) to 3d CAMERA space 
template<class S>
vcg::Point3<S> Camera<S>::UnProject(const vcg::Point2<S> & p, const S & d) const
{
	vcg::Point3<S> np = Point3<S>(p[0], p[1], d);

	if(!IsOrtho())
	{
		np[0] /= FocalMm/d;
		np[1] /= FocalMm/d;
	}

	return np;
}

/// transforms local plane coords to vieport (pixel) coords
template<class S>
vcg::Point2<S> Camera<S>::LocalToViewportPx(const vcg::Point2<S> & p) const
{
	vcg::Point2<S> np;

	np[0] = (p[0] / PixelSizeMm.X()) + CenterPx.X();
	np[1] = (p[1] / PixelSizeMm.Y()) + CenterPx.Y();

	return np;
}

/// transforms vieport (pixel) coords to local plane coords 
template<class S>
vcg::Point2<S> Camera<S>::ViewportPxToLocal(const vcg::Point2<S> & p) const
{
	vcg::Point2<S>  ps;
	ps[0] = (p[0]-CenterPx.X()) * PixelSizeMm.X();
	ps[1] = (p[1]-CenterPx.Y()) * PixelSizeMm.Y();
	return ps;
}

/// transforms local plane coords to [0-1] coords
template<class S>
vcg::Point2<S> Camera<S>::LocalTo_0_1(const vcg::Point2<S> & p) const
{
	vcg::Point2<S>  ps;
	ps[0] = ( p[0]/PixelSizeMm.X() + CenterPx.X() ) / (S)ViewportPx[0];
	ps[1] = ( p[1]/PixelSizeMm.Y() + CenterPx.Y() ) / (S)ViewportPx[1];
	return ps;
}

/// transforms local plane coords to [-1 1] coords
template<class S>
vcg::Point2<S> Camera<S>::LocalTo_neg1_1(const vcg::Point2<S> & p) const
{
	vcg::Point2<S>  ps;
	ps[0] = 2.0f * p[0] / ( PixelSizeMm.X() * (S)ViewportPx[0] );
	ps[1] = 2.0f * p[1] / ( PixelSizeMm.Y() * (S)ViewportPx[1] );
	return ps;
}

//--- basic camera setup (GL-like)

/// set the camera specifying the perspective view
template<class S>
void Camera<S>::SetPerspective(	S AngleDeg, S AspectRatio, S Focal, vcg::Point2<int> Viewport)
{
	cameraType = PERSPECTIVE;
	S halfsize[2];

	halfsize[1] = tan(math::ToRad(AngleDeg/2.0f)) * Focal;	
	halfsize[0] = halfsize[1]*AspectRatio;

	SetFrustum(-halfsize[0],halfsize[0],-halfsize[1],halfsize[1],Focal,Viewport);
}

/// set the camera specifying the frustum view
template<class S>
void Camera<S>::SetFrustum(	S sx, S dx, S bt, S tp, S Focal, vcg::Point2<int> Viewport)
{
	S vp[2];
	vp[0] = dx-sx;
	vp[1] = tp-bt;

	ViewportPx[0] = vp[0];
	if(vp[1] != -1)
		ViewportPx[1] = vp[1];			// the user specified the viewport
	else
		ViewportPx[1] = ViewportPx[0];	// default viewport

	PixelSizeMm[0] = vp[0] / (S)Viewport[0];
	PixelSizeMm[1] = vp[1] / (S)Viewport[1];

	CenterPx[0] = -sx/vp[0] * (S)Viewport[0];
	CenterPx[1] = -bt/vp[1] * (S)Viewport[1];

	FocalMm =Focal;
}

//--- special cameras setup

/// set the camera specifying the cavalieri view
template<class S>
void Camera<S>::SetCavalieri(S sx, S dx, S bt, S tp, S Focal, vcg::Point2<int> Viewport)
{
	cameraType = CAVALIERI;
	SetFrustum(sx, dx, bt, tp, Focal,Viewport);
}

/// set the camera specifying the isometric view
template<class S>
void Camera<S>::SetIsometric(S sx, S dx, S bt, S tp, S Focal, vcg::Point2<int> Viewport)
{
	cameraType = ISOMETRIC;
	SetFrustum(sx, dx, bt, tp, Focal,Viewport);
}

/// returns the frustum
template<class S>
void Camera<S>:: GetFrustum( S & sx, S & dx, S & bt, S & tp, S & nr)
{
	dx = CenterPx.X()* PixelSizeMm.X();			//scaled center
	sx = -( (S)ViewportPx.X() - CenterPx.X() ) * PixelSizeMm.X();

	bt = -CenterPx.Y()* PixelSizeMm.Y();
	tp = ( (S)ViewportPx.Y() - CenterPx.Y() ) * PixelSizeMm.Y();

	nr = FocalMm;
}

}
#endif




