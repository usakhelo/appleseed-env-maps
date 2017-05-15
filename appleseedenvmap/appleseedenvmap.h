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

    // ISubMap methods.
    virtual int NumSubTexmaps() override;
    virtual Texmap* GetSubTexmap(int i) override;
    virtual void SetSubTexmap(int i, Texmap* texmap) override;
    virtual int MapSlotType(int i) override;
    virtual MSTR GetSubTexmapSlotName(int i) override;

    //TODO: Returns TRUE if this texture can be used in the interactive renderer
    virtual BOOL SupportTexDisplay() { return FALSE; }
    virtual void ActivateTexDisplay(BOOL onoff);
    virtual DWORD_PTR GetActiveTexHandle(TimeValue t, TexHandleMaker& thmaker);

    //From Animatable
    virtual Class_ID  ClassID() override;
    virtual SClass_ID SuperClassID() override;
    virtual void GetClassName(TSTR& s) override;
    virtual void DeleteThis() override;

    virtual RefTargetHandle Clone(RemapDir &remap);
    virtual RefResult NotifyRefChanged(
        const Interval&     changeInt,
        RefTargetHandle     hTarget,
        PartID&             partID,
        RefMessage          message,
        BOOL                propagate) override;
    virtual int NumRefs() override;
    virtual RefTargetHandle GetReference(int i) override;

    virtual int NumSubs() override;
    virtual Animatable* SubAnim(int i) override;
    virtual TSTR SubAnimName(int i) override;
    virtual int SubNumToRefNum(int subNum) override;

    virtual int	NumParamBlocks() override;
    virtual IParamBlock2* GetParamBlock(int i) override;
    virtual IParamBlock2* GetParamBlockByID(BlockID id) override;
    
    // Loading/Saving
    virtual IOResult Save(ISave *isave);
    virtual IOResult Load(ILoad *iload);
    
    //From Texmap
    virtual RGBA   EvalColor(ShadeContext& sc);
    virtual float  EvalMono(ShadeContext& sc);
    virtual Point3 EvalNormalPerturb(ShadeContext& sc);

protected:
    virtual void SetReference(int i, RefTargetHandle rtarg);

private:
    IParamBlock2*    m_pblock;          // ref 0
    Interval         m_params_validity;

    float m_sun_theta;
    float m_sun_phi;
    float m_turbidity;
    Texmap* m_turbidity_map;
    BOOL m_turbidity_map_on;
    float m_turb_multiplier;
    float m_lumin_multiplier;
    float m_lumin_gamma;
    float m_sat_multiplier;
    float m_horizon_shift;
    float m_ground_albedo;
    int m_sky_type;
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
    virtual const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME_CDESC); }
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



