#!/bin/csh

python expand_files.py prg_1_tex_tde4_anamorphic_deg_4_rotate_squeeze_xy.fsh expand.xpnd		> ../2019.1/LD_3DE4_Anamorphic_Standard_Degree_4.glsl
python expand_files.py prg_1_tex_tde4_anamorphic_deg_4_rotate_squeeze_xy_rescaled.fsh expand.xpnd	> ../2019.1/LD_3DE4_Anamorphic_Rescaled_Degree_4.glsl
python expand_files.py prg_1_tex_tde4_anamorphic_deg_6.fsh expand.xpnd					> ../2019.1/LD_3DE4_Anamorphic_Degree_6.glsl
python expand_files.py prg_1_tex_tde4_radial_standard_degree_4.fsh expand.xpnd				> ../2019.1/LD_3DE4_Radial_Standard_Degree_4.glsl
python expand_files.py prg_1_tex_tde4_classic_3de_mixed.fsh expand.xpnd					> ../2019.1/LD_3DE_Classic_LD_Model.glsl
python expand_files.py prg_1_tex_tde4_radial_fisheye_degree_8.fsh expand.xpnd				> ../2019.1/LD_3DE4_Radial_Fisheye_Degree_8.glsl

#shader_builder -p -m ld_3de4_anamorphic_standard_deg_4.glsl

chmod 775 ../2019.1/*.glsl
