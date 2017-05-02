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

#define appleseedenvmap_CLASS_ID	Class_ID(0x52848b4a, 0x5e6cb361)

#define NSUBTEX   1 // TODO: number of sub-textures supported by this plugin
// Reference Indexes
#define COORD_REF  0
#define PBLOCK_REF 1

class appleseedenvmap;

class appleseedenvmapSampler: public MapSampler
{
private:
	appleseedenvmap* mTexture;
public:
	appleseedenvmapSampler() : mTexture(nullptr) { }
	appleseedenvmapSampler(appleseedenvmap *c) { mTexture= c; }
	~appleseedenvmapSampler() { }

	void   Set(appleseedenvmap *c) { mTexture = c; }
	AColor Sample(ShadeContext& sc, float u,float v);
	AColor SampleFilter(ShadeContext& sc, float u,float v, float du, float dv);
	float  SampleMono(ShadeContext& sc, float u,float v);
	float  SampleMonoFilter(ShadeContext& sc, float u,float v, float du, float dv);
} ;


class appleseedenvmap : public Texmap {
public:
	//Constructor/Destructor
	appleseedenvmap();
	virtual ~appleseedenvmap();

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
	virtual Class_ID  ClassID() {return appleseedenvmap_CLASS_ID;}
	virtual SClass_ID SuperClassID() { return TEXMAP_CLASS_ID; }
	virtual void GetClassName(TSTR& s) {s = GetString(IDS_CLASS_NAME);}

	virtual RefTargetHandle Clone( RemapDir &remap );
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



class appleseedenvmapClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new appleseedenvmap(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return TEXMAP_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return appleseedenvmap_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("appleseedenvmap"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetappleseedenvmapDesc() { 
	static appleseedenvmapClassDesc appleseedenvmapDesc;
	return &appleseedenvmapDesc; 
}




enum { appleseedenvmap_params };


//TODO: Add enums for various parameters
enum { 
	pb_spin,
	pb_coords,
};




static ParamBlockDesc2 appleseedenvmap_param_blk ( appleseedenvmap_params, _T("params"),  0, GetappleseedenvmapDesc(), 
	P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF, 
	//rollout
	IDD_PANEL, IDS_PARAMS, 0, 0, NULL,
	// params
	pb_spin, 			_T("spin"), 		TYPE_FLOAT, 	P_ANIMATABLE, 	IDS_SPIN, 
		p_default, 		0.1f, 
		p_range, 		0.0f,1000.0f, 
		p_ui, 			TYPE_SPINNER,		EDITTYPE_FLOAT, IDC_EDIT,	IDC_SPIN, 0.01f, 
		p_end,
	pb_coords,			_T("coords"),		TYPE_REFTARG,	P_OWNERS_REF,	IDS_COORDS,
		p_refno,		COORD_REF, 
		p_end,
	p_end
	);




ParamDlg* appleseedenvmap::uvGenDlg;

//--- appleseedenvmap -------------------------------------------------------
appleseedenvmap::appleseedenvmap()
	: pblock(nullptr)
{
	for (int i = 0; i < NSUBTEX; i++)
		subtex[i] = nullptr;
	//TODO: Add all the initializing stuff
	GetappleseedenvmapDesc()->MakeAutoParamBlocks(this);
	Reset();
}

appleseedenvmap::~appleseedenvmap()
{

}

//From MtlBase
void appleseedenvmap::Reset()
{
	if (uvGen) 
		uvGen->Reset();
	else
		ReplaceReference( 0, GetNewDefaultUVGen());
	//TODO: Reset texmap back to its default values
	ivalid.SetEmpty();
}

void appleseedenvmap::Update(TimeValue /*t*/, Interval& /*valid*/)
{
	//TODO: Add code to evaluate anything prior to rendering
}

Interval appleseedenvmap::Validity(TimeValue /*t*/)
{
	//TODO: Update ivalid here
	return ivalid;
}

ParamDlg* appleseedenvmap::CreateParamDlg(HWND hwMtlEdit, IMtlParams* imp)
{
	IAutoMParamDlg* masterDlg = GetappleseedenvmapDesc()->CreateParamDlgs(hwMtlEdit, imp, this);
	uvGenDlg = uvGen->CreateParamDlg(hwMtlEdit, imp);
	masterDlg->AddDlg(uvGenDlg);
	//TODO: Set the user dialog proc of the param block, and do other initialization
	return masterDlg;
}

BOOL appleseedenvmap::SetDlgThing(ParamDlg* dlg)
{
	if (dlg == uvGenDlg)
		uvGenDlg->SetThing(uvGen);
	else
		return FALSE;
	return TRUE;
}

void appleseedenvmap::SetSubTexmap(int i, Texmap* m)
{
	ReplaceReference(i+2,m);
	//TODO Store the 'i-th' sub-texmap managed by the texture
}

TSTR appleseedenvmap::GetSubTexmapSlotName(int /*i*/)
{
	//TODO: Return the slot name of the 'i-th' sub-texmap
	return TSTR(_T(""));
}


//From ReferenceMaker
RefTargetHandle appleseedenvmap::GetReference(int i)
{
	//TODO: Return the references based on the index
	switch (i) {
		case 0: return uvGen;
		case 1: return pblock;
		default: return subtex[i-2];
		}
}

void appleseedenvmap::SetReference(int i, RefTargetHandle rtarg)
{
	//TODO: Store the reference handle passed into its 'i-th' reference
	switch(i) {
		case 0: uvGen = (UVGen *)rtarg; break;
		case 1:	pblock = (IParamBlock2 *)rtarg; break;
		default: subtex[i-2] = (Texmap *)rtarg; break;
	}
}

RefResult appleseedenvmap::NotifyRefChanged(const Interval& /*changeInt*/, RefTargetHandle hTarget, PartID& /*partID*/, RefMessage message, BOOL /*propagate*/ )
{
	switch (message)
	{
	case REFMSG_TARGET_DELETED:
		{
			if      (hTarget == uvGen) { uvGen = nullptr; }
			else if (hTarget == pblock) { pblock = nullptr; }
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
RefTargetHandle appleseedenvmap::Clone(RemapDir &remap)
{
	appleseedenvmap *mnew = new appleseedenvmap();
	*((MtlBase*)mnew) = *((MtlBase*)this); // copy superclass stuff
	//TODO: Add other cloning stuff
	BaseClone(this, mnew, remap);
	return (RefTargetHandle)mnew;
}


Animatable* appleseedenvmap::SubAnim(int i)
{
	//TODO: Return 'i-th' sub-anim
	switch (i) {
		case 0: return uvGen;
		case 1: return pblock;
		default: return subtex[i-2];
		}
}

TSTR appleseedenvmap::SubAnimName(int i)
{
	//TODO: Return the sub-anim names
	switch (i) {
		case 0: return GetString(IDS_COORDS);
		case 1: return GetString(IDS_PARAMS);
		default: return GetSubTexmapTVName(i-1);
		}
}

IOResult appleseedenvmap::Save(ISave* /*isave*/)
{
	//TODO: Add code to allow plug-in to save its data
	return IO_OK;
}

IOResult appleseedenvmap::Load(ILoad* /*iload*/)
{
	//TODO: Add code to allow plug-in to load its data
	return IO_OK;
}

AColor appleseedenvmap::EvalColor(ShadeContext& /*sc*/)
{
	//TODO: Evaluate the color of texture map for the context.
	return AColor (0.0f,0.0f,0.0f,0.0f);
}

float appleseedenvmap::EvalMono(ShadeContext& sc)
{
	//TODO: Evaluate the map for a "mono" channel
	return Intens(EvalColor(sc));
}

Point3 appleseedenvmap::EvalNormalPerturb(ShadeContext& /*sc*/)
{
	//TODO: Return the perturbation to apply to a normal for bump mapping
	return Point3(0, 0, 0);
}

ULONG appleseedenvmap::LocalRequirements(int subMtlNum)
{
	//TODO: Specify various requirements for the material
	return uvGen->Requirements(subMtlNum);
}

void appleseedenvmap::ActivateTexDisplay(BOOL /*onoff*/)
{
	//TODO: Implement this only if SupportTexDisplay() returns TRUE
}

DWORD_PTR appleseedenvmap::GetActiveTexHandle(TimeValue /*t*/, TexHandleMaker& /*maker*/)
{
	//TODO: Return the texture handle to this texture map
	return 0;
}
