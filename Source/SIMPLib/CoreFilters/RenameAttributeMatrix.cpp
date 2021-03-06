/* ============================================================================
* Copyright (c) 2009-2016 BlueQuartz Software, LLC
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
* Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
* contributors may be used to endorse or promote products derived from this software
* without specific prior written permission.
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
* The code contained herein was partially funded by the followig contracts:
*    United States Air Force Prime Contract FA8650-07-D-5800
*    United States Air Force Prime Contract FA8650-10-D-5210
*    United States Prime Contract Navy N00173-07-C-2068
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "RenameAttributeMatrix.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AttributeMatrixSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/SIMPLibVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
RenameAttributeMatrix::RenameAttributeMatrix()
: m_SelectedAttributeMatrixPath("", "", "")
, m_NewAttributeMatrix("")
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
RenameAttributeMatrix::~RenameAttributeMatrix() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RenameAttributeMatrix::setupFilterParameters()
{
  FilterParameterVector parameters;

  {
    AttributeMatrixSelectionFilterParameter::RequirementType req;
    parameters.push_back(SIMPL_NEW_AM_SELECTION_FP("Attribute Matrix to Rename", SelectedAttributeMatrixPath, FilterParameter::RequiredArray, RenameAttributeMatrix, req));
  }

  parameters.push_back(SIMPL_NEW_STRING_FP("New Attribute Matrix Name", NewAttributeMatrix, FilterParameter::Parameter, RenameAttributeMatrix));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RenameAttributeMatrix::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedAttributeMatrixPath(reader->readDataArrayPath("SelectedAttributeMatrixPath", getSelectedAttributeMatrixPath()));
  setNewAttributeMatrix(reader->readString("NewAttributeMatrix", getNewAttributeMatrix()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RenameAttributeMatrix::initialize()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RenameAttributeMatrix::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);

  if(m_NewAttributeMatrix.isEmpty())
  {
    setErrorCondition(-11004);
    QString ss = QObject::tr("The new Attribute Matrix name must be set");
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  QString amName = getSelectedAttributeMatrixPath().getAttributeMatrixName();

  DataContainer::Pointer dc = getDataContainerArray()->getPrereqDataContainer(this, getSelectedAttributeMatrixPath().getDataContainerName());
  getDataContainerArray()->getPrereqAttributeMatrixFromPath<AbstractFilter>(this, getSelectedAttributeMatrixPath(), -301);

  if(getErrorCondition() < 0)
  {
    return;
  }

  bool check = dc->renameAttributeMatrix(amName, getNewAttributeMatrix());
  if(!check)
  {
    setErrorCondition(-11006);
    QString ss = QObject::tr("Attempt to rename Attribute Matrix '%1' to '%2' failed").arg(amName).arg(getNewAttributeMatrix());
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RenameAttributeMatrix::preflight()
{
  setInPreflight(true);
  emit preflightAboutToExecute();
  emit updateFilterParameters(this);
  dataCheck();
  emit preflightExecuted();
  setInPreflight(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RenameAttributeMatrix::execute()
{
  setErrorCondition(0);
  setWarningCondition(0);
  dataCheck(); // calling the dataCheck will rename the array, so nothing is required here
  if(getErrorCondition() < 0)
  {
    return;
  }

}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer RenameAttributeMatrix::newFilterInstance(bool copyFilterParameters) const
{
  RenameAttributeMatrix::Pointer filter = RenameAttributeMatrix::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString RenameAttributeMatrix::getCompiledLibraryName() const
{
  return Core::CoreBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString RenameAttributeMatrix::getBrandingString() const
{
  return "SIMPLib Core Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString RenameAttributeMatrix::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << SIMPLib::Version::Major() << "." << SIMPLib::Version::Minor() << "." << SIMPLib::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString RenameAttributeMatrix::getGroupName() const
{
  return SIMPL::FilterGroups::CoreFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid RenameAttributeMatrix::getUuid()
{
  return QUuid("{ee29e6d6-1f59-551b-9350-a696523261d5}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString RenameAttributeMatrix::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::MemoryManagementFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString RenameAttributeMatrix::getHumanLabel() const
{
  return "Rename Attribute Matrix";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DataArrayPath::RenameContainer RenameAttributeMatrix::getRenamedPaths()
{
  DataArrayPath oldPath = getSelectedAttributeMatrixPath();
  DataArrayPath newPath = getSelectedAttributeMatrixPath();
  newPath.setAttributeMatrixName(getNewAttributeMatrix());

  DataArrayPath::RenameContainer container;
  container.push_back(DataArrayPath::RenameType(oldPath, newPath));

  return container;
}
