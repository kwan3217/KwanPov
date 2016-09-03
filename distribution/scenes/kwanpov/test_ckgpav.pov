#furnsh "generic/generic.tm" //load generic kernels like lsk, de4xx, etc.
#furnsh "MSL/msl.tm" //metakernel pulls in the right sclk kernel
#declare ET=str2et("2012-08-06 05:10:00UTC");
#declare qpx=pxform("MSL_ROVER","J2000",ET);
#debug concat("Q_pxform: <",str(qpx.x,0,6),",",str(qpx.y,0,6),",",str(qpx.z,0,6),",",str(qpx.t,0,6),">\n")
#declare sclk=sce2c(-76,ET);
#declare qckgp=ckgp(-76000,sclk,0,"J2000");
#debug concat("Q_ckgp:   <",str(qckgp.x,0,6),",",str(qckgp.y,0,6),",",str(qckgp.z,0,6),",",str(qckgp.t,0,6),">\n")
#declare av=<0,0,0>;
#declare qckgpav1=ckgpav(-76000,sclk,0,"J2000");
#debug concat("Q_ckgpav1: <",str(qckgpav1.x,0,6),",",str(qckgpav1.y,0,6),",",str(qckgpav1.z,0,6),",",str(qckgpav1.t,0,6),">\n")
#declare qckgpav2=ckgpav(-76000,sclk,0,"J2000",av);
#debug concat("Q_ckgpav2: <",str(qckgpav2.x,0,6),",",str(qckgpav2.y,0,6),",",str(qckgpav2.z,0,6),",",str(qckgpav2.t,0,6),">\n")
#debug concat("av:       <",str(av.x,0,6),",",str(av.y,0,6),",",str(av.z,0,6),">\n")
