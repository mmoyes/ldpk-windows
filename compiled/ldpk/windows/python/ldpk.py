import lens_distortion_plugins as ldp
import math

# Convenience functions including get-functionality
# for the seven built-in parameters.
class builtin:
	def __init__(self):
		self._fl_cm = 2.0
		self._w_fb_cm = 1.6
		self._h_fb_cm = 0.9
		self._x_lco_cm = 0.0
		self._y_lco_cm = 0.0
		self._r_pa = 1.0
		self._fd_cm = 100.0
# Name of distortion model
	def name(self):
		return self.getModelName()
# Invoked by derived classes
	def init_builtin_parameters(self):
		self.setParameterValueDouble("tde4_focal_length_cm",self._fl_cm)
		self.setParameterValueDouble("tde4_filmback_width_cm",self._w_fb_cm)
		self.setParameterValueDouble("tde4_filmback_height_cm",self._h_fb_cm)
		self.setParameterValueDouble("tde4_lens_center_offset_x_cm",self._x_lco_cm )
		self.setParameterValueDouble("tde4_lens_center_offset_y_cm",self._y_lco_cm)
		self.setParameterValueDouble("tde4_pixel_aspect",self._r_pa)
		self.setParameterValueDouble("tde4_custom_focus_distance_cm",self._fd_cm)
# Getter in gui notation
	def getParameterValue(self,p):
		return self.parameter_value(p)
# Setters and Getters in gui notation, i.e. long names, jquery-style
	def builtin_parameter_value(self,p,v = None):
		if	p == "tde4_focal_length_cm":		return self.fl_cm(v)
		elif	p == "tde4_filmback_width_cm":		return self.w_fb_cm(v)
		elif	p == "tde4_filmback_height_cm":		return self.h_fb_cm(v)
		elif	p == "tde4_lens_center_offset_x_cm":	return self.x_lco_cm(v)
		elif	p == "tde4_lens_center_offset_y_cm":	return self.y_lco_cm(v)
		elif	p == "tde4_pixel_aspect":		return self.r_pa(v)
		elif	p == "tde4_custom_focus_distance_cm":	return self.fd_cm(v)
		raise KeyError(p)
# Setters and Getters in mathematical notation, jquery-style
	def fl_cm(self,q = None):
		if q != None:
			self._fl_cm = q
			self.setParameterValueDouble("tde4_focal_length_cm",q)
			return self
		else:
			return self._fl_cm
	def w_fb_cm(self,q = None):
		if q != None:
			self._w_fb_cm = q
			self.setParameterValueDouble("tde4_filmback_width_cm",q)
			return self
		else:
			return self._w_fb_cm
	def h_fb_cm(self,q = None):
		if q != None:
			self._h_fb_cm = q
			self.setParameterValueDouble("tde4_filmback_height_cm",q)
			return self
		else:
			return self._h_fb_cm
	def x_lco_cm(self,q = None):
		if q != None:
			self._x_lco_cm = q
			self.setParameterValueDouble("tde4_lens_center_offset_x_cm",q)
			return self
		else:
			return self._x_lco_cm
	def y_lco_cm(self,q = None):
		if q != None:
			self._y_lco_cm = q
			self.setParameterValueDouble("tde4_lens_center_offset_y_cm",q)
			return self
		else:
			return self._y_lco_cm
	def r_pa(self,q = None):
		if q != None:
			self._r_pa = q
			self.setParameterValueDouble("tde4_pixel_aspect",q)
			return self
		else:
			return self._r_pa
	def fd_cm(self,q = None):
		if q != None:
			self._fd_cm = q
			self.setParameterValueDouble("tde4_custom_focus_distance_cm",q)
			return self
		else:
			return self._fd_cm
	def camera(self,c = None):
		if c is not None:
			self.setCamera(*c)
			self._fl_cm = c[0]
			self._w_fb_cm = c[1]
			self._h_fb_cm = c[2]
			self._x_lco_cm = c[3]
			self._y_lco_cm = c[4]
			self._r_pa = c[5]
			self._fd_cm = c[6]
			return self
		else:
			return self.getCamera()
# Derived quanities: Lens center in cm with respect to lower left corner.
	def x_lc_cm(self):
		return self.w_fb_cm() * (1.0/2.0) + self.x_lco_cm()
	def y_lc_cm(self):
		return self.h_fb_cm() * (1.0/2.0) + self.y_lco_cm()
	def r_fb_cm(self):
		return math.sqrt(self.w_fb_cm() * self.w_fb_cm() + self.h_fb_cm() * self.h_fb_cm()) * (1.0/2.0)
# Iterating over model dependent parameter names
	def __iter__(self):
		return self.parameters.__iter__()
# Convenience method: Get value by parameter index (as opposed to parameter name)
	def get_value(self,i):
		return self.getParameterValue(self.getParameterName(i))
# Convenience method: Get default value by parameter index (as opposed to parameter name)
	def get_default_value(self,i):
		return self.getParameterDefaultValue(self.getParameterName(i))
# Convenience method: Get parameter range by parameter index (as opposed to parameter name)
	def get_range(self,i):
		return self.getParameterRange(self.getParameterName(i))
# Prepare for evaluation
	def prepare(self):
		"""Invoke this method after specifying camera and distortion parameters.
		This will call the tde4_ld_plugin-method initializeParameters()."""
		self.initializeParameters()

class anamorphic_standard_degree_4(builtin,ldp.anamorphic_deg_4_rotate_squeeze_xy):
	def __init__(self):
		builtin.__init__(self)
		ldp.anamorphic_deg_4_rotate_squeeze_xy.__init__(self)
		builtin.init_builtin_parameters(self)
		self.parameters = [self.getParameterName(i) for i in range(self.getNumParameters())]
		self._cx02 = self.getParameterDefaultValue("Cx02 - Degree 2")
		self._cy02 = self.getParameterDefaultValue("Cy02 - Degree 2")
		self._cx22 = self.getParameterDefaultValue("Cx22 - Degree 2")
		self._cy22 = self.getParameterDefaultValue("Cy22 - Degree 2")
		self._cx04 = self.getParameterDefaultValue("Cx04 - Degree 4")
		self._cy04 = self.getParameterDefaultValue("Cy04 - Degree 4")
		self._cx24 = self.getParameterDefaultValue("Cx24 - Degree 4")
		self._cy24 = self.getParameterDefaultValue("Cy24 - Degree 4")
		self._cx44 = self.getParameterDefaultValue("Cx44 - Degree 4")
		self._cy44 = self.getParameterDefaultValue("Cy44 - Degree 4")
		self._phi_mnt = self.getParameterDefaultValue("Lens Rotation")
		self._sx = self.getParameterDefaultValue("Squeeze-X")
		self._sy = self.getParameterDefaultValue("Squeeze-Y")
# Getter in gui notation
	def getParameterValue(self,p):
		if	p == "Cx02 - Degree 2":	return self._cx02
		elif	p == "Cy02 - Degree 2":	return self._cy02
		elif	p == "Cx22 - Degree 2":	return self._cx22
		elif	p == "Cy22 - Degree 2":	return self._cy22
		elif	p == "Cx04 - Degree 4":	return self._cx04
		elif	p == "Cy04 - Degree 4":	return self._cy04
		elif	p == "Cx24 - Degree 4":	return self._cx24
		elif	p == "Cy24 - Degree 4":	return self._cy24
		elif	p == "Cx44 - Degree 4":	return self._cx44
		elif	p == "Cy44 - Degree 4":	return self._cy44
		elif	p == "Lens Rotation":	return self._phi_mnt
		elif	p == "Squeeze-X":	return self._sx
		elif	p == "Squeeze-Y":	return self._sy
		return builtin.getParameterValue(self,p)
# Setters and Getters in mathematical notation.
	def cx02(self,q = None):
		if q != None:
			self._cx02 = q
			self.setParameterValueDouble("Cx02 - Degree 2",q)
			return self
		else:
			return self._cx02
	def cy02(self,q = None):
		if q != None:
			self._cy02 = q
			self.setParameterValueDouble("Cy02 - Degree 2",q)
			return self
		else:
			return self._cy02
	def cx22(self,q = None):
		if q != None:
			self._cx22 = q
			self.setParameterValueDouble("Cx22 - Degree 2",q)
			return self
		else:
			return self._cx22
	def cy22(self,q = None):
		if q != None:
			self._cy22 = q
			self.setParameterValueDouble("Cy22 - Degree 2",q)
			return self
		else:
			return self._cy22
	def cx04(self,q = None):
		if q != None:
			self._cx04 = q
			self.setParameterValueDouble("Cx04 - Degree 4",q)
			return self
		else:
			return self._cx04
	def cy04(self,q = None):
		if q != None:
			self._cy04 = q
			self.setParameterValueDouble("Cy04 - Degree 4",q)
			return self
		else:
			return self._cy04
	def cx24(self,q = None):
		if q != None:
			self._cx24 = q
			self.setParameterValueDouble("Cx24 - Degree 4",q)
			return self
		else:
			return self._cx24
	def cy24(self,q = None):
		if q != None:
			self._cy24 = q
			self.setParameterValueDouble("Cy24 - Degree 4",q)
			return self
		else:
			return self._cy24
	def cx44(self,q = None):
		if q != None:
			self._cx44 = q
			self.setParameterValueDouble("Cx44 - Degree 4",q)
			return self
		else:
			return self._cx44
	def cy44(self,q = None):
		if q != None:
			self._cy44 = q
			self.setParameterValueDouble("Cy44 - Degree 4",q)
			return self
		else:
			return self._cy44
	def phi_mnt(self,q = None):
		if q != None:
			self._phi_mnt = q
			self.setParameterValueDouble("Lens Rotation",q)
			return self
		else:
			return self._phi_mnt
	def sx(self,q = None):
		if q != None:
			self._sx = q
			self.setParameterValueDouble("Squeeze-X",q)
			return self
		else:
			return self._sx
	def sy(self,q = None):
		if q != None:
			self._sy = q
			self.setParameterValueDouble("Squeeze-Y",q)
			return self
		else:
			return self._sy

class anamorphic_rescaled_degree_4(builtin,ldp.anamorphic_deg_4_rotate_squeeze_xy_rescaled):
	def __init__(self):
		builtin.__init__(self)
		ldp.anamorphic_deg_4_rotate_squeeze_xy_rescaled.__init__(self)
		builtin.init_builtin_parameters(self)
		self.parameters = [self.getParameterName(i) for i in range(self.getNumParameters())]
		self._cx02 = self.getParameterDefaultValue("Cx02 - Degree 2")
		self._cy02 = self.getParameterDefaultValue("Cy02 - Degree 2")
		self._cx22 = self.getParameterDefaultValue("Cx22 - Degree 2")
		self._cy22 = self.getParameterDefaultValue("Cy22 - Degree 2")
		self._cx04 = self.getParameterDefaultValue("Cx04 - Degree 4")
		self._cy04 = self.getParameterDefaultValue("Cy04 - Degree 4")
		self._cx24 = self.getParameterDefaultValue("Cx24 - Degree 4")
		self._cy24 = self.getParameterDefaultValue("Cy24 - Degree 4")
		self._cx44 = self.getParameterDefaultValue("Cx44 - Degree 4")
		self._cy44 = self.getParameterDefaultValue("Cy44 - Degree 4")
		self._phi_mnt = self.getParameterDefaultValue("Lens Rotation")
		self._sx = self.getParameterDefaultValue("Squeeze-X")
		self._sy = self.getParameterDefaultValue("Squeeze-Y")
		self._s_rscl = self.getParameterDefaultValue("Rescale")
# Getter in gui notation
	def getParameterValue(self,p):
		if	p == "Cx02 - Degree 2":	return self._cx02
		elif	p == "Cy02 - Degree 2":	return self._cy02
		elif	p == "Cx22 - Degree 2":	return self._cx22
		elif	p == "Cy22 - Degree 2":	return self._cy22
		elif	p == "Cx04 - Degree 4":	return self._cx04
		elif	p == "Cy04 - Degree 4":	return self._cy04
		elif	p == "Cx24 - Degree 4":	return self._cx24
		elif	p == "Cy24 - Degree 4":	return self._cy24
		elif	p == "Cx44 - Degree 4":	return self._cx44
		elif	p == "Cy44 - Degree 4":	return self._cy44
		elif	p == "Lens Rotation":	return self._phi_mnt
		elif	p == "Squeeze-X":	return self._sx
		elif	p == "Squeeze-Y":	return self._sy
		elif	p == "Rescale":		return self._s_rscl
		return builtin.getParameterValue(self,p)
# Setters and Getters in mathematical notation.
	def cx02(self,q = None):
		if q != None:
			self._cx02 = q
			self.setParameterValueDouble("Cx02 - Degree 2",q)
			return self
		else:
			return self._cx02
	def cy02(self,q = None):
		if q != None:
			self._cy02 = q
			self.setParameterValueDouble("Cy02 - Degree 2",q)
			return self
		else:
			return self._cy02
	def cx22(self,q = None):
		if q != None:
			self._cx22 = q
			self.setParameterValueDouble("Cx22 - Degree 2",q)
			return self
		else:
			return self._cx22
	def cy22(self,q = None):
		if q != None:
			self._cy22 = q
			self.setParameterValueDouble("Cy22 - Degree 2",q)
			return self
		else:
			return self._cy22
	def cx04(self,q = None):
		if q != None:
			self._cx04 = q
			self.setParameterValueDouble("Cx04 - Degree 4",q)
			return self
		else:
			return self._cx04
	def cy04(self,q = None):
		if q != None:
			self._cy04 = q
			self.setParameterValueDouble("Cy04 - Degree 4",q)
			return self
		else:
			return self._cy04
	def cx24(self,q = None):
		if q != None:
			self._cx24 = q
			self.setParameterValueDouble("Cx24 - Degree 4",q)
			return self
		else:
			return self._cx24
	def cy24(self,q = None):
		if q != None:
			self._cy24 = q
			self.setParameterValueDouble("Cy24 - Degree 4",q)
			return self
		else:
			return self._cy24
	def cx44(self,q = None):
		if q != None:
			self._cx44 = q
			self.setParameterValueDouble("Cx44 - Degree 4",q)
			return self
		else:
			return self._cx44
	def cy44(self,q = None):
		if q != None:
			self._cy44 = q
			self.setParameterValueDouble("Cy44 - Degree 4",q)
			return self
		else:
			return self._cy44
	def phi_mnt(self,q = None):
		if q != None:
			self._phi_mnt = q
			self.setParameterValueDouble("Lens Rotation",q)
			return self
		else:
			return self._phi_mnt
	def sx(self,q = None):
		if q != None:
			self._sx = q
			self.setParameterValueDouble("Squeeze-X",q)
			return self
		else:
			return self._sx
	def sy(self,q = None):
		if q != None:
			self._sy = q
			self.setParameterValueDouble("Squeeze-Y",q)
			return self
		else:
			return self._sy
	def s_rscl(self,q = None):
		if q != None:
			self._s_rscl = q
			self.setParameterValueDouble("Rescale",q)
			return self
		else:
			return self._s_rscl

class anamorphic_degree_6(builtin,ldp.anamorphic_deg_6):
	def __init__(self):
		builtin.__init__(self)
		ldp.anamorphic_deg_6.__init__(self)
		builtin.init_builtin_parameters(self)
		self.parameters = [self.getParameterName(i) for i in range(self.getNumParameters())]
		self._cx02 = self.getParameterDefaultValue("Cx02 - Degree 2")
		self._cy02 = self.getParameterDefaultValue("Cy02 - Degree 2")
		self._cx22 = self.getParameterDefaultValue("Cx22 - Degree 2")
		self._cy22 = self.getParameterDefaultValue("Cy22 - Degree 2")
		self._cx04 = self.getParameterDefaultValue("Cx04 - Degree 4")
		self._cy04 = self.getParameterDefaultValue("Cy04 - Degree 4")
		self._cx24 = self.getParameterDefaultValue("Cx24 - Degree 4")
		self._cy24 = self.getParameterDefaultValue("Cy24 - Degree 4")
		self._cx44 = self.getParameterDefaultValue("Cx44 - Degree 4")
		self._cy44 = self.getParameterDefaultValue("Cy44 - Degree 4")
		self._cx06 = self.getParameterDefaultValue("Cx06 - Degree 6")
		self._cy06 = self.getParameterDefaultValue("Cy06 - Degree 6")
		self._cx26 = self.getParameterDefaultValue("Cx26 - Degree 6")
		self._cy26 = self.getParameterDefaultValue("Cy26 - Degree 6")
		self._cx46 = self.getParameterDefaultValue("Cx46 - Degree 6")
		self._cy46 = self.getParameterDefaultValue("Cy46 - Degree 6")
		self._cx66 = self.getParameterDefaultValue("Cx66 - Degree 6")
		self._cy66 = self.getParameterDefaultValue("Cy66 - Degree 6")
# Getter in gui notation
	def getParameterValue(self,p):
		if	p == "Cx02 - Degree 2":	return self._cx02
		elif	p == "Cy02 - Degree 2":	return self._cy02
		elif	p == "Cx22 - Degree 2":	return self._cx22
		elif	p == "Cy22 - Degree 2":	return self._cy22
		elif	p == "Cx04 - Degree 4":	return self._cx04
		elif	p == "Cy04 - Degree 4":	return self._cy04
		elif	p == "Cx24 - Degree 4":	return self._cx24
		elif	p == "Cy24 - Degree 4":	return self._cy24
		elif	p == "Cx44 - Degree 4":	return self._cx44
		elif	p == "Cy44 - Degree 4":	return self._cy44
		elif	p == "Cx06 - Degree 6":	return self._cx06
		elif	p == "Cy06 - Degree 6":	return self._cy06
		elif	p == "Cx26 - Degree 6":	return self._cx26
		elif	p == "Cy26 - Degree 6":	return self._cy26
		elif	p == "Cx46 - Degree 6":	return self._cx46
		elif	p == "Cy46 - Degree 6":	return self._cy46
		elif	p == "Cx66 - Degree 6":	return self._cx66
		elif	p == "Cy66 - Degree 6":	return self._cy66
		return builtin.getParameterValue(self,p)
# Setters and Getters in mathematical notation.
	def cx02(self,q = None):
		if q != None:
			self._cx02 = q
			self.setParameterValueDouble("Cx02 - Degree 2",q)
			return self
		else:
			return self._cx02
	def cy02(self,q = None):
		if q != None:
			self._cy02 = q
			self.setParameterValueDouble("Cy02 - Degree 2",q)
			return self
		else:
			return self._cy02
	def cx22(self,q = None):
		if q != None:
			self._cx22 = q
			self.setParameterValueDouble("Cx22 - Degree 2",q)
			return self
		else:
			return self._cx22
	def cy22(self,q = None):
		if q != None:
			self._cy22 = q
			self.setParameterValueDouble("Cy22 - Degree 2",q)
			return self
		else:
			return self._cy22
	def cx04(self,q = None):
		if q != None:
			self._cx04 = q
			self.setParameterValueDouble("Cx04 - Degree 4",q)
			return self
		else:
			return self._cx04
	def cy04(self,q = None):
		if q != None:
			self._cy04 = q
			self.setParameterValueDouble("Cy04 - Degree 4",q)
			return self
		else:
			return self._cy04
	def cx24(self,q = None):
		if q != None:
			self._cx24 = q
			self.setParameterValueDouble("Cx24 - Degree 4",q)
			return self
		else:
			return self._cx24
	def cy24(self,q = None):
		if q != None:
			self._cy24 = q
			self.setParameterValueDouble("Cy24 - Degree 4",q)
			return self
		else:
			return self._cy24
	def cx44(self,q = None):
		if q != None:
			self._cx44 = q
			self.setParameterValueDouble("Cx44 - Degree 4",q)
			return self
		else:
			return self._cx44
	def cy44(self,q = None):
		if q != None:
			self._cy44 = q
			self.setParameterValueDouble("Cy44 - Degree 4",q)
			return self
		else:
			return self._cy44
	def cx06(self,q = None):
		if q != None:
			self._cx06 = q
			self.setParameterValueDouble("Cx06 - Degree 6",q)
			return self
		else:
			return self._cx06
	def cy06(self,q = None):
		if q != None:
			self._cy06 = q
			self.setParameterValueDouble("Cy06 - Degree 6",q)
			return self
		else:
			return self._cy06
	def cx26(self,q = None):
		if q != None:
			self._cx26 = q
			self.setParameterValueDouble("Cx26 - Degree 6",q)
			return self
		else:
			return self._cx26
	def cy26(self,q = None):
		if q != None:
			self._cy26 = q
			self.setParameterValueDouble("Cy26 - Degree 6",q)
			return self
		else:
			return self._cy26
	def cx46(self,q = None):
		if q != None:
			self._cx46 = q
			self.setParameterValueDouble("Cx46 - Degree 6",q)
			return self
		else:
			return self._cx46
	def cy46(self,q = None):
		if q != None:
			self._cy46 = q
			self.setParameterValueDouble("Cy46 - Degree 6",q)
			return self
		else:
			return self._cy46
	def cx66(self,q = None):
		if q != None:
			self._cx66 = q
			self.setParameterValueDouble("Cx66 - Degree 6",q)
			return self
		else:
			return self._cx66
	def cy66(self,q = None):
		if q != None:
			self._cy66 = q
			self.setParameterValueDouble("Cy66 - Degree 6",q)
			return self
		else:
			return self._cy66

class radial_fisheye_degree_8(builtin,ldp.radial_deg_8):
	def __init__(self):
		builtin.__init__(self)
		ldp.radial_deg_8.__init__(self)
		builtin.init_builtin_parameters(self)
		self.parameters = [self.getParameterName(i) for i in range(self.getNumParameters())]
# model-specific parameters in mathematical notation
		self._c2 = self.getParameterDefaultValue("Distortion - Degree 2")
		self._c4 = self.getParameterDefaultValue("Quartic Distortion - Degree 4")
		self._c6 = self.getParameterDefaultValue("Degree 6")
		self._c8 = self.getParameterDefaultValue("Degree 8")
# Getter in gui notation
	def getParameterValue(self,p):
		if	p == "Distortion - Degree 2":		return self._c2
		elif	p == "Quartic Distortion - Degree 4":	return self._c4
		elif	p == "Degree 6":			return self._c6
		elif	p == "Degree 8":			return self._c8
		return builtin.getParameterValue(self,p)
# Setters and Getters in mathematical notation.
	def c2(self,q = None):
		if q != None:
			self._c2 = q
			self.setParameterValueDouble("Distortion - Degree 2",q)
			return self
		else:
			return self._c2
	def c4(self,q = None):
		if q != None:
			self._c4 = q
			self.setParameterValueDouble("Quartic Distortion - Degree 4",q)
			return self
		else:
			return self._c4
	def c6(self,q = None):
		if q != None:
			self._c6 = q
			self.setParameterValueDouble("Degree 6",q)
			return self
		else:
			return self._c6
	def c8(self,q = None):
		if q != None:
			self._c8 = q
			self.setParameterValueDouble("Degree 8",q)
			return self
		else:
			return self._c8

class radial_standard_degree_4(builtin,ldp.radial_decentered_deg_4_cylindric):
	def __init__(self):
		builtin.__init__(self)
		ldp.radial_decentered_deg_4_cylindric.__init__(self)
		builtin.init_builtin_parameters(self)
		self.parameters = [self.getParameterName(i) for i in range(self.getNumParameters())]
# model-specific parameters in mathematical notation
		self._c2 = self.getParameterDefaultValue("Distortion - Degree 2")
		self._u2 = self.getParameterDefaultValue("U - Degree 2")
		self._v2 = self.getParameterDefaultValue("V - Degree 2")
		self._c4 = self.getParameterDefaultValue("Quartic Distortion - Degree 4")
		self._u4 = self.getParameterDefaultValue("U - Degree 4")
		self._v4 = self.getParameterDefaultValue("V - Degree 4")
		self._phi_bs = self.getParameterDefaultValue("Phi - Cylindric Direction")
		self._b_bs = self.getParameterDefaultValue("B - Cylindric Bending")
# Getter in gui notation
	def getParameterValue(self,p):
		if	p == "Distortion - Degree 2":		return self._c2
		elif	p == "U - Degree 2":			return self._u2
		elif	p == "V - Degree 2":			return self._v2
		elif	p == "Quartic Distortion - Degree 4":	return self._c4
		elif	p == "U - Degree 4":			return self._u4
		elif	p == "V - Degree 4":			return self._v4
		elif	p == "Phi - Cylindric Direction":	return self._phi_bs
		elif	p == "B - Cylindric Bending":		return self._b_bs
		return builtin.getParameterValue(self,p)
# Setters and Getters in mathematical notation.
	def c2(self,q = None):
		if q != None:
			self._c2 = q
			self.setParameterValueDouble("Distortion - Degree 2",q)
			return self
		else:
			return self._c2
	def u2(self,q = None):
		if q != None:
			self._u2 = q
			self.setParameterValueDouble("U - Degree 2",q)
			return self
		else:
			return self._u2
	def v2(self,q = None):
		if q != None:
			self._v2 = q
			self.setParameterValueDouble("V - Degree 2",q)
			return self
		else:
			return self._v2
	def c4(self,q = None):
		if q != None:
			self._c4 = q
			self.setParameterValueDouble("Quartic Distortion - Degree 4",q)
			return self
		else:
			return self._c4
	def u4(self,q = None):
		if q != None:
			self._u4 = q
			self.setParameterValueDouble("U - Degree 4",q)
			return self
		else:
			return self._u4
	def v4(self,q = None):
		if q != None:
			self._v4 = q
			self.setParameterValueDouble("V - Degree 4",q)
			return self
		else:
			return self._v4
	def phi_bs(self,q = None):
		if q != None:
			self._phi_bs = q
			self.setParameterValueDouble("Phi - Cylindric Direction",q)
			return self
		else:
			return self._phi_bs
	def b_bs(self,q = None):
		if q != None:
			self._b_bs = q
			self.setParameterValueDouble("B - Cylindric Bending",q)
			return self
		else:
			return self._b_bs
			
class classic_3de_mixed(builtin,ldp.classic_3de_mixed):
	def __init__(self):
		builtin.__init__(self)
		ldp.classic_3de_mixed.__init__(self)
		builtin.init_builtin_parameters(self)
		self.parameters = [self.getParameterName(i) for i in range(self.getNumParameters())]
# model-specific parameters in mathematical notation
		self._c2 = self.getParameterDefaultValue("Distortion")
		self._s_anam = self.getParameterDefaultValue("Anamorphic Squeeze")
		self._cx = self.getParameterDefaultValue("Curvature X")
		self._cy = self.getParameterDefaultValue("Curvature Y")
		self._c4 = self.getParameterDefaultValue("Quartic Distortion")
# Getter in gui notation
	def getParameterValue(self,p):
		if	p == "Distortion":		return self._c2
		elif	p == "Anamorphic Squeeze":	return self._s_anam
		elif	p == "Curvature X":		return self._cx
		elif	p == "Curvature Y":		return self._cy
		elif	p == "Quartic Distortion":	return self._c4
		return builtin.getParameterValue(self,p)
# Setters and Getters in mathematical notation.
	def c2(self,q = None):
		if q != None:
			self._c2 = q
			self.setParameterValueDouble("Distortion",q)
			return self
		else:
			return self._c2
	def s_anam(self,q = None):
		if q != None:
			self._s_anam = q
			self.setParameterValueDouble("Anamorphic Squeeze",q)
			return self
		else:
			return self._s_anam
	def cx(self,q = None):
		if q != None:
			self._cx = q
			self.setParameterValueDouble("Curvature X",q)
			return self
		else:
			return self._cx
	def cy(self,q = None):
		if q != None:
			self._cy = q
			self.setParameterValueDouble("Curvature Y",q)
			return self
		else:
			return self._cy
	def c4(self,q = None):
		if q != None:
			self._c4 = q
			self.setParameterValueDouble("Quartic Distortion",q)
			return self
		else:
			return self._c4

################################################################
# Experimental                                                 #
################################################################
class radial_homomorphic_degree_2(builtin,ldp.radial_homomorphic_decentered_deg_2):
	def __init__(self):
		builtin.__init__(self)
		ldp.radial_homomorphic_decentered_deg_2.__init__(self)
		builtin.init_builtin_parameters(self)
		self.parameters = [self.getParameterName(i) for i in range(self.getNumParameters())]
# model-specific parameters in mathematical notation
		self._c2 = self.getParameterDefaultValue("C - Degree 2")
		self._u2 = self.getParameterDefaultValue("U - Degree 2")
		self._v2 = self.getParameterDefaultValue("V - Degree 2")
# Getter in gui notation
	def getParameterValue(self,p):
		if	p == "C, Degree 2":	return self._c2
		return builtin.getParameterValue(self,p)
# Setters and Getters in mathematical notation.
	def c2(self,q = None):
		if q != None:
			self._c2 = q
			self.setParameterValueDouble("C - Degree 2",q)
			return self
		else:
			return self._c2
	def u2(self,q = None):
		if q != None:
			self._c2 = q
			self.setParameterValueDouble("U - Degree 2",q)
			return self
		else:
			return self._c2
	def v2(self,q = None):
		if q != None:
			self._c2 = q
			self.setParameterValueDouble("V - Degree 2",q)
			return self
		else:
			return self._c2

class radial_fisheye_equidistant_degree_8(builtin,ldp.radial_fisheye_equidistant_deg_8):
	def __init__(self):
		builtin.__init__(self)
		ldp.radial_fisheye_equidistant_deg_8.__init__(self)
		builtin.init_builtin_parameters(self)
		self.parameters = [self.getParameterName(i) for i in range(self.getNumParameters())]
# model-specific parameters in mathematical notation
		self._c2 = self.getParameterDefaultValue("Degree 2")
		self._c4 = self.getParameterDefaultValue("Degree 4")
		self._c6 = self.getParameterDefaultValue("Degree 6")
		self._c8 = self.getParameterDefaultValue("Degree 8")
# Getter in gui notation
	def getParameterValue(self,p):
		if	p == "Degree 2":	return self._c2
		elif	p == "Degree 4":	return self._c4
		elif	p == "Degree 6":	return self._c6
		elif	p == "Degree 8":	return self._c8
		return builtin.getParameterValue(self,p)
# Setters and Getters in mathematical notation.
	def c2(self,q = None):
		if q != None:
			self._c2 = q
			self.setParameterValueDouble("Degree 2",q)
			return self
		else:
			return self._c2
	def c4(self,q = None):
		if q != None:
			self._c4 = q
			self.setParameterValueDouble("Degree 4",q)
			return self
		else:
			return self._c4
	def c6(self,q = None):
		if q != None:
			self._c6 = q
			self.setParameterValueDouble("Degree 6",q)
			return self
		else:
			return self._c6
	def c8(self,q = None):
		if q != None:
			self._c8 = q
			self.setParameterValueDouble("Degree 8",q)
			return self
		else:
			return self._c8

class radial_fisheye_equisolid_degree_8(builtin,ldp.radial_fisheye_equisolid_deg_8):
	def __init__(self):
		builtin.__init__(self)
		ldp.radial_fisheye_equisolid_deg_8.__init__(self)
		builtin.init_builtin_parameters(self)
		self.parameters = [self.getParameterName(i) for i in range(self.getNumParameters())]
# model-specific parameters in mathematical notation
		self._c2 = self.getParameterDefaultValue("Degree 2")
		self._c4 = self.getParameterDefaultValue("Degree 4")
		self._c6 = self.getParameterDefaultValue("Degree 6")
		self._c8 = self.getParameterDefaultValue("Degree 8")
# Getter in gui notation
	def getParameterValue(self,p):
		if	p == "Degree 2":	return self._c2
		elif	p == "Degree 4":	return self._c4
		elif	p == "Degree 6":	return self._c6
		elif	p == "Degree 8":	return self._c8
		return builtin.getParameterValue(self,p)
# Setters and Getters in mathematical notation.
	def c2(self,q = None):
		if q != None:
			self._c2 = q
			self.setParameterValueDouble("Degree 2",q)
			return self
		else:
			return self._c2
	def c4(self,q = None):
		if q != None:
			self._c4 = q
			self.setParameterValueDouble("Degree 4",q)
			return self
		else:
			return self._c4
	def c6(self,q = None):
		if q != None:
			self._c6 = q
			self.setParameterValueDouble("Degree 6",q)
			return self
		else:
			return self._c6
	def c8(self,q = None):
		if q != None:
			self._c8 = q
			self.setParameterValueDouble("Degree 8",q)
			return self
		else:
			return self._c8

class radial_fisheye_orthographic_degree_8(builtin,ldp.radial_fisheye_orthographic_deg_8):
	def __init__(self):
		builtin.__init__(self)
		ldp.radial_fisheye_orthographic_deg_8.__init__(self)
		builtin.init_builtin_parameters(self)
		self.parameters = [self.getParameterName(i) for i in range(self.getNumParameters())]
# model-specific parameters in mathematical notation
		self._c2 = self.getParameterDefaultValue("Degree 2")
		self._c4 = self.getParameterDefaultValue("Degree 4")
		self._c6 = self.getParameterDefaultValue("Degree 6")
		self._c8 = self.getParameterDefaultValue("Degree 8")
# Getter in gui notation
	def getParameterValue(self,p):
		if	p == "Degree 2":	return self._c2
		elif	p == "Degree 4":	return self._c4
		elif	p == "Degree 6":	return self._c6
		elif	p == "Degree 8":	return self._c8
		return builtin.getParameterValue(self,p)
# Setters and Getters in mathematical notation.
	def c2(self,q = None):
		if q != None:
			self._c2 = q
			self.setParameterValueDouble("Degree 2",q)
			return self
		else:
			return self._c2
	def c4(self,q = None):
		if q != None:
			self._c4 = q
			self.setParameterValueDouble("Degree 4",q)
			return self
		else:
			return self._c4
	def c6(self,q = None):
		if q != None:
			self._c6 = q
			self.setParameterValueDouble("Degree 6",q)
			return self
		else:
			return self._c6
	def c8(self,q = None):
		if q != None:
			self._c8 = q
			self.setParameterValueDouble("Degree 8",q)
			return self
		else:
			return self._c8

class radial_fisheye_stereographic_degree_8(builtin,ldp.radial_fisheye_stereographic_deg_8):
	def __init__(self):
		builtin.__init__(self)
		ldp.radial_fisheye_stereographic_deg_8.__init__(self)
		builtin.init_builtin_parameters(self)
		self.parameters = [self.getParameterName(i) for i in range(self.getNumParameters())]
# model-specific parameters in mathematical notation
		self._c2 = self.getParameterDefaultValue("Degree 2")
		self._c4 = self.getParameterDefaultValue("Degree 4")
		self._c6 = self.getParameterDefaultValue("Degree 6")
		self._c8 = self.getParameterDefaultValue("Degree 8")
# Getter in gui notation
	def getParameterValue(self,p):
		if	p == "Degree 2":	return self._c2
		elif	p == "Degree 4":	return self._c4
		elif	p == "Degree 6":	return self._c6
		elif	p == "Degree 8":	return self._c8
		return builtin.getParameterValue(self,p)
# Setters and Getters in mathematical notation.
	def c2(self,q = None):
		if q != None:
			self._c2 = q
			self.setParameterValueDouble("Degree 2",q)
			return self
		else:
			return self._c2
	def c4(self,q = None):
		if q != None:
			self._c4 = q
			self.setParameterValueDouble("Degree 4",q)
			return self
		else:
			return self._c4
	def c6(self,q = None):
		if q != None:
			self._c6 = q
			self.setParameterValueDouble("Degree 6",q)
			return self
		else:
			return self._c6
	def c8(self,q = None):
		if q != None:
			self._c8 = q
			self.setParameterValueDouble("Degree 8",q)
			return self
		else:
			return self._c8


################################################################
# Automatic documentation                                      #
################################################################

list_of_models = [
	anamorphic_standard_degree_4,
	anamorphic_rescaled_degree_4,
	anamorphic_degree_6,
	radial_fisheye_degree_8,
	radial_standard_degree_4,
	classic_3de_mixed,
	radial_homomorphic_degree_2,
	radial_fisheye_equidistant_degree_8,
	radial_fisheye_equisolid_degree_8,
	radial_fisheye_orthographic_degree_8,
	radial_fisheye_stereographic_degree_8
	]
