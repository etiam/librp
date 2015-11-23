//
// $Source: /u/jasonr/stuff/pt/RCS/rr.C,v $
// $Revision: 1.4 $
// $Author: jasonr $
// $Date: 1997/12/10 23:08:28 $
//

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "defs.h"
//#include "macros.h"
//#include "vector.h"
//#include "matrix.h"
//#include "linkedlist.h"
//#include "stack.h"
//#include "attribute.h"
//#include "polygon.h"
//#include "pointspolygons.h"
//#include "polylist.h"
//#include "arealight.h"
//#include "pointlight.h"
//#include "sphere.h"

#include "rr.h"

//extern World		wWorld;
//Stack			sTransform,sAttribute;
//Matrix			mCurrent;
//Attribute		aCurrent;

RtVoid RrAttribute(RtToken name, RtInt n, RtToken tokens[], RtPointer parms[],
		    RtInt plengths[])
{
// if(strcmp(name,"identifier") == 0)
//  aCurrent.sName = strdup((char*)parms[0]);
}

RtVoid RrAttributeBegin()
{
// aCurrent.mCurrent = mCurrent;
// sAttribute.Push((void*)new Attribute(aCurrent));
}

RtVoid RrAttributeEnd()
{
// aCurrent = *(Attribute*)sAttribute.Pop();
// mCurrent = aCurrent.mCurrent;
}

RtVoid RrBegin(RtToken)
{
// mCurrent = identity();
}

RtVoid RrColor(RtColor color)
{
// aCurrent.cColor = Color(color[0],color[1],color[2]);
}

RtVoid RrConcatTransform(RtMatrix matrix)
{
// int		i,j;
// Matrix		mTemp;
//
// for(i=0; i < 4; i++)
//  for(j=0; j < 4; j++)
//   mTemp.m[j][i] = matrix[i][j];
//
// mCurrent *= mTemp;
}

RtVoid RrCropWindow(RtFloat left, RtFloat right, RtFloat top, RtFloat bot)
{
}

RtVoid RrCylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat tmax)
{
}

RtToken RrDeclare(char *name, char *declaration)
{
 return(0);
}

RtVoid RrDisplayV(RtString name, RtToken type, RtToken mode,
		  RtInt n, RtToken tokens[], RtPointer parms[],
		  RtInt plengths[])
{
}

RtVoid RrEnd()
{
}

RtVoid RrFrameAspectRatio(RtFloat aspect)
{
}

RtVoid RrFormat(RtInt xres, RtInt yres, RtFloat aspect)
{
}

RtLightHandle RrLightSourceV(RtToken name, RtInt n, RtToken tokens[],
			     RtPointer parms[], RtInt plenghts[])
{
 return(0);
}

RtVoid RrNuPatch(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin,
                 RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[],
                 RtFloat vmin, RtFloat vmax,
                 RtInt n, RtToken tokens[], RtPointer parms[],
		 RtInt plengths[])
{
}

RtVoid RrPixelSamples(RtFloat xsamples, RtFloat ysamples)
{
}

RtVoid RrPointsPolygonsV(RtInt npolys, RtInt nverts[], RtInt verts[],
			 RtInt n, RtToken tokens[], RtPointer parms[],
			 RtInt plengths[])
{
// int			i,j,k = 0;
// float                  *fP,*fN,*fST;
// double			dArea = 0;
// Point          	pMin(MAXFLOAT),pMax(MINFLOAT);
// PointsPolygons		pointspolygons;
// PolyList		*plPolys;
// AreaLight		*alLight;
// PointLight		*plLight;
// Sphere			*sSphere;
//// LinkedList		*llGeometryPtr = &wWorld.llGeometry;
//// LinkedList		*llLightsPtr = &wWorld.llLights;
//
// // Parse arguments
// pointspolygons.parse(npolys,nverts,verts);
// pointspolygons.Parms::parse(n,tokens,parms,plengths);
//
// // Check if current shader is "arealight"
// if(strstr(aCurrent.sShader->sName,"arealight") != NULL)
// {
//  alLight = new AreaLight;
//  if(npolys > 1)
//  {
//   std::cerr << "ptrace: warning, arealights defined by single polygon, ignoring";
//   std::cerr << " all but first";
//  }
//
//  // Copy current attributes to this object
//  alLight->aAttribute = aCurrent;
//
//  // Get illuminance value from shader
//  for(i=0; i < aCurrent.sShader->n; i++)
//  {
//   if(strcmp(aCurrent.sShader->tokens[i],"Illuminance") == 0)
//    alLight->dIlluminance = (double)(*(float*)aCurrent.sShader->parms[i]);
//   if(strcmp(aCurrent.sShader->tokens[i],"Coneangle") == 0)
//    alLight->dConeAngle = (double)(*(float*)aCurrent.sShader->parms[i]);
//  }
//
//  // Get pointer to P
//  for(i=0; i < n; i++)
//  {
//   if(strcmp(tokens[i],"P") == 0)
//    fP = (float*)parms[i];
//  }
//
//  // Alloc space for polygon
//  alLight->pLight.iCount = nverts[0];
//  alLight->pLight.vList = new Point[nverts[0]];
//
//  // Store P & calculate center
//  alLight->pLight.pCenter = 0.0;
//  for(j=0; j < nverts[0]; j++)
//  {
//   // Vertices
//   alLight->pLight.vList[j].x = fP[verts[j]*3];
//   alLight->pLight.vList[j].y = fP[verts[j]*3+1];
//   alLight->pLight.vList[j].z = fP[verts[j]*3+2];
//   alLight->pLight.pCenter += alLight->pLight.vList[j];
//  }
//  alLight->pLight.pCenter /= (double)nverts[0];
//
//  // Transform light to camera
//  alLight->TransformtoCamera(mCurrent);
//
//  // Calculate a length dependent on size of light (for display purposes)
//  alLight->dLightSize = Length(alLight->pLight.vList[0]-
//		      	       alLight->pLight.vList[1]) * 0.2;
//
//  // Calculate normal
//  alLight->vNormal = (alLight->pLight.vList[0] - alLight->pLight.vList[1]) %
//	             (alLight->pLight.vList[2] - alLight->pLight.vList[1]);
//  alLight->vNormal = Normalize(alLight->vNormal);
//
//  // Calculate pP,vU & vV;
//  alLight->pP = alLight->pLight.vList[0];
//  alLight->vU = alLight->pLight.vList[1] - alLight->pLight.vList[0];
//  alLight->vV = alLight->pLight.vList[2] - alLight->pLight.vList[1];
//
//  // Store in display list
////  llLightsPtr->Add((void*)alLight);
// }
//
// // Check if current shader is "sphere" type
// else if(strstr(aCurrent.sShader->sName,"sphere") != NULL)
// {
//  sSphere = new Sphere;
//  sSphere->aAttribute = aCurrent;
//  sSphere->pCenter = 0.0;
//
//  // Get pointer to P
//  for(i=0; i < n; i++)
//  {
//   if(strcmp(tokens[i],"P") == 0)
//    fP = (float*)parms[i];
//  }
//
//  // Find centroid
//  for(i=0,j=0; i < npolys; i++)
//   j += nverts[i];
//  for(i=0,k=0; i < j; i++)
//   k = MAX(verts[i],k);
//  k++;
//  for(i=0; i < k*3; i+=3)
//   sSphere->pCenter += (mCurrent * Point(fP[i],fP[i+1],fP[i+2]));
//  sSphere->pCenter /= (double)k;
//  sSphere->dRadius = Length(sSphere->pCenter-
//			    (mCurrent*Point(fP[0],fP[1],fP[2])));
//
//  // Store in display list
////  llGeometryPtr->Add((void*)sSphere);
// }
//
// // Check if current shader is "pointlight" type
// else if(strstr(aCurrent.sShader->sName,"pointlight") != NULL)
// {
//  plLight = new PointLight;
//  plLight->aAttribute = aCurrent;
//  plLight->pCenter = 0.0;
//
//  // Get illuminance value from shader
//  for(i=0; i < aCurrent.sShader->n; i++)
//  {
//   if(strcmp(aCurrent.sShader->tokens[i],"Illuminance") == 0)
//    plLight->dIlluminance = (double)(*(float*)aCurrent.sShader->parms[i]);
//  }
//
//  // Get pointer to P
//  for(i=0; i < n; i++)
//  {
//   if(strcmp(tokens[i],"P") == 0)
//    fP = (float*)parms[i];
//  }
//
//  // Find centroid
//  for(i=0,j=0; i < npolys; i++)
//   j += nverts[i];
//  for(i=0,k=0; i < j; i++)
//   k = MAX(verts[i],k);
//  k++;
//  for(i=0; i < k*3; i+=3)
//   plLight->pCenter += (mCurrent * Point(fP[i],fP[i+1],fP[i+2]));
//  plLight->pCenter /= (double)k;
//  plLight->dRadius = Length(plLight->pCenter-
//			    (mCurrent*Point(fP[0],fP[1],fP[2])));
//
//  // Store in display list
////  llLightsPtr->Add((void*)plLight);
// }
//
// // Otherwise it's regular geometry
// else
// {
//  plPolys = new PolyList;
//  plPolys->aAttribute = aCurrent;
//  plPolys->iCount = npolys;
//  plPolys->pList = new Polygon[npolys];
//
//  // Get pointers to P,N and st
//  for(i=0; i < n; i++)
//  {
//   if(strcmp(tokens[i],"P") == 0)
//    fP = (float*)parms[i];
//   if(strcmp(tokens[i],"N") == 0)
//    fN = (float*)parms[i];
//   if(strcmp(tokens[i],"st") == 0)
//    fST = (float*)parms[i];
//  }
//
//  // Alloc space for each polygon
//  for(i=0; i < npolys; i++)
//  {
//   plPolys->pList[i].vParent = (void*)plPolys;
//   plPolys->pList[i].aAttribute = aCurrent;
//   plPolys->pList[i].iCount = nverts[i];
//   plPolys->pList[i].vList = new Point[nverts[i]];
//   plPolys->pList[i].nList = new Point[nverts[i]];
//   plPolys->pList[i].stList = new Point[nverts[i]];
//
//   // Store P,N and st
//   plPolys->pList[i].pCenter = 0.0;
//   for(j=0; j < nverts[i]; j++)
//   {
//    // Vertices
//    plPolys->pList[i].vList[j].x = fP[verts[k+j]*3];
//    plPolys->pList[i].vList[j].y = fP[verts[k+j]*3+1];
//    plPolys->pList[i].vList[j].z = fP[verts[k+j]*3+2];
//
//    // Normal
//    plPolys->pList[i].nList[j].x = fN[verts[k+j]*3];
//    plPolys->pList[i].nList[j].y = fN[verts[k+j]*3+1];
//    plPolys->pList[i].nList[j].z = fN[verts[k+j]*3+2];
//
//    // s & t
//    plPolys->pList[i].stList[j].x = fST[verts[k+j]*2];
//    plPolys->pList[i].stList[j].y = fST[verts[k+j]*2+1];
//
//    plPolys->aAttribute = aCurrent;
//    plPolys->pList[i].pCenter += plPolys->pList[i].vList[j];
//   }
//   plPolys->pList[i].pCenter /= (double)nverts[i];
//   k += nverts[i];
//  }
//
//  // Transform polylist to camera
//  plPolys->TransformtoCamera(mCurrent);
//
//  // Calculate normal and area for individual polys
//  for(i=0; i < npolys; i++)
//  {
//   plPolys->pList[i].vNormal = (plPolys->pList[i].vList[0]-
//			        plPolys->pList[i].vList[1]) %
//	               	       (plPolys->pList[i].vList[2] -
//			        plPolys->pList[i].vList[1]);
//   plPolys->pList[i].vNormal = Normalize(plPolys->pList[i].vNormal);
//   dArea += plPolys->pList[i].Area();
//  }
//
//  // Increment ID
////  plPolys->iID = wWorld.iGeomCount++;
//
//  // Store in display list
////  llGeometryPtr->Add((void*)plPolys);
// }
}

RtVoid RrPolygonV(RtInt nverts, RtInt n, RtToken tokens[], RtPointer parms[],
		  RtInt plengths[])
{
}

RtVoid RrProjectionV(RtToken name, RtInt n, RtToken tokens[],
		     RtPointer parms[], RtInt plengths[])
{
}

RtVoid RrReverseOrientation()
{
}

RtVoid RrRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
{
}

RtVoid RrSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat tmax)
{
}

RtVoid RrSurfaceV(RtToken name, RtInt n, RtToken tokens[], RtPointer parms[],
		  RtInt plengths[])
{
// int		i;
// Shader		*sShader = new Shader;
//
// sShader->sName = strdup(name);
// sShader->Parms::parse(n,tokens,parms,plengths);
// aCurrent.sShader = sShader;
//
// // Try to get some common attributes from shader
// for(i=0; i < n; i++)
// {
//  if(strcmp(tokens[i],"Ks") == 0)
//   aCurrent.dKs = (double)(*(float*)parms[i]);
//  else if(strcmp(tokens[i],"Kd") == 0)
//   aCurrent.dKd = (double)(*(float*)parms[i]);
//  else if(strcmp(tokens[i],"Ka") == 0)
//   aCurrent.dKa = (double)(*(float*)parms[i]);
//  else if(strcmp(tokens[i],"roughness") == 0)
//   aCurrent.dRoughness = (double)(*(float*)parms[i]);
//  else if(strcmp(tokens[i],"samples") == 0)
//   aCurrent.iSamples = (int)(*(float*)parms[i]);
//  else if(strcmp(tokens[i],"filterwidth") == 0)
//   aCurrent.dFilterWidth = (double)(*(float*)parms[i]);
//  else if(strcmp(tokens[i],"brushname") == 0)
//   aCurrent.sBrushName = strdup((char*)parms[i]);
//  else if(strcmp(tokens[i],"brushsize") == 0)
//   aCurrent.dBrushSize = (double)(*(float*)parms[i]);
//  else if(strcmp(tokens[i],"falloff") == 0)
//   aCurrent.bFalloff = (char)(*(float*)parms[i]);
//  else if(strcmp(tokens[i],"scale") == 0)
//   aCurrent.dScale = (double)(*(float*)parms[i]);
// }
}

RtVoid RrTransformBegin(void)
{
// sTransform.Push((void*)new Matrix(mCurrent));
}

RtVoid RrTransformEnd(void)
{
// mCurrent = *(Matrix*)sTransform.Pop();
}

RtVoid RrTranslate(RtFloat dx, RtFloat dy, RtFloat dz)
{
}

RtVoid RrTrimCurve(RtInt nloops, RtInt ncurves[], RtInt order[],
                   RtFloat knot[], RtFloat min[], RtFloat max[],
                   RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[])
{
}

RtVoid RrWorldBegin(void)
{
// mCurrent = identity();
}

RtVoid RrWorldEnd(void)
{
}

