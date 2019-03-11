import ldpk
m = ldpk.radial_fisheye_degree_8()
m.w_fb_cm(2.0).h_fb_cm(1.0).fl_cm(0.5)
m.prepare()
print m.distort([.5,.5])
print m.distort([.75,.75])
print m.distort([1.0,1.0])
