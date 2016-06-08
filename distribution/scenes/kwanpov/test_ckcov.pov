#furnsh "generic/generic.tm" //load generic kernels like lsk, de4xx, etc.
#furnsh "MSL/msl.tm"
//Notice that we don't necessarily ever furnish this kernel below
#declare Ncov=ckcov("MSL/ck/msl_edl_v01.bc",-76000,"INTERVAL",0,"TDB",1000);
#debug concat("Number of intervals: ",str(Ncov,0,0),"\n")
#declare I=0;
#while(I<Ncov)
  #declare etcov0=ckgetcov(I,0);
  #declare etcov1=ckgetcov(I,1);
  #debug concat("Interval ",str(I,0,0),": ")
  #debug concat("start - ",etcal(etcov0))
  #debug concat(" end - ",etcal(etcov1),"\n")
  #declare I=I+1;
#end
