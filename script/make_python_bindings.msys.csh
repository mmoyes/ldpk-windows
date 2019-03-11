#!/bin/csh

set basedir="/home/user/dev/ldpk-2.0"
set CC="g++"
set SHARED="-shared"
set OPT="-O2 -fPIC"


# setenv INCPYTHON "-I /usr/include/python2.7"
# setenv INCPYTHON "-I /mingw64/include/python3.7m"
setenv INCPYTHON "-I C:\Python27\include"


cd $basedir/source/ldpk
set echo
set INCPATH="-I $basedir/include"
set TARGET_DIR="$basedir/compiled/ldpk/windows/python"

# Compile the python module. Import this e.g. with:
# >>> import lens_distortion_plugins as ldp
$CC $OPT $SHARED -D_hypot=hypot -DMS_WIN64 tde4_lens_distortion_plugins.module.C $INCPYTHON $INCPATH -o $TARGET_DIR/lens_distortion_plugins.pyd -LC:/Python27/libs -lpython27

#cp -v ../../python/linux/lens_distortion_plugins.so ../../compiled/ldpk/linux/python/
