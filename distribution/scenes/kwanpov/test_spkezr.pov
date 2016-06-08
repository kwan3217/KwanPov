#furnsh "generic/generic.tm" //load generic kernels like lsk, de4xx, etc.
#furnsh "MSL/msl.tm" //metakernel pulls in the right sclk kernel
#declare ET=str2et("2012-07-24 12:34:56UTC");
#declare vel=<0,0,0>;
#declare pos=spkezr("MSL",ET,"J2000","LT+S","399",vel);
#debug concat("Pos: <",str(pos.x,0,6),",",str(pos.y,0,6),",",str(pos.z,0,6),">\n")
#debug concat("Pos: <",str(vel.x,0,6),",",str(vel.y,0,6),",",str(vel.z,0,6),">\n")
