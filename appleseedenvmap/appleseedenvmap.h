#pragma once

//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Includes for Plugins
// AUTHOR: 
//***************************************************************************/

// 3ds Max headers.
#include "3dsmaxsdk_preinclude.h"
#include "resource.h"
#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <maxtypes.h>
#include <stdmat.h>
#include <imtl.h>
#include <macrorec.h>
#include <IMaterialBrowserEntryInfo.h>

extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;

#define appleseedenvmap_CLASS_ID	Class_ID(0x52848b4a, 0x5e6cb361)

#define NSUBTEX   1 // TODO: number of sub-textures supported by this plugin
// Reference Indexes
#define COORD_REF  0
#define PBLOCK_REF 1

class AppleseedEnvMap;

class AppleseedEnvMapSampler : public MapSampler
{
private:
    AppleseedEnvMap* mTexture;
public:
    AppleseedEnvMapSampler() : mTexture(nullptr) { }
    AppleseedEnvMapSampler(AppleseedEnvMap *c) { mTexture = c; }
    ~AppleseedEnvMapSampler() { }

    void   Set(AppleseedEnvMap *c) { mTexture = c; }
    AColor Sample(ShadeContext& sc, float u, float v);
    AColor SampleFilter(ShadeContext& sc, float u, float v, float du, float dv);
    float  SampleMono(ShadeContext& sc, float u, float v);
    float  SampleMonoFilter(ShadeContext& sc, float u, float v, float du, float dv);
};

class AppleseedEnvMap : public Texmap {
public:
    //Constructor/Destructor
    AppleseedEnvMap();
    virtual ~AppleseedEnvMap();

    //From MtlBase
    virtual ParamDlg* CreateParamDlg(HWND hwMtlEdit, IMtlParams* imp);
    virtual BOOL SetDlgThing(ParamDlg* dlg);
    virtual void Update(TimeValue t, Interval& valid);
    virtual void Reset();
    virtual Interval Validity(TimeValue t);
    virtual ULONG LocalRequirements(int subMtlNum);

    //TODO: Return the number of sub-textures
    virtual int NumSubTexmaps() { return NSUBTEX; }
    //TODO: Return the pointer to the 'i-th' sub-texmap
    virtual Texmap* GetSubTexmap(int i) { return subtex[i]; }
    virtual void SetSubTexmap(int i, Texmap *m);
    virtual TSTR GetSubTexmapSlotName(int i);

    //From Texmap
    virtual RGBA   EvalColor(ShadeContext& sc);
    virtual float  EvalMono(ShadeContext& sc);
    virtual Point3 EvalNormalPerturb(ShadeContext& sc);

    //TODO: Returns TRUE if this texture can be used in the interactive renderer
    virtual BOOL SupportTexDisplay() { return FALSE; }
    virtual void ActivateTexDisplay(BOOL onoff);
    virtual DWORD_PTR GetActiveTexHandle(TimeValue t, TexHandleMaker& thmaker);
    //TODO: Return UV transformation matrix for use in the viewports
    virtual void GetUVTransform(Matrix3 &uvtrans) { uvGen->GetUVTransform(uvtrans); }
    //TODO: Return the tiling state of the texture for use in the viewports
    virtual int    GetTextureTiling() { return  uvGen->GetTextureTiling(); }
    virtual int    GetUVWSource() { return uvGen->GetUVWSource(); }
    virtual UVGen* GetTheUVGen() { return uvGen; }

    //TODO: Return anim index to reference index
    virtual int SubNumToRefNum(int subNum) { return subNum; }

    // Loading/Saving
    virtual IOResult Load(ILoad *iload);
    virtual IOResult Save(ISave *isave);

    //From Animatable
    virtual Class_ID  ClassID() { return appleseedenvmap_CLASS_ID; }
    virtual SClass_ID SuperClassID() { return TEXMAP_CLASS_ID; }
    virtual void GetClassName(TSTR& s) { s = GetString(IDS_CLASS_NAME); }

    virtual RefTargetHandle Clone(RemapDir &remap);
    virtual RefResult NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate);


    virtual int NumSubs() { return 2 + NSUBTEX; }
    virtual Animatable* SubAnim(int i);
    virtual TSTR SubAnimName(int i);

    // TODO: Maintain the number or references here
    virtual int NumRefs() { return 2 + NSUBTEX; }
    virtual RefTargetHandle GetReference(int i);


    virtual int	NumParamBlocks() { return 1; }					// return number of ParamBlocks in this instance
    virtual IParamBlock2* GetParamBlock(int /*i*/) { return pblock; } // return i'th ParamBlock
    virtual IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock->ID() == id) ? pblock : NULL; } // return id'd ParamBlock

    virtual void DeleteThis() { delete this; }

protected:
    virtual void SetReference(int i, RefTargetHandle rtarg);

private:
    UVGen*           uvGen;           // ref 0
    IParamBlock2*    pblock;          // ref 1
    Texmap*          subtex[NSUBTEX]; // Other refs

    static ParamDlg* uvGenDlg;
    Interval         ivalid;
};

//
// AppleseedEnvMap material browser info.
//

class AppleseedEnvMapBrowserEntryInfo
    : public IMaterialBrowserEntryInfo
{
public:
    virtual const MCHAR* GetEntryName() const override;
    virtual const MCHAR* GetEntryCategory() const override;
    virtual Bitmap* GetEntryThumbnail() const override;
};

class AppleseedEnvMapClassDesc : public ClassDesc2
{
public:
    virtual int IsPublic() { return TRUE; }
    virtual void* Create(BOOL /*loading = FALSE*/) { return new AppleseedEnvMap(); }
    virtual const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
    virtual SClass_ID SuperClassID() { return TEXMAP_CLASS_ID; }
    virtual Class_ID ClassID() { return appleseedenvmap_CLASS_ID; }
    virtual const TCHAR* Category() { return GetString(IDS_CATEGORY); }
    virtual FPInterface* GetInterface(Interface_ID id) override;
    virtual const TCHAR* InternalName() { return _T("appleseedenvmap"); }	// returns fixed parsable name (scripter-visible name)
    virtual HINSTANCE HInstance() { return hInstance; }					// returns owning module handle

private:
    AppleseedEnvMapBrowserEntryInfo m_browser_entry_info;
};

extern AppleseedEnvMapClassDesc g_appleseed_appleseedenvmap_classdesc;



