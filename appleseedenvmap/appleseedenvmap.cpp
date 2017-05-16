#include "appleseedenvmap.h"

namespace
{
    const TCHAR* AppleseedEnvMapFriendlyClassName = _T("appleseed Environment Map");
}

AppleseedEnvMapClassDesc g_appleseed_appleseedenvmap_classdesc;

enum { ParamBlockIdEnvMap };
enum { ParamBlockRefEnvMap };

enum ParamId 
{
    ParamIdSunTheta             = 0,
    ParamIdSunPhi               = 1,
    ParamIdTurbidity            = 2,
    ParamIdTurbidityMap         = 3,
    ParamIdTurbidityMapOn       = 4,
    ParamIdTurbMultiplier       = 5,
    ParamIdLuminMultiplier      = 6,
    ParamIdLuminGamma           = 7,
    ParamIdSatMultiplier        = 8,
    ParamIdHorizonShift         = 10,
    ParamIdGroundAlbedo         = 11,
    ParamIdSkyType              = 12
};

enum TexmapId
{
    TexmapIdTurbidity           = 0,
    TexmapCount                 // keep last
};

const MSTR g_texmap_slot_names[TexmapCount] =
{
    _T("Turbidity"),
};

const ParamId g_texmap_id_to_param_id[TexmapCount] =
{
    ParamIdTurbidityMap
};

static ParamBlockDesc2 appleseedenvmap_param_blk (
    ParamBlockIdEnvMap,
    _T("appleseedEnvironmentMapParams"), 
    0,
    &g_appleseed_appleseedenvmap_classdesc,
    P_AUTO_CONSTRUCT + P_AUTO_UI,

     // --- P_AUTO_CONSTRUCT arguments ---
    ParamBlockRefEnvMap,                     // parameter block's reference number

    // --- P_AUTO_UI arguments for Disney rollup ---
    IDD_ENVMAP_PANEL,                        // ID of the dialog template
    IDS_ENVMAP_PARAMS,                  // ID of the dialog's title string
    0,                                          // IParamMap2 creation/deletion flag mask
    0,                                          // rollup creation flag
    nullptr,   

    // params
    ParamIdSunTheta, _T("sun_theta"), TYPE_FLOAT, P_ANIMATABLE, IDS_THETA,
        p_default, 45.0f,
        p_range, 0.0f, 90.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_THETA, IDC_SPIN_THETA, 0.01f,
    p_end,

    ParamIdSunPhi, _T("sun_phi"), TYPE_FLOAT, P_ANIMATABLE, IDS_PHI,
        p_default, 90.0f,
        p_range, 0.0f, 360.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_PHI, IDC_SPIN_PHI, 0.01f,
    p_end,

    ParamIdTurbidity, _T("turbitidy"), TYPE_FLOAT, P_ANIMATABLE, IDS_TURBIDITY,
        p_default, 1.0f,
        p_range, 0.0f, 1.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_TURBIDITY, IDC_SPIN_TURBIDITY, 0.01f,
    p_end,

    ParamIdTurbidityMap, _T("turbitidy_map"), TYPE_TEXMAP, 0, IDS_TURB_MAP,
        p_subtexno, TexmapIdTurbidity,
        p_ui, TYPE_TEXMAPBUTTON, IDC_PICK_TURB_TEXTURE,
    p_end,

    ParamIdTurbidityMapOn, _T("turbitidy_map_on"), TYPE_BOOL, 0, IDS_TURB_MAP_ON,
        p_default, TRUE,
        p_ui, TYPE_SINGLECHEKBOX, IDC_TURB_TEX_ON,
    p_end,

    ParamIdTurbMultiplier, _T("turbitidy_multiplier"), TYPE_FLOAT, P_ANIMATABLE, IDS_TURB_MULTIPLIER,
        p_default, 2.0f,
        p_range, 0.0f, 8.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_TURB_MULTIPLIER, IDC_SPIN_TURB_MULTIPLIER, 0.01f,
    p_end,

    ParamIdLuminMultiplier, _T("luminance_multiplier"), TYPE_FLOAT, P_ANIMATABLE, IDS_LUMINANCE_MULTIPLIER,
        p_default, 1.0f,
        p_range, 0.0f, 10.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_LUMIN_MULTIPLIER, IDC_SPIN_LUMIN_MULTIPLIER, 0.01f,
    p_end,

    ParamIdLuminGamma, _T("luminance_gamma"), TYPE_FLOAT, P_ANIMATABLE, IDS_LUMINANCE_GAMMA,
        p_default, 1.0f,
        p_range, 0.0f, 3.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_LUMIN_GAMMA, IDC_SPIN_LUMIN_GAMMA, 0.01f,
    p_end,

    ParamIdSatMultiplier, _T("saturation_multiplier"), TYPE_FLOAT, P_ANIMATABLE, IDS_SAT_MULITPLIER,
        p_default, 1.0f,
        p_range, 0.0f, 10.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_SATUR_MULTIPLIER, IDC_SPIN_SATUR_MULTIPLIER, 0.01f,
    p_end,

    ParamIdHorizonShift, _T("horizon_shift"), TYPE_FLOAT, P_ANIMATABLE, IDS_HORIZON_SHIFT,
        p_default, 0.0f,
        p_range, -10.0f, 10.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_HORIZON_SHIFT, IDC_SPIN_HORIZON_SHIFT, 0.01f,
    p_end,

    ParamIdGroundAlbedo, _T("ground_albedo"), TYPE_FLOAT, P_ANIMATABLE, IDS_GROUND_ALBEDO,
        p_default, 0.3f,
        p_range, 0.0f, 1.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_GROUND_ALBEDO, IDC_SPIN_GROUND_ALBEDO, 0.01f,
    p_end,

    ParamIdSkyType, _T("sky_type"), TYPE_INT, 0, IDS_SKY_TYPE,
        p_ui, TYPE_INT_COMBOBOX, IDC_COMBO_SKY_TYPE,
        2, IDS_SKY_TYPE_1, IDS_SKY_TYPE_2,
        p_vals, 0, 1,
        p_default, 0,
    p_end,

    p_end
);

AppleseedEnvMap::AppleseedEnvMap()
    : m_pblock(nullptr)
    , m_sun_theta(45.0f)
    , m_sun_phi(90.0f)
    , m_turbidity(1.0f)
    , m_turbidity_map(nullptr)
    , m_turbidity_map_on(true)
    , m_turb_multiplier(2.0f)
    , m_lumin_multiplier(1.0f)
    , m_lumin_gamma(1.0f)
    , m_sat_multiplier(1.0f)
    , m_horizon_shift(0.0f)
    , m_ground_albedo(0.3f)
    , m_sky_type(0)
{
    g_appleseed_appleseedenvmap_classdesc.MakeAutoParamBlocks(this);
    Reset();
}

void AppleseedEnvMap::Reset()
{
    m_params_validity.SetEmpty();
}

void AppleseedEnvMap::Update(TimeValue t, Interval& valid)
{
    if (!m_params_validity.InInterval(t))
    {
        m_params_validity.SetInfinite();

        m_pblock->GetValue(ParamIdSunTheta, t, m_sun_theta, m_params_validity);
        m_pblock->GetValue(ParamIdSunPhi, t, m_sun_phi, m_params_validity);

        m_pblock->GetValue(ParamIdTurbidity, t, m_turbidity, m_params_validity);
        m_pblock->GetValue(ParamIdTurbidityMap, t, m_turbidity_map, m_params_validity);
        m_pblock->GetValue(ParamIdTurbidityMapOn, t, m_turbidity_map_on, m_params_validity);

        m_pblock->GetValue(ParamIdTurbMultiplier, t, m_turb_multiplier, m_params_validity);
        m_pblock->GetValue(ParamIdLuminMultiplier, t, m_lumin_multiplier, m_params_validity);
        m_pblock->GetValue(ParamIdLuminGamma, t, m_lumin_gamma, m_params_validity);
        m_pblock->GetValue(ParamIdSatMultiplier, t, m_sat_multiplier, m_params_validity);
        m_pblock->GetValue(ParamIdHorizonShift, t, m_horizon_shift, m_params_validity);
        m_pblock->GetValue(ParamIdGroundAlbedo, t, m_ground_albedo, m_params_validity);

        m_pblock->GetValue(ParamIdSkyType, t, m_sky_type, m_params_validity);
        
        NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
    }

    valid &= m_params_validity;
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
    appleseedenvmap_param_blk.SetUserDlgProc(new EnvMapParamMapDlgProc());

    return masterDlg;
}

int AppleseedEnvMap::NumSubTexmaps()
{
    return TexmapCount;
}

Texmap* AppleseedEnvMap::GetSubTexmap(int i)
{
    return (i == 0) ? m_turbidity_map : nullptr;
}

int AppleseedEnvMap::MapSlotType(int i)
{
    return MAPSLOT_TEXTURE;
}

void AppleseedEnvMap::SetSubTexmap(int i, Texmap* texmap)
{
    if (i > 0)
      return;
    const auto texmap_id = static_cast<TexmapId>(i);
    const auto param_id = g_texmap_id_to_param_id[texmap_id];
    m_pblock->SetValue(param_id, 0, texmap);
    appleseedenvmap_param_blk.InvalidateUI( param_id );

    ReplaceReference(i + 1, texmap);
}

TSTR AppleseedEnvMap::GetSubTexmapSlotName(int i)
{
    const auto texmap_id = static_cast<TexmapId>(i);
    return g_texmap_slot_names[texmap_id];
}

Class_ID AppleseedEnvMap::ClassID()
{
    return appleseedenvmap_CLASS_ID;
}

SClass_ID AppleseedEnvMap::SuperClassID()
{
    return TEXMAP_CLASS_ID;
}

void AppleseedEnvMap::GetClassName(TSTR& s)
{
    s = GetString(IDS_CLASS_NAME);
}

void AppleseedEnvMap::DeleteThis()
{
    delete this;
}

RefTargetHandle AppleseedEnvMap::Clone(RemapDir &remap)
{
    AppleseedEnvMap *mnew = new AppleseedEnvMap();
    *((MtlBase*)mnew) = *((MtlBase*)this); // copy superclass stuff
    //TODO: Add other cloning stuff
    BaseClone(this, mnew, remap);
    return (RefTargetHandle)mnew;
}

RefResult AppleseedEnvMap::NotifyRefChanged(const Interval& /*changeInt*/, RefTargetHandle hTarget, PartID& /*partID*/, RefMessage message, BOOL /*propagate*/)
{
    switch (message)
    {
        case REFMSG_TARGET_DELETED:
            if (hTarget == m_pblock) 
            { 
                m_pblock = nullptr; 
            }
            else
            {
                if (m_turbidity_map == hTarget)
                {
                    m_turbidity_map = nullptr;
                    break;
                }
            }
            break;
        case REFMSG_CHANGE:
            m_params_validity.SetEmpty();
            if (hTarget == m_pblock)
                appleseedenvmap_param_blk.InvalidateUI(m_pblock->LastNotifyParamID());
            break;
    }
    return(REF_SUCCEED);
}

//From ReferenceMaker
int AppleseedEnvMap::NumRefs()
{
    return 2; //pblock + subtexture
}

RefTargetHandle AppleseedEnvMap::GetReference(int i)
{
    switch (i) {
    case ParamBlockIdEnvMap: return m_pblock;
    case 1: return m_turbidity_map;
    default: return nullptr;
    }
}

void AppleseedEnvMap::SetReference(int i, RefTargetHandle rtarg)
{
    switch (i) {
    case ParamBlockIdEnvMap: m_pblock = (IParamBlock2 *)rtarg; break;
    case 1: m_turbidity_map = (Texmap *)rtarg; break;
    }
}

int AppleseedEnvMap::NumSubs()
{
    return 2; //pblock + subtexture
}

Animatable* AppleseedEnvMap::SubAnim(int i)
{
    switch (i) {
    case 0: return m_pblock;
    case 1: return m_turbidity_map;
    default: return nullptr;
    }
}

TSTR AppleseedEnvMap::SubAnimName(int i)
{
    switch (i) {
    case 0: return GetString(IDS_ENVMAP_PARAMS);
    case 1: return GetSubTexmapTVName(i - 1);
    default: return nullptr;
    }
}

int AppleseedEnvMap::SubNumToRefNum(int subNum)
{
    return subNum;
}

int AppleseedEnvMap::NumParamBlocks()
{
    return 1;
}

IParamBlock2* AppleseedEnvMap::GetParamBlock(int i)
{
    return i == 0 ? m_pblock : nullptr;
}

IParamBlock2* AppleseedEnvMap::GetParamBlockByID(BlockID id)
{
    return id == m_pblock->ID() ? m_pblock : nullptr;
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
    //preview color for scanline renderer
    return AColor(0.13f, 0.58f, 1.0f, 1.0f);
}

Point3 AppleseedEnvMap::EvalNormalPerturb(ShadeContext& /*sc*/)
{
    return Point3(0, 0, 0);
}

//
// AppleseedEnvMapBrowserEntryInfo class implementation.
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
    HBITMAP hBitmap = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_PNG1));

    BITMAP bmp; 
    PBITMAPINFO pbmi; 
    WORD    cClrBits; 

    // Retrieve the bitmap color format, width, and height.  
    if (!GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp)) 
        return nullptr;

    // Convert the color format to a count of bits.  
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
    if (cClrBits == 1) 
        cClrBits = 1; 
    else if (cClrBits <= 4) 
        cClrBits = 4; 
    else if (cClrBits <= 8) 
        cClrBits = 8; 
    else if (cClrBits <= 16) 
        cClrBits = 16; 
    else if (cClrBits <= 24) 
        cClrBits = 24; 
    else cClrBits = 32; 

    // Allocate memory for the BITMAPINFO structure. (This structure  
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD  
    // data structures.)  

     if (cClrBits < 24) 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER) + 
                    sizeof(RGBQUAD) * (1<< cClrBits)); 

     // There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel 

     else 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER)); 

    // Initialize the fields in the BITMAPINFO structure.  

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
    if (cClrBits < 24) 
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

    // If the bitmap is not compressed, set the BI_RGB flag.  
    pbmi->bmiHeader.biCompression = BI_RGB; 

    // Compute the number of bytes in the array of color  
    // indices and store the result in biSizeImage.  
    // The width must be DWORD aligned unless the bitmap is RLE 
    // compressed. 
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
                                  * pbmi->bmiHeader.biHeight; 
    // Set biClrImportant to 0, indicating that all of the  
    // device colors are important.  
     pbmi->bmiHeader.biClrImportant = 0; 

    BitmapInfo  biTmp;
    biTmp.SetName(_T(""));
	biTmp.SetType(BMM_TRUE_24);
    biTmp.SetWidth(100);
	biTmp.SetHeight(100);

    Bitmap* bmp1 = TheManager->Create(&biTmp);
    bmp1->FromDib(pbmi);
    DeleteObject(hBitmap);
    LocalFree(pbmi);
    return bmp1;
}

bool AppleseedEnvMapBrowserEntryInfo::HasEntryThumbnail() const
{
    return true;
}

FPInterface* AppleseedEnvMapClassDesc::GetInterface(Interface_ID id)
{
    if (id == IMATERIAL_BROWSER_ENTRY_INFO_INTERFACE)
        return &m_browser_entry_info;

    return ClassDesc2::GetInterface(id);
}

INT_PTR EnvMapParamMapDlgProc::DlgProc(
        TimeValue   t,
        IParamMap2* map,
        HWND        hwnd,
        UINT        umsg,
        WPARAM      wparam,
        LPARAM      lparam)
{
    switch (umsg)
    {
    case WM_INITDIALOG:
        enable_disable_controls(hwnd, map);
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wparam))
        {
        case IDC_COMBO_SKY_TYPE:
            switch (HIWORD(wparam))
            {
            case CBN_SELCHANGE:
                enable_disable_controls(hwnd, map);
                return TRUE;

            default:
                return FALSE;
            }

        default:
            return FALSE;
        }

    default:
        return FALSE;
    }
}

void EnvMapParamMapDlgProc::enable_disable_controls(HWND hwnd, IParamMap2* map)
{
    auto selected = SendMessage(GetDlgItem(hwnd, IDC_COMBO_SKY_TYPE), CB_GETCURSEL, 0, 0);
	map->Enable(ParamIdGroundAlbedo, selected == 0 ? TRUE : FALSE);
}
