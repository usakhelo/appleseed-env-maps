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
// DESCRIPTION: Appwizard generated plugin
// AUTHOR:
//***************************************************************************/

#include "appleseedenvmap.h"

namespace
{
    const TCHAR* AppleseedEnvMapFriendlyClassName = _T("appleseed Environment Map");
}

AppleseedEnvMapClassDesc g_appleseed_appleseedenvmap_classdesc;

enum { appleseedenvmap_params };

enum {
    pb_spin,
};


static ParamBlockDesc2 appleseedenvmap_param_blk (
    appleseedenvmap_params,
    _T("params"), 
    0,
    &g_appleseed_appleseedenvmap_classdesc,
    P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF,

    //rollout
    IDD_PANEL, IDS_PARAMS, 0, 0, NULL,
    // params
    pb_spin, _T("spin"), TYPE_FLOAT, P_ANIMATABLE, IDS_SPIN,
    p_default, 0.1f,
    p_range, 0.0f, 1000.0f,
    p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT, IDC_SPIN, 0.01f,
    p_end,
    p_end
);

//--- appleseedenvmap -------------------------------------------------------
AppleseedEnvMap::AppleseedEnvMap()
    : pblock(nullptr)
{
    for (int i = 0; i < NSUBTEX; i++)
        subtex[i] = nullptr;
    //TODO: Add all the initializing stuff
    g_appleseed_appleseedenvmap_classdesc.MakeAutoParamBlocks(this);
    Reset();
}

AppleseedEnvMap::~AppleseedEnvMap()
{

}

//From MtlBase
void AppleseedEnvMap::Reset()
{
    //TODO: Reset texmap back to its default values
    ivalid.SetEmpty();
}

void AppleseedEnvMap::Update(TimeValue t, Interval& valid)
{
    if (!ivalid.InInterval(t))
    {
        ivalid.SetInfinite();

        //m_pblock->GetValue(ParamIdBaseColor, t, m_base_color, m_params_validity);
        
        NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
    }

    valid &= ivalid;
}

Interval AppleseedEnvMap::Validity(TimeValue t)
{
    Interval valid = FOREVER;
    Update(t, valid);
    return valid;
}

ParamDlg* AppleseedEnvMap::CreateParamDlg(HWND hwMtlEdit, IMtlParams* imp)
{
    IAutoMParamDlg* masterDlg = g_appleseed_appleseedenvmap_classdesc.CreateParamDlgs(hwMtlEdit, imp, this);
    //TODO: Set the user dialog proc of the param block, and do other initialization
    return masterDlg;
}

BOOL AppleseedEnvMap::SetDlgThing(ParamDlg* dlg)
{
    //if (dlg == uvGenDlg)
    //    uvGenDlg->SetThing(uvGen);
    //else
    //    return FALSE;
    return TRUE;
}

void AppleseedEnvMap::SetSubTexmap(int i, Texmap* m)
{
    ReplaceReference(i + 2, m);
    //TODO Store the 'i-th' sub-texmap managed by the texture
}

TSTR AppleseedEnvMap::GetSubTexmapSlotName(int /*i*/)
{
    //TODO: Return the slot name of the 'i-th' sub-texmap
    return TSTR(_T(""));
}


//From ReferenceMaker
RefTargetHandle AppleseedEnvMap::GetReference(int i)
{
    //TODO: Return the references based on the index
    switch (i) {
    case 0: return pblock;
    default: return subtex[i - 2];
    }
}

void AppleseedEnvMap::SetReference(int i, RefTargetHandle rtarg)
{
    //TODO: Store the reference handle passed into its 'i-th' reference
    switch (i) {
    case 0:	pblock = (IParamBlock2 *)rtarg; break;
    default: subtex[i - 2] = (Texmap *)rtarg; break;
    }
}

RefResult AppleseedEnvMap::NotifyRefChanged(const Interval& /*changeInt*/, RefTargetHandle hTarget, PartID& /*partID*/, RefMessage message, BOOL /*propagate*/)
{
    switch (message)
    {
    case REFMSG_TARGET_DELETED:
    {
        if (hTarget == pblock) { pblock = nullptr; }
        else
        {
            for (int i = 0; i < NSUBTEX; i++)
            {
                if (subtex[i] == hTarget)
                {
                    subtex[i] = nullptr;
                    break;
                }
            }
        }
    }
    break;
    }
    return(REF_SUCCEED);
}

//From ReferenceTarget
RefTargetHandle AppleseedEnvMap::Clone(RemapDir &remap)
{
    AppleseedEnvMap *mnew = new AppleseedEnvMap();
    *((MtlBase*)mnew) = *((MtlBase*)this); // copy superclass stuff
    //TODO: Add other cloning stuff
    BaseClone(this, mnew, remap);
    return (RefTargetHandle)mnew;
}


Animatable* AppleseedEnvMap::SubAnim(int i)
{
    //TODO: Return 'i-th' sub-anim
    switch (i) {
    case 0: return pblock;
    default: return subtex[i - 2];
    }
}

TSTR AppleseedEnvMap::SubAnimName(int i)
{
    //TODO: Return the sub-anim names
    switch (i) {
    case 0: return GetString(IDS_PARAMS);
    default: return GetSubTexmapTVName(i - 1);
    }
}

IOResult AppleseedEnvMap::Save(ISave* /*isave*/)
{
    //TODO: Add code to allow plug-in to save its data
    return IO_OK;
}

IOResult AppleseedEnvMap::Load(ILoad* /*iload*/)
{
    //TODO: Add code to allow plug-in to load its data
    return IO_OK;
}

AColor AppleseedEnvMap::EvalColor(ShadeContext& /*sc*/)
{
    //TODO: Evaluate the color of texture map for the context.
    return AColor(0.0f, 0.0f, 0.0f, 0.0f);
}

float AppleseedEnvMap::EvalMono(ShadeContext& sc)
{
    //TODO: Evaluate the map for a "mono" channel
    return Intens(EvalColor(sc));
}

Point3 AppleseedEnvMap::EvalNormalPerturb(ShadeContext& /*sc*/)
{
    //TODO: Return the perturbation to apply to a normal for bump mapping
    return Point3(0, 0, 0);
}

ULONG AppleseedEnvMap::LocalRequirements(int subMtlNum)
{
    //TODO: Specify various requirements for the material
    return 0;
}

void AppleseedEnvMap::ActivateTexDisplay(BOOL /*onoff*/)
{
    //TODO: Implement this only if SupportTexDisplay() returns TRUE
}

DWORD_PTR AppleseedEnvMap::GetActiveTexHandle(TimeValue /*t*/, TexHandleMaker& /*maker*/)
{
    //TODO: Return the texture handle to this texture map
    return 0;
}

//
// AppleseedDisneyMtlBrowserEntryInfo class implementation.
//

const MCHAR* AppleseedEnvMapBrowserEntryInfo::GetEntryName() const
{
    return AppleseedEnvMapFriendlyClassName;
}

const MCHAR* AppleseedEnvMapBrowserEntryInfo::GetEntryCategory() const
{
    return _T("Maps\\appleseed");
}

Bitmap* AppleseedEnvMapBrowserEntryInfo::GetEntryThumbnail() const
{
    // todo: implement.
    return nullptr;
}

FPInterface* AppleseedEnvMapClassDesc::GetInterface(Interface_ID id)
{
    if (id == IMATERIAL_BROWSER_ENTRY_INFO_INTERFACE)
        return &m_browser_entry_info;

    return ClassDesc2::GetInterface(id);
}

AColor AppleseedEnvMapSampler::Sample(ShadeContext & sc, float u, float v)
{
    return AColor();
}

AColor AppleseedEnvMapSampler::SampleFilter(ShadeContext & sc, float u, float v, float du, float dv)
{
    return AColor();
}

float AppleseedEnvMapSampler::SampleMono(ShadeContext & sc, float u, float v)
{
    return 0.0f;
}

float AppleseedEnvMapSampler::SampleMonoFilter(ShadeContext & sc, float u, float v, float du, float dv)
{
    return 0.0f;
}
