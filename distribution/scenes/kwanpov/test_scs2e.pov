#furnsh "generic/generic.tm" //metakernel pulls in leapsecond kernel
#furnsh "MSL/msl.tm" //metakernel pulls in the right sclk kernel
#declare ET=scs2e(-76,"1/0000000000.00000");
#debug concat(str(ET,0,6),"\n")
