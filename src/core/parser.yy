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
} Arg;

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
#include <vector>
#include <map>
#include <memory>

#include "driver.h"
#include "scanner.h"

#undef yylex
#define yylex scanner.yylex

#define        ALLOC_BLOCK 20000
#define        MAX_ARGS    100

void                    initNumbersList();
int                     buildArgList(Arg *arg);
RtMatrix                buildMatrix(std::vector<float> *numberlist);

std::string             source;
int                     linenum = 1;
unsigned int            nl;
unsigned int            sl;

std::vector<Arg>    args;

std::map<uint, std::vector<float>>          numbers;
std::map<uint, std::vector<std::string>>    strings;

RtTokens                tokens;
RtPointers              vals;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wwritable-strings"

%}

%union
{
    float                       number;
    std::string *               string;
    Arg *                       arg;
    std::vector<float> *        numbers;
    std::vector<std::string> *  strings;
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

%type  <numbers> number
%type  <numbers> numbers

%type  <numbers> numberlist
%type  <numbers> bracketnumberlist

%type  <strings> string
%type  <strings> strings

%type  <strings> stringlist
%type  <strings> bracketstringlist

%type  <arg>     arg
%type  <arg>     arglist

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
        |           illuminate
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
        
attribute:          tATTRIBUTE tSTRING arglist
{
    int     argcount;

    argcount = buildArgList($3);
    driver.Attribute(*$2, argcount, tokens, vals);
}

attributebegin:     tATTRIBUTEBEGIN
{
    driver.AttributeBegin();
}

attributeend:       tATTRIBUTEEND
{
    driver.AttributeEnd();
}

basis:              tBASIS tSTRING tNUMBER tSTRING tNUMBER
{
    RtMatrix umatrix = {{{{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}}};
    RtMatrix vmatrix = {{{{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}}};
    auto uname = *$2;
    auto vname = *$4;
    #pragma unused(uname)
    #pragma unused(vname)
    
    driver.Basis(umatrix, $3, vmatrix, $5);
}
        |           tBASIS tSTRING tNUMBER bracketnumberlist tNUMBER
{
    RtMatrix umatrix = {{{{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}}};
    auto vmatrix = buildMatrix($4);
    
    auto uname = *$2;
    #pragma unused(uname)
    
    if(vmatrix.size() != 16)
    {
        std::stringstream err;
        err << "Wrong number of numeric parameters to v basis: " << vmatrix.size();
        Rp::Parser::error(err.str());
    }
    else
    {
        driver.Basis(umatrix, $3, vmatrix, $5);
    }
}
        |           tBASIS bracketnumberlist tNUMBER tSTRING tNUMBER
{
    auto umatrix = buildMatrix($2);
    RtMatrix vmatrix = {{{{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}}};
    
    auto vname = *$4;
    #pragma unused(vname)

    if(umatrix.size() != 16)
    {
        std::stringstream err;
        err << "Wrong number of numeric parameters to u basis: " << umatrix.size();
        Rp::Parser::error(err.str());
    }
    else
    {
        driver.Basis(umatrix, $3, vmatrix, $5);
    }
}
        |           tBASIS bracketnumberlist tNUMBER bracketnumberlist tNUMBER
{
    auto umatrix = buildMatrix($2);
    auto vmatrix = buildMatrix($4);
    
    if(umatrix.size() != 16)
    {
        std::stringstream err;
        err << "Wrong number of numeric parameters to u basis: " << umatrix.size();
        Rp::Parser::error(err.str());
    }
    else if(vmatrix.size() != 16)
    {
        std::stringstream err;
        err << "Wrong number of numeric parameters to v basis: " << vmatrix.size();
        Rp::Parser::error(err.str());
    }
    else
    {
        driver.Basis(umatrix, $3, vmatrix, $5);
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

color:              tCOLOR bracketnumberlist
{
    RtColor    color;
    const auto &numbers = *$2;

    if(numbers.size() != 3)
    {
        std::stringstream err;
        err << "wrong number of color parameters: " << numbers.size();
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
        |           tCOLOR numberlist
{
    RtColor    color;
    const auto &numbers = *$2;

    if(numbers.size() != 3)
    {
        std::stringstream err;
        err << "wrong number of color parameters: " << numbers.size();
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

concattransform:    tCONCATTRANSFORM bracketnumberlist
{
    const auto &numbers = *$2;
    
    if(numbers.size() != 16)
    {
        std::stringstream err;
        err << "wrong number of matrix parameters: " << numbers.size();
        Rp::Parser::error(err.str());
    }
    else
    {
        driver.ConcatTransform(buildMatrix($2));
    }
}

coordinatesystem:   tCOORDINATESYSTEM tSTRING
{
    driver.CoordinateSystem(*$2);
}

cropwindow:         tCROPWINDOW tNUMBER tNUMBER tNUMBER tNUMBER
{
    driver.CropWindow($2,$3,$4,$5);
}

cylinder:           tCYLINDER tNUMBER tNUMBER tNUMBER tNUMBER
{
    driver.Cylinder($2, $3, $4, $5);
}

declare:            tDECLARE tSTRING tSTRING
{
    driver.Declare(*$2, *$3);
}

display:            tDISPLAY tSTRING tSTRING tSTRING
{
    driver.Display(*$2, *$3, *$4, 0, RtTokens(), RtPointers());
}

display:            tDISPLAY tSTRING tSTRING tSTRING arglist
{
    int     argcount;

    argcount = buildArgList($5);
    driver.Display(*$2, *$3, *$4, argcount, tokens, vals);
}

errorhandler:       tERRORHANDLER tSTRING
{
    driver.ErrorHandler(*$2);
}

exposure:           tEXPOSURE tNUMBER tNUMBER
{
    driver.Exposure($2, $3);
}

format:             tFORMAT tNUMBER tNUMBER tNUMBER
{
    driver.Format((int)$2, (int)$3, (float)$4);
}

frameaspectratio:   tFRAMEASPECTRATIO tNUMBER
{
    driver.FrameAspectRatio($2);
}

framebegin:         tFRAMEBEGIN tNUMBER
{
    driver.FrameBegin($2);
}

frameend:           tFRAMEEND
{
    driver.FrameEnd();
}

geometricapproximation: tGEOMETRICAPPROXIMATION tSTRING tNUMBER
{
    driver.GeometricApproximation(*$2, $3);
}

hider:              tHIDER tSTRING arglist
{
    int     argcount;

    argcount = buildArgList($3);
    driver.Hider(*$2, argcount, tokens, vals);
}

identity:           tIDENTITY
{
}

illuminate:         tILLUMINATE tSTRING tNUMBER
{
    driver.Illuminate(*$2, $3);
}

lightsource:        tLIGHTSOURCE tSTRING tSTRING arglist
{
    int     argcount;

    argcount = buildArgList($4);
    driver.LightSource(*$2, 0, argcount, tokens, vals);
}
        |           tLIGHTSOURCE tSTRING tSTRING
{
    driver.LightSource(*$2, 0, 0, RtTokens(), RtPointers());
}
        |           tLIGHTSOURCE tSTRING tNUMBER arglist
{
    int     argcount;

    argcount = buildArgList($4);
    driver.LightSource(*$2, $3, argcount, tokens, vals);
}
        |           tLIGHTSOURCE tSTRING tNUMBER
{
    driver.LightSource(*$2, $3, 0, RtTokens(), RtPointers());
}


matte:              tMATTE tNUMBER
{
    driver.Matte($2);
} 

nupatch:            tNUPATCH tNUMBER tNUMBER bracketnumberlist tNUMBER tNUMBER tNUMBER tNUMBER bracketnumberlist tNUMBER tNUMBER arglist
{
}

opacity:            tOPACITY bracketnumberlist
{
}

option:             tOPTION tSTRING arglist
{
    int     argcount;

    argcount = buildArgList($3);
    driver.Option(*$2, argcount, tokens, vals); 
}

orientation:        tORIENTATION tSTRING
{
    driver.Orientation(*$2);
}

pixelfilter:        tPIXELFILTER tSTRING tNUMBER tNUMBER
{
    driver.PixelFilter(*$2, $3, $4);
}

pixelsamples:       tPIXELSAMPLES tNUMBER tNUMBER
{
    driver.PixelSamples($2, $3);
}

pointsgeneralpolygons:    tPOINTSGENERALPOLYGONS bracketnumberlist bracketnumberlist bracketnumberlist arglist
{
    RtInts nloops, nverts, verts;
    const auto &nloopslist = *$2;
    const auto &nvertslist = *$3;
    const auto &vertslist  = *$4;
    
    std::copy(std::begin(nloopslist), std::end(nloopslist), std::back_inserter(nloops));
    std::copy(std::begin(nvertslist), std::end(nvertslist), std::back_inserter(nverts));
    std::copy(std::begin(vertslist), std::end(vertslist), std::back_inserter(verts));
        
    auto argcount = buildArgList($5);

    driver.PointsGeneralPolygons(nloops.size(), nloops, nverts, verts, argcount, tokens, vals);
}

pointspolygons:     tPOINTSPOLYGONS bracketnumberlist bracketnumberlist arglist
{
}

polygon:            tPOLYGON tNUMBER arglist
{
    int    argcount;

    argcount = buildArgList($3);
    driver.Polygon($2, argcount, tokens, vals);
}

projection:         tPROJECTION tSTRING
{
    driver.Projection(*$2, 0, RtTokens(), RtPointers());
}
        |           tPROJECTION tSTRING arglist
{
    int    argcount;

    argcount = buildArgList($3);
    driver.Projection(*$2, argcount, tokens, vals);
}

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
}

rotate:             tROTATE tNUMBER tNUMBER tNUMBER tNUMBER
{
    driver.Rotate($2, $3, $4, $5);
}

scale:              tSCALE tNUMBER tNUMBER tNUMBER
{
    driver.Scale($2, $3, $4);
}

screenwindow:       tSCREENWINDOW tNUMBER tNUMBER tNUMBER tNUMBER
{
    driver.ScreenWindow($2, $3, $4, $5);
}

shadingrate:        tSHADINGRATE tNUMBER
{
    driver.ShadingRate($2);
}

shadinginterpolation:   tSHADINGINTERPOLATION tSTRING
{
    driver.ShadingInterpolation(*$2);
}

sides:              tSIDES tNUMBER
{
    driver.Sides($2);
}

sphere:             tSPHERE tNUMBER tNUMBER tNUMBER tNUMBER
{
    driver.Sphere($2, $3, $4, $5);
}

surface:            tSURFACE tSTRING arglist
{
    int    argcount;

    argcount = buildArgList($3);
    driver.Surface(*$2, argcount, tokens, vals);
}

surface:            tSURFACE tSTRING
{
    driver.Surface(*$2, 0, RtTokens(), RtPointers());
}

shutter:            tSHUTTER tNUMBER tNUMBER
{
}

translate:          tTRANSLATE tNUMBER tNUMBER tNUMBER
{
    driver.Translate($2, $3, $4);
}

trimcurve:          tTRIMCURVE bracketnumberlist bracketnumberlist bracketnumberlist bracketnumberlist bracketnumberlist
                               bracketnumberlist bracketnumberlist bracketnumberlist bracketnumberlist
{
}

transform:          tTRANSFORM bracketnumberlist
{
}

transformbegin:     tTRANSFORMBEGIN
{
    driver.TransformBegin();
}

transformend:       tTRANSFORMEND
{
    driver.TransformEnd();
}

version:            tVERSION tNUMBER
{
}

worldbegin:         tWORLDBEGIN
{
    driver.WorldBegin();
}

worldend:           tWORLDEND
{
    driver.WorldEnd();
}

arg:                tSTRING stringlist
{
    Arg arg;
    
    arg.label = *$1;
    arg.vals.push_back(*$2);
    args.push_back(arg);

    $$ = &arg;
}
        |           tSTRING bracketstringlist
{
    Arg arg;
    
    arg.label = *$1;
    
    const auto &strs = *$2;
    arg.vals.reserve(strs.size());
    std::copy(std::begin(strs), std::end(strs), std::back_inserter(arg.vals));
    
    args.push_back(arg);

    $$ = &arg;
}

        |           tSTRING numberlist
{
    Arg arg;
    
    arg.label = *$1;

    const auto &nums = *$2;
    arg.vals.reserve(nums.size());
    std::copy(std::begin(nums), std::end(nums), std::back_inserter(arg.vals));
    
    args.push_back(arg);

    $$ = &arg;
}
        |           tSTRING bracketnumberlist
{
    Arg arg;
    
    arg.label = *$1;

    const auto &nums = *$2;
    arg.vals.reserve(nums.size());
    std::copy(std::begin(nums), std::end(nums), std::back_inserter(arg.vals));
    
    args.push_back(arg);

    $$ = &arg;
}

arglist:            arglist arg
{
    $$ = $1;
}
        |           arg
{
    $$ = $1;
}

number:             tNUMBER
{
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
}

numberlist:         numbers
{
    nl++;
    $$ = $1;
}

bracketnumberlist:  '[' numbers ']'
{
    nl++;
    $$ = $2;
}



string:             tSTRING
{
    auto &strs = strings[sl];
    strs.push_back(*$1);
        
    $$ = &strs;
}

strings:            strings string
{
    $$ = $1; 
}
        |           string
{
    $$ = $1; 
}

stringlist:         strings
{
    sl++;
    $$ = $1; 
}

bracketstringlist:  '[' strings ']' 
{
    sl++;
    $$ = $2; 
}



%%

#pragma clang diagnostic pop

void
initNumbersList()
{
    nl = 0;
    numbers.clear();
}

int 
buildArgList(Arg *arg)
{
    vals.clear();
    tokens.clear();
   
    for (const auto &arg : args)
    {
        tokens.push_back(arg.label);
        vals.push_back(arg.vals);
    }
     
    args.clear();
    
    return tokens.size();
}

RtMatrix
buildMatrix(std::vector<float> *numberlist)
{
    RtMatrix matrix;
    const auto &nums = *numberlist;
    
    matrix[0][0] = nums[0];
    matrix[0][1] = nums[1];
    matrix[0][2] = nums[2];
    matrix[0][3] = nums[3];
    matrix[1][0] = nums[4];
    matrix[1][1] = nums[5];
    matrix[1][2] = nums[6];
    matrix[1][3] = nums[7];
    matrix[2][0] = nums[8];
    matrix[2][1] = nums[9];
    matrix[2][2] = nums[10];
    matrix[2][3] = nums[11];
    matrix[3][0] = nums[12];
    matrix[3][1] = nums[13];
    matrix[3][2] = nums[14];
    matrix[3][3] = nums[15];
    
    return matrix;
}

void
Rp::Parser::error(const std::string &message)
{
   std::cerr << "Error: " << message << " (at line " << linenum << " of " << scanner.filename() << ")\n";
}
