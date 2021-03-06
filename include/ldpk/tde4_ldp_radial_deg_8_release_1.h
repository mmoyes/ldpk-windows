// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014,2015,2016,2017,2018 - Science-D-Visions. Current version: 2.0


#ifndef tde4_ldp_radial_deg_8_release_1_sdv
#define tde4_ldp_radial_deg_8_release_1_sdv

#include <ldpk/ldpk_ldp_builtin.h>
#include <ldpk/ldpk_generic_radial_distortion.h>

//! @file tde4_ldp_radial_deg_8_release_1.h
//! @brief Plugin class for radial distortion

//! @brief Plugin class for radial distortion.
//! Does not compensate for decentering.
//! Parameters can be calculated by 3DE's Matrix Tool.
template <class VEC2,class MAT2>
class tde4_ldp_radial_deg_8_release_1:public ldpk::ldp_builtin<VEC2,MAT2>
	{
private:
	typedef VEC2 vec2_type;
	typedef MAT2 mat2_type;
	typedef ldpk::ldp_builtin<VEC2,MAT2> base_type;

	ldpk::generic_radial_distortion<vec2_type,mat2_type,4> _distortion;

	static const char* _para[4];

	bool decypher(const char* name,int& i)
		{
		typedef base_type bt;
		int n;
		getNumParameters(n);
		for(i = 0;i < n;++i)
			{
			if(0 == strcmp(name,_para[i]))
				{
				return true;
				}
			}
		return false;
		}
	bool initializeParameters()
		{
		typedef base_type bt;
		bt::check_builtin_parameters();
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
			{
			return true;
			}
		if(!decypher(identifier,i))
			{
			return false;
			}
		if(_distortion.get_coeff(i) != v)
			{
			bt::no_longer_uptodate_lut();
			}
		_distortion.set_coeff(i,v);
		return true;
		}

// Overwriting tde4_ldp_common
	bool undistort(double x0,double y0,double &x1,double &y1)
		{
		typedef base_type bt;
		vec2_type p_plain_dn = bt::map_unit_to_dn(vec2_type(x0,y0));

// Clipping to a reasonable area, still n times as large as the image.
		vec2_type q_dn = _distortion.eval(p_plain_dn);
		if(norm2(q_dn) > 50.0) q_dn = 50.0 * unit(q_dn);
		
		vec2_type q = bt::map_dn_to_unit(q_dn);
		x1 = q[0];
		y1 = q[1];
		return true;
		}
	bool distort(double x0,double y0,double &x1,double &y1)
		{
		typedef base_type bt;
// The distort-method without initial values is not constant by semantics,
// since it may cause an update of the lookup-tables. Implementing a Nuke node
// it turned out that we need to prevent threads from trying so simultaneously.
// By the following double check of is_uptodate_lut() we keep the mutex lock
// out of our frequently called distort stuff (for performance reasons) and
// prevent threads from updating without need.
		if(!bt::is_uptodate_lut())
			{
			bt::lock();
			if(!bt::is_uptodate_lut())
				{
				bt::update_lut();
				}
			bt::unlock();
			}

// Get initial value from lookup-table
		vec2_type qs = bt::get_lut().get_initial_value(vec2_type(x0,y0));
// Call version of distort with initial value.
		vec2_type p_plain_dn = _distortion.map_inverse(bt::map_unit_to_dn(vec2_type(x0,y0)),bt::map_unit_to_dn(qs));

		vec2_type q = bt::map_dn_to_unit(p_plain_dn);
		x1 = q[0];
		y1 = q[1];
		return true;
		}
	bool distort(double x0,double y0,double x1_start,double y1_start,double &x1,double &y1)
		{
		typedef base_type bt;
		vec2_type p_plain_dn = _distortion.map_inverse(bt::map_unit_to_dn(vec2_type(x0,y0)),bt::map_unit_to_dn(vec2_type(x1_start,y1_start)));

		vec2_type q = bt::map_dn_to_unit(p_plain_dn);
		x1 = q[0];
		y1 = q[1];
		return true;
		}
public:
	tde4_ldp_radial_deg_8_release_1()
		{ }
	bool getModelName(char *name)
		{
#ifdef LDPK_COMPILE_AS_PLUGIN_SDV
		strcpy(name,"3DE4 Radial - Fisheye, Degree 8, Release 1 [Plugin]");
#else
		strcpy(name,"3DE4 Radial - Fisheye, Degree 8, Release 1");
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
	};

template <class VEC2,class MAT2>
const char* tde4_ldp_radial_deg_8_release_1<VEC2,MAT2>::_para[4] = {
	"Distortion - Degree 2",
	"Quartic Distortion - Degree 4",
	"Degree 6",
	"Degree 8"
	};

#endif
