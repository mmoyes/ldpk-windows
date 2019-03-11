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

uniform float cx06_degree_6;
uniform float cy06_degree_6;
uniform float cx26_degree_6;
uniform float cy26_degree_6;
uniform float cx46_degree_6;
uniform float cy46_degree_6;
uniform float cx66_degree_6;
uniform float cy66_degree_6;

float cx_for_x2 = cx02_degree_2 + cx22_degree_2;
float cx_for_y2 = cx02_degree_2 - cx22_degree_2;

float cx_for_x4 = cx04_degree_4 + cx24_degree_4 + cx44_degree_4;
float cx_for_x2_y2 = 2.0 * cx04_degree_4 - 6.0 * cx44_degree_4;
float cx_for_y4 = cx04_degree_4 - cx24_degree_4 + cx44_degree_4;

float cx_for_x6 = cx06_degree_6 + cx26_degree_6 + cx46_degree_6 + cx66_degree_6;
float cx_for_x4_y2 = 3.0 * cx06_degree_6 + cx26_degree_6 - 5.0 * cx46_degree_6 - 15.0 * cx66_degree_6;
float cx_for_x2_y4 = 3.0 * cx06_degree_6 - cx26_degree_6 - 5.0 * cx46_degree_6 + 15.0 * cx66_degree_6;
float cx_for_y6 = cx06_degree_6 - cx26_degree_6 + cx46_degree_6 - cx66_degree_6;

float cy_for_x2 = cy02_degree_2 + cy22_degree_2;
float cy_for_y2 = cy02_degree_2 - cy22_degree_2;

float cy_for_x4 = cy04_degree_4 + cy24_degree_4 + cy44_degree_4;
float cy_for_x2_y2 = 2.0 * cy04_degree_4 - 6.0 * cy44_degree_4;
float cy_for_y4 = cy04_degree_4 - cy24_degree_4 + cy44_degree_4;

float cy_for_x6 = cy06_degree_6 + cy26_degree_6 + cy46_degree_6 + cy66_degree_6;
float cy_for_x4_y2 = 3.0 * cy06_degree_6 + cy26_degree_6 - 5.0 * cy46_degree_6 - 15.0 * cy66_degree_6;
float cy_for_x2_y4 = 3.0 * cy06_degree_6 - cy26_degree_6 - 5.0 * cy46_degree_6 + 15.0 * cy66_degree_6;
float cy_for_y6 = cy06_degree_6 - cy26_degree_6 + cy46_degree_6 - cy66_degree_6;

#include <snp/basics/prg_texture2d_bicubic_catmull_rom.snp>

/***************************************************************
* model-dependent functions                                    *
***************************************************************/
vec2 anamorphic(vec2 p_diag)
	{
// apply undistortion
	float x = p_diag[0];
	float y = p_diag[1];
	float x2 = x * x;
	float y2 = y * y;
	float x4 = x2 * x2;
	float y4 = y2 * y2;
	float x6 = x4 * x2;
	float y6 = y4 * y2;

	vec2 q_diag;
	q_diag[0] = x * (1.0
			+ x2 * cx_for_x2 + y2 * cx_for_y2
			+ x4 * cx_for_x4 + x2 * y2 * cx_for_x2_y2 + y4 * cx_for_y4
			+ x6 * cx_for_x6 + x4 * y2 * cx_for_x4_y2 + x2 * y4 * cx_for_x2_y4 + y6 * cx_for_y6);
	q_diag[1] = y * (1.0
			+ x2 * cy_for_x2 + y2 * cy_for_y2
			+ x4 * cy_for_x4 + x2 * y2 * cy_for_x2_y2 + y4 * cy_for_y4
			+ x6 * cy_for_x6 + x4 * y2 * cy_for_x4_y2 + x2 * y4 * cy_for_x2_y4 + y6 * cy_for_y6);
	return q_diag;
	}
vec2 undistort_diag(vec2 p_diag)
	{
	return anamorphic(p_diag);
	}
mat2 jacobian(vec2 p_diag)
	{
	mat2 m;
	float x = p_diag[0];
	float x2 = x * x;
	float x3 = x2 * x;
	float x4 = x2 * x2;
	float x5 = x4 * x;
	float x6 = x4 * x2;
	float y = p_diag[1];
	float y2 = y * y;
	float y3 = y2 * y;
	float y4 = y2 * y2;
	float y5 = y4 * y;
	float y6 = y4 * y2;
// Note that in glsl [i][j] is the component at column i and row j,
// whereas in the LDPK we use the notation [i][j] like in mathematics
// and physics, so that matrix multiplication is (a b)[i][k] = \sum_j a[i][j] b[j][k]
	m[0][0] = 1.0 + x2 * 3.0 * cx_for_x2 + y2 * cx_for_y2
		+ x4 * 5.0 * cx_for_x4 + x2 * y2 * 3.0 * cx_for_x2_y2 + y4 * cx_for_y4
		+ x6 * 7.0 * cx_for_x6 + x4 * y2 * 5.0 * cx_for_x4_y2 + x2 * y4 * 3.0 * cx_for_x2_y4 + y6 * cx_for_y6;
	m[1][1] = 1.0 + y2 * 3.0 * cy_for_y2 + x2 * cy_for_x2
		+ y4 * 5.0 * cy_for_y4 + x2 * y2 * 3.0 * cy_for_x2_y2 + x4 * cy_for_x4
		+ x6 * cy_for_x6 + x4 * y2 * 3.0 * cy_for_x4_y2 + x2 * y4 * 5.0 * cy_for_x2_y4 + y6 * 7.0 * cy_for_y6;
	m[1][0] = x * y * 2.0 * cx_for_y2
		+ x * y3 * 4.0 * cx_for_y4 + x3 * y * 2.0 * cx_for_x2_y2
		+ x5 * y * 2.0 * cx_for_x4_y2 + x3 * y3 * 4.0 * cx_for_x2_y4 + x * y5 * 6.0 * cx_for_y6;
	m[0][1] = x * y * 2.0 * cy_for_x2
		+ x3 * y * 4.0 * cy_for_x4 + x * y3 * 2.0 * cy_for_x2_y2
		+ x5 * y * 6.0 * cy_for_x6 + x3 * y3 * 4.0 * cy_for_x4_y2 + x * y5 * 2.0 * cy_for_x2_y4;
	return m;
	}

#include <snp/ldpk/prg_model_independent_2.snp>
