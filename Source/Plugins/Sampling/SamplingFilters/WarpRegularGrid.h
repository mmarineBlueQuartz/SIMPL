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

#ifndef _WarpRegularGrid_H_
#define _WarpRegularGrid_H_

#include <QtCore/QString>

#include "DREAM3DLib/DREAM3DLib.h"
#include "DREAM3DLib/Common/DREAM3DSetGetMacros.h"
#include "DREAM3DLib/DataArrays/IDataArray.h"

#include "DREAM3DLib/Common/AbstractFilter.h"
#include "DREAM3DLib/DataContainers/VolumeDataContainer.h"

#include "Sampling/SamplingConstants.h"
/**
 * @class WarpRegularGrid WarpRegularGrid.h DREAM3DLib/SamplingFilters/WarpRegularGrid.h
 * @brief
 * @author
 * @date Jun 10, 2014
 * @version 1.0
 */
class WarpRegularGrid : public AbstractFilter
{
    Q_OBJECT /* Need this for Qt's signals and slots mechanism to work */
  public:
    DREAM3D_SHARED_POINTERS(WarpRegularGrid)
    DREAM3D_STATIC_NEW_MACRO(WarpRegularGrid)
    DREAM3D_TYPE_MACRO_SUPER(WarpRegularGrid, AbstractFilter)

    virtual ~WarpRegularGrid();
    DREAM3D_FILTER_PARAMETER(QString, NewDataContainerName)
    Q_PROPERTY(QString NewDataContainerName READ getNewDataContainerName WRITE setNewDataContainerName)
    DREAM3D_FILTER_PARAMETER(DataArrayPath, CellAttributeMatrixPath)
    Q_PROPERTY(DataArrayPath CellAttributeMatrixPath READ getCellAttributeMatrixPath WRITE setCellAttributeMatrixPath)

    DREAM3D_FILTER_PARAMETER(Float3x3_t, Coefficients)
    Q_PROPERTY(Float3x3_t Coefficients READ getCoefficients WRITE setCoefficients)
    DREAM3D_FILTER_PARAMETER(FloatVec3_t, Resolution)
    Q_PROPERTY(FloatVec3_t Resolution READ getResolution WRITE setResolution)
    DREAM3D_FILTER_PARAMETER(bool, SaveAsNewDataContainer)
    Q_PROPERTY(bool SaveAsNewDataContainer READ getSaveAsNewDataContainer WRITE setSaveAsNewDataContainer)

    virtual const QString getCompiledLibraryName();
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters);
    virtual const QString getGroupName();
    virtual const QString getSubGroupName()  { return DREAM3D::FilterSubGroups::ResolutionFilters; }
    virtual const QString getHumanLabel();

    virtual void setupFilterParameters();
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

    /**
     * @brief Reimplemented from @see AbstractFilter class
     */
    virtual void execute();
    virtual void preflight();

  signals:
    void updateFilterParameters(AbstractFilter* filter);
    void parametersChanged();
    void preflightAboutToExecute();
    void preflightExecuted();

  protected:
    WarpRegularGrid();

    void determine_warped_points(FloatArrayType::Pointer coordsPtr);

  private:

    void dataCheck();

    float m_NewMinX, m_NewMinY, m_NewMinZ, m_NewMaxX, m_NewMaxY, m_NewMaxZ;

    WarpRegularGrid(const WarpRegularGrid&); // Copy Constructor Not Implemented
    void operator=(const WarpRegularGrid&); // Operator '=' Not Implemented
};

#endif /* WarpRegularGrid_H_ */





