// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014,2015,2016,2017,2018 - Science-D-Visions. Current version: 2.0


#pragma once

#include <ldpk/ldpk_ldp_builtin.h>
#include <ldpk/ldpk_generic_radial_distortion_1d.h>
#include <ldpk/ldpk_vec2d.h>

//! @file tde4_ldp_radial_fisheye_base_deg_8.h
//! @brief Plugin class for radial distortion

//! @brief Plugin class for radial distortion.
//! Does not compensate for decentering.

template <class VEC2,class MAT2>
class tde4_ldp_radial_fisheye_base_deg_8:public ldpk::ldp_builtin<VEC2,MAT2>
	{
private:
	typedef VEC2 vec2_type;
	typedef MAT2 mat2_type;
	typedef ldpk::ldp_builtin<VEC2,MAT2> base_type;

	ldpk::generic_radial_distortion_1d<4> _distortion;

	static const char* _para[4];
protected:
	double _r_clip_factor;
	double _fl_dn;
private:
	bool decypher(const char* name,int& i)
		{
		typedef base_type bt;
		int n;
		getNumParameters(n);
		for(i = 0;i < n;++i)
			{
			if(0 == strcmp(name,_para[i]))
				{ return true; }
			}
		return false;
		}
	bool initializeParameters()
		{
		typedef base_type bt;
		bt::check_builtin_parameters();
// Focal length in dn-coordinates.
		_fl_dn = bt::fl_cm() / bt::r_fb_cm();
		return true;
		}
	bool getNumParameters(int& n)
		{
		n = 4;
		return true;
		}
	bool getParameterName(int i,char* identifier)
		{
		strcpy(identifier,_para[i]);
		return true;
		}
	bool setParameterValue(const char *identifier,double v)
		{
		typedef base_type bt;
		int i;
// Does the base class know the parameter?
		if(bt::set_builtin_parameter_value(identifier,v))
			{ return true; }
		if(!decypher(identifier,i))
			{ return false; }
		if(_distortion.get_coeff(i) != v)
			{ bt::no_longer_uptodate_lut(); }
		_distortion.set_coeff(i,v);
		return true;
		}
	virtual bool remap_fe2plain(double r_ed_dn,double& r_plain_dn) = 0;
	virtual bool remap_plain2fe(double r_plain_dn,double& r_ed_dn) = 0;
// Overwriting tde4_ldp_common
	bool undistort(double x0,double y0,double &x1,double &y1)
		{
		typedef base_type bt;

		vec2_type p_ed_dist_dn = bt::map_unit_to_dn(vec2_type(x0,y0));

// Distance from origin in dn-coordinates
		double r_ed_dist_dn = norm2(p_ed_dist_dn);
// The literature commonly says, undistorting should be done
// before applying the remapping. Usually it's applied to
// elongation angle theta = r/f. That's what we do here.
// theta = r/f is the equidistant projection, so here we have g o F^-1.
		double r_ed_undist_dn = _fl_dn * _distortion(r_ed_dist_dn / _fl_dn);
// Remap equidistant to gnomonic
		double r_plain_undist_dn;
		if(!remap_fe2plain(r_ed_undist_dn,r_plain_undist_dn))
			{ return false; }
// Calc undistorted point from undistorted radius
		vec2_type p_plain_undist_dn;
		if(dotsq(p_ed_dist_dn) == 0.0)
			{ p_plain_undist_dn = vec2_type(0,0); }
		else
			{ p_plain_undist_dn = r_plain_undist_dn * unit(p_ed_dist_dn); }
// Map back to unit coordinates		
		vec2_type p_plain_undist_unit = bt::map_dn_to_unit(p_plain_undist_dn);
		x1 = p_plain_undist_unit[0];
		y1 = p_plain_undist_unit[1];
		return true;
		}
	bool distort(double x0,double y0,double &x1,double &y1)
		{
		typedef base_type bt;

		vec2_type p_plain_undis_dn = bt::map_unit_to_dn(vec2_type(x0,y0));
// Distance from origin in dn-coordinates
		double r_ed_undis_dn,r_plain_undis_dn = norm2(p_plain_undis_dn);
// Remap gnomonic to equidistant
		if(!remap_plain2fe(r_plain_undis_dn,r_ed_undis_dn))
			{ return false; }
// Apply distortion. We use the point itself as initial value, since coefficients will be small.
		double r_ed_dis_dn = _fl_dn * _distortion.map_inverse(r_ed_undis_dn / _fl_dn,r_ed_undis_dn / _fl_dn);
// Calc distorted point from distorted radius
		vec2_type p_ed_dis_dn;
		if(dotsq(p_plain_undis_dn) == 0.0)
			{ p_ed_dis_dn = vec2_type(0,0); }
		else
			{ p_ed_dis_dn = r_ed_dis_dn * unit(p_plain_undis_dn); }
// Map back to unit coordinates		
		vec2_type p_ed_dis_unit = bt::map_dn_to_unit(p_ed_dis_dn);
		x1 = p_ed_dis_unit[0];
		y1 = p_ed_dis_unit[1];
		return true;
		}
public:
// Mutex initialized and destroyed in baseclass.
	tde4_ldp_radial_fisheye_base_deg_8():_r_clip_factor(50.0)
		{ }
	~tde4_ldp_radial_fisheye_base_deg_8()
		{ }
	double r_clip_factor() const
		{ return _r_clip_factor; }
	void r_clip_factor(double f)
		{ _r_clip_factor = f; }
	virtual bool getModelName(char *name) = 0;
	bool getParameterType(const char* identifier,tde4_ldp_ptype& ptype)
		{
		typedef base_type bt;
		int i;
		if(bt::get_builtin_parameter_type(identifier,ptype)) return true;
		if(!decypher(identifier,i)) return false;
		ptype = TDE4_LDP_ADJUSTABLE_DOUBLE;
		return true;
		}
	bool getParameterDefaultValue(const char* identifier,double& v)
		{
		typedef base_type bt;
		int i;
		if(!decypher(identifier,i)) return false;
		v = 0.0;
		return true;
		}
	bool getParameterRange(const char* identifier,double& a,double& b)
		{
		typedef base_type bt;
		int i;
		if(!decypher(identifier,i)) return false;
		a = -0.5;
		b = 0.5;
		return true;
		}
	};

template <class VEC2,class MAT2>
const char* tde4_ldp_radial_fisheye_base_deg_8<VEC2,MAT2>::_para[4] = {
	"Degree 2",
	"Degree 4",
	"Degree 6",
	"Degree 8"
	};
