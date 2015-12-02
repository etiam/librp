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

void                    initNumbersList();
int                     buildArgList(ArgNode *node);
RtMatrix                buildMatrix(int start);

std::string             source;
int                     linenum = 1;
int                     iTLC, iTLCS;
int                     iPPnv, iPPvll, iPGPnlp;
int                     iVOrder, iVOrderL;
int                     iNC, iNO, iNK, iNMn, iNMx, iNN, iNU, iNV;
unsigned int            nl;

std::vector<ArgNode>    nodes;

//std::vector<float>      numbers;
std::vector<std::vector<float>>      numbers;

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
//   float *          numbers;
   std::vector<float> *          numbers;
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

%type  <numbers> numberlist
%type  <numbers> bracketnumberlist

%type  <numbers> numbers
%type  <numbers> number

/* destructor rule for <string> objects */
%destructor { if ($$)  { delete ($$); ($$) = nullptr; } } <string>

%%

genericlist:        genericlist { initNumbersList(); } generic 
{ 
}
        |           { initNumbersList(); } generic 
{
}

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
        |           tBASIS tSTRING tNUMBER {iTLC = 0; iTLCS = 0;} bracketnumberlist tNUMBER
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
        |           tBASIS {iTLC = 0; iTLCS = 0;} bracketnumberlist tNUMBER tSTRING tNUMBER
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
        |           tBASIS {iTLC = 0; iTLCS = 0;} bracketnumberlist tNUMBER bracketnumberlist tNUMBER
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

color:              tCOLOR {iTLC = 0; iTLCS = 0;} bracketnumberlist
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
//        color[0] = numbers[0];
//        color[1] = numbers[1];
//        color[2] = numbers[2];
        driver.Color(color);
    }
}
        |           tCOLOR {iTLC = 0; iTLCS = 0;} numberlist
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
//        color[0] = numbers[0];
//        color[1] = numbers[1];
//        color[2] = numbers[2];
        driver.Color(color);
    }
};

concattransform:    tCONCATTRANSFORM bracketnumberlist
{
    if(numbers[0].size() != 16)
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
                    tNUMBER tNUMBER {iTLC = 0; iTLCS = 0;} bracketnumberlist
                    tNUMBER tNUMBER
                    tNUMBER tNUMBER {iVOrder = iTLC;} bracketnumberlist
                    tNUMBER tNUMBER {iVOrderL = iTLC;} arglist
{
};

opacity:            tOPACITY {iTLC = 0; iTLCS = 0;} bracketnumberlist
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

pointsgeneralpolygons:    tPOINTSGENERALPOLYGONS bracketnumberlist {nl++;}
                                                 bracketnumberlist {nl++;}
                                                 bracketnumberlist
                                                 arglist
{
    RtInts nloops, nverts, verts;
    const auto &nloop = *$2;
    
    std::copy(std::begin(nloop), std::end(nloop), std::back_inserter(nloops));
    std::copy(std::begin(numbers[1]), std::end(numbers[1]), std::back_inserter(nverts));
    std::copy(std::begin(numbers[2]), std::end(numbers[2]), std::back_inserter(verts));
        
    auto argcount = buildArgList($7);

    driver.PointsGeneralPolygons(nloops.size(), nloops, nverts, verts, argcount, tokens, vals);
};

pointspolygons:     tPOINTSPOLYGONS {iTLC = 0; iTLCS = 0;} bracketnumberlist
                                    {iPPnv = iTLC;} bracketnumberlist
                                    {iPPvll = iTLC;} arglist
{
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

arg:                tSTRING tSTRING
{
    ArgNode node;
    
    node.label = *$1;
    node.vals.push_back(*$2);
    nodes.push_back(node);

    $$ = &node;
}
        |           tSTRING {nl++;} numberlist
{
    ArgNode node;
    
    node.label = *$1;

    const auto &nums = *$3;
    node.vals.reserve(nums.size());
    std::copy(std::begin(nums), std::end(nums), std::back_inserter(node.vals));
    
    nodes.push_back(node);

    $$ = &node;
}
        |           tSTRING {nl++;} bracketnumberlist
{
    ArgNode node;
    
    node.label = *$1;

    const auto &nums = *$3;
    node.vals.reserve(nums.size());
    std::copy(std::begin(nums), std::end(nums), std::back_inserter(node.vals));
    
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
    // grow numbers[] array if needed    
    if (nl+1 > numbers.size())
        numbers.push_back(std::vector<float>());
        
    auto &nums = numbers[nl];
    nums.push_back($1);
    
    $$ = &nums;
}

numbers:            numbers number
{
    $$ = $1;
}
        |           number
{
    $$ = $1;
};

numberlist:         numbers
{
    $$ = $1;
    std::cout << std::endl << "FOOBAR " << nl << " " << $1->size() << std::endl;
}

bracketnumberlist:     '[' numbers ']'
{
    $$ = $2;
    std::cout << std::endl << "[FOOBAR] " << nl << " " << $2->size() << std::endl;
}

%%

#pragma clang diagnostic pop

void
initNumbersList()
{
    nl = 0;
    numbers.clear();
    numbers.push_back(std::vector<float>());
}

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
    auto i = start;
    const auto &nums = numbers[0];
    

    matrix[0][0] = nums[i++];
    matrix[0][1] = nums[i++];
    matrix[0][2] = nums[i++];
    matrix[0][3] = nums[i++];
    matrix[1][0] = nums[i++];
    matrix[1][1] = nums[i++];
    matrix[1][2] = nums[i++];
    matrix[1][3] = nums[i++];
    matrix[2][0] = nums[i++];
    matrix[2][1] = nums[i++];
    matrix[2][2] = nums[i++];
    matrix[2][3] = nums[i++];
    matrix[3][0] = nums[i++];
    matrix[3][1] = nums[i++];
    matrix[3][2] = nums[i++];
    matrix[3][3] = nums[i++];
    
    return matrix;
}

void
Rp::Parser::error(const std::string &message)
{
   std::cerr << "Error: " << message << " (at line " << linenum << " of " << scanner.filename() << ")\n";
}
