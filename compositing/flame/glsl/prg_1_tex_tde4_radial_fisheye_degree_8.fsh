#include <snp/num/prg_clenshaw_1_rollout.snp>
#include <snp/ldpk/prg_model_independent_1.snp>

/***************************************************************
* model-dependent parameters                                   *
***************************************************************/
uniform float c2;
uniform float c4;
uniform float c6;
uniform float c8;

#include <snp/basics/prg_texture2d_bicubic_catmull_rom.snp>

/***************************************************************
* model-dependent functions                                    *
***************************************************************/
vec2 radial(vec2 p_diag)
	{
// apply undistortion
	float x = p_diag[0];
	float y = p_diag[1];
	float x2 = x * x;
	float y2 = y * y;
	float r2 = x2 + y2;
	float r4 = r2 * r2;
	float r6 = r2 * r4;
	float r8 = r4 * r4;

	float r_corr = 1.0 + c2 * r2 + c4 * r4 + c6 * r6 + c8 * r8;
	vec2 q_diag;
	q_diag[0] = x * r_corr;
	q_diag[1] = y * r_corr;
	return q_diag;
	}
vec2 undistort_diag(vec2 p_diag)
	{
	return radial(p_diag);
	}
// At this point we need the jacobian without equisolid-angle projection,
// just the plain radial model function.
mat2 jacobian(vec2 p_diag)
	{
	mat2 m;
	float x = p_diag[0];
	float y = p_diag[1];
	float x2 = x * x;
	float y2 = y * y;
	float r2 = x2 + y2;
	float r4 = r2 * r2;
	float r6 = r2 * r4;
	float r8 = r4 * r4;

	float r_corr = 1.0 + c2 * r2 + c4 * r4 + c6 * r6 + c8 * r8;
	float dr_corr = 2.0 * c2 + 4.0 * c4 * r2 + 6.0 * c6 * r4 + 8.0 * c8 * r6;
	m = mat2(r_corr,0.0,0.0,r_corr) + tensq(p_diag) * dr_corr;
	return m;
	}

#include <snp/ldpk/prg_model_independent_2_equisolid_angle.snp>
