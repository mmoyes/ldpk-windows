// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014,2015,2016,2017,2018 - Science-D-Visions. Current version: 2.0


#pragma once

#include <ldpk/ldpk_ldp_builtin.h>
#include <ldpk/ldpk_generic_radial_distortion_1d.h>

//! @file tde4_ldp_radial_fisheye_equidistant_longlat_z_deg_8.h
//! @brief Plugin class for radial distortion

//! @brief Plugin class for radial distortion.
//! Does not compensate for decentering.
template <class VEC2,class MAT2>
class tde4_ldp_radial_fisheye_equidistant_longlat_z_deg_8:public ldpk::ldp_builtin<VEC2,MAT2>
	{
private:
	typedef VEC2 vec2;
	typedef MAT2 mat2;
	typedef ldpk::ldp_builtin<VEC2,MAT2> base_type;

	ldpk::generic_radial_distortion_1d<4> _distortion;

	static const char* _para[4];
	double _r_clip_factor;
	double _fl_dn;

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
// Overwriting tde4_ldp_common
	bool undistort(double x0,double y0,double &x1,double &y1)
		{
		typedef base_type bt;

		vec2 p_ed_dist_dn = bt::map_unit_to_dn(vec2(x0,y0));
// Distance from origin in dn-coordinates
		double r_ed_dist_dn = norm2(p_ed_dist_dn);

// The literature commonly says, undistorting should be done
// before applying the remapping. Usually it's applied to
// elongation angle theta = r/f. That's what we do here.
// The result is the angle without distortion.
		double the_undist = _distortion(r_ed_dist_dn / _fl_dn);
// Coordinates in longlat image.
		double phi_new = ::atan2(p_ed_dist_dn[1],p_ed_dist_dn[0]);
		double the_new = the_undist;
		if((the_new < -M_PI) || (the_new > M_PI))
			{ return false; }
// spherical coordinates to unit vector, pole at z.
		double x = ::sin(-the_new) * ::cos(phi_new);
		double y = ::sin(-the_new) * ::sin(phi_new);
		double z = ::cos(the_new);
		double r = ::sqrt(x * x + z * z);
// unit vector to spherical coordinates, pole at y
		double phi = ::atan2(x,-z);
		double the = ::atan2(r,y);
		if(phi < 0.0)
			{ phi += 2.0 * M_PI; }
// spherical coordinates to unit coordinates
		x1 = phi / (2.0 * M_PI);
		y1 = the / M_PI;
		return true;
		}
	bool distort(double x0,double y0,double &x1,double &y1)
		{
		typedef base_type bt;
// unit coordinates to spherical coordinates
		double phi = x0 * (2.0 * M_PI);
		double the = y0 * M_PI;
// spherical coordinates to unit vector, pole at y
		double z = -::sin(the) * ::cos(phi);
		double y = ::cos(the);
		double x = +::sin(the) * ::sin(phi);
// unit vector to other spherical coordinates, pole at z.
		double r = ::sqrt(x * x + y * y);
		double the_new = -::atan2(r,z);
		double phi_new = ::atan2(y,x);
// Spherical coordinates to equidistant position in dn-coordinates and distort
		double r_ed_dist_dn = _fl_dn * _distortion.map_inverse(the_new);
		vec2 p_ed_dn = r_ed_dist_dn * vec2(::cos(phi_new),::sin(phi_new));
// dn- to unit-coordinates
		vec2 q_ed_unit = bt::map_dn_to_unit(p_ed_dn);
		x1 = q_ed_unit[0];
		y1 = q_ed_unit[1];
		return true;
		}

public:
// Mutex initialized and destroyed in baseclass.
	tde4_ldp_radial_fisheye_equidistant_longlat_z_deg_8():_r_clip_factor(50.0)
		{ }
	~tde4_ldp_radial_fisheye_equidistant_longlat_z_deg_8()
		{ }
	double r_clip_factor() const
		{ return _r_clip_factor; }
	void r_clip_factor(double f)
		{ _r_clip_factor = f; }
	bool getModelName(char *name)
		{
#ifdef LDPK_COMPILE_AS_PLUGIN_SDV
		strcpy(name,"3DE4 Radial - Fisheye, Equidistant, Longlat-Z, Degree 8 [Plugin]");
#else
		strcpy(name,"3DE4 Radial - Fisheye, Equidistant, Longlat-Z, Degree 8");
#endif
		return true;
		}
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
/*	bool getJacobianMatrix(double x0,double y0,double& m00,double& m01,double& m10,double& m11)
		{
		typedef base_type bt;
		m00 = m[0][0];m01 = m[0][1];m10 = m[1][0];m11 = m[1][1];
		return true;
		}*/
	};

template <class VEC2,class MAT2>
const char* tde4_ldp_radial_fisheye_equidistant_longlat_z_deg_8<VEC2,MAT2>::_para[4] = {
	"Degree 2",
	"Degree 4",
	"Degree 6",
	"Degree 8"
	};
