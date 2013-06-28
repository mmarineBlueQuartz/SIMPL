/* ============================================================================
 * Copyright (c) 2011 Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2011 Dr. Michael A. Groeber (US Air Force Research Laboratories)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Groeber, Michael A. Jackson, the US Air Force,
 * BlueQuartz Software nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  This code was written under United States Air Force Contract number
 *                           FA8650-07-D-5800
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "FindGrainReferenceCAxisMisorientations.h"

#include <limits>


#include "DREAM3DLib/Common/DREAM3DMath.h"
#include "DREAM3DLib/Common/MatrixMath.h"
#include "DREAM3DLib/Common/Constants.h"

#include "DREAM3DLib/GenericFilters/FindCellQuats.h"
#include "DREAM3DLib/StatisticsFilters/FindAvgCAxes.h"
#include "DREAM3DLib/StatisticsFilters/FindEuclideanDistMap.h"

const static float m_pi = static_cast<float>(M_PI);

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindGrainReferenceCAxisMisorientations::FindGrainReferenceCAxisMisorientations() :
AbstractFilter(),
m_GrainIdsArrayName(DREAM3D::CellData::GrainIds),
m_CellPhasesArrayName(DREAM3D::CellData::Phases),
m_QuatsArrayName(DREAM3D::CellData::Quats),
m_GrainReferenceCAxisMisorientationsArrayName(DREAM3D::CellData::GrainReferenceCAxisMisorientations),
m_AvgCAxesArrayName(DREAM3D::FieldData::AvgCAxes),
m_GrainAvgCAxisMisorientationsArrayName(DREAM3D::FieldData::GrainAvgCAxisMisorientations),
m_GrainIds(NULL),
m_CellPhases(NULL),
m_GrainReferenceCAxisMisorientations(NULL),
m_AvgCAxes(NULL),
m_GrainAvgCAxisMisorientations(NULL),
m_Quats(NULL)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindGrainReferenceCAxisMisorientations::~FindGrainReferenceCAxisMisorientations()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindGrainReferenceCAxisMisorientations::setupFilterParameters()
{
}

// -----------------------------------------------------------------------------
void FindGrainReferenceCAxisMisorientations::readFilterParameters(AbstractFilterParametersReader* reader)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindGrainReferenceCAxisMisorientations::writeFilterParameters(AbstractFilterParametersWriter* writer)

{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindGrainReferenceCAxisMisorientations::dataCheck(bool preflight, size_t voxels, size_t fields, size_t ensembles)
{
  setErrorCondition(0);
  std::stringstream ss;
  VoxelDataContainer* m = getVoxelDataContainer();

  GET_PREREQ_DATA(m, DREAM3D, CellData, GrainIds, ss, -300, int32_t, Int32ArrayType, voxels, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellData, CellPhases, ss, -300, int32_t, Int32ArrayType,  voxels, 1)
  GET_PREREQ_DATA(m, DREAM3D, CellData, Quats, ss, -303, float, FloatArrayType, voxels, 5)

  CREATE_NON_PREREQ_DATA(m, DREAM3D, CellData, GrainReferenceCAxisMisorientations, ss, float, FloatArrayType, 0, voxels, 1)

  GET_PREREQ_DATA(m, DREAM3D, FieldData, AvgCAxes, ss, -303, float, FloatArrayType, fields, 3)


  CREATE_NON_PREREQ_DATA(m, DREAM3D, FieldData, GrainAvgCAxisMisorientations, ss, float, FloatArrayType, 0, fields, 1)
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindGrainReferenceCAxisMisorientations::preflight()
{
  dataCheck(true, 1,1,1);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindGrainReferenceCAxisMisorientations::execute()
{
  setErrorCondition(0);
  std::stringstream ss;
  VoxelDataContainer* m = getVoxelDataContainer();
  if(NULL == m)
  {
    setErrorCondition(-999);
    notifyErrorMessage("The DataContainer Object was NULL", -999);
    return;
  }

  int64_t totalPoints = m->getTotalPoints();
  int64_t totalFields = m->getNumFieldTuples();

  dataCheck(false, totalPoints, totalFields, m->getNumEnsembleTuples());
  if (getErrorCondition() < 0)
  {
    return;
  }

//  float** avgmiso = new float *[m->getNumFieldTuples()];
//  for (size_t i = 1; i < m->getNumFieldTuples(); i++)
//  {
//    avgmiso[i] = new float[2];
//    for (int j = 0; j < 2; j++)
//    {
//      avgmiso[i][j] = 0.0;
//    }
//  }
  size_t numFields = m->getNumFieldTuples();
  int avgMisoComps = 2;
  FloatArrayType::Pointer avgmisoPtr = FloatArrayType::CreateArray(numFields, avgMisoComps, "AvgMiso_Temp");
  avgmisoPtr->initializeWithZeros();
  float* avgmiso = avgmisoPtr->GetPointer(0);

  float q1[5];
  //float q2[5];

  typedef DataArray<unsigned int> XTalType;

  float w;
  size_t udims[3] = {0,0,0};
  m->getDimensions(udims);
#if (CMP_SIZEOF_SIZE_T == 4)
  typedef uint32_t DimType;
  uint32_t maxUInt32 = std::numeric_limits<uint32_t>::max();
  // We have more points than can be allocated on a 32 bit machine. Assert Now.
  if(totalPoints > maxUInt32)
  {
    setErrorCondition(-666);
    ss.str("");
    ss << "The volume is too large for a 32 bit machine. Try reducing the input volume size. Total Voxels: " << totalPoints;
    notifyErrorMessage(ss.str(), getErrorCondition());
    return;
  }
#else
  typedef int64_t DimType;
#endif

  DimType xPoints = static_cast<DimType>(udims[0]);
  DimType yPoints = static_cast<DimType>(udims[1]);
  DimType zPoints = static_cast<DimType>(udims[2]);

  DimType point;

  float g1[3][3];
  float g1t[3][3];
  float caxis[3] = {0,0,1};
  float c1[3];
  float AvgCAxis[3];
  size_t index = 0;

  for (DimType col = 0; col < xPoints; col++)
  {
    for (DimType row = 0; row < yPoints; row++)
    {
      for (DimType plane = 0; plane < zPoints; plane++)
      {
        point = (plane * xPoints * yPoints) + (row * xPoints) + col;
        if (m_GrainIds[point] > 0 && m_CellPhases[point] > 0)
        {
      q1[0] = 1;
          q1[1] = m_Quats[point*5 + 1];
          q1[2] = m_Quats[point*5 + 2];
          q1[3] = m_Quats[point*5 + 3];
          q1[4] = m_Quats[point*5 + 4];
        OrientationMath::QuattoMat(q1, g1);
        //transpose the g matricies so when caxis is multiplied by it
        //it will give the sample direction that the caxis is along
        MatrixMath::transpose3x3(g1, g1t);
        MatrixMath::multiply3x3with3x1(g1t, caxis, c1);
        //normalize so that the magnitude is 1
        MatrixMath::normalize3x1(c1);

          AvgCAxis[0] = m_AvgCAxes[3*m_GrainIds[point]];
          AvgCAxis[1] = m_AvgCAxes[3*m_GrainIds[point]+1];
          AvgCAxis[2] = m_AvgCAxes[3*m_GrainIds[point]+2];
        //normalize so that the magnitude is 1
        MatrixMath::normalize3x1(AvgCAxis);

        w = ((c1[0]*AvgCAxis[0])+(c1[1]*AvgCAxis[1])+(c1[2]*AvgCAxis[2]));
      if(w < -1) w = -1;
      if(w > 1) w = 1;
        w = acosf(w);
      w = w *(180.0f/m_pi);
      if(w > 90.0) w = 180.0-w;

          m_GrainReferenceCAxisMisorientations[point] = w;
          index = m_GrainIds[point] * avgMisoComps;
          avgmiso[index]++;
          avgmiso[index+1] = avgmiso[index] + w;
        }
        if (m_GrainIds[point] == 0 || m_CellPhases[point] == 0)
        {
          m_GrainReferenceCAxisMisorientations[point] = 0;
        }
      }
    }
  }


  for (size_t i = 1; i < numFields; i++)
  {
    index = i * avgMisoComps;
    m_GrainAvgCAxisMisorientations[i] = avgmiso[index+1] / avgmiso[index];
    if(avgmiso[index] == 0) { m_GrainAvgCAxisMisorientations[i] = 0.0; }
  }

  // Clean up all the heap allocated memory
//  for (size_t i = 1; i < m->getNumFieldTuples(); i++)
//  {
//    delete[] avgmiso[i];
//  }
//  delete avgmiso;

  notifyStatusMessage("Completed");
}
