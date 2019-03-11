#!/bin/csh

unset echo

set basedir=`dirname $PWD/$0`
set CC="g++"
set OPT="-O2 -fPIC"
set SHARED="-dynamiclib"

# Insert your python header directory here or let me find the latest 2.7
if -e /usr/include/python2.7 then
	setenv INCPYTHON "-I /usr/include/python2.7"
else if -e /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/python2.7 then
	setenv INCPYTHON "-I /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/python2.7"
else
	unset echo
	echo "No python headers found. Is python installed?"
	exit
endif


cd $basedir/../source/ldpk
set echo
set INCPATH="-I ../../include"
set TARGET_DIR="../../python/osx"

# Compile the python module. Import this e.g. with:
# >>> import lens_distortion_plugins as ldp
$CC $OPT $SHARED tde4_lens_distortion_plugins.module.C $INCPYTHON $INCPATH -lpython -o $TARGET_DIR/lens_distortion_plugins.so

#cp -v ../../python/osx/lens_distortion_plugins.so ../../compiled/ldpk/osx/python/
