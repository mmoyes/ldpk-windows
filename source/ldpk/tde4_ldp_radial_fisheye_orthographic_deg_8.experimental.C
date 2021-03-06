// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014,2015,2016,2017,2018 - Science-D-Visions. Current version: 2.0


#include <ldpk/tde4_ldp_radial_fisheye_orthographic_deg_8.experimental.h>

#ifdef LDPK_COMPILE_AS_PLUGIN_SDV
extern "C" tde4_ld_plugin* tde4ldp_create_fct()
	{
	return new tde4_ldp_radial_fisheye_orthographic_deg_8<ldpk::vec2d,ldpk::mat2d>;
	}

extern "C" void tde4ldp_destroy_fct(tde4_ld_plugin* p)
	{
	delete p;
	}
#endif
