#declare FolsomXYZ=< -1288488.9373, -4720620.9617,  4079778.3407>;
#declare Re=6378137.0;
#declare Rp=6356752.31425;
#declare NXYZ=FolsomXYZ/Re;
#declare LLA=xyz2lla(NXYZ,1-(Rp/Re));
#debug concat("Lat: ",str(        LLA.lat ,0,12),"\n")
#debug concat("deg: ",str(degrees(LLA.lat),0,12),"\n")
#debug concat("Lon: ",str(        LLA.lon ,0,12),"\n")
#debug concat("deg: ",str(degrees(LLA.lon),0,12),"\n")
#debug concat("Alt: ",str(LLA.alt*Re,0,12),"\n")

/*
% 
%   running XYZ2LLA returns the following:
%   >> [lat,lon,alt]=xyz2lla(-1288488.9373,-4720620.9617,4079778.3407,1e-15)
%   lat =
%      0.69828684115439
%   lon =
%     -1.83725477406124
%   alt =
%      1.612599931541830e+003
%
%   Chris Jeppesen
%   ASEN5090 - GPS Applications
%   31 Jan 2003
  %Default conv if none is specified
  if ~exist('conv','var')
    conv=1e-10;
  end
  %Ellipsoid equatorial radius, m
  a=6378137.0
*/

#declare Z=-1;
#while(Z<=1)
  #declare LLA=xyz2lla(<1,0,Z>,0.5);
  #debug concat(str(Z,0,2),",",str(LLA.lat,0,6),",",str(LLA.alt,0,6),"\n")
  #declare Z=Z+0.01;
#end
