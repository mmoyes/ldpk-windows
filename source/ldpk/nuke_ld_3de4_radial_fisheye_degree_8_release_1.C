// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014,2015,2016,2017,2018 - Science-D-Visions. Current version: 2.0


#include <ldpk/nuke_ld_3de4_base.h>
#include <ldpk/tde4_ldp_radial_deg_8_release_1.h>

#define CLASS_LDP_SDV         tde4_ldp_radial_deg_8_release_1
#define CLASS_IOP_SDV     iop_tde4_ldp_radial_deg_8_release_1
#define CREATE_SDV create_iop_tde4_ldp_radial_deg_8_release_1

//! @file nuke_ld_3de4_radial_fisheye_degree_8_release_1.C

// This is the name that NUKE will use to store this operator in the
// scripts. So that NUKE can locate the plugin, this must also be the
// name of the compiled plugin (with .so/.dll/.dylib added to the end):
static const char* const CLASS = "LD_3DE4_Radial_Fisheye_Degree_8_Release_1";

// This text will be displayed in a popup help box on the node's panel:
static const char* const HELP = "3DE4 radial lens distortion model with polynomial fisheye projection.\n";

//! Nuke plugin based on tde4_ldp_radial_deg_8
class iop_tde4_ldp_radial_deg_8_release_1:public nuke_ld_3de4_base
	{
public:
	CLASS_IOP_SDV(Node* node):nuke_ld_3de4_base(node,new CLASS_LDP_SDV<ldpk::vec2d,ldpk::mat2d>())
		{ }
	const char* Class() const
		{ return CLASS; }
	const char* node_help() const
		{ return HELP; }
	};

// So wird das woanders auch gemacht. Bauen wir einfach nach.
static DD::Image::Iop* CREATE_SDV(Node* node)
	{
	return (new DD::Image::NukeWrapper (new CLASS_IOP_SDV(node)))->noMix()->noMask();
	}
const DD::Image::Iop::Description nuke_ld_3de4_base::description(CLASS,0,CREATE_SDV);

// g++ -g -O2 -shared -fPIC ld_3de4_radial_fisheye_degree_8.C nuke_ld_3de4_base.C -I $server/software/linux64/nuke/Nuke7.0v9/include -I ../../include -L $server/software/linux64/nuke/Nuke7.0v9/lib -o ../../compiled/nuke/Nuke7.0/LD_3DE4_Radial_Fisheye_Degree_8.so
