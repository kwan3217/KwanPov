#furnsh "generic/generic.tm" //metakernel pulls in leapsecond kernel
#furnsh "MSL/msl.tm" //metakernel pulls in the right sclk kernel
#declare ET=str2et("2012-07-24 12:34:56UTC");
#declare sclk=sce2c(-76,ET);
#debug concat(str(sclk,0,6),"\n")

