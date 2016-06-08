#furnsh "msl.tm" //metakernel pulls in the right sclk kernel as well as a leapsecond kernel
#declare ET=str2et("2012-07-24 12:34:56UTC");
#declare sclk=sce2c(-76,ET);
#debug concat(str(sclk,0,6),"\n")
