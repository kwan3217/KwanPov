/*
 * spice.cpp
 *
 *  Created on: Jul 1, 2015
 *      Author: jeppesen
 */
#include "backend/parser/spice.h"
#include "backend/parser/parse.h"
#include "boost/filesystem.hpp"

namespace pov {

DBL Parser::Parse_Gdpool() {
   char *Name;
   int start;
   int room=1,n;
   DBL values=0;
   SpiceBoolean found;

   GET (LEFT_PAREN_TOKEN);

   Name=Parse_C_String(false);

   Parse_Comma();

   start=(int)Parse_Float();

   Parse_Comma();

   gdpool_c(Name,start,room,&n,&values,&found);
//   Debug_Info("Getting kernel pool constant %s (value %f, found %d)\n",Name,values,found);
   if(failed_c()) {
     char explain[320];
     getmsg_c("EXPLAIN",319,explain);
     Error(explain);
   }
   POV_FREE(Name);

   EXPECT
     CASE (FLOAT_FUNCT_TOKEN)
       // All of these functions return a VECTOR result
       if(Token.Function_Id == FLOAT_ID_TOKEN)
       {
         *((DBL*)Token.Data)=found;
       }
       else
       {
         UNGET
       }
       EXIT
     END_CASE

     OTHERWISE
       UNGET
       EXIT
     END_CASE
   END_EXPECT

   GET (RIGHT_PAREN_TOKEN);
   return values;
}

void Parser::Parse_Spkezr(Vector3d& Res) {
  Vector3d Vel;
  char *Target,*Ref,*Abcorr,*Obs;
  SpiceDouble et,lt;
  SpiceDouble starg[6];

  GET (LEFT_PAREN_TOKEN);
  Target = Parse_C_String(false);

  Parse_Comma();

  et = Parse_Float();

  Parse_Comma();

  Ref = Parse_C_String(false);

  Parse_Comma();

  Abcorr = Parse_C_String(false);

  Parse_Comma();

  Obs = Parse_C_String(false);

  spkezr_c(Target,et,Ref,Abcorr,Obs,starg,&lt);
  if(failed_c()) {
    char explain[320];
    getmsg_c("EXPLAIN",319,explain);
    Error(explain);
  }
  POV_FREE(Target);
  POV_FREE(Ref);
  POV_FREE(Abcorr);
  POV_FREE(Obs);

  Res[X]=starg[0];
  Res[Y]=starg[1];
  Res[Z]=starg[2];
  Vel[X]=starg[3];
  Vel[Y]=starg[4];
  Vel[Z]=starg[5];

  Parse_Comma();

  EXPECT
    CASE (VECTOR_FUNCT_TOKEN)
      // All of these functions return a VECTOR result
      if(Token.Function_Id == VECTOR_ID_TOKEN) {
    	    (*reinterpret_cast<Vector3d *>(Token.Data)) = Vel;
      } else {
        UNGET
      }
      EXIT
    END_CASE

    OTHERWISE
      UNGET
      EXIT
    END_CASE
  END_EXPECT

  GET (RIGHT_PAREN_TOKEN);
}

void Parser::Parse_Pxform(EXPRESS& Res) {
  char *From,*To;
  SpiceDouble et;
  SpiceDouble smatx[3][3];
  SpiceDouble quat[4];

  GET (LEFT_PAREN_TOKEN);
  From = Parse_C_String(false);

  Parse_Comma();

  To = Parse_C_String(false);

  Parse_Comma();

  et = Parse_Float();

  pxform_c(From,To,et,smatx);

  POV_FREE(From);
  POV_FREE(To);

  if(failed_c()) {
    Res[T]=0;
    Res[X]=0;
    Res[Y]=0;
    Res[Z]=0;
    reset_c();
  } else {
    m2q_c(smatx,quat);

    Res[T]=quat[0];
    Res[X]=quat[1];
    Res[Y]=quat[2];
    Res[Z]=quat[3];
  }

  GET (RIGHT_PAREN_TOKEN);

}

void Parser::Parse_Ckgp(EXPRESS& Res) {
  char *Ref;
  SpiceInt inst;
  SpiceDouble sclkdp,clkout,tol;
  SpiceDouble cmat[3][3];
  SpiceDouble quat[4];
  SpiceBoolean found;

  GET (LEFT_PAREN_TOKEN);

  inst = (SpiceInt)Parse_Float();

  Parse_Comma();
  sclkdp = (SpiceDouble)Parse_Float();

  Parse_Comma();
  tol = (SpiceInt)Parse_Float();

  Parse_Comma();
  Ref = Parse_C_String(false);

  ckgp_c(inst,sclkdp,tol,Ref,cmat,&clkout,&found);
  if(failed_c()) {
    char explain[320];
    getmsg_c("EXPLAIN",319,explain);
    Error(explain);
  }

  POV_FREE(Ref);
  if(found) {
    m2q_c(cmat,quat);
    Res[T]=quat[0];
    Res[X]=quat[1];
    Res[Y]=quat[2];
    Res[Z]=quat[3];
  } else {
    Res[T]=0;
    Res[X]=0;
    Res[Y]=0;
    Res[Z]=0;
  }

  GET (RIGHT_PAREN_TOKEN);

}

void Parser::Parse_Ckgpav(EXPRESS& Res) {
  char *Ref;
  SpiceInt inst;
  SpiceDouble sclkdp,clkout,tol;
  SpiceDouble cmat[3][3];
  SpiceDouble quat[4];
  SpiceDouble av[3];
  Vector3d Av;
  SpiceBoolean found;

  GET (LEFT_PAREN_TOKEN);

  inst = (SpiceInt)Parse_Float();

  Parse_Comma();
  sclkdp = (SpiceDouble)Parse_Float();

  Parse_Comma();
  tol = (SpiceInt)Parse_Float();

  Parse_Comma();
  Ref = Parse_C_String(false);

  ckgpav_c(inst,sclkdp,tol,Ref,cmat,av,&clkout,&found);
  if(failed_c()) {
    char explain[320];
    getmsg_c("EXPLAIN",319,explain);
    Error(explain);
  }
  POV_FREE(Ref);
  if(found) {
    m2q_c(cmat,quat);
    Res[T]=quat[0];
    Res[X]=quat[1];
    Res[Y]=quat[2];
    Res[Z]=quat[3];
    Av[X]=av[X];
    Av[Y]=av[Y];
    Av[Z]=av[Z];
  } else {
    Res[T]=0;
    Res[X]=0;
    Res[Y]=0;
    Res[Z]=0;
    Av[X]=0;
    Av[Y]=0;
    Av[Z]=0;
  }

  Parse_Comma();

  EXPECT
    CASE (VECTOR_FUNCT_TOKEN)
      // All of these functions return a VECTOR result
      if(Token.Function_Id == VECTOR_ID_TOKEN) {
    	if(found) (*reinterpret_cast<Vector3d *>(Token.Data)) = Av;
      } else {
    	UNGET
      }
      EXIT
    END_CASE

	OTHERWISE
      UNGET
      EXIT
      END_CASE
  END_EXPECT

  GET (RIGHT_PAREN_TOKEN);

}

UCS2 *Parser::Parse_Etcal() {
  char temp[256],explain[320];
  DBL et;

  GET(LEFT_PAREN_TOKEN);
  et = Parse_Float();
  GET(RIGHT_PAREN_TOKEN);

  etcal_c(et,255,temp);
  if(failed_c()) {
    getmsg_c("EXPLAIN",319,explain);
    Error(explain);
  }

  return String_To_UCS2(temp);
}

UCS2 *Parser::Parse_Timout() {
  char temp[256];
  char* pictur;
  DBL et;

  GET(LEFT_PAREN_TOKEN);
  et = Parse_Float();
  Parse_Comma();
  pictur=Parse_C_String();
  GET(RIGHT_PAREN_TOKEN);

  timout_c(et,pictur,255,temp);
  POV_FREE(pictur);

  return String_To_UCS2(temp);
}

void Parser::Furnish() {

  if (Skip_Spaces () != true) Error ("Expecting a string after FURNSH.");

  char *tempascii;
  UCS2String temp;
  tempascii = Parse_C_String(true);
  temp = ASCIItoUCS2String(tempascii);
  POV_FREE(tempascii);

  UCS2String foundfile(sceneData->FindFile(GetPOVMSContext(), temp, POV_File_Unknown));

  if(foundfile.empty() == true) {
    Error("Cannot find kernel '%s'", UCS2toASCIIString(temp).c_str());
    return;
  }

  Debug_Info("Furnishing SPICE kernel: %s\n",UCS2toASCIIString(foundfile).c_str());
  SpiceChar filtyp[10],source[10];
  SpiceInt handle;
  SpiceBoolean found;
  boost::filesystem::path path1(UCS2toASCIIString(foundfile).c_str());

  erract_c("SET",0,"RETURN");
  kinfo_c(path1.filename().c_str(),9,9,filtyp,source,&handle,&found);
  Debug_Info("Kernel already loaded? %s\n",found?"yes":"no");
  if(!found) {
	char curdir[512];
    getcwd(curdir,511);
    chdir(path1.parent_path().c_str());
    furnsh_c(path1.filename().c_str());
    chdir(curdir);
    if(failed_c()) {
      char explain[320];
      getmsg_c("EXPLAIN",319,explain);
      Error(explain);
    }
  }
}

}
