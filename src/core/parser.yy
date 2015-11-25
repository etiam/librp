%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {Rp}
%define parser_class_name {Parser}

%code requires
{
#include "yacc.h"

namespace Rp
{
  class Driver;
  class Scanner;
}
}

%parse-param { Scanner  &scanner  }
%parse-param { Driver  &driver  }

%{
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#undef VERSION

#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "yacc.h"
#include "driver.h"
#include "scanner.h"

#undef yylex
#define yylex scanner.yylex

#define        ALLOC_BLOCK 20000
#define        MAX_ARGS    100

enum
{
    ARGSTRING,
    ARGNUMLIST
};

ArgNode *       NewNode();
int             buildRIarglist(ArgNode*);
bool            alloctemplist(int);
void            debug();

char *          sSource;
char            sErrorString[100];
int             iLineNum = 1;

int             iTLS = 0, iTLC, iTLCS;
int             iPPnv,iPPvll,iPGPnlp;
int             iVOrder,iVOrderL;
int             iNC,iNO,iNK,iNMn,iNMx,iNN,iNU,iNV;
float *         dTempList;
ArgNode *       anTempNode;

std::vector<std::string>    tokens;

RtPointer       parms[MAX_ARGS];
int             plengths[MAX_ARGS];

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wwritable-strings"

%}

%union
{
   float            dval;
   std::string *    string;
   ArgNode *        anNode;
   float *          dValue;
}

/* token types */
%token tATTRIBUTE
%token tATTRIBUTEBEGIN
%token tATTRIBUTEEND
%token tBASIS
%token tBEGIN
%token tCAMERA
%token tCLIPPING
%token tCOLOR
%token tCONCATTRANSFORM
%token tCOORDINATESYSTEM
%token tCROPWINDOW
%token tCYLINDER
%token tDECLARE
%token tDISPLACEMENT
%token tDISPLAY
%token tDISPLAYCHANNEL
%token tEND
%token tERRORHANDLER
%token tEXPOSURE
%token tFORMAT
%token tFRAMEASPECTRATIO
%token tFRAMEBEGIN
%token tFRAMEEND
%token tGEOMETRICAPPROXIMATION
%token tHIDER
%token tHIERARCHICALSUBDIVISIONMESH
%token tIDENTITY
%token tILLUMINATE
%token tLIGHTSOURCE
%token tMATTE
%token tMOTIONBEGIN
%token tMOTIONEND
%token tNUPATCH
%token tOPACITY
%token tOPTION
%token tORIENTATION
%token tPIXELSAMPLES
%token tPIXELFILTER
%token tPIXELVARIANCE
%token tPOINTSGENERALPOLYGONS
%token tPOINTSPOLYGONS
%token tPOLYGON
%token tPROCEDURAL
%token tPROJECTION
%token tQUANTIZE
%token tREADARCHIVE
%token tRELATIVEDETAIL
%token tRESOURCE
%token tRESOURCEBEGIN
%token tRESOURCEEND
%token tREVERSEORIENTATION
%token tROTATE
%token tSCALE
%token tSCREENWINDOW
%token tSHADER
%token tSHADINGINTERPOLATION
%token tSHADINGRATE
%token tSHUTTER
%token tSIDES
%token tSPHERE
%token tSUBDIVISIONMESH
%token tSURFACE
%token tSYSTEM
%token tTRANSFORM
%token tTRANSFORMBEGIN
%token tTRANSFORMEND
%token tTRANSLATE
%token tTRIMCURVE
%token tVERSION
%token tWORLDBEGIN
%token tWORLDEND

%token <string> tSTRING
%token <string> tSTRINGBRACKET
%token <dval>   tNUMBER 

%type  <anNode> arg
%type  <anNode> arglist

%type  <dValue> bracketnumberlist
%type  <dValue> numberlist
%type  <dValue> number

/* destructor rule for <sval> objects */
%destructor { if ($$)  { delete ($$); ($$) = nullptr; } } <string>

%%

genericlist:        genericlist generic {}
        |            generic {};

generic:            attribute
        |           attributebegin
        |           attributeend
        |           basis
//        |           begin
        |           camera
        |           clipping
        |           color
        |           concattransform
        |           coordinatesystem
        |           cropwindow
        |           cylinder
        |           declare
//        |           displacement
        |           display
//        |           displaychannel
//        |           end
        |           errorhandler
        |           exposure
        |           format
        |           frameaspectratio
        |           framebegin
        |           frameend
        |           geometricapproximation
        |           hider
//        |           hierarchicalsubdivisionmesh
        |           identity
//        |           illuminate
        |           lightsource
        |           matte
//        |           motionbegin
//        |           motionend
        |           nupatch
        |           opacity
        |           option
        |           orientation
        |           pixelsamples
        |           pixelfilter
//        |           pixelvariance
        |           pointsgeneralpolygons
        |           pointspolygons
        |           polygon
//        |           procedural
        |           projection
        |           quantize
//        |           readarchive
        |           relativedetail
//        |           resource
//        |           resourcebegin
//        |           resourceend
        |           reverseorientation
        |           rotate
        |           scale
        |           screenwindow
//        |           shader
        |           shadinginterpolation
        |           shadingrate
        |           shutter
        |           sides
        |           sphere
//        |           subdivisionmesh
        |           surface
//        |           system
        |           transform
        |           transformbegin
        |           transformend
        |           translate
        |           trimcurve
        |           version
        |           worldbegin
        |           worldend;
        
attribute:          tATTRIBUTE tSTRING {iTLC=0;} arglist
{
    int     iArgCount;

    iArgCount = buildRIarglist($4);
    driver.Attribute($2, iArgCount, &tokens[0], parms, plengths);
};

attributebegin:     tATTRIBUTEBEGIN
{
    driver.AttributeBegin();
};

attributeend:       tATTRIBUTEEND
{
    driver.AttributeEnd();
};

basis:              tBASIS tSTRING number tSTRING number
{
}
        |           tBASIS tSTRING number {iTLC = 0; iTLCS = 0;} bracketnumberlist number
{
}
        |           tBASIS {iTLC = 0; iTLCS = 0;} bracketnumberlist number tSTRING number
{
}
        |           tBASIS {iTLC = 0; iTLCS = 0;} bracketnumberlist number {iTLC = 0; iTLCS = 0;} bracketnumberlist number
{
}
        
camera:             tCAMERA tSTRING
{
    driver.Camera(*$2);
}

clipping:           tCLIPPING tNUMBER tNUMBER
{
}

color:              tCOLOR {iTLC = 0; iTLCS = 0;} bracketnumberlist
{
    RtColor    color;

    if(iTLC - iTLCS != 3)
    {
        sprintf(sErrorString,"Wrong number of color samples: %i, expecting 3", iTLC - iTLCS);
        Rp::Parser::error(sErrorString);
    }
    else
    {
        color[0] = dTempList[0];
        color[1] = dTempList[1];
        color[2] = dTempList[2];
        driver.Color(color);
    }
}
        |           tCOLOR {iTLC = 0; iTLCS = 0;} numberlist
{
    RtColor    color;

    if(iTLC - iTLCS != 3)
    {
        sprintf(sErrorString,"Wrong number of color samples: %i, expecting 3", iTLC - iTLCS);
        Rp::Parser::error(sErrorString);
    }
    else
    {
        color[0] = dTempList[0];
        color[1] = dTempList[1];
        color[2] = dTempList[2];
        driver.Color(color);
    }
};

concattransform:    tCONCATTRANSFORM {iTLC = 0; iTLCS = 0;} bracketnumberlist
{
    RtMatrix    matrix;

    if(iTLC - iTLCS != 16)
    {
        sprintf(sErrorString,"Bad number of numeric parameters to Transform: %i", iTLC - iTLCS);
        Rp::Parser::error(sErrorString);
    }
    else
    {
        matrix[0][0] = dTempList[0];
        matrix[0][1] = dTempList[1];
        matrix[0][2] = dTempList[2];
        matrix[0][3] = dTempList[3];
        matrix[1][0] = dTempList[4];
        matrix[1][1] = dTempList[5];
        matrix[1][2] = dTempList[6];
        matrix[1][3] = dTempList[7];
        matrix[2][0] = dTempList[8];
        matrix[2][1] = dTempList[9];
        matrix[2][2] = dTempList[10];
        matrix[2][3] = dTempList[11];
        matrix[3][0] = dTempList[12];
        matrix[3][1] = dTempList[13];
        matrix[3][2] = dTempList[14];
        matrix[3][3] = dTempList[15];
        
        driver.ConcatTransform(matrix);
    }
}

coordinatesystem:   tCOORDINATESYSTEM tSTRING
{
    driver.CoordinateSystem(*$2);
};

cropwindow:         tCROPWINDOW tNUMBER tNUMBER tNUMBER tNUMBER
{
    driver.CropWindow($2,$3,$4,$5);
};

cylinder:           tCYLINDER tNUMBER tNUMBER tNUMBER tNUMBER
{
    driver.Cylinder($2, $3, $4, $5);
};

declare:            tDECLARE tSTRING tSTRING
{
    driver.Declare(*$2, *$3);
}

display:            tDISPLAY tSTRING tSTRING tSTRING
{
    driver.Display(*$2, *$3, *$4, 0, nullptr, nullptr, nullptr);
}

display:            tDISPLAY tSTRING tSTRING tSTRING {iTLC = 0;} arglist
{
    int     iArgCount;

    iArgCount = buildRIarglist($6);
    driver.Display(*$2, *$3, *$4, iArgCount, &tokens[0], parms, plengths);
};

errorhandler:       tERRORHANDLER tSTRING
{
    driver.ErrorHandler(*$2);
};

exposure:           tEXPOSURE tNUMBER tNUMBER
{
    driver.Exposure($2, $3);
};

format:             tFORMAT tNUMBER tNUMBER tNUMBER
{
    driver.Format((int)$2, (int)$3, (float)$4);
};

frameaspectratio:   tFRAMEASPECTRATIO tNUMBER
{
    driver.FrameAspectRatio($2);
};

framebegin:         tFRAMEBEGIN tNUMBER
{
};

frameend:           tFRAMEEND
{
};

geometricapproximation: tGEOMETRICAPPROXIMATION tSTRING tNUMBER
{
    driver.GeometricApproximation(*$2, $3);
}

hider:              tHIDER tSTRING {iTLC=0;} arglist
{
    int     iArgCount;

    iArgCount = buildRIarglist($4);
    driver.Hider(*$2, iArgCount, &tokens[0], parms, plengths);
};

identity:           tIDENTITY
{
};

lightsource:        tLIGHTSOURCE {iTLC=0;} tSTRING tNUMBER arglist
{
    int     iArgCount;

    iArgCount = buildRIarglist($5);
    driver.LightSource(*$3, iArgCount, &tokens[0], parms, plengths);
};

matte:              tMATTE tNUMBER
{
    driver.Matte($2);
} 

nupatch:            tNUPATCH
                    tNUMBER tNUMBER {iTLC = 0; iTLCS = 0;} bracketnumberlist
                    tNUMBER tNUMBER
                    tNUMBER tNUMBER {iVOrder = iTLC;} bracketnumberlist
                    tNUMBER tNUMBER {iVOrderL = iTLC;} arglist
{
    int        i,iArgCount;
    float    *uknot,*vknot;

    uknot = new float[(int)$2+(int)$3];
    for(i=0; i < iVOrder; i++)
        uknot[i] = dTempList[i];
    vknot = new float[(int)$8+(int)$9];
    for(i=iVOrder; i < iVOrderL; i++)
        vknot[i-iVOrder] = dTempList[i];
    iArgCount = buildRIarglist($15);
    driver.NuPatch((int)$2, (int)$3, uknot, $6, $7, (int)$8, (int)$9, vknot, $12, $13, iArgCount, &tokens[0], parms, plengths);
    delete(uknot);
    delete(vknot);
};

opacity:            tOPACITY {iTLC = 0; iTLCS = 0;} bracketnumberlist
{
};

option:             tOPTION tSTRING {iTLC=0;} arglist
{
    int     iArgCount;

    iArgCount = buildRIarglist($4);
    driver.Option(*$2, iArgCount, &tokens[0], parms, plengths);
};

orientation:        tORIENTATION tSTRING
{
    driver.Orientation(*$2);
};

pixelfilter:        tPIXELFILTER tSTRING tNUMBER tNUMBER
{
    driver.PixelFilter(*$2, $3, $4);
}

pixelsamples:       tPIXELSAMPLES tNUMBER tNUMBER
{
    driver.PixelSamples($2, $3);
};

pointsgeneralpolygons:    tPOINTSGENERALPOLYGONS {iTLC = 0; iTLCS = 0;} bracketnumberlist
                                                 {iPGPnlp = iTLC;} bracketnumberlist
                                                 {iPPnv = iTLC;} bracketnumberlist
                                                 {iPPvll = iTLC;} arglist
{
    int     i, iArgCount;
    int   *nverts, *verts, *nloops;

    auto npolys = iPGPnlp;
    auto nverts_size = 0;
    for(i=0; i < npolys; i++)
        nverts_size += static_cast<int>(dTempList[i]);
   
    nloops = new int[npolys];     
    nverts = new int[nverts_size];
    verts = new int[iPPvll-iPPnv];
    
    for(i=0; i < npolys; i++)
        nloops[i] = static_cast<int>(dTempList[i]);
    
    for(i=iPGPnlp; i < iPPnv; i++)
        nverts[i-iPGPnlp] = (int)dTempList[i];

    for(i=iPPnv; i < iPPvll; i++)
        verts[i-iPPnv] = (int)dTempList[i];
        
    iArgCount = buildRIarglist($9);
    driver.PointsGeneralPolygons(npolys, nloops, nverts, verts, iArgCount, &tokens[0], parms, plengths);
    delete nverts;
    delete verts;
};

pointspolygons:     tPOINTSPOLYGONS {iTLC = 0; iTLCS = 0;} bracketnumberlist
                                    {iPPnv = iTLC;} bracketnumberlist
                                    {iPPvll = iTLC;} arglist
{
    int     i,iArgCount;
    int   *nverts,*verts;

    nverts = new int[iPPnv];
    verts = new int[iPPvll-iPPnv];
    for(i=0; i < iPPnv; i++)
        nverts[i] = (int)dTempList[i];
    for(i=iPPnv; i < iPPvll; i++)
        verts[i-iPPnv] = (int)dTempList[i];
        
    iArgCount = buildRIarglist($7);
    driver.PointsPolygons(iPPnv, nverts, verts, iArgCount, &tokens[0], parms);
    delete nverts;
    delete verts;
};

polygon:            tPOLYGON {iTLC = 0;} arglist
{
    int    iArgCount;

    iArgCount = buildRIarglist($3);
    driver.Polygon($3->iListCount / 3, iArgCount, &tokens[0], parms, plengths);
};

projection:         tPROJECTION tSTRING
{
    driver.Projection(*$2, 0, NULL, NULL, NULL);
};
        |           tPROJECTION tSTRING {iTLC = 0;} arglist
{
    int    iArgCount;

    iArgCount = buildRIarglist($4);
    driver.Projection(*$2, iArgCount, &tokens[0], parms, plengths);
};

quantize:           tQUANTIZE tSTRING tNUMBER tNUMBER tNUMBER tNUMBER
{
    driver.Quantize(*$2, $3, $4, $5, $6);
}

relativedetail:     tRELATIVEDETAIL tNUMBER
{
    driver.RelativeDetail($2);
}

reverseorientation: tREVERSEORIENTATION
{
    driver.ReverseOrientation();
};

rotate:             tROTATE tNUMBER tNUMBER tNUMBER tNUMBER
{
    driver.Rotate($2, $3, $4, $5);
};

scale:              tSCALE tNUMBER tNUMBER tNUMBER
{
    driver.Scale($2, $3, $4);
};

screenwindow:       tSCREENWINDOW tNUMBER tNUMBER tNUMBER tNUMBER
{
};

shadingrate:        tSHADINGRATE tNUMBER
{
};

shadinginterpolation:   tSHADINGINTERPOLATION tSTRING
{
    driver.ShadingInterpolation(*$2);
};

sides:              tSIDES tNUMBER
{
    driver.Sides($2);
}

sphere:             tSPHERE tNUMBER tNUMBER tNUMBER tNUMBER
{
    driver.Sphere($2, $3, $4, $5);
};

surface:            tSURFACE tSTRING {iTLC = 0;} arglist
{
    int    iArgCount;

    iArgCount = buildRIarglist($4);
    driver.Surface(*$2, iArgCount, &tokens[0], parms, plengths);
};

surface:            tSURFACE tSTRING
{
    driver.Surface(*$2, 0, NULL, NULL, NULL);
};

shutter:            tSHUTTER tNUMBER tNUMBER
{
};

translate:          tTRANSLATE tNUMBER tNUMBER tNUMBER
{
    driver.Translate($2, $3, $4);
};

trimcurve:          tTRIMCURVE {iTLC = 0; iTLCS = 0;} bracketnumberlist
                               {iNC = iTLC;} bracketnumberlist
                               {iNO = iTLC;} bracketnumberlist
                               {iNK = iTLC;} bracketnumberlist
                               {iNMn = iTLC;} bracketnumberlist
                               {iNMx = iTLC;} bracketnumberlist
                               {iNN = iTLC;} bracketnumberlist
                               {iNU = iTLC;} bracketnumberlist
                               {iNV = iTLC;} bracketnumberlist
{
    int         i;
    int        *ncurves,*order,*n;
    float    *knot,*min,*max,*u,*v,*w;

    ncurves = new int[iNC];
    order = new int[iNO-iNC];
    knot = new float[iNK-iNO];
    min = new float[iNMn-iNK];
    max = new float[iNMx-iNMn];
    n = new int[iNN-iNMx];
    u = new float[iNU-iNN];
    v = new float[iNV-iNU];
    w = new float[iTLC-iNV];
    for(i=0; i < iNC; i++)
        ncurves[i] = (int)dTempList[i];
    for(i=iNC; i < iNO; i++)
        order[i-iNC] = (int)dTempList[i];
    for(i=iNO; i < iNK; i++)
        knot[i-iNO] = dTempList[i];
    for(i=iNK; i < iNMn; i++)
        min[i-iNK] = dTempList[i];
    for(i=iNMn; i < iNMx; i++)
        max[i-iNMn] = dTempList[i];
    for(i=iNMx; i < iNN; i++)
        n[i-iNMx] = (int)dTempList[i];
    for(i=iNN; i < iNU; i++)
        u[i-iNN] = dTempList[i];
    for(i=iNU; i < iNV; i++)
        v[i-iNU] = dTempList[i];
    for(i=iNV; i < iTLC; i++)
        w[i-iNV] = dTempList[i];

    driver.TrimCurve(iNC, ncurves, order, knot, min, max, n, u, v, w);
};

transform:          tTRANSFORM bracketnumberlist
{
};

transformbegin:     tTRANSFORMBEGIN
{
    driver.TransformBegin();
};

transformend:       tTRANSFORMEND
{
    driver.TransformEnd();
};

version:            tVERSION tNUMBER
{
};

worldbegin:         tWORLDBEGIN
{
    driver.WorldBegin();
};

worldend:           tWORLDEND
{
    driver.WorldEnd();
};

arglist:            arglist arg
{
    ArgNode *   anTemp;

    for(anTemp = $1;anTemp->Next; anTemp=anTemp->Next);
    anTemp->Next = $2;
    $$ = $1;
};
        |           arg
{
    $$ = $1;
};

arg:                tSTRING tSTRINGBRACKET
{
    anTempNode = NewNode();
    anTempNode->iType = ARGSTRING;
    anTempNode->sLabel = const_cast<char*>($1->c_str());
    anTempNode->sValue = const_cast<char*>($2->c_str());
    $$ = anTempNode;
}
        |           tSTRING tSTRING
{
    anTempNode = NewNode();
    anTempNode->iType = ARGSTRING;
    anTempNode->sLabel = const_cast<char*>($1->c_str());
    anTempNode->sValue = const_cast<char*>($2->c_str());
    $$ = anTempNode;
}
        |           tSTRING {iTLCS = iTLC;} numberlist
{
    anTempNode = NewNode();
    anTempNode->iType = ARGNUMLIST;
    anTempNode->sLabel = const_cast<char*>($1->c_str());
    anTempNode->dValue = $3;
    anTempNode->iListCount = iTLC - iTLCS;
    $$ = anTempNode;
}
        |           tSTRING {iTLCS = iTLC;} bracketnumberlist
{
    anTempNode = NewNode();
    anTempNode->iType = ARGNUMLIST;
    anTempNode->sLabel = const_cast<char*>($1->c_str());
    anTempNode->dValue = $3;
    anTempNode->iListCount = iTLC - iTLCS;
    $$ = anTempNode;
}

bracketnumberlist:  '[' numberlist ']'
{
    $$ = $2;
}

numberlist:         numberlist number
{
    $$ = $1;
}
        |           number
{
    $$ = $1;
};

number:             tNUMBER
{
    if(iTLC+1 > iTLS)
    {
        iTLS += ALLOC_BLOCK;
        alloctemplist(iTLS);
    }
    dTempList[iTLC++] = $1;
    $$ = &dTempList[iTLCS];
}
%%

#pragma clang diagnostic pop

bool alloctemplist(int iSize)
{
    dTempList = (float*)realloc((void*)dTempList,(iTLS*sizeof(float)));
    if(dTempList == NULL)
    {
        fprintf(stderr,"ERROR: alloctemplist(): outta memory\n");
        exit(1);
    }
    return(true);
}

ArgNode *
NewNode()
{
    ArgNode *   temp;

    temp = new ArgNode;
    temp->sLabel = temp->sValue = NULL;
    temp->Next = NULL;
    temp->iListCount = 0;
    temp->dValue = NULL;
    return(temp);
}

int buildRIarglist(ArgNode *anNode)
{
    int        iArgCount = 0;

    tokens.clear();
    for (anTempNode = anNode; anTempNode; anTempNode = anTempNode->Next)
    {
        tokens.push_back(anTempNode->sLabel);
        switch (anTempNode->iType)
        {
        case ARGSTRING:
            parms[iArgCount] = anTempNode->sValue;
            plengths[iArgCount] = strlen(anTempNode->sValue);
            break;
            
        case ARGNUMLIST:
            parms[iArgCount] = anTempNode->dValue;
            plengths[iArgCount] = sizeof(float) * anTempNode->iListCount;
            break;
        }
        iArgCount++;
    }
    return (iArgCount);
}

void FreeNode(ArgNode *anNode)
{
    while(anNode->Next)
        FreeNode(anNode->Next);
    delete(anNode->sLabel);
    delete(anNode->sValue);
    delete(anNode);
}

void debug()
{
    printf("debug");
}

void
Rp::Parser::error(const std::string &message)
{
//   std::cerr << "Error: " << err_message << "\n";
   std::cerr << "Error: " << message << " (at line " << iLineNum << " in " << "source" << ")\n";
}
