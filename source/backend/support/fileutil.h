/*******************************************************************************
 * fileutil.h
 *
 * This module contains all defines, typedefs, and prototypes for fileutil.cpp.
 *
 * ---------------------------------------------------------------------------
 * Persistence of Vision Ray Tracer ('POV-Ray') version 3.7.
 * Copyright 1991-2013 Persistence of Vision Raytracer Pty. Ltd.
 *
 * POV-Ray is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * POV-Ray is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * ---------------------------------------------------------------------------
 * POV-Ray is based on the popular DKB raytracer version 2.12.
 * DKBTrace was originally written by David K. Buck.
 * DKBTrace Ver 2.0-2.12 were written by David K. Buck & Aaron A. Collins.
 * ---------------------------------------------------------------------------
 * $File: //depot/clipka/upov/source/backend/support/fileutil.h $
 * $Revision: #1 $
 * $Change: 5916 $
 * $DateTime: 2013/07/17 19:49:27 $
 * $Author: clipka $
 *******************************************************************************/

#ifndef POV_UTIL_H
#define POV_UTIL_H

#include <boost/date_time/posix_time/posix_time.hpp>

#include "base/povms.h"
#include "base/stringutilities.h"
#include "base/fileinputoutput.h"
#include "backend/parser/parse.h"

namespace pov
{

using namespace pov_base;

IStream *Locate_File(Parser *p, shared_ptr<SceneData>& sd, const UCS2String& filename, unsigned int stype, UCS2String& buffer, bool err_flag = false);
boost::posix_time::ptime Get_File_Time(Parser *p, shared_ptr<SceneData>& sd, const UCS2String& filename, unsigned int stype, bool err_flag = false);
IMemStream *Internal_Font_File(const int font_id, UCS2String& buffer);

}

#endif
