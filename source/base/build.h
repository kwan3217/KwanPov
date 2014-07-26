//******************************************************************************
///
/// @file base/build.h
///
/// This file contains preprocessor defines specifying details of an individual
/// build.
///
/// @copyright
/// @parblock
///
/// UberPOV Raytracer version 1.37.
/// Copyright 2013-2014 Christoph Lipka.
///
/// UberPOV is free software: you can redistribute it and/or modify
/// it under the terms of the GNU Affero General Public License as
/// published by the Free Software Foundation, either version 3 of the
/// License, or (at your option) any later version.
///
/// UberPOV is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU Affero General Public License for more details.
///
/// You should have received a copy of the GNU Affero General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
/// ---------------------------------------------------------------------------
///
/// UberPOV is based on the popular Persistence of Vision Ray Tracer
/// ('POV-Ray') version 3.7, Copyright 1991-2013 Persistence of Vision
/// Raytracer Pty. Ltd.
///
/// ----------------------------------------------------------------------------
///
/// POV-Ray is based on the popular DKB raytracer version 2.12.
/// DKBTrace was originally written by David K. Buck.
/// DKBTrace Ver 2.0-2.12 were written by David K. Buck & Aaron A. Collins.
///
/// @endparblock
///
//******************************************************************************

#ifndef BASE_BUILD_H
#define BASE_BUILD_H

#ifndef POV_RAY_IS_OFFICIAL
/// Controls whether the build identifies as an official POV-Ray build.
/// @attention Please always leave this set to 0. Having your custom build pose as an official POV-Ray build would
///            violate trade marks of Persistence of Vision Raytracer Pty. Ltd.
#define POV_RAY_IS_OFFICIAL 0
#endif

#ifndef BRANCH_BUILD_IS_OFFICIAL
/// Controls whether the build identifies as an official UberPOV release.
/// @attention Please leave this set to 0 unless you're indeed preparing an official release build.
///            Having your custom build pose as an official UberPOV release would violate common standards of moral
///            and respect.
#define BRANCH_BUILD_IS_OFFICIAL 0
#endif

#ifndef BUILT_BY
/// Specifies the person or organization responsible for this build.
/// @attention Please set this to your real name, and/or include a working email or website address to contact you.
#define BUILT_BY "YOUR NAME (YOUR EMAIL)"
#undef BUILT_BY // remove this once you have filled in the above
#endif

#ifndef STANDALONE_BUILD
/// Controls whether the build runs in stand-alone or piggyback mode.
///   - 0: Piggyback mode; share configuration files, registry settings, sample files etc. with an official POV-Ray
///        installation.
///   - 1: Stand-alone mode; use an own set of configuration files, registry settings, sample files etc., to avoid
///        interference with any existing POV-Ray installation.
#define STANDALONE_BUILD 1
#endif

#endif // BASE_BUILD_H
