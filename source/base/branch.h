/*******************************************************************************
 * branch.h
 *
 * ---------------------------------------------------------------------------
 * UberPOV Ray Tracer version 1.37.
 * Copyright 2013 Christoph Lipka.
 *
 * UberPOV is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * UberPOV is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * ---------------------------------------------------------------------------
 * UberPOV is based on the popular Persistence of Vision Ray Tracer
 * ('POV-Ray') version 3.7, Copyright 1991-2013 Persistence of Vision
 * Raytracer Pty. Ltd.
 * ---------------------------------------------------------------------------
 * POV-Ray is based on the popular DKB raytracer version 2.12.
 * DKBTrace was originally written by David K. Buck.
 * DKBTrace Ver 2.0-2.12 were written by David K. Buck & Aaron A. Collins.
 * ---------------------------------------------------------------------------
 * $File: //depot/clipka/upov/source/base/branch.h $
 * $Revision: #3 $
 * $Change: 5970 $
 * $DateTime: 2013/07/29 10:32:25 $
 * $Author: clipka $
 *******************************************************************************/

#ifndef POVRAY_BRANCH_H
#define POVRAY_BRANCH_H

#define BRANCH_NAME             "UberPOV"
#define BRANCH_FULL_NAME        "UberPOV Raytracer"
#define BRANCH_MAINTAINER       "Christoph Lipka"
#define BRANCH_CONTACT          "http://www.lipka-koeln.de"
#define BRANCH_VERSION          "1.37.0.0"
#define BRANCH_COPYRIGHT        "Copyright 2013 Christoph Lipka."

#define BRANCH_DEVELOPERS       "Christoph Lipka" // alphabetically ordered comma-separated list of strings, e.g. , "John Doe", "Frank N. Furter"
//#define BRANCH_CONTRIBUTORS     // alphabetically ordered comma-separated list of strings, e.g. , "John Doe", "Frank N. Furter"

#define BRANCH_BUILD_IS_OFFICIAL 0


// TODO FIXME - as long as it doesn't have its own installer and set of distribution files,
// and instead relies on the files coming with POV-Ray proper, UberPOV hijacks POV-Ray's
// registry section and directory trees.

#define REGKEY      "POV-Ray"
#define REGVERKEY   "v3.7"

#define PATHKEY     "POV-Ray"
#define PATHVERKEY  "v3.7"


#ifndef BRANCH_BUILD_IS_OFFICIAL
#define BRANCH_BUILD_IS_OFFICIAL 0
#endif

#endif // POVRAY_BRANCH_H
