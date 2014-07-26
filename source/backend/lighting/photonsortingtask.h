//******************************************************************************
///
/// @file backend/lighting/photonsortingtask.h
///
/// @todo   What's in here?
///
/// @copyright
/// @parblock
///
/// UberPOV Raytracer version 1.37.
/// Portions Copyright 2013-2014 Christoph Lipka.
///
/// UberPOV 1.37 is an experimental unofficial branch of POV-Ray 3.7, and is
/// subject to the same licensing terms and conditions.
///
/// ----------------------------------------------------------------------------
///
/// Persistence of Vision Ray Tracer ('POV-Ray') version 3.7.
/// Copyright 1991-2013 Persistence of Vision Raytracer Pty. Ltd.
///
/// POV-Ray is free software: you can redistribute it and/or modify
/// it under the terms of the GNU Affero General Public License as
/// published by the Free Software Foundation, either version 3 of the
/// License, or (at your option) any later version.
///
/// POV-Ray is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU Affero General Public License for more details.
///
/// You should have received a copy of the GNU Affero General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
/// ----------------------------------------------------------------------------
///
/// POV-Ray is based on the popular DKB raytracer version 2.12.
/// DKBTrace was originally written by David K. Buck.
/// DKBTrace Ver 2.0-2.12 were written by David K. Buck & Aaron A. Collins.
///
/// @endparblock
///
//*******************************************************************************

#ifndef PHOTONSORTINGTASK_H
#define PHOTONSORTINGTASK_H

#include "base/povms.h"
#include "backend/frame.h"
#include "backend/colour/colutils.h"
#include "backend/control/messagefactory.h"
#include "backend/lighting/photons.h"
#include "backend/parser/parse.h"
#include "backend/render/rendertask.h"
#include "backend/render/trace.h"

namespace pov
{

using namespace pov_base;

class PhotonSortingTask : public RenderTask
{
    public:
        MessageFactory messageFactory;
        Timer timer;

        vector<PhotonMap*> surfaceMaps;
        vector<PhotonMap*> mediaMaps;
        PhotonShootingStrategy* strategy;

        PhotonSortingTask(ViewData *vd, const vector<PhotonMap*>& surfaceMaps, const vector<PhotonMap*>& mediaMaps, PhotonShootingStrategy* strategy, size_t seed);
        ~PhotonSortingTask();

        void Run();
        void Stopped();
        void Finish();

        void SendProgress();

        void sortPhotonMap();
        int save();
        int load();
    private:
        class CooperateFunction : public Trace::CooperateFunctor
        {
            public:
                CooperateFunction(Task& t) : task(t) { }
                virtual void operator()() { task.Cooperate(); }
            private:
                Task& task;
        };

        CooperateFunction cooperate;
};

}
#endif
