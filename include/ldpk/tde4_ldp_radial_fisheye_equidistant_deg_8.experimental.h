// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014,2015,2016,2017,2018 - Science-D-Visions. Current version: 2.0


#pragma once

#include <ldpk/tde4_ldp_radial_fisheye_base_deg_8.experimental.h>

//! @file tde4_ldp_radial_fisheye_equidistant_deg_8.h
//! @brief Plugin class for radial distortion

//! @brief Plugin class for radial distortion.
//! Does not compensate for decentering.
template <class VEC2,class MAT2>
class tde4_ldp_radial_fisheye_equidistant_deg_8:public tde4_ldp_radial_fisheye_base_deg_8<VEC2,MAT2>
	{
private:
	typedef VEC2 vec2_type;
	typedef MAT2 mat2_type;
	typedef tde4_ldp_radial_fisheye_base_deg_8<VEC2,MAT2> base_type;
	using base_type::_fl_dn;
	using base_type::_r_clip_factor;

	bool remap_fe2plain(double r_ed_dn,double& r_plain_dn)
		{
		typedef base_type bt;
		bool rc = true;
// Angular elongation in equidistant lenses: theta = r / f
		double theta = r_ed_dn / _fl_dn;
// We can only visualize ]-90deg,+90deg[ in plain images.
		if(theta >= M_PI / 2.0)
			{ return false; }
// Now remap to distance in plain filmback
		r_plain_dn = _fl_dn * tan(theta);
// Optionally clip distance.
		if(r_plain_dn > _r_clip_factor)
			{ r_plain_dn = _r_clip_factor; }
		return true;
		}
	bool remap_plain2fe(double r_plain_dn,double& r_ed_dn)
		{
		typedef base_type bt;
// Angular elongation in gnomonic lenses: theta = arctan(r / f)
		double theta = atan2(r_plain_dn,_fl_dn);
// Now remap to distance in equidistant lens
		r_ed_dn = _fl_dn * theta;
		return true;
		}
	bool getModelName(char *name)
		{
#ifdef LDPK_COMPILE_AS_PLUGIN_SDV
		strcpy(name,"3DE4 Radial - Fisheye, Equidistant, Degree 8 [Plugin]");
#else
		strcpy(name,"3DE4 Radial - Fisheye, Equidistant, Degree 8");
#endif
		return true;
		}
	};
