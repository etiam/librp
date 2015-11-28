%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {Rp}
%define parser_class_name {Parser}

%code requires
{
#include <vector>
#include <boost/any.hpp>
#include "driver.h"

namespace Rp
{
  class Driver;
  class Scanner;
}

typedef struct
{
    std::string                 label;
    std::vector<boost::any>     vals;
} ArgNode;

}

%parse-param { Scanner  &scanner  }
%parse-param { Driver  &driver  }

%{

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#undef VERSION

#include <string>
#include <sstream>
#include <memory>

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

int                     buildArgList(ArgNode *node);
RtMatrix                buildMatrix(int start);

std::string             source;
int                     linenum = 1;
int                     iTLC, iTLCS;
int                     iPPnv, iPPvll, iPGPnlp;
int                     iVOrder, iVOrderL;
int                     iNC, iNO, iNK, iNMn, iNMx, iNN, iNU, iNV;

std::vector<ArgNode>    nodes;
std::vector<float>      numbers;
RtTokens                tokens;
RtPointers              vals;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wwritable-strings"

%}

%union
{
   float            number;
   std::string *    string;
   ArgNode *        node;
   float *          numbers;
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

%token <string>  tSTRING
%token <number>  tNUMBER 

%type  <node>    arg
%type  <node>    arglist

%type  <numbers> bracketnumbers
%type  <numbers> numbers
%type  <numbers> number

/* destructor rule for <string> objects */
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

    iArgCount = buildArgList($4);
    driver.Attribute(*$2, iArgCount, tokens, vals);
};

attributebegin:     tATTRIBUTEBEGIN
{
    driver.AttributeBegin();
};

attributeend:       tATTRIBUTEEND
{
    driver.AttributeEnd();
};

basis:              tBASIS tSTRING tNUMBER tSTRING tNUMBER
{
    RtMatrix umatrix = {{{{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}}};;
    RtMatrix vmatrix = {{{{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}}};;
    auto uname = *$2;
    auto vname = *$4;
    #pragma unused(uname)
    #pragma unused(vname)
    
    driver.Basis(umatrix, $3, vmatrix, $5);
}
        |           tBASIS tSTRING tNUMBER {iTLC = 0; iTLCS = 0;} bracketnumbers tNUMBER
{
    RtMatrix umatrix = {{{{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}}};;
    auto uname = *$2;
    #pragma unused(uname)
    
    if(iTLC - iTLCS != 16)
    {
        std::stringstream err;
        err << "Wrong number of numeric parameters to Basis: " <<  iTLC - iTLCS;
        Rp::Parser::error(err.str());
    }
    else
    {
        driver.Basis(umatrix, $3, buildMatrix(iTLCS), $6);
    }
}
        |           tBASIS {iTLC = 0; iTLCS = 0;} bracketnumbers tNUMBER tSTRING tNUMBER
{
    RtMatrix vmatrix = {{{{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}}};;
    auto vname = *$5;
    #pragma unused(vname)

    if(iTLC - iTLCS != 16)
    {
        std::stringstream err;
        err << "Wrong number of numeric parameters to Basis: " <<  iTLC - iTLCS;
        Rp::Parser::error(err.str());
    }
    else
    {
        driver.Basis(buildMatrix(iTLCS), $4, vmatrix, $6);
    }
}
        |           tBASIS {iTLC = 0; iTLCS = 0;} bracketnumbers tNUMBER bracketnumbers tNUMBER
{
    if(iTLC - iTLCS != 32)
    {
        std::stringstream err;
        err << "Wrong number of numeric parameters to Basis: " <<  iTLC - iTLCS;
        Rp::Parser::error(err.str());
    }
    else
    {
        driver.Basis(buildMatrix(iTLCS), $4, buildMatrix(iTLCS+16), $6);
    }
}
        
camera:             tCAMERA tSTRING
{
    driver.Camera(*$2);
}

clipping:           tCLIPPING tNUMBER tNUMBER
{
    driver.Clipping($2, $3);
}

color:              tCOLOR {iTLC = 0; iTLCS = 0;} bracketnumbers
{
    RtColor    color;

    if(iTLC - iTLCS != 3)
    {
        std::stringstream err;
        err << "Wrong number of color parameters to Color: " <<  iTLC - iTLCS;
        Rp::Parser::error(err.str());
    }
    else
    {
        color[0] = numbers[0];
        color[1] = numbers[1];
        color[2] = numbers[2];
        driver.Color(color);
    }
}
        |           tCOLOR {iTLC = 0; iTLCS = 0;} numbers
{
    RtColor    color;

    if(iTLC - iTLCS != 3)
    {
        std::stringstream err;
        err << "Wrong number of color parameters to Color: " <<  iTLC - iTLCS;
        Rp::Parser::error(err.str());
    }
    else
    {
        color[0] = numbers[0];
        color[1] = numbers[1];
        color[2] = numbers[2];
        driver.Color(color);
    }
};

concattransform:    tCONCATTRANSFORM {iTLC = 0; iTLCS = 0;} bracketnumbers
{
    if(iTLC - iTLCS != 16)
    {
        std::stringstream err;
        err << "Wrong number of numeric parameters to ConcatTransform: " <<  iTLC - iTLCS;
        Rp::Parser::error(err.str());
    }
    else
    {
        driver.ConcatTransform(buildMatrix(iTLCS));
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
    driver.Display(*$2, *$3, *$4, 0, RtTokens(), RtPointers());
}

display:            tDISPLAY tSTRING tSTRING tSTRING {iTLC = 0;} arglist
{
    int     iArgCount;

    iArgCount = buildArgList($6);
    driver.Display(*$2, *$3, *$4, iArgCount, tokens, vals);
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
    driver.FrameBegin($2);
};

frameend:           tFRAMEEND
{
    driver.FrameEnd();
};

geometricapproximation: tGEOMETRICAPPROXIMATION tSTRING tNUMBER
{
    driver.GeometricApproximation(*$2, $3);
}

hider:              tHIDER tSTRING {iTLC=0;} arglist
{
    int     iArgCount;

    iArgCount = buildArgList($4);
    driver.Hider(*$2, iArgCount, tokens, vals);
};

identity:           tIDENTITY
{
};

lightsource:        tLIGHTSOURCE {iTLC=0;} tSTRING tNUMBER arglist
{
    int     iArgCount;

    iArgCount = buildArgList($5);
    driver.LightSource(*$3, iArgCount, tokens, vals);
};

matte:              tMATTE tNUMBER
{
    driver.Matte($2);
} 

nupatch:            tNUPATCH
                    tNUMBER tNUMBER {iTLC = 0; iTLCS = 0;} bracketnumbers
                    tNUMBER tNUMBER
                    tNUMBER tNUMBER {iVOrder = iTLC;} bracketnumbers
                    tNUMBER tNUMBER {iVOrderL = iTLC;} arglist
{
    int        i,iArgCount;
    float    *uknot,*vknot;

    uknot = new float[(int)$2+(int)$3];
    for(i=0; i < iVOrder; i++)
        uknot[i] = numbers[i];
    vknot = new float[(int)$8+(int)$9];
    for(i=iVOrder; i < iVOrderL; i++)
        vknot[i-iVOrder] = numbers[i];
    iArgCount = buildArgList($15);
    driver.NuPatch((int)$2, (int)$3, uknot, $6, $7, (int)$8, (int)$9, vknot, $12, $13, iArgCount, tokens, vals);
    delete(uknot);
    delete(vknot);
};

opacity:            tOPACITY {iTLC = 0; iTLCS = 0;} bracketnumbers
{
};

option:             tOPTION tSTRING {iTLC=0;} arglist
{
    int     iArgCount;

    iArgCount = buildArgList($4);
    driver.Option(*$2, iArgCount, tokens, vals); 
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

pointsgeneralpolygons:    tPOINTSGENERALPOLYGONS {iTLC = 0; iTLCS = 0;} bracketnumbers
                                                 {iPGPnlp = iTLC;} bracketnumbers
                                                 {iPPnv = iTLC;} bracketnumbers
                                                 {iPPvll = iTLC;} arglist
{
    int     i, iArgCount;
    RtInts  nloops, nverts, verts;

    auto npolys = iPGPnlp;
    auto nverts_size = 0;
    for(i=0; i < npolys; i++)
        nverts_size += static_cast<int>(numbers[i]);
   
    nloops.reserve(npolys);     
    nverts.reserve(nverts_size);
    verts.reserve(iPPvll-iPPnv);
    
    for(i=0; i < npolys; i++)
        nloops.push_back(static_cast<int>(numbers[i]));
    
    for(i=iPGPnlp; i < iPPnv; i++)
        nverts.push_back(static_cast<int>(numbers[i]));

    for(i=iPPnv; i < iPPvll; i++)
        verts.push_back(static_cast<int>(numbers[i]));
        
    iArgCount = buildArgList($9);
    driver.PointsGeneralPolygons(npolys, nloops, nverts, verts, iArgCount, tokens, vals);
};

pointspolygons:     tPOINTSPOLYGONS {iTLC = 0; iTLCS = 0;} bracketnumbers
                                    {iPPnv = iTLC;} bracketnumbers
                                    {iPPvll = iTLC;} arglist
{
    int     i,iArgCount;
    int   *nverts,*verts;

    nverts = new int[iPPnv];
    verts = new int[iPPvll-iPPnv];
    for(i=0; i < iPPnv; i++)
        nverts[i] = (int)numbers[i];
    for(i=iPPnv; i < iPPvll; i++)
        verts[i-iPPnv] = (int)numbers[i];
        
    iArgCount = buildArgList($7);
    driver.PointsPolygons(iPPnv, nverts, verts, iArgCount, tokens, vals);
    delete nverts;
    delete verts;
};

polygon:            tPOLYGON tNUMBER {iTLC = 0;} arglist
{
    int    iArgCount;

    iArgCount = buildArgList($4);
    driver.Polygon($2, iArgCount, tokens, vals);
};

projection:         tPROJECTION tSTRING
{
    driver.Projection(*$2, 0, RtTokens(), RtPointers());
};
        |           tPROJECTION tSTRING {iTLC = 0;} arglist
{
    int    iArgCount;

    iArgCount = buildArgList($4);
    driver.Projection(*$2, iArgCount, tokens, vals);
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
    driver.ScreenWindow($2, $3, $4, $5);
};

shadingrate:        tSHADINGRATE tNUMBER
{
    driver.ShadingRate($2);
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

    iArgCount = buildArgList($4);
    driver.Surface(*$2, iArgCount, tokens, vals);
};

surface:            tSURFACE tSTRING
{
    driver.Surface(*$2, 0, RtTokens(), RtPointers());
};

shutter:            tSHUTTER tNUMBER tNUMBER
{
};

translate:          tTRANSLATE tNUMBER tNUMBER tNUMBER
{
    driver.Translate($2, $3, $4);
};

trimcurve:          tTRIMCURVE {iTLC = 0; iTLCS = 0;} bracketnumbers
                               {iNC = iTLC;} bracketnumbers
                               {iNO = iTLC;} bracketnumbers
                               {iNK = iTLC;} bracketnumbers
                               {iNMn = iTLC;} bracketnumbers
                               {iNMx = iTLC;} bracketnumbers
                               {iNN = iTLC;} bracketnumbers
                               {iNU = iTLC;} bracketnumbers
                               {iNV = iTLC;} bracketnumbers
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
        ncurves[i] = (int)numbers[i];
    for(i=iNC; i < iNO; i++)
        order[i-iNC] = (int)numbers[i];
    for(i=iNO; i < iNK; i++)
        knot[i-iNO] = numbers[i];
    for(i=iNK; i < iNMn; i++)
        min[i-iNK] = numbers[i];
    for(i=iNMn; i < iNMx; i++)
        max[i-iNMn] = numbers[i];
    for(i=iNMx; i < iNN; i++)
        n[i-iNMx] = (int)numbers[i];
    for(i=iNN; i < iNU; i++)
        u[i-iNN] = numbers[i];
    for(i=iNU; i < iNV; i++)
        v[i-iNU] = numbers[i];
    for(i=iNV; i < iTLC; i++)
        w[i-iNV] = numbers[i];

    driver.TrimCurve(iNC, ncurves, order, knot, min, max, n, u, v, w);
};

transform:          tTRANSFORM bracketnumbers
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

arg:                tSTRING tSTRING
{
    ArgNode node;
    
    node.label = *$1;
    node.vals.push_back(*$2);
    nodes.push_back(node);

    $$ = &node;
}
        |           tSTRING {iTLCS = iTLC;} numbers
{
    ArgNode node;
    
    node.label = *$1;

    auto nums = $3;
    node.vals.reserve(iTLC - iTLCS);
    std::copy(nums, nums + iTLC-iTLCS, std::back_inserter(node.vals));
    
    nodes.push_back(node);

    $$ = &node;
}
        |           tSTRING {iTLCS = iTLC;} bracketnumbers
{
    ArgNode node;
    
    node.label = *$1;

    auto nums = $3;
    node.vals.reserve(iTLC - iTLCS);
    std::copy(nums, nums + iTLC-iTLCS, std::back_inserter(node.vals));
    
    nodes.push_back(node);

    $$ = &node;
}

arglist:            arglist arg
{
    $$ = $1;
};
        |           arg
{
    $$ = $1;
};

number:             tNUMBER
{
    if (iTLC+1 > numbers.capacity())
    {
        numbers.reserve(numbers.capacity() + 100);
        numbers.resize(numbers.capacity() + 100);
    }
    numbers[iTLC++] = $1;
    $$ = &numbers[iTLCS];
}

numbers:         numbers number
{
    $$ = $1;
}
        |           number
{
    $$ = $1;
};

bracketnumbers:  '[' numbers ']'
{
    $$ = $2;
}

%%

#pragma clang diagnostic pop

int 
buildArgList(ArgNode *node)
{
    vals.clear();
    tokens.clear();
   
    for (const auto &node : nodes)
    {
        tokens.push_back(node.label);
        vals.push_back(node.vals);
    }
     
    nodes.clear();
    
    return tokens.size();
}


RtMatrix
buildMatrix(int start)
{
    RtMatrix matrix;
    int i = start;
    
    matrix[0][0] = numbers[i++];
    matrix[0][1] = numbers[i++];
    matrix[0][2] = numbers[i++];
    matrix[0][3] = numbers[i++];
    matrix[1][0] = numbers[i++];
    matrix[1][1] = numbers[i++];
    matrix[1][2] = numbers[i++];
    matrix[1][3] = numbers[i++];
    matrix[2][0] = numbers[i++];
    matrix[2][1] = numbers[i++];
    matrix[2][2] = numbers[i++];
    matrix[2][3] = numbers[i++];
    matrix[3][0] = numbers[i++];
    matrix[3][1] = numbers[i++];
    matrix[3][2] = numbers[i++];
    matrix[3][3] = numbers[i++];
    
    return matrix;
}

void
Rp::Parser::error(const std::string &message)
{
   std::cerr << "Error: " << message << " (at line " << linenum << " in " << "source" << ")\n";
}
