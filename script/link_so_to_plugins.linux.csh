#!/bin/csh -v
# Auf belsazar ausfuehren.

unset verbose

foreach VERSION ($nuke_full_versions)
	rm -rf		/server/software/linux64/nuke/Nuke${VERSION}/plugins/LD_3DE*.so
	echo "rm -rf	/server/software/linux64/nuke/Nuke${VERSION}/plugins/LD_3DE*.so"
end

foreach VERSION ($nuke_full_versions)
	set vers = `echo $VERSION | awk '{print substr($0,1,length($0)-2)}'`
	set full = $VERSION
	ln -s 		$LDPK/compiled/nuke/linux/Nuke${vers}/*.so /server/software/linux64/nuke/Nuke${full}/plugins
	echo "ln -s	$LDPK/compiled/nuke/linux/Nuke${vers}/*.so /server/software/linux64/nuke/Nuke${full}/plugins"
	set vers = `echo $VERSION | sed s/'v[0-9]*'/''/g`
	echo "ln -s	$LDPK/compiled/nuke/linux/Nuke${vers}/*.so /server/software/linux64/nuke/Nuke${full}/plugins"
end

echo "make tgz-archive for early adopters"
foreach VERSION ($nuke_versions)
	pushd $LDPK/compiled/nuke/linux/Nuke${VERSION}/ > /dev/null
	rm		ldpk_nuke_${VERSION}.linux.tgz
	tar cfz		ldpk_nuke_${VERSION}.linux.tgz *.so --exclude-from LD_3DE4_All_Parameter_Types.so
	echo $PWD": tar cfz	ldpk_nuke_${VERSION}.linux.tgz *.so --exclude-from LD_3DE4_All_Parameter_Types.so"
	chmod 666	ldpk_nuke_${VERSION}.linux.tgz
	popd > /dev/null
end
ll $LDPK/compiled/nuke/linux/Nuke*/*tgz

