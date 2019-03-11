#include <snp/num/prg_clenshaw_1_rollout.snp>
#include <snp/ldpk/prg_model_independent_1.snp>

/***************************************************************
* model-dependent parameters                                   *
***************************************************************/
uniform float cx02_degree_2;
uniform float cy02_degree_2;
uniform float cx22_degree_2;
uniform float cy22_degree_2;
uniform float cx04_degree_4;
uniform float cy04_degree_4;
uniform float cx24_degree_4;
uniform float cy24_degree_4;
uniform float cx44_degree_4;
uniform float cy44_degree_4;
uniform float lens_rotation;
uniform float squeeze_x;
uniform float squeeze_y;

float cx_for_x2 = cx02_degree_2 + cx22_degree_2;
float cx_for_y2 = cx02_degree_2 - cx22_degree_2;

float cx_for_x4 = cx04_degree_4 + cx24_degree_4 + cx44_degree_4;
float cx_for_x2_y2 = 2.0 * cx04_degree_4 - 6.0 * cx44_degree_4;
float cx_for_y4 = cx04_degree_4 - cx24_degree_4 + cx44_degree_4;

float cy_for_x2 = cy02_degree_2 + cy22_degree_2;
float cy_for_y2 = cy02_degree_2 - cy22_degree_2;

float cy_for_x4 = cy04_degree_4 + cy24_degree_4 + cy44_degree_4;
float cy_for_x2_y2 = 2.0 * cy04_degree_4 - 6.0 * cy44_degree_4;
float cy_for_y4 = cy04_degree_4 - cy24_degree_4 + cy44_degree_4;

#include <snp/basics/prg_texture2d_bicubic_catmull_rom.snp>

/***************************************************************
* model-dependent functions                                    *
***************************************************************/
mat2 rotate_mat(float phi)
	{ return mat2(cos(phi),sin(phi),-sin(phi),cos(phi)); }
vec2 rotate(float phi,vec2 p)
	{ return rotate_mat(phi) * p; }
mat2 squeeze_x_mat(float sq)
	{ return mat2(sq,0.0,0.0,1.0); }
mat2 squeeze_y_mat(float sq)
	{ return mat2(1.0,0.0,0.0,sq); }

mat2 rot_sqx_sqy_pa = rotate_mat(radians(lens_rotation)) * squeeze_x_mat(squeeze_x) * squeeze_y_mat(squeeze_y) * squeeze_x_mat(tde4_pixel_aspect);
mat2 pa_rot = squeeze_x_mat(tde4_pixel_aspect) * rotate_mat(radians(lens_rotation));
mat2 pa_rot_inv = invert(pa_rot);

vec2 eval_rot_sqx_sqy_pa(vec2 p)
	{ return rot_sqx_sqy_pa * p; }
vec2 eval_pa_rot_inv(vec2 p)
	{ return pa_rot_inv * p; }

vec2 anamorphic(vec2 p_diag)
	{
// apply undistortion
	float x = p_diag[0];
	float y = p_diag[1];
	float x2 = x * x;
	float y2 = y * y;
	float x4 = x2 * x2;
	float y4 = y2 * y2;

	vec2 q_diag;
	q_diag[0] = x * (1.0
			+ x2 * cx_for_x2 + y2 * cx_for_y2
			+ x4 * cx_for_x4 + x2 * y2 * cx_for_x2_y2 + y4 * cx_for_y4);
	q_diag[1] = y * (1.0
			+ x2 * cy_for_x2 + y2 * cy_for_y2
			+ x4 * cy_for_x4 + x2 * y2 * cy_for_x2_y2 + y4 * cy_for_y4);
	return q_diag;
	}
vec2 undistort_diag(vec2 p_diag)
	{
	return eval_rot_sqx_sqy_pa(anamorphic(eval_pa_rot_inv(p_diag)));
	}
mat2 jacobian(vec2 p_diag)
	{
	vec2 p = eval_pa_rot_inv(p_diag);
	float x = p[0];
	float x2 = x * x;
	float x3 = x2 * x;
	float x4 = x2 * x2;
	float y = p[1];
	float y2 = y * y;
	float y3 = y2 * y;
	float y4 = y2 * y2;

	mat2 m;
// Note that in glsl [i][j] is the component at column i and row j,
// whereas in the LDPK we use the notation [i][j] like in mathematics
// and physics, so that matrix multiplication is (a b)[i][k] = \sum_j a[i][j] b[j][k]
	m[0][0] = 1.0 + x2 * 3.0 * cx_for_x2 + y2 * cx_for_y2
		+ x4 * 5.0 * cx_for_x4 + x2 * y2 * 3.0 * cx_for_x2_y2 + y4 * cx_for_y4;
	m[1][1] = 1.0 + y2 * 3.0 * cy_for_y2 + x2 * cy_for_x2
		+ y4 * 5.0 * cy_for_y4 + x2 * y2 * 3.0 * cy_for_x2_y2 + x4 * cy_for_x4;
	m[1][0] = x * y * 2.0 * cx_for_y2
		+ x * y3 * 4.0 * cx_for_y4 + x3 * y * 2.0 * cx_for_x2_y2;
	m[0][1] = x * y * 2.0 * cy_for_x2
		+ x3 * y * 4.0 * cy_for_x4 + x * y3 * 2.0 * cy_for_x2_y2;
	return rot_sqx_sqy_pa * m * pa_rot_inv;
	}

#include <snp/ldpk/prg_model_independent_2.snp>
