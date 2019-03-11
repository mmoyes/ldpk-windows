#!/bin/csh -x
# internal/sdv: execute on box01.

set basedir="/home/user/dev/ldpk-2.0"
set CC="g++"
set SHARED="-shared"
set OPT="-O2 -fPIC"
set INCPATH="-I $basedir/include"

set INCDLFCN="-I /home/user/dev/dlfcn-win32"
set LIBDLFCN="-L /home/user/dev/dlfcn-win32"

# New in version 1.1: compile helper classes into a library.
# Weta's Nuke-plugins, for instance, use the plugin loader.
cd $basedir/source/ldpk
$CC $OPT $INCPATH ldpk_model_parser.C -c
$CC $OPT $INCPATH $INCDLFCN ldpk_plugin_loader.C -c
$CC $OPT $INCPATH ldpk_table_generator.C -c
rm -f $basedir/lib/libldpk.lib
ar cruv $basedir/lib/libldpk.lib ldpk_model_parser.o ldpk_plugin_loader.o ldpk_table_generator.o

cd $basedir/test
$CC $OPT test_lookup_table_iter.C	$INCPATH $INCDLFCN -o ../bin/test_lookup_table_iter.exe
$CC $OPT test_radial_decentered_distortion.C $INCPATH $INCDLFCN -o ../bin/test_radial_decentered_distortion.exe
$CC $OPT test_generic_radial_distortion.C	$INCPATH $INCDLFCN -o ../bin/test_generic_radial_distortion.exe
$CC $OPT test_model_visualizer.C $INCPATH $INCDLFCN ../lib/libldpk.lib -lpsapi $LIBDLFCN -ldl -o ../bin/test_model_visualizer.exe -lpsapi

$CC $OPT test_plugin_loader.C $INCPATH $INCDLFCN ../lib/libldpk.lib $LIBDLFCN -ldl -o ../bin/test_plugin_loader.exe -lpsapi

cd $basedir/source/ldpk
set INCPATH="-I ../../include"
set TARGET_DIR="../../compiled/ldpk/windows/lib"

# Model "3DE4 Radial - Fisheye, Degree 8"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_radial_deg_8.C -o $TARGET_DIR/tde4_ldp_radial_deg_8.dll
# Model "3DE Classic LD Model"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_classic_3de_mixed.C -o $TARGET_DIR/tde4_ldp_classic_3de_mixed.dll
# Model "3DE4 Anamorphic - Standard, Degree 4"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy.C -o $TARGET_DIR/tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy.dll
# Model "3DE4 Anamorphic - Rescaled, Degree 4"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy_rescaled.C -o $TARGET_DIR/tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy_rescaled.dll
# Model "3DE4 Anamorphic - Degree 6"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_anamorphic_deg_6.C -o $TARGET_DIR/tde4_ldp_anamorphic_deg_6.dll
# Model "3DE4 Radial - Standard, Degree 4"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_radial_decentered_deg_4_cylindric.C -o $TARGET_DIR/tde4_ldp_radial_decentered_deg_4_cylindric.dll

# Model "3DE4 Radial - Homomorphic, Degree 2"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_radial_homomorphic_decentered_deg_2.experimental.C -o $TARGET_DIR/tde4_ldp_radial_homomorphic_decentered_deg_2.dll

# Model "3DE4 Radial - Fisheye, Equidistant, Degree 8"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_radial_fisheye_equidistant_deg_8.experimental.C -o $TARGET_DIR/tde4_ldp_radial_fisheye_equidistant_deg_8.dll
# Model "3DE4 Radial - Fisheye, Equisolid, Degree 8"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_radial_fisheye_equisolid_deg_8.experimental.C -o $TARGET_DIR/tde4_ldp_radial_fisheye_equisolid_deg_8.dll
# Model "3DE4 Radial - Fisheye, Orthographic, Degree 8"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_radial_fisheye_orthographic_deg_8.experimental.C -o $TARGET_DIR/tde4_ldp_radial_fisheye_orthographic_deg_8.dll
# Model "3DE4 Radial - Fisheye, Stereographic, Degree 8"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_radial_fisheye_stereographic_deg_8.experimental.C -o $TARGET_DIR/tde4_ldp_radial_fisheye_stereographic_deg_8.dll

# Model "3DE4 All Parameter Types"
$CC $OPT $SHARED -DLDPK_COMPILE_AS_PLUGIN_SDV $INCPATH tde4_ldp_all_par_types.C -o $TARGET_DIR/tde4_ldp_all_par_types.dll
