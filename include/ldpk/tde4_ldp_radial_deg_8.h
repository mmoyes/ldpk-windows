// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014,2015,2016,2017,2018 - Science-D-Visions. Current version: 2.0


#pragma once

#include <ldpk/ldpk_ldp_builtin.h>
#include <ldpk/ldpk_generic_radial_distortion.h>

//! @file tde4_ldp_radial_deg_8.h
//! @brief Plugin class for radial distortion

//! @brief Plugin class for radial distortion.
//! Does not compensate for decentering.
//! Parameters can be calculated by 3DE's Matrix Tool.
template <class VEC2,class MAT2>
class tde4_ldp_radial_deg_8:public ldpk::ldp_builtin<VEC2,MAT2>
	{
private:
	typedef VEC2 vec2_type;
	typedef MAT2 mat2_type;
	typedef ldpk::ldp_builtin<VEC2,MAT2> base_type;

	ldpk::generic_radial_distortion<vec2_type,mat2_type,4> _distortion;

	static const char* _para[4];
	double _r_clip_factor;

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
//! @brief esa stands for "equisolid-angle".
//! The method will fill p_plain_dn with some more or less
//! reasonable value even if it returns false (bad arg for tan()).
	bool remap_esa2plain(const vec2_type& p_esa_dn,vec2_type& p_plain_dn)
		{
		bool rc = true;
		double f_dn = this->fl_cm() / this->r_fb_cm();
// Remove fisheye projection
		double r_esa_dn = norm2(p_esa_dn);
		if(r_esa_dn == 0.0)
			{
			p_plain_dn = p_esa_dn;
			return rc;
			}
		double arg = r_esa_dn / (2.0 * f_dn),arg_clip = arg;
// Black areas, undefined.
		if(arg_clip >= 1.0)
			{
			arg_clip = 0.9999;
			}
		double phi = 2.0 * asin(arg_clip);
		if(phi >= M_PI / 2.0)
			{
// This can and will happen in re-distorting footage...
			phi = M_PI / 2.0 - 1e-5;
			rc = false;
			}
		double r_plain_dn = f_dn * tan(phi);
		if(r_plain_dn > _r_clip_factor)
			{
// ... as well as this.
			r_plain_dn = _r_clip_factor;
			}
		p_plain_dn = r_plain_dn * unit(p_esa_dn);
		return rc;
		}
	bool remap_plain2esa(const vec2_type& p_plain_dn,vec2_type& p_esa_dn)
		{
		typedef base_type bt;
		double f_dn = bt::fl_cm() / bt::r_fb_cm();
// Apply fisheye projection
		double r_plain_dn = norm2(p_plain_dn);
		if(r_plain_dn == 0.0)
			{
			p_esa_dn = p_plain_dn;
			return true;
			}
		double phi = atan2(r_plain_dn,f_dn);
		double r_esa_dn = 2.0 * f_dn * std::sin(phi / 2.0);
		p_esa_dn = r_esa_dn * unit(p_plain_dn);
		return true;
		}
// Overwriting tde4_ldp_common
	bool undistort(double x0,double y0,double &x1,double &y1)
		{
		typedef base_type bt;
		vec2_type p_esa_dn = bt::map_unit_to_dn(vec2_type(x0,y0));
		vec2_type p_plain_dn;
		if(!remap_esa2plain(p_esa_dn,p_plain_dn)) return false;

// Clipping to a reasonable area, still n times as large as the image.
		vec2_type q_dn = _distortion.eval(p_plain_dn);
		if(norm2(q_dn) > 100.0)
			{ q_dn = 100.0 * unit(q_dn); }
		
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
// Get initial value from lookup-table. Note that we have defined our LUT
// as a table of distortion vales for the model function (i.e. without projection).
// We get the initial value and feed it into the distortion class (no projection),
// so everything should be fine.
		vec2_type qs = bt::get_lut().get_initial_value(vec2_type(x0,y0));
// Call version of distort with initial value.
		vec2_type p_plain_dn = _distortion.map_inverse(bt::map_unit_to_dn(vec2_type(x0,y0)),bt::map_unit_to_dn(qs));
		vec2_type p_esa_dn;

		if(!remap_plain2esa(p_plain_dn,p_esa_dn)) return false;

		vec2_type q = bt::map_dn_to_unit(p_esa_dn);
		x1 = q[0];
		y1 = q[1];
		return true;
		}
	bool distort(double x0,double y0,double x1_start,double y1_start,double &x1,double &y1)
		{
		typedef base_type bt;
		vec2_type p_plain_dn = _distortion.map_inverse(bt::map_unit_to_dn(vec2_type(x0,y0)),bt::map_unit_to_dn(vec2_type(x1_start,y1_start)));
		vec2_type p_esa_dn;
		if(!remap_plain2esa(p_plain_dn,p_esa_dn)) return false;

		vec2_type q = bt::map_dn_to_unit(p_esa_dn);
		x1 = q[0];
		y1 = q[1];
		return true;
		}
	bool undistort_gnomonic(double x0,double y0,double &x1,double &y1)
		{
		typedef base_type bt;
		vec2_type q = bt::map_dn_to_unit(
			_distortion.eval(
				bt::map_unit_to_dn(vec2_type(x0,y0))));
		x1 = q[0];
		y1 = q[1];
		return true;
		}
	bool distort_gnomonic(double x0,double y0,double &x1,double &y1)
		{
		typedef base_type bt;
// see distort().
		if(!bt::is_uptodate_lut())
			{
			bt::lock();
			if(!bt::is_uptodate_lut())
				{
				bt::update_lut();
				}
			bt::unlock();
			}
// Get initial value from lookup-table. See distort().
		vec2_type qs = bt::get_lut().get_initial_value(vec2_type(x0,y0));
// Call version of distort with initial value.
		vec2_type p_plain_dn = _distortion.map_inverse(bt::map_unit_to_dn(vec2_type(x0,y0)),bt::map_unit_to_dn(qs));
		x1 = p_plain_dn[0];
		y1 = p_plain_dn[1];
		return true;
		}
	bool distort_gnomonic(double x0,double y0,double x1_start,double y1_start,double &x1,double &y1)
		{
		typedef base_type bt;
		vec2_type p_plain_dn = _distortion.map_inverse(bt::map_unit_to_dn(vec2_type(x0,y0)),bt::map_unit_to_dn(vec2_type(x1_start,y1_start)));
		vec2_type q = bt::map_dn_to_unit(p_plain_dn);
		x1 = q[0];
		y1 = q[1];
		return true;
		}
public:
// Mutex initialized and destroyed in baseclass.
	tde4_ldp_radial_deg_8():_r_clip_factor(5.0)
		{ }
	~tde4_ldp_radial_deg_8()
		{ }
	double r_clip_factor() const
		{ return _r_clip_factor; }
	void r_clip_factor(double f)
		{ _r_clip_factor = f; }
	bool getModelName(char *name)
		{
#ifdef LDPK_COMPILE_AS_PLUGIN_SDV
		strcpy(name,"3DE4 Radial - Fisheye, Degree 8 [Plugin]");
#else
		strcpy(name,"3DE4 Radial - Fisheye, Degree 8");
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
	bool getJacobianMatrix(double x0,double y0,double& m00,double& m01,double& m10,double& m11)
		{
		typedef base_type bt;
// The function we need to derive is a concatenation of a simple radially symmetric
// undistort funtion and the equisolid-angle transform. We compute this by Leipnitz's rule.
// Exterior derivative:
		vec2_type p_esa_dn = bt::map_unit_to_dn(vec2_type(x0,y0));
		vec2_type p_plain_dn;
		if(!remap_esa2plain(p_esa_dn,p_plain_dn))
			{
			m00 = 1;m01 = 0;m10 = 0;m11 = 1;
			return false;
			}
		mat2_type m = _distortion.jacobi(p_plain_dn);
// Interior derivative
		mat2_type e;
		double f_dn = this->fl_cm() / this->r_fb_cm();
		double r_esa_dn = norm2(p_esa_dn);
		double r_esa_div_2f = r_esa_dn / (2.0 * f_dn);
// Test (a): make sure argument of arcsine is valid.
		if(fabs(r_esa_div_2f) > 0.99)
			{
			m00 = 1;m01 = 0;m10 = 0;m11 = 1;
			return false;
			}
		double asn = asin(r_esa_div_2f);
// Test (b): make sure cos(..) is greater than 0.
		if(fabs(2.0 * asn) > 0.99 * M_PI / 2.0)
			{
			m00 = 1;m01 = 0;m10 = 0;m11 = 1;
			return false;
			}
		double csn = cos(2.0 * asn);
// Argument of square root is positive because of test (a).
		double sqr = sqrt(1.0 - r_esa_div_2f * r_esa_div_2f);
		if(r_esa_dn > 1e-12)
			{
// The analytic expression is undefined at 0...
			e = (mat2_type(1) - tensq(p_esa_dn / r_esa_dn)) * (f_dn / r_esa_dn) * tan(2.0 * asn)
			  + (tensq(p_esa_dn / r_esa_dn)) / (csn * csn * sqr);
			}
		else
			{
// ...but has well-defined limit
			e = mat2_type(1);
			}

// Maps between unit and diagnorm coordinates.
		mat2_type u2d((bt::w_fb_cm() / 2) / bt::r_fb_cm(),0.0,0.0,(bt::h_fb_cm() / 2) / bt::r_fb_cm());
		mat2_type d2u(bt::r_fb_cm() / (bt::w_fb_cm() / 2),0.0,0.0,bt::r_fb_cm() / (bt::h_fb_cm() / 2));
		m = d2u * m * e * u2d;
// We have to make sure, that the matrix doesn't run berserk in the outer regions
		if(tr(trans(m) * m) > 100.0)
			{
// Chill.
			m = mat2_type(0);
			}
		m00 = m[0][0];m01 = m[0][1];m10 = m[1][0];m11 = m[1][1];
		return true;
		}
	};

template <class VEC2,class MAT2>
const char* tde4_ldp_radial_deg_8<VEC2,MAT2>::_para[4] = {
	"Distortion - Degree 2",
	"Quartic Distortion - Degree 4",
	"Degree 6",
	"Degree 8"
	};
