#!/bin/csh

set basedir=`dirname $PWD/$0`
set CC="g++"
set SHARED="-shared"
set OPT="-O2 -fPIC"

# Insert your host and python header directory here:
if($HOST == "gilgamesh" || $HOST == "lillith") then
	setenv INCPYTHON "-I/usr/include/python2.7"
else
	setenv INCPYTHON "-I /server/devel/extern/linux64/python_sdv_dynamic/include/python2.7"
endif

cd $basedir/../source/ldpk
set echo
set INCPATH="-I ../../include"
set TARGET_DIR="../../python"

# Compile the python module. Import this e.g. with:
# >>> import lens_distortion_plugins as ldp
$CC $OPT $SHARED tde4_lens_distortion_plugins.module.C $INCPYTHON $INCPATH -o $TARGET_DIR/linux/lens_distortion_plugins.so

#cp -v ../../python/linux/lens_distortion_plugins.so ../../compiled/ldpk/linux/python/
