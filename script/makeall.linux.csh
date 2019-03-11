#!/bin/csh -x
# internal/sdv: execute on box01.

set basedir=`dirname $PWD/$0`
set CC="g++"
set SHARED="-shared"
set OPT="-O2 -fPIC"

# New in version 1.1: compile helper classes into a library.
# Weta's Nuke-plugins, for instance, use the plugin loader.
#cd $basedir/../source/ldpk
#$CC $OPT $INCPATH ldpk_model_parser.C -c
#$CC $OPT $INCPATH ldpk_plugin_loader.C -c
#$CC $OPT $INCPATH ldpk_table_generator.C -c
#rm -f ../../lib/libldpk.a
#ar cruv ../../lib/libldpk.a ldpk_model_parser.o ldpk_plugin_loader.o ldpk_table_generator.o

#cd $basedir/../test
#$CC $OPT test_lookup_table_iter.C		$INCPATH -o ../bin/test_lookup_table_iter
#$CC $OPT test_radial_decentered_distortion.C	$INCPATH -o ../bin/test_radial_decentered_distortion
#$CC $OPT test_generic_radial_distortion.C	$INCPATH -o ../bin/test_generic_radial_distortion
#$CC $OPT test_model_visualizer.C		$INCPATH ../lib/libldpk.a -ldl -o ../bin/test_model_visualizer
#$CC $OPT test_plugin_loader.C			$INCPATH ../lib/libldpk.a -ldl -o ../bin/test_plugin_loader

cd $basedir/../source/ldpk
set INCPATH="-I ../../include"
set TARGET_DIR="../../compiled/ldpk/linux/lib"

# Model "3DE4 Radial - Fisheye, Degree 8"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_radial_deg_8.C -o $TARGET_DIR/tde4_ldp_radial_deg_8.so
# Model "3DE Classic LD Model"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_classic_3de_mixed.C -o $TARGET_DIR/tde4_ldp_classic_3de_mixed.so
# Model "3DE4 Anamorphic - Standard, Degree 4"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy.C -o $TARGET_DIR/tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy.so
# Model "3DE4 Anamorphic - Rescaled, Degree 4"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy_rescaled.C -o $TARGET_DIR/tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy_rescaled.so
# Model "3DE4 Anamorphic - Degree 6"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_anamorphic_deg_6.C -o $TARGET_DIR/tde4_ldp_anamorphic_deg_6.so
# Model "3DE4 Radial - Standard, Degree 4"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_radial_decentered_deg_4_cylindric.C -o $TARGET_DIR/tde4_ldp_radial_decentered_deg_4_cylindric.so

# Model "3DE4 Radial - Homomorphic, Degree 2"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_radial_homomorphic_decentered_deg_2.experimental.C -o $TARGET_DIR/tde4_ldp_radial_homomorphic_decentered_deg_2.so

# Model "3DE4 Radial - Fisheye, Equidistant, Degree 8"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_radial_fisheye_equidistant_deg_8.experimental.C -o $TARGET_DIR/tde4_ldp_radial_fisheye_equidistant_deg_8.so
# Model "3DE4 Radial - Fisheye, Equisolid, Degree 8"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_radial_fisheye_equisolid_deg_8.experimental.C -o $TARGET_DIR/tde4_ldp_radial_fisheye_equisolid_deg_8.so
# Model "3DE4 Radial - Fisheye, Orthographic, Degree 8"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_radial_fisheye_orthographic_deg_8.experimental.C -o $TARGET_DIR/tde4_ldp_radial_fisheye_orthographic_deg_8.so
# Model "3DE4 Radial - Fisheye, Stereographic, Degree 8"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_radial_fisheye_stereographic_deg_8.experimental.C -o $TARGET_DIR/tde4_ldp_radial_fisheye_stereographic_deg_8.so

# Model "3DE4 All Parameter Types"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_all_par_types.C -o $TARGET_DIR/tde4_ldp_all_par_types.so

