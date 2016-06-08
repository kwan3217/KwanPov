#furnsh "generic/generic.tm" //load generic kernels like lsk, de4xx, etc.
#furnsh "MSL/msl.tm" //metakernel pulls in the right sclk kernel
#declare UTC="2012-08-06 05:10:00UTC";
#declare ET=str2et(UTC);
#declare vel=<0,0,0>;
#declare pos=spkezr("MSL",ET,"J2000","NONE","399",vel);
#debug concat("At time ",str(ET,0,6), "(",UTC,")\n")
#debug "Relative to Earth:\n"
#debug concat("Pos: <",str(pos.x,0,6),",",str(pos.y,0,6),",",str(pos.z,0,6),">\n")
#debug concat("Vel: <",str(vel.x,0,6),",",str(vel.y,0,6),",",str(vel.z,0,6),">\n")
