#furnsh "generic/generic.tm" //load generic kernels like lsk, de4xx, etc.
#furnsh "MSL/msl.tm" //metakernel pulls in the right sclk kernel
#declare ET=str2et("2012-08-06 05:10:00UTC");
#declare vel=<0,0,0>;
#declare pos=spkezr("MSL",ET,"J2000","LT+S","399",vel);
#declare q=pxform("MSL_ROVER","J2000",ET);
#debug concat("Pos: <",str(pos.x,0,6),",",str(pos.y,0,6),",",str(pos.z,0,6),">\n")
#debug concat("Vel: <",str(vel.x,0,6),",",str(vel.y,0,6),",",str(vel.z,0,6),">\n")
#debug concat("Q: <",str(q.x,0,6),",",str(q.y,0,6),",",str(q.z,0,6),",",str(q.t,0,6),">\n")
