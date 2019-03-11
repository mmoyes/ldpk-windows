#!/bin/csh -x

set echo

set basedir=`dirname $PWD/$0`

################################################################
# See documentation for appropriate version of gcc             #
################################################################
set CC="g++"

################################################################
# Think about this: is it correct for your version of Nuke?    #
# How about compiling according to C++11 standard? -std=c++11  #
################################################################
setenv OPT "-O2 -fPIC"

setenv SHARED "-shared"
setenv INC_LDPK "-I $basedir/../include"
setenv LIB_NUKE "-lDDImage"

################################################################
# Switch to ldpk source directory                              #
################################################################
cd $basedir/../source/ldpk

unset echo

################################################################
# Please edit these according to your needs:                   #
################################################################
setenv NUKE_VERSION "11.2"
setenv INC_NUKE "-I $server/software/linux64/nuke/Nuke11.2v2/include"
setenv LIBDIR_NUKE "-L $server/software/linux64/nuke/Nuke11.2v2"

################################################################
# your results here please:                                    #
################################################################
setenv TARGET_DIR "/tmp/lib/nuke/linux/Nuke$NUKE_VERSION"
mkdir -p $TARGET_DIR
set echo

$CC $OPT $SHARED nuke_ld_3de4_anamorphic_degree_6.C			nuke_ld_3de4_base.C $INC_LDPK $INC_NUKE $LIBDIR_NUKE $LIB_NUKE -o $TARGET_DIR/LD_3DE4_Anamorphic_Degree_6.so
$CC $OPT $SHARED nuke_ld_3de4_anamorphic_standard_degree_4.C		nuke_ld_3de4_base.C $INC_LDPK $INC_NUKE $LIBDIR_NUKE $LIB_NUKE -o $TARGET_DIR/LD_3DE4_Anamorphic_Standard_Degree_4.so
$CC $OPT $SHARED nuke_ld_3de4_anamorphic_rescaled_degree_4.C		nuke_ld_3de4_base.C $INC_LDPK $INC_NUKE $LIBDIR_NUKE $LIB_NUKE -o $TARGET_DIR/LD_3DE4_Anamorphic_Rescaled_Degree_4.so
$CC $OPT $SHARED nuke_ld_3de4_radial_fisheye_degree_8.C			nuke_ld_3de4_base.C $INC_LDPK $INC_NUKE $LIBDIR_NUKE $LIB_NUKE -o $TARGET_DIR/LD_3DE4_Radial_Fisheye_Degree_8.so
$CC $OPT $SHARED nuke_ld_3de4_radial_standard_degree_4.C		nuke_ld_3de4_base.C $INC_LDPK $INC_NUKE $LIBDIR_NUKE $LIB_NUKE -o $TARGET_DIR/LD_3DE4_Radial_Standard_Degree_4.so
$CC $OPT $SHARED nuke_ld_3de_classic_ld_model.C				nuke_ld_3de4_base.C $INC_LDPK $INC_NUKE $LIBDIR_NUKE $LIB_NUKE -o $TARGET_DIR/LD_3DE_Classic_LD_Model.so
$CC $OPT $SHARED nuke_ld_3de4_all_par_types.C				nuke_ld_3de4_base.C $INC_LDPK $INC_NUKE $LIBDIR_NUKE $LIB_NUKE -o $TARGET_DIR/LD_3DE4_All_Parameter_Types.so

unset echo
echo "**********************************************************"
echo "Plugins should now be in $TARGET_DIR."
echo "Please copy them to your Nuke plugins directory."
echo "All plugin names start with 'LD_3DE...'."
echo "**********************************************************"

