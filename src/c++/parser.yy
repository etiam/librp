%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {Rp}
%define parser_class_name {Parser}

%code requires
{
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

#include <iostream>
#include <cstdlib>
#include <fstream>

#include "yacc.h"
#include "driver.h"

#undef yylex
#define yylex scanner.yylex
    
#define        ALLOC_BLOCK 20000
#define        MAX_ARGS    100

enum 
{
    ARGSTRING,
    ARGNUMLIST
};
    
typedef     int         RtInt;
typedef     float       RtFloat;
typedef     short       RtBoolean;
typedef     char *      RtString;
typedef     char *      RtToken;
typedef     void *      RtPointer;
typedef     RtFloat     RtColor[3];
typedef     RtFloat     RtPoint[3];
typedef     RtFloat     RtMatrix[4][4];
typedef     RtPointer   RtLightHandle;

#define     RtVoid      void
#define     RI_NULL     ((RtToken)0)
 
void            yyerror(char*);
int             yyparse();
extern int      yywrap();
extern FILE *   yyin;

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
RtToken         tokens[MAX_ARGS];
RtPointer       parms[MAX_ARGS];
RtInt           plengths[MAX_ARGS];

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wwritable-strings"

%}

/* token types */
%union 
{
   float       dval;
   char *      string;
   ArgNode *   anNode;
   float *     dValue;
}

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
//        |           basis
//        |           begin
//        |           camera
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
//        |           errorhandler
        |           exposure
        |           format
        |           frameaspectratio
        |           framebegin
        |           frameend
//        |           geometricapproximation
        |           hider
//        |           hierarchicalsubdivisionmesh
        |           identity
//        |           illuminate
        |           lightsource
//        |           matte
//        |           motionbegin
//        |           motionend
        |           nupatch
        |           opacity
        |           option
        |           orientation
        |           pixelsamples
//        |           pixelfilter
//        |           pixelvariance
        |           pointsgeneralpolygons
        |           pointspolygons
        |           polygon
//        |           procedural
        |           projection
//        |           quantize
//        |           readarchive
//        |           relativedetail
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
//        |           sides
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
    driver.RrAttributeV($2, iArgCount, tokens, parms, plengths); 
};

attributebegin:     tATTRIBUTEBEGIN
{
 RrAttributeBegin();
};

attributeend:       tATTRIBUTEEND
{
 RrAttributeEnd();
};

clipping:           tCLIPPING tNUMBER tNUMBER
{
};

color:              tCOLOR {iTLC = 0; iTLCS = 0;} bracketnumberlist
{
    RtColor    color;
    
    if(iTLC - iTLCS != 3)
    {
        sprintf(sErrorString,"Wrong number of color samples: %i, expecting 3", iTLC - iTLCS);
        yyerror(sErrorString);
    }
    else
    {
        color[0] = dTempList[0];
        color[1] = dTempList[1];
        color[2] = dTempList[2];
        RrColor(color);
    }
}; 
        |
                    tCOLOR {iTLC = 0; iTLCS = 0;} numberlist
{
    RtColor    color;
    
    if(iTLC - iTLCS != 3)
    {
        sprintf(sErrorString,"Wrong number of color samples: %i, expecting 3", iTLC - iTLCS);
        yyerror(sErrorString);
    }
    else
    {
        color[0] = dTempList[0];
        color[1] = dTempList[1];
        color[2] = dTempList[2];
        RrColor(color);
    }
}; 

concattransform:    tCONCATTRANSFORM {iTLC = 0; iTLCS = 0;} bracketnumberlist 
{
    RtMatrix    matrix;
    
    if(iTLC - iTLCS != 16)
    {
        sprintf(sErrorString,"Bad number of numeric parameters to Transform: %i", iTLC - iTLCS);
        yyerror(sErrorString);
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
        RrConcatTransform(matrix);
    }
}

coordinatesystem:   tCOORDINATESYSTEM tSTRING
{
};

cropwindow:         tCROPWINDOW tNUMBER tNUMBER tNUMBER tNUMBER
{
    RrCropWindow($2,$3,$4,$5);
};

cylinder:           tCYLINDER tNUMBER tNUMBER tNUMBER tNUMBER
{
    RrCylinder($2,$3,$4,$5);
};

declare:            tDECLARE tSTRING tSTRING
{
    RrDeclare($2,$3);
}

display:            tDISPLAY tSTRING tSTRING tSTRING {iTLC = 0;} arglist
{
    int     iArgCount;
    
    iArgCount = buildRIarglist($6);
    RrDisplayV($2,$3,$4,iArgCount,tokens,parms,plengths);
};

exposure:           tEXPOSURE tNUMBER tNUMBER
{
};

format:             tFORMAT tNUMBER tNUMBER tNUMBER
{
    RrFormat((int)$2,(int)$3,(float)$4);
};

frameaspectratio:   tFRAMEASPECTRATIO tNUMBER
{
    RrFrameAspectRatio($2);
};

framebegin:         tFRAMEBEGIN tNUMBER
{
};

frameend:           tFRAMEEND
{
};

hider:              tHIDER tSTRING {iTLC=0;} arglist
{
};

identity:           tIDENTITY
{
};

lightsource:        tLIGHTSOURCE {iTLC=0;} tSTRING tNUMBER arglist
{
    int     iArgCount;
    
    iArgCount = buildRIarglist($5);
    RrLightSourceV($3,iArgCount,tokens,parms,plengths); 
};

nupatch:            tNUPATCH 
                    tNUMBER tNUMBER {iTLC = 0; iTLCS = 0;} bracketnumberlist 
                    tNUMBER tNUMBER
                    tNUMBER tNUMBER {iVOrder = iTLC;} bracketnumberlist
                    tNUMBER tNUMBER {iVOrderL = iTLC;} arglist
{
    int        i,iArgCount;
    RtFloat    *uknot,*vknot;
    
    uknot = new RtFloat[(int)$2+(int)$3];
    for(i=0; i < iVOrder; i++)
        uknot[i] = dTempList[i];
    vknot = new RtFloat[(int)$8+(int)$9];
    for(i=iVOrder; i < iVOrderL; i++)
        vknot[i-iVOrder] = dTempList[i];
    iArgCount = buildRIarglist($15);
    RrNuPatch((int)$2,(int)$3,uknot,$6,$7,(int)$8,(int)$9,vknot,$12,$13, iArgCount,tokens,parms,plengths);
    delete(uknot);
    delete(vknot);
};

opacity:            tOPACITY {iTLC = 0; iTLCS = 0;} bracketnumberlist
{
}; 

option:             tOPTION tSTRING {iTLC=0;} arglist
{
};

orientation:        tORIENTATION tSTRING
{
};

pixelsamples:       tPIXELSAMPLES tNUMBER tNUMBER
{
    RrPixelSamples($2,$3);
};

pointsgeneralpolygons:    tPOINTSGENERALPOLYGONS 
            {iTLC = 0; iTLCS = 0;} bracketnumberlist 
            {iPGPnlp = iTLC;} bracketnumberlist
            {iPPnv = iTLC;} bracketnumberlist 
            {iPPvll = iTLC;} arglist
{
 int    i,iArgCount;
 RtInt    *nverts,*verts;

 nverts = new RtInt[iPPnv-iPGPnlp];
 verts = new RtInt[iPPvll-iPPnv];
 for(i=iPGPnlp; i < iPPnv; i++)
  nverts[i-iPGPnlp] = (int)dTempList[i];
 for(i=iPPnv; i < iPPvll; i++)
  verts[i-iPPnv] = (int)dTempList[i];
 iArgCount = buildRIarglist($9);
 RrPointsPolygonsV(iPPnv-iPGPnlp,nverts,verts,iArgCount,tokens,parms,plengths);
 delete nverts;
 delete verts;
};

pointspolygons:     tPOINTSPOLYGONS {iTLC = 0; iTLCS = 0;} bracketnumberlist 
                                    {iPPnv = iTLC;} bracketnumberlist 
                                    {iPPvll = iTLC;} arglist
{
    int    i,iArgCount;
    RtInt    *nverts,*verts;
    
    nverts = new RtInt[iPPnv];
    verts = new RtInt[iPPvll-iPPnv];
    for(i=0; i < iPPnv; i++)
        nverts[i] = (int)dTempList[i];
    for(i=iPPnv; i < iPPvll; i++)
        verts[i-iPPnv] = (int)dTempList[i];
    iArgCount = buildRIarglist($7);
    RrPointsPolygonsV(iPPnv,nverts,verts,iArgCount,tokens,parms,plengths);
    delete nverts;
    delete verts;
};

polygon:            tPOLYGON {iTLC = 0;} arglist
{
    int    iArgCount;
    
    iArgCount = buildRIarglist($3);
    RrPolygonV($3->iListCount/3,iArgCount,tokens,parms,plengths); 
};

projection:         tPROJECTION tSTRING
{
    RrProjectionV($2,0,NULL,NULL,NULL);
};
        |
                    tPROJECTION tSTRING {iTLC = 0;} arglist
{
    int    iArgCount;
    
    iArgCount = buildRIarglist($4);
    RrProjectionV($2,iArgCount,tokens,parms,plengths);
};

reverseorientation: tREVERSEORIENTATION
{
    RrReverseOrientation();
};

rotate:             tROTATE tNUMBER tNUMBER tNUMBER tNUMBER
{
    RrRotate($2,$3,$4,$5);
};

scale:              tSCALE tNUMBER tNUMBER tNUMBER
{
};

screenwindow:       tSCREENWINDOW tNUMBER tNUMBER tNUMBER tNUMBER
{
};

shadingrate:        tSHADINGRATE tNUMBER
{
};

shadinginterpolation:   tSHADINGINTERPOLATION tSTRING
{
};

sphere:             tSPHERE tNUMBER tNUMBER tNUMBER tNUMBER
{
    RrSphere($2,$3,$4,$5);
};

surface:            tSURFACE tSTRING {iTLC = 0;} arglist
{
    int    iArgCount;
    
    iArgCount = buildRIarglist($4);
    RrSurfaceV($2,iArgCount,tokens,parms,plengths); 
};

surface:            tSURFACE tSTRING
{
    RrSurfaceV($2,0,NULL,NULL,NULL);
};

shutter:            tSHUTTER tNUMBER tNUMBER
{
};

translate:          tTRANSLATE tNUMBER tNUMBER tNUMBER
{
    RrTranslate($2,$3,$4);
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
    RtInt        *ncurves,*order,*n;
    RtFloat    *knot,*min,*max,*u,*v,*w;
    
    ncurves = new RtInt[iNC];
    order = new RtInt[iNO-iNC];
    knot = new RtFloat[iNK-iNO];
    min = new RtFloat[iNMn-iNK];
    max = new RtFloat[iNMx-iNMn];
    n = new RtInt[iNN-iNMx];
    u = new RtFloat[iNU-iNN];
    v = new RtFloat[iNV-iNU];
    w = new RtFloat[iTLC-iNV];
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
    
    RrTrimCurve(iNC,ncurves,order,knot,min,max,n,u,v,w); 
};

transform:          tTRANSFORM bracketnumberlist
{
};
 
transformbegin:     tTRANSFORMBEGIN 
{
    RrTransformBegin();
};

transformend:       tTRANSFORMEND
{
    RrTransformEnd();
};

version:            tVERSION tNUMBER
{
};

worldbegin:         tWORLDBEGIN
{
    RrWorldBegin();
};

worldend:           tWORLDEND
{
    RrWorldEnd();
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
    anTempNode->sLabel = $1;
    anTempNode->sValue = $2; 
    $$ = anTempNode;
}
        |           tSTRING tSTRING
{
    anTempNode = NewNode();
    anTempNode->iType = ARGSTRING;
    anTempNode->sLabel = $1;
    anTempNode->sValue = $2; 
    $$ = anTempNode;
}
        |           tSTRING{iTLCS = iTLC;}numberlist 
{
    anTempNode = NewNode();
    anTempNode->iType = ARGNUMLIST;
    anTempNode->sLabel = $1;
    anTempNode->dValue = $3;
    anTempNode->iListCount = iTLC - iTLCS;
$$ = anTempNode;
}
        |           tSTRING{iTLCS = iTLC;}bracketnumberlist 
{
    anTempNode = NewNode();
    anTempNode->iType = ARGNUMLIST;
    anTempNode->sLabel = $1;
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
    return(TRUE);
}

bool 
parse(const std::string &filename, bool debug)
{
    FILE           *fPtr;

    RrBegin(RI_NULL);
    if(!filename.empty())
    {
        fPtr = fopen(filename.c_str(),"r");
        if(!fPtr)
        {
            fprintf(stderr,"couldn't open \"%s\"\n", filename.c_str());
            return(FALSE);
        }
        yyin = fPtr;
        sSource = strdup(filename.c_str());
    }
    else
    {
        yyin = stdin;
        sSource = strdup("stdin");
    }
    yydebug = static_cast<int>(debug);
    yyparse();
    RrEnd();
    return(TRUE);
}

void yyerror(char *s)
{
    fprintf(stderr,"%s (at line %d in %s)\n",s,iLineNum,sSource);
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

    for(anTempNode = anNode; anTempNode ; anTempNode = anTempNode->Next)
    {
        tokens[iArgCount] = anTempNode->sLabel;
        switch(anTempNode->iType)
        {
        case ARGSTRING : parms[iArgCount] = anTempNode->sValue;
        plengths[iArgCount] = strlen(anTempNode->sValue);
        break;
        case ARGNUMLIST: parms[iArgCount] = anTempNode->dValue;
        plengths[iArgCount] = sizeof(float)*anTempNode->iListCount;
        break;
        }
        iArgCount++;
    } 
    return(iArgCount);
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
Rp::Parser::error(const std::string &err_message)
{
   std::cerr << "Error: " << err_message << "\n"; 
}
