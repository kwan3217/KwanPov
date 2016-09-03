//******************************************************************************
///
/// @file backend/scene/scene.cpp
///
/// @todo   What's in here?
///
/// @copyright
/// @parblock
///
/// UberPOV Raytracer version 1.37.
/// Portions Copyright 2013-2015 Christoph Lipka.
///
/// UberPOV 1.37 is an experimental unofficial branch of POV-Ray 3.7, and is
/// subject to the same licensing terms and conditions.
///
/// ----------------------------------------------------------------------------
///
/// Persistence of Vision Ray Tracer ('POV-Ray') version 3.7.
/// Copyright 1991-2015 Persistence of Vision Raytracer Pty. Ltd.
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
//******************************************************************************

#include <sstream>

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <sys/stat.h>

// frame.h must always be the first POV file included (pulls in platform config)
#include "backend/frame.h"
#include "backend/scene/scene.h"

#include "backend/control/renderbackend.h"
#include "backend/scene/objects.h"
#include "backend/scene/threaddata.h"
#include "backend/scene/view.h"
#include "backend/parser/parse.h"
#include "backend/bounding/boundingtask.h"
#include "backend/texture/texture.h"
#include "backend/shape/truetype.h"
#include "backend/vm/fnpovfpu.h"
#include "backend/shape/mesh.h"
#include "base/timer.h"
#include "base/povmsgid.h"
#include "base/platformbase.h"
#include "base/fileinputoutput.h"

// this must be the last file included
#include "base/povdebug.h"

namespace pov
{

SceneData::SceneData() :
    fog(NULL),
    rainbow(NULL),
    skysphere(NULL),
    functionVM(NULL)
{
	ckcovStruct.base=NULL;
    atmosphereIOR = 1.0;
    atmosphereDispersion = 0.0;
    backgroundTrans  = FilterTransm(0.0, 1.0);
    ambientLight = ColourModelRGB::Whitepoint::Colour;
    generalWhitepoint = ColourModelRGB::Whitepoint::Colour;

    iridWavelengths = PseudoColour::DominantWavelengths;

    languageVersion = OFFICIAL_VERSION_NUMBER;
    languageVersionSet = false;
    languageVersionLate = false;
    warningLevel = 10; // all warnings
    stringEncoding = 0; // ASCII
    noiseGenerator = kNoiseGen_RangeCorrected;
    explicitNoiseGenerator = false; // scene has not set the noise generator explicitly
    numberOfWaves = 10;
    parsedMaxTraceLevel = MAX_TRACE_LEVEL_DEFAULT;
    parsedAdcBailout = 1.0 / 255.0; // adc bailout sufficient for displays
    gammaMode = kPOVList_GammaMode_None; // default setting for 3.62, which in turn is the default for the language
    workingGamma.reset();
    workingGammaToSRGB.reset();
    inputFileGammaSet = false; // TODO remove for 3.7x
    inputFileGamma = SRGBGammaCurve::Get();

    mmPerUnit = 10;
    useSubsurface = false;
    subsurfaceSamplesDiffuse = 50;
    subsurfaceSamplesSingle = 50;
    subsurfaceUseRadiosity = false;

    bspMaxDepth = 0;
    bspObjectIsectCost = bspBaseAccessCost = bspChildAccessCost = bspMissChance = 0.0f;

    Fractal_Iteration_Stack_Length = 0;
    Max_Blob_Components = 1000; // TODO FIXME - this gets set in the parser but allocated *before* that in the scene data, and if it is 0 here, a malloc may fail there because the memory requested is zero [trf]
    Max_Bounding_Cylinders = 100; // TODO FIXME - see note for Max_Blob_Components
    functionPatternCount = 0;
    boundingSlabs = NULL;

    splitUnions = false;
    removeBounds = true;

    TTFonts = NULL;

    tree = NULL;

    functionVM = new FunctionVM();
}

SceneData::~SceneData()
{
    lightSources.clear();
    lightGroupLightSources.clear();
    Destroy_Skysphere(skysphere);
    while (fog != NULL)
    {
        FOG *next = fog->Next;
        Destroy_Fog(fog);
        fog = next;
    }
    while (rainbow != NULL)
    {
        RAINBOW *next = rainbow->Next;
        Destroy_Rainbow(rainbow);
        rainbow = next;
    }
    if(boundingSlabs != NULL)
        Destroy_BBox_Tree(boundingSlabs);
    if(TTFonts != NULL)
        FreeFontInfo(TTFonts);
    // TODO: perhaps ObjectBase::~ObjectBase would be a better place
    //       to handle cleanup of individual objects ?
    Destroy_Object(objects);
    delete functionVM;

    if(tree != NULL)
        delete tree;

    if(ckcovStruct.base!=NULL) {
    	POV_FREE(ckcovStruct.base);
    	ckcovStruct.base=NULL;
    }
}

UCS2String SceneData::FindFile(POVMSContext ctx, const UCS2String& filename, unsigned int stype)
{
    vector<UCS2String> filenames;
    UCS2String foundfile;
    bool tryExactFirst;

    // if filename extension, matches one of the standard ones, try the exact name first
    // (otherwise, try it last)
    UCS2String::size_type pos = filename.find_last_of('.');
    tryExactFirst = false;
    if (pos != UCS2String::npos)
    {
        for (size_t i = 0; i < POV_FILE_EXTENSIONS_PER_TYPE; i++)
        {
            if ( ( strlen(gPOV_File_Extensions[stype].ext[i]) > 0 ) &&
                 ( filename.compare(pos,filename.length()-pos, ASCIItoUCS2String(gPOV_File_Extensions[stype].ext[i])) == 0 ) )
            {
                // match
                tryExactFirst = true;
                break;
            }
        }
    }

    // build list of files to search for
    if (tryExactFirst)
        filenames.push_back(filename);

    // add filename with extension variants to list of files to search for
    for (size_t i = 0; i < POV_FILE_EXTENSIONS_PER_TYPE; i++)
    {
        if (strlen(gPOV_File_Extensions[stype].ext[i]) > 0)
        {
            UCS2String fn(filename);
            fn += ASCIItoUCS2String(gPOV_File_Extensions[stype].ext[i]);
            filenames.push_back(fn);
        }
    }

    if (!tryExactFirst)
        filenames.push_back(filename);

#ifdef USE_SCENE_FILE_MAPPING
    // see if the file is available locally
    for(vector<UCS2String>::const_iterator i(filenames.begin()); i != filenames.end(); i++)
    {
        FilenameToFilenameMap::iterator ilocalfile(scene2LocalFiles.find(*i));

        if(ilocalfile != scene2LocalFiles.end())
            return *i;
    }

    // see if the file is available as temporary file
    for(vector<UCS2String>::const_iterator i(filenames.begin()); i != filenames.end(); i++)
    {
        FilenameToFilenameMap::iterator itempfile(scene2TempFiles.find(*i));

        if(itempfile != scene2TempFiles.end())
            return *i;
    }
#endif

    // otherwise, request to find the file
    RenderBackend::SendFindFile(ctx, sceneId, frontendAddress, filenames, foundfile);

    return foundfile;
}

IStream *SceneData::ReadFile(POVMSContext ctx, const UCS2String& origname, const UCS2String& filename, unsigned int stype)
{
    UCS2String scenefile(filename);
    UCS2String localfile;
    UCS2String fileurl;

#ifdef USE_SCENE_FILE_MAPPING
    // see if the file is available locally
    FilenameToFilenameMap::iterator ilocalfile(scene2LocalFiles.find(scenefile));

    // if available locally, open it end return
    if(ilocalfile != scene2LocalFiles.end())
        return NewIStream(ilocalfile->second.c_str(), stype);

    // now try the original name as given in the scene
    if((ilocalfile = scene2LocalFiles.find(origname)) != scene2LocalFiles.end())
        return NewIStream(ilocalfile->second.c_str(), stype);

    // see if the file is available as temporary file
    FilenameToFilenameMap::iterator itempfile(scene2TempFiles.find(scenefile));

    // if available as temporary file, open it end return
    if(itempfile != scene2TempFiles.end())
        return NewIStream(itempfile->second.c_str(), stype);

    // otherwise, request the file
    RenderBackend::SendReadFile(ctx, sceneId, frontendAddress, scenefile, localfile, fileurl);

    // if it is available locally, add it to the map and then open it
    if(localfile.length() > 0)
    {
        scene2LocalFiles[scenefile] = localfile;
        local2SceneFiles[localfile] = scenefile;

        // yes this is a hack
        scene2LocalFiles[origname] = localfile;

        return NewIStream(localfile.c_str(), stype);
    }

    // if it is available remotely ...
    if(fileurl.length() > 0)
    {
        // create a temporary file
        UCS2String tempname = POV_PLATFORM_BASE.CreateTemporaryFile();
        OStream *tempfile = NewOStream(tempname.c_str(), stype, false);

        if(tempfile == NULL)
        {
            POV_PLATFORM_BASE.DeleteTemporaryFile(tempname);
            throw POV_EXCEPTION_CODE(kCannotOpenFileErr);
        }

        // download the file from the URL
        // TODO - handle referrer
        if(POV_PLATFORM_BASE.ReadFileFromURL(tempfile, fileurl) == false)
        {
            delete tempfile;
            POV_PLATFORM_BASE.DeleteTemporaryFile(tempname);
            throw POV_EXCEPTION_CODE(kNetworkConnectionErr);
        }

        delete tempfile;

        // add the temporary file to the map
        scene2TempFiles[scenefile] = tempname;
        temp2SceneFiles[tempname] = scenefile;

        return NewIStream(tempname.c_str(), stype);
    }

    // file not found
    return NULL;
#else
    return NewIStream(filename.c_str(), stype);
#endif
}

/* TODO FIXME - this is the correct code but it has a bug. The code above is just a hack [trf]
IStream *SceneData::ReadFile(POVMSContext ctx, const UCS2String& filename, unsigned int stype)
{
    UCS2String scenefile(filename);
    UCS2String localfile;
    UCS2String fileurl;

    // see if the file is available locally
    FilenameToFilenameMap::iterator ilocalfile(scene2LocalFiles.find(scenefile));

    // if available locally, open it end return
    if(ilocalfile != scene2LocalFiles.end())
        return NewIStream(ilocalfile->second.c_str(), stype);

    // see if the file is available as temporary file
    FilenameToFilenameMap::iterator itempfile(scene2TempFiles.find(scenefile));

    // if available as temporary file, open it end return
    if(itempfile != scene2TempFiles.end())
        return NewIStream(itempfile->second.c_str(), stype);

    // otherwise, request the file
    RenderBackend::SendReadFile(ctx, sceneId, frontendAddress, scenefile, localfile, fileurl);

    // if it is available locally, add it to the map and then open it
    if(localfile.length() > 0)
    {
        scene2LocalFiles[scenefile] = localfile;
        local2SceneFiles[localfile] = scenefile;

        return NewIStream(localfile.c_str(), stype);
    }

    // if it is available remotely ...
    if(fileurl.length() > 0)
    {
        // create a temporary file
        UCS2String tempname = POV_PLATFORM_BASE.CreateTemporaryFile();
        OStream *tempfile = NewOStream(tempname.c_str(), stype, false);

        if(tempfile == NULL)
        {
            POV_PLATFORM_BASE.DeleteTemporaryFile(tempname);
            throw POV_EXCEPTION_CODE(kCannotOpenFileErr);
        }

        // download the file from the URL
        // TODO - handle referrer
        if(POV_PLATFORM_BASE.ReadFileFromURL(tempfile, fileurl) == false)
        {
            delete tempfile;
            POV_PLATFORM_BASE.DeleteTemporaryFile(tempname);
            throw POV_EXCEPTION_CODE(kNetworkConnectionErr);
        }

        delete tempfile;

        // add the temporary file to the map
        scene2TempFiles[scenefile] = tempname;
        temp2SceneFiles[tempname] = scenefile;

        return NewIStream(tempname.c_str(), stype);
    }

    // file not found
    return NULL;
}
*/

boost::posix_time::ptime SceneData::GetFileTime(POVMSContext ctx, const UCS2String& origname, const UCS2String& filename, unsigned int stype)
{
#ifdef USE_SCENE_FILE_MAPPING
    #error not implemented yet
#else
    struct stat s;
    string fn = UCS2toASCIIString(filename);
    if (stat(fn.c_str(),&s) == 0)
        return boost::posix_time::from_time_t(s.st_mtime);
    else
        return boost::posix_time::ptime(boost::posix_time::not_a_date_time);
#endif
}

OStream *SceneData::CreateFile(POVMSContext ctx, const UCS2String& filename, unsigned int stype, bool append)
{
    UCS2String scenefile(filename);

#ifdef USE_SCENE_FILE_MAPPING
    // see if the file is available as temporary file
    FilenameToFilenameMap::iterator itempfile(scene2TempFiles.find(scenefile));

    // if available as temporary file, open it end return
    if(itempfile != scene2TempFiles.end())
        return NewOStream(itempfile->second.c_str(), stype, append);

    // otherwise, create a temporary file ...
    UCS2String tempname = POV_PLATFORM_BASE.CreateTemporaryFile();
    OStream *tempfile = NewOStream(tempname.c_str(), stype, append);

    // failed to open file
    if(tempfile == NULL)
        return NULL;

    // add the temporary file to the map
    scene2TempFiles[scenefile] = tempname;
    temp2SceneFiles[tempname] = scenefile;
#else
    // this is a workaround for the incomplete scene temp file support
    // until someone has time to finish it.

    OStream *tempfile = NewOStream(scenefile.c_str(), stype, append);
    if (tempfile == NULL)
        return NULL;
#endif

    // let the frontend know that a new file was created
    RenderBackend::SendCreatedFile(ctx, sceneId, frontendAddress, scenefile);

    return tempfile;
}

Scene::Scene(POVMSAddress backendAddr, POVMSAddress frontendAddr, RenderBackend::SceneId sid) :
    sceneData(new SceneData()),
    stopRequsted(false),
    parserControlThread(NULL)
{
    sceneData->tree = NULL;
    sceneData->sceneId = sid;
    sceneData->backendAddress = backendAddr;
    sceneData->frontendAddress = frontendAddr;
}

Scene::~Scene()
{
    stopRequsted = true; // NOTE: Order is important here, set this before stopping the queue!
    parserTasks.Stop();

    if(parserControlThread != NULL)
        parserControlThread->join();
    delete parserControlThread;

    for(vector<SceneThreadData *>::iterator i(sceneThreadData.begin()); i != sceneThreadData.end(); i++)
        delete (*i);
    sceneThreadData.clear();
}

void Scene::StartParser(POVMS_Object& parseOptions)
{
    size_t seed = 0; // TODO

    // A scene can only be parsed once
    if(parserControlThread == NULL)
#ifndef USE_OFFICIAL_BOOST
        parserControlThread = new boost::thread(boost::bind(&Scene::ParserControlThread, this), 1024 * 64);
#else
        parserControlThread = new boost::thread(boost::bind(&Scene::ParserControlThread, this));
#endif
    else
        return;

    if (parseOptions.Exist(kPOVAttrib_Version))
    {
        sceneData->languageVersion = clip(int(parseOptions.GetFloat(kPOVAttrib_Version) * 100.0f + .5f), 100, 10000);
        sceneData->languageVersionSet = true;
    }

    sceneData->warningLevel = clip(parseOptions.TryGetInt(kPOVAttrib_WarningLevel, 9), 0, 9);

    sceneData->inputFile = parseOptions.TryGetUCS2String(kPOVAttrib_InputFile, "object.pov");
    sceneData->headerFile = parseOptions.TryGetUCS2String(kPOVAttrib_IncludeHeader, "");

    sceneData->defaultFileType = parseOptions.TryGetInt(kPOVAttrib_OutputFileType, DEFAULT_OUTPUT_FORMAT); // TODO - should get DEFAULT_OUTPUT_FORMAT from the front-end
    sceneData->clocklessAnimation = parseOptions.TryGetBool(kPOVAttrib_ClocklessAnimation, false); // TODO - experimental code

    sceneData->splitUnions = parseOptions.TryGetBool(kPOVAttrib_SplitUnions, false);
    sceneData->removeBounds = parseOptions.TryGetBool(kPOVAttrib_RemoveBounds, true);
    sceneData->boundingMethod = clip<int>(parseOptions.TryGetInt(kPOVAttrib_BoundingMethod, 1), 1, 2);
    if(parseOptions.TryGetBool(kPOVAttrib_Bounding, true) == false)
        sceneData->boundingMethod = 0;

    sceneData->outputAlpha = parseOptions.TryGetBool(kPOVAttrib_OutputAlpha, false);
    if (!sceneData->outputAlpha)
        // if we're not outputting an alpha channel, precompose the scene background against a black "background behind the background"
        // (NB: Here, background color is still at its default of <0,0,0,0,1> = full transparency; we're changing that to opaque black.)
        sceneData->backgroundTrans.Clear();

    // NB a value of '0' for any of the BSP parameters tells the BSP code to use its internal default
    sceneData->bspMaxDepth = parseOptions.TryGetInt(kPOVAttrib_BSP_MaxDepth, 0);
    sceneData->bspObjectIsectCost = clip<float>(parseOptions.TryGetFloat(kPOVAttrib_BSP_ISectCost, 0.0f), 0.0f, HUGE_VAL);
    sceneData->bspBaseAccessCost = clip<float>(parseOptions.TryGetFloat(kPOVAttrib_BSP_BaseAccessCost, 0.0f), 0.0f, HUGE_VAL);
    sceneData->bspChildAccessCost = clip<float>(parseOptions.TryGetFloat(kPOVAttrib_BSP_ChildAccessCost, 0.0f), 0.0f, HUGE_VAL);
    sceneData->bspMissChance = clip<float>(parseOptions.TryGetFloat(kPOVAttrib_BSP_MissChance, 0.0f), 0.0f, 1.0f - EPSILON);

    sceneData->realTimeRaytracing = parseOptions.TryGetBool(kPOVAttrib_RealTimeRaytracing, false);

    if(parseOptions.Exist(kPOVAttrib_Declare) == true)
    {
        POVMS_List ds;

        parseOptions.Get(kPOVAttrib_Declare, ds);
        for(int i = 1; i <= ds.GetListSize(); i++)
        {
            std::ostringstream sstr;
            POVMS_Attribute a;
            POVMS_Object d;

            ds.GetNth(i, d);
            d.Get(kPOVAttrib_Value, a);
            switch (a.Type())
            {
                case kPOVMSType_CString:
                    sstr << "\"" + d.TryGetString(kPOVAttrib_Value, "") + "\"";
                    break;

                case kPOVMSType_Float:
                    sstr << d.TryGetFloat(kPOVAttrib_Value, 0.0);
                    break;

                default:
                    // shouldn't happen unless we make a coding error
                    throw POV_EXCEPTION(kParamErr, "Invalid type passed in declare list");
            }

            sceneData->declaredVariables.insert(make_pair(d.GetString(kPOVAttrib_Identifier), sstr.str()));
        }
    }
    sceneData->declaredVariables.insert(make_pair(string("unofficial"), string("\"patch\"")));

    // do parsing
    sceneThreadData.push_back(dynamic_cast<SceneThreadData *>(parserTasks.AppendTask(new Parser(sceneData, bool(parseOptions.Exist(kPOVAttrib_Clock)), parseOptions.TryGetFloat(kPOVAttrib_Clock, 0.0), seed))));

    // wait for parsing
    parserTasks.AppendSync();

    // do bounding - we always call this even if the bounding is turned off
    // because it also generates object statistics
    sceneThreadData.push_back(dynamic_cast<SceneThreadData *>(parserTasks.AppendTask(new BoundingTask(sceneData, parseOptions.TryGetInt(kPOVAttrib_BoundingThreshold, 1), seed))));

    // wait for bounding
    parserTasks.AppendSync();

    // wait for bounding to finish
    parserTasks.AppendSync();

    // send statistics
    parserTasks.AppendFunction(boost::bind(&Scene::SendStatistics, this, _1));

    // send done message and compatibility data
    parserTasks.AppendFunction(boost::bind(&Scene::SendDoneMessage, this, _1));
}

void Scene::StopParser()
{
    parserTasks.Stop();

    RenderBackend::SendSceneFailedResult(sceneData->sceneId, kUserAbortErr, sceneData->frontendAddress);
}

void Scene::PauseParser()
{
    parserTasks.Pause();
}

void Scene::ResumeParser()
{
    parserTasks.Resume();
}

bool Scene::IsParsing()
{
    return parserTasks.IsRunning();
}

bool Scene::IsPaused()
{
    return parserTasks.IsPaused();
}

bool Scene::Failed()
{
    return parserTasks.Failed();
}

shared_ptr<View> Scene::NewView(unsigned int width, unsigned int height, RenderBackend::ViewId vid)
{
    if(parserTasks.IsDone() == false)
        throw POV_EXCEPTION_CODE(kNotNowErr);

    if((parserTasks.IsDone() == false) || (parserTasks.Failed() == true))
        throw POV_EXCEPTION_CODE(kNotNowErr);

    return shared_ptr<View>(new View(sceneData, width, height, vid));
}

void Scene::GetStatistics(POVMS_Object& parserStats)
{
    struct TimeData
    {
        POV_LONG cpuTime;
        POV_LONG realTime;
        size_t samples;

        TimeData() : cpuTime(0), realTime(0), samples(0) { }
    };

    TimeData timeData[SceneThreadData::kMaxTimeType];

    for(vector<SceneThreadData *>::iterator i(sceneThreadData.begin()); i != sceneThreadData.end(); i++)
    {
        timeData[(*i)->timeType].realTime = max(timeData[(*i)->timeType].realTime, (*i)->realTime);
        timeData[(*i)->timeType].cpuTime += (*i)->cpuTime;
        timeData[(*i)->timeType].samples++;
    }

    for(size_t i = SceneThreadData::kUnknownTime; i < SceneThreadData::kMaxTimeType; i++)
    {
        if(timeData[i].samples > 0)
        {
            POVMS_Object elapsedTime(kPOVObjectClass_ElapsedTime);

            elapsedTime.SetLong(kPOVAttrib_RealTime, timeData[i].realTime);
            elapsedTime.SetLong(kPOVAttrib_CPUTime, timeData[i].cpuTime);
            elapsedTime.SetInt(kPOVAttrib_TimeSamples, POVMSInt(timeData[i].samples));

            switch(i)
            {
                case SceneThreadData::kParseTime:
                    parserStats.Set(kPOVAttrib_ParseTime, elapsedTime);
                    break;
                case SceneThreadData::kBoundingTime:
                    parserStats.Set(kPOVAttrib_BoundingTime, elapsedTime);
                    break;
            }
        }
    }

    parserStats.SetInt(kPOVAttrib_FiniteObjects, sceneData->numberOfFiniteObjects);
    parserStats.SetInt(kPOVAttrib_InfiniteObjects, sceneData->numberOfInfiniteObjects);
    parserStats.SetInt(kPOVAttrib_LightSources, POVMSInt(sceneData->lightSources.size()));
    parserStats.SetInt(kPOVAttrib_Cameras, POVMSInt(sceneData->cameras.size()));

    if(sceneData->boundingMethod == 2)
    {
        parserStats.SetInt(kPOVAttrib_BSPNodes, sceneData->nodes);
        parserStats.SetInt(kPOVAttrib_BSPSplitNodes, sceneData->splitNodes);
        parserStats.SetInt(kPOVAttrib_BSPObjectNodes, sceneData->objectNodes);
        parserStats.SetInt(kPOVAttrib_BSPEmptyNodes, sceneData->emptyNodes);
        parserStats.SetInt(kPOVAttrib_BSPMaxObjects, sceneData->maxObjects);
        parserStats.SetFloat(kPOVAttrib_BSPAverageObjects, sceneData->averageObjects);
        parserStats.SetInt(kPOVAttrib_BSPMaxDepth, sceneData->maxDepth);
        parserStats.SetFloat(kPOVAttrib_BSPAverageDepth, sceneData->averageDepth);
        parserStats.SetInt(kPOVAttrib_BSPAborts, sceneData->aborts);
        parserStats.SetFloat(kPOVAttrib_BSPAverageAborts, sceneData->averageAborts);
        parserStats.SetFloat(kPOVAttrib_BSPAverageAbortObjects, sceneData->averageAbortObjects);
    }
}

void Scene::SendStatistics(TaskQueue&)
{
    POVMS_Message parserStats(kPOVObjectClass_ParserStatistics, kPOVMsgClass_SceneOutput, kPOVMsgIdent_ParserStatistics);

    GetStatistics(parserStats);

    parserStats.SetInt(kPOVAttrib_SceneId, sceneData->sceneId);
    parserStats.SetSourceAddress(sceneData->backendAddress);
    parserStats.SetDestinationAddress(sceneData->frontendAddress);

    POVMS_SendMessage(parserStats);

    for(vector<SceneThreadData *>::iterator i(sceneThreadData.begin()); i != sceneThreadData.end(); i++)
        delete (*i);
    sceneThreadData.clear();
}

void Scene::SendDoneMessage(TaskQueue&)
{
    POVMS_Message doneMessage(kPOVObjectClass_ResultData, kPOVMsgClass_SceneOutput, kPOVMsgIdent_Done);
    doneMessage.SetInt(kPOVAttrib_SceneId, sceneData->sceneId);
    doneMessage.SetSourceAddress(sceneData->backendAddress);
    doneMessage.SetDestinationAddress(sceneData->frontendAddress);
    doneMessage.SetInt(kPOVAttrib_LegacyGammaMode, sceneData->gammaMode);
    if (sceneData->workingGamma)
    {
        doneMessage.SetInt(kPOVAttrib_WorkingGammaType, sceneData->workingGamma->GetTypeId());
        doneMessage.SetFloat(kPOVAttrib_WorkingGamma, sceneData->workingGamma->GetParam());
    }
    POVMS_SendMessage(doneMessage);
}

void Scene::ParserControlThread()
{
    bool sentFailedResult = false;

    while(stopRequsted == false)
    {
        while((parserTasks.Process() == true) && (stopRequsted == false)) { }

        if((parserTasks.IsDone() == true) && (parserTasks.Failed() == true) && (sentFailedResult == false))
        {
            RenderBackend::SendSceneFailedResult(sceneData->sceneId, parserTasks.FailureCode(kUncategorizedError), sceneData->frontendAddress);
            sentFailedResult = true;
        }

        if(stopRequsted == false)
        {
            boost::thread::yield();
            Delay(10);
        }
    }
}

}
