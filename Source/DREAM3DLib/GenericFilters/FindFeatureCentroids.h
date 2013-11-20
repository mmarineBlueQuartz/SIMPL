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
#ifndef FindFeatureCentroids_H_
#define FindFeatureCentroids_H_

#include <vector>
#include <QtCore/QString>

#include "DREAM3DLib/DREAM3DLib.h"
#include "DREAM3DLib/Common/DREAM3DSetGetMacros.h"
#include "DREAM3DLib/DataArrays/IDataArray.h"

#include "DREAM3DLib/Common/AbstractFilter.h"
#include "DREAM3DLib/DataContainers/VolumeDataContainer.h"

/*
 *
 */
class DREAM3DLib_EXPORT FindFeatureCentroids : public AbstractFilter
{
  public:
    DREAM3D_SHARED_POINTERS(FindFeatureCentroids)
    DREAM3D_STATIC_NEW_MACRO(FindFeatureCentroids)
    DREAM3D_TYPE_MACRO_SUPER(FindFeatureCentroids, AbstractFilter)

    virtual ~FindFeatureCentroids();
    DREAM3D_INSTANCE_STRING_PROPERTY(DataContainerName)
    DREAM3D_INSTANCE_STRING_PROPERTY(CellAttributeMatrixName)
    
   //------ Required Cell Data
    DREAM3D_INSTANCE_STRING_PROPERTY(FeatureIdsArrayName)
    //------ Created Feature Data
    DREAM3D_INSTANCE_STRING_PROPERTY(CentroidsArrayName)

    DREAM3D_INSTANCE_STRING_PROPERTY(ActiveArrayName)



    virtual const QString getGroupName() { return DREAM3D::FilterGroups::GenericFilters; }
    virtual const QString getSubGroupName() { return DREAM3D::FilterSubGroups::MiscFilters; }
    virtual const QString getHumanLabel() { return "Find Feature Centroids"; }

    /**
    * @brief This method will write the options to a file
    * @param writer The writer that is used to write the options to a file
    */
    virtual int writeFilterParameters(AbstractFilterParametersWriter* writer, int index);

    /**
    * @brief This method will read the options from a file
    * @param reader The reader that is used to read the options from a file
    */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index);

    virtual void execute();
    virtual void preflight();

  protected:
    FindFeatureCentroids();

    void find_centroids();

  private:
    int32_t* m_FeatureIds;
    float* m_Centroids;

    DECLARE_WRAPPED_ARRAY(featurecenters, m_FeatureCenters, float) // N x 5 Array


    void dataCheck(bool preflight, size_t voxels, size_t features, size_t ensembles);

    FindFeatureCentroids(const FindFeatureCentroids&); // Copy Constructor Not Implemented
    void operator=(const FindFeatureCentroids&); // Operator '=' Not Implemented

};

#endif /* FindFeatureCentroids_H_ */
