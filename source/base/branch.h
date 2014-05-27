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
 * $Revision: #4 $
 * $Change: 6106 $
 * $DateTime: 2013/11/20 10:18:06 $
 * $Author: clipka $
 *******************************************************************************/

#ifndef POVRAY_BRANCH_H
#define POVRAY_BRANCH_H

#define BRANCH_NAME             "UberPOV"
#define BRANCH_FULL_NAME        "UberPOV Raytracer"
#define BRANCH_MAINTAINER       "Christoph Lipka"
#define BRANCH_CONTACT          "http://www.lipka-koeln.de"
#define BRANCH_VERSION          "1.37-dev"
#define BRANCH_COPYRIGHT        "Copyright 2013-2014 Christoph Lipka."

///
/// Primary developers of this branch, in alphabetical order.
/// Comma-separated list of strings, e.g. @code
///     #define BRANCH_DEVELOPERS "John Doe", "Frank N. Furter", "R. Daneel Olivaw"
/// @endcode.
///
#define BRANCH_DEVELOPERS       "Christoph Lipka"

///
/// Additional contributors to this branch, in alphabetical order.
/// Comma-separated list of strings, e.g. @code
///     #define BRANCH_CONTRIBUTORS "John Doe", "Frank N. Furter", "R. Daneel Olivaw"
/// @endcode.
/// Leave undefined if there are no additional contributors.
///
#define BRANCH_CONTRIBUTORS     "Christian Froeschlin"


// TODO FIXME - as long as it doesn't have its own installer and set of distribution files,
// and instead relies on the files coming with POV-Ray proper, UberPOV hijacks POV-Ray's
// registry section and directory trees.

#define REGKEY      "POV-Ray"
#define REGVERKEY   "v3.7"

#define PATHKEY     "POV-Ray"
#define PATHVERKEY  "v3.7"


///
/// *************************************************************************************************************
///
/// @name Patches With Notable Limitations
///
/// The following macros enable patches which have known noteworthy limitations.
///
/// @{
///

///
/// Experimental patch providing a mechanism to persist data between frames in an animation.
/// Kudos to Christian Froeschlin, who published the basis for this patch on the povray.unofficial.patches newsgroup.
///
/// @note     This patch will also persist data between successive renders in a GUI.
///           Currently, no mechanism has been implemented to force cleanup of persistent data from the GUI.
///           Cleanup from SDL can be achieved using the @c #undef statement.
///
/// @warning  This patch requires parsing to be single-threaded.
///
#define EXPERIMENTAL_UPOV_PERSISTENT 1

///
/// @}
///
/// *************************************************************************************************************
///

#ifndef BRANCH_BUILD_IS_OFFICIAL
#define BRANCH_BUILD_IS_OFFICIAL 0
#endif

#endif // POVRAY_BRANCH_H
