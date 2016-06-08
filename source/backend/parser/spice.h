/*
 * spice.h
 *
 *  Created on: Jul 1, 2015
 *      Author: jeppesen
 */

#ifndef SOURCE_BACKEND_PARSER_SPICE_H_
#define SOURCE_BACKEND_PARSER_SPICE_H_

#include "backend/frame.h"
#include "SpiceUsr.h"


namespace pov
{

void Parse_Spkezr(Vector3d& Res);
void Parse_Pxform(Vector3d& Res);
void Parse_Ckgp(Vector3d& Res);
void Parse_Ckgpav(Vector3d& Res);
DBL Parse_Gdpool();
UCS2 *Parse_Etcal(bool pathname);
UCS2 *Parse_Timout(bool pathname);
void Furnish (void);

}
#endif /* SOURCE_BACKEND_PARSER_SPICE_H_ */
