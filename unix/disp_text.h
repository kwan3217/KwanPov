/*******************************************************************************
 * disp_text.h
 *
 * Written by Christoph Hormann <chris_hormann@gmx.de>
 *
 * Template for a text mode render display system.
 *
 * ---------------------------------------------------------------------------
 * UberPOV Raytracer version 1.37.
 * Partial Copyright 2013 Christoph Lipka.
 *
 * UberPOV 1.37 is an experimental unofficial branch of POV-Ray 3.7, and is
 * subject to the same licensing terms and conditions.
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
 * $File: //depot/clipka/upov/unix/disp_text.h $
 * $Revision: #3 $
 * $Change: 5944 $
 * $DateTime: 2013/07/22 09:27:03 $
 * $Author: clipka $
 *******************************************************************************/

#ifndef _DISP_TEXT_H
#define _DISP_TEXT_H

#include "vfe.h"
#include "unixoptions.h"
#include "disp.h"

namespace pov_frontend
{
	using namespace std;
	using namespace vfe;
	using namespace vfePlatform;

	class UnixTextDisplay : public UnixDisplay
	{
		public:
			static const UnixOptionsProcessor::Option_Info Options[];
			static bool Register(vfeUnixSession *session);

			UnixTextDisplay(unsigned int w, unsigned int h, GammaCurvePtr gamma, float glareDesaturation, vfeSession *session, bool visible) :
				UnixDisplay(w, h, gamma, glareDesaturation, session, visible) {};
			virtual ~UnixTextDisplay() {} ;
			void Initialise() {};
			void Close() {};
			void Show() {};
			void Hide() {};
			bool TakeOver(UnixDisplay *display) { return false; };
			void DrawPixel(unsigned int x, unsigned int y, const RGBA8& colour);
			bool HandleEvents() { return false; };
			void UpdateScreen(bool Force = false) {};
			void PauseWhenDoneNotifyStart() {};
			bool PauseWhenDoneResumeIsRequested() { return true; };
			void PauseWhenDoneNotifyEnd() {};
	};
}

#endif
