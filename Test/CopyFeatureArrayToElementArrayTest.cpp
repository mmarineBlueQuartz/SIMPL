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

#include <stdlib.h>

#include <iostream>

#include "SIMPLib/CoreFilters/CopyFeatureArrayToElementArray.h"
#include "SIMPLib/DataArrays/StringDataArray.hpp"
#include "SIMPLib/Utilities/UnitTestSupport.hpp"

#include "SIMPLTestFileLocations.h"

class CopyFeatureArrayToElementArrayTest
{
public:
  CopyFeatureArrayToElementArrayTest()
  {
  }
  virtual ~CopyFeatureArrayToElementArrayTest()
  {
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestFilterAvailability()
  {
    // Now instantiate the CreateDataArray Filter from the FilterManager
    QString filtName = "CopyFeatureArrayToElementArray";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
    if(nullptr == filterFactory.get())
    {
      std::stringstream ss;
      ss << "The CopyFeatureArrayToElementArrayTest Requires the use of the " << filtName.toStdString() << " filter which is found in the SIMPLib Plugin";
      DREAM3D_TEST_THROW_EXCEPTION(ss.str())
    }

    // Now instantiate the CreateDataContainer Filter from the FilterManager
    filtName = "CreateDataContainer";
    filterFactory = fm->getFactoryForFilter(filtName);
    if(nullptr == filterFactory.get())
    {
      std::stringstream ss;
      ss << "The CopyFeatureArrayToElementArrayTest Requires the use of the " << filtName.toStdString() << " filter which is found in the SIMPLib Plugin";
      DREAM3D_TEST_THROW_EXCEPTION(ss.str())
    }

    // Now instantiate the CreateDataContainer Filter from the FilterManager
    filtName = "CreateAttributeMatrix";
    fm = FilterManager::Instance();
    filterFactory = fm->getFactoryForFilter(filtName);
    if(nullptr == filterFactory.get())
    {
      std::stringstream ss;
      ss << "The CopyFeatureArrayToElementArrayTest Requires the use of the " << filtName.toStdString() << " filter which is found in the SIMPLib Plugin";
      DREAM3D_TEST_THROW_EXCEPTION(ss.str())
    }
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  DataContainerArray::Pointer CreateDataContainerArray(int32_t value = 0)
  {
    DataContainerArray::Pointer dca = DataContainerArray::New();
    DataContainer::Pointer dc = DataContainer::New(SIMPL::Defaults::DataContainerName);
    dca->addDataContainer(dc);

    AttributeMatrix::Pointer featMatrix = AttributeMatrix::New(QVector<size_t>(1, 1), SIMPL::Defaults::CellFeatureAttributeMatrixName, AttributeMatrix::Type::CellFeature);
    dc->addAttributeMatrix(SIMPL::Defaults::CellFeatureAttributeMatrixName, featMatrix);

    AttributeMatrix::Pointer elementMatrix = AttributeMatrix::New(QVector<size_t>(1, 1), SIMPL::Defaults::AttributeMatrixName, AttributeMatrix::Type::Cell);
    dc->addAttributeMatrix(SIMPL::Defaults::AttributeMatrixName, elementMatrix);

    DataArray<int32_t>::Pointer featureArray = DataArray<int32_t>::CreateArray(1, "FeatureArray");
    featureArray->setValue(0, value);
    featMatrix->addAttributeArray("FeatureArray", featureArray);

    DataArray<int32_t>::Pointer idArray = DataArray<int32_t>::CreateArray(1, "FeatureIds");
    idArray->setValue(0, value);
    elementMatrix->addAttributeArray("FeatureIds", idArray);

    return dca;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  DataContainerArray::Pointer CreateUnsupportedDataContainerArray(int32_t value = 0)
  {
    DataContainerArray::Pointer dca = DataContainerArray::New();
    DataContainer::Pointer dc = DataContainer::New(SIMPL::Defaults::DataContainerName);
    dca->addDataContainer(dc);

    AttributeMatrix::Pointer featMatrix = AttributeMatrix::New(QVector<size_t>(1, 1), SIMPL::Defaults::CellFeatureAttributeMatrixName, AttributeMatrix::Type::CellFeature);
    dc->addAttributeMatrix(SIMPL::Defaults::CellFeatureAttributeMatrixName, featMatrix);

    AttributeMatrix::Pointer elementMatrix = AttributeMatrix::New(QVector<size_t>(1, 1), SIMPL::Defaults::AttributeMatrixName, AttributeMatrix::Type::Cell);
    dc->addAttributeMatrix(SIMPL::Defaults::AttributeMatrixName, elementMatrix);

    StringDataArray::Pointer featureArray = StringDataArray::CreateArray(1, "FeatureArray");
    featureArray->setValue(0, "Feature");
    featMatrix->addAttributeArray("FeatureArray", featureArray);

    DataArray<int32_t>::Pointer idArray = DataArray<int32_t>::CreateArray(1, "FeatureIds");
    idArray->setValue(0, value);
    elementMatrix->addAttributeArray("FeatureIds", idArray);

    return dca;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  AbstractFilter::Pointer CreateFilter()
  {
    // Now instantiate the CreateDataArray Filter from the FilterManager
    QString filtName = "CopyFeatureArrayToElementArray";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
    if(nullptr != filterFactory.get())
    {
      // If we get this far, the Factory is good so creating the filter should not fail unless something has
      // horribly gone wrong in which case the system is going to come down quickly after this.
      AbstractFilter::Pointer filter = filterFactory->create();

      DataArrayPath path = DataArrayPath(SIMPL::Defaults::DataContainerName, SIMPL::Defaults::CellFeatureAttributeMatrixName, "FeatureArray");

      DataContainerArray::Pointer dca = CreateDataContainerArray();
      filter->setDataContainerArray(dca);

      QVariant var;
      int err = 0;
      bool propWasSet = false;

      var.setValue(path);
      propWasSet = filter->setProperty("SelectedFeatureArrayPath", var); // Copied array
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);

      path = DataArrayPath(SIMPL::Defaults::DataContainerName, SIMPL::Defaults::AttributeMatrixName, "FeatureIds");
      var.setValue(path);
      propWasSet = filter->setProperty("FeatureIdsArrayPath", var); // Feature IDs
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);

      /* ==== Test The Maximum Value for the primitive type ===== */
      QString arrayName = "CopiedArrayName";
      var.setValue(arrayName);
      propWasSet = filter->setProperty("CreatedArrayName", var); // initialize with
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);

      return filter;
    }
    else
    {
      QString ss = QObject::tr("CopyFeatureArrayToElementArrayTest Error creating filter '%1'. Filter was not created/executed. Please notify the developers.").arg(filtName);
      DREAM3D_REQUIRE_EQUAL(0, 1)
    }

    return nullptr;
  }



  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int ExecuteFilter(AbstractFilter::Pointer filter, DataArrayPath selectedFeaturePath, DataArrayPath featureIdsPath, QString arrayName)
  {
    QVariant var;
    bool propWasSet = false;

    var.setValue(selectedFeaturePath);
    propWasSet = filter->setProperty("SelectedFeatureArrayPath", var); // Copied array
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);

    var.setValue(featureIdsPath);
    propWasSet = filter->setProperty("FeatureIdsArrayPath", var); // Feature IDs
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);

    var.setValue(arrayName);
    propWasSet = filter->setProperty("CreatedArrayName", var); // initialize with
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);

    filter->execute();
    return filter->getErrorCondition();
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestCopyFeatureArrayToElementArray()
  {
    AbstractFilter::Pointer filter = CreateFilter();
    DataArrayPath selectedFeaturePath = DataArrayPath(SIMPL::Defaults::DataContainerName, SIMPL::Defaults::CellFeatureAttributeMatrixName, "FeatureArray");
    DataArrayPath featureIdPath = DataArrayPath(SIMPL::Defaults::DataContainerName, SIMPL::Defaults::AttributeMatrixName, "FeatureIds");

    int err;

    DataContainerArray::Pointer dca = CreateDataContainerArray();
    filter->setDataContainerArray(dca);
    err = ExecuteFilter(filter, selectedFeaturePath, featureIdPath, "ArrayCopy");
    DREAM3D_REQUIRE_EQUAL(err, 0);

    // Empty array name
    dca = CreateDataContainerArray();
    filter->setDataContainerArray(dca);
    err = ExecuteFilter(filter, selectedFeaturePath, featureIdPath, "");
    DREAM3D_REQUIRE_EQUAL(err, -11002);

    // Invalid AttributeMatrix type for FeatureIdsArrayPath
    dca = CreateDataContainerArray();
    filter->setDataContainerArray(dca);
    err = ExecuteFilter(filter, selectedFeaturePath, selectedFeaturePath, "ArrayCopy");
    DREAM3D_REQUIRE_EQUAL(err, -11003);

    // Invalid AttributeMatrix type for SelectedFeatureArrayPath
    dca = CreateDataContainerArray();
    filter->setDataContainerArray(dca);
    err = ExecuteFilter(filter, featureIdPath, featureIdPath, "ArrayCopy");
    DREAM3D_REQUIRE_EQUAL(err, -11004);

    // Feature ID mismatch
    dca = CreateDataContainerArray(2);
    filter->setDataContainerArray(dca);
    err = ExecuteFilter(filter, selectedFeaturePath, featureIdPath, "ArrayCopy");
    DREAM3D_REQUIRE_EQUAL(err, -5555);

    // Unsupported array type
    dca = CreateUnsupportedDataContainerArray();
    filter->setDataContainerArray(dca);
    err = ExecuteFilter(filter, selectedFeaturePath, featureIdPath, "ArrayCopy");
    DREAM3D_REQUIRE_EQUAL(err, -401);
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    std::cout << "#### CopyFeatureArrayToElementArrayTest Starting ####" << std::endl;

    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(TestFilterAvailability());

    DREAM3D_REGISTER_TEST(TestCopyFeatureArrayToElementArray())
  }

private:
  CopyFeatureArrayToElementArrayTest(const CopyFeatureArrayToElementArrayTest&); // Copy Constructor Not Implemented
  void operator=(const CopyFeatureArrayToElementArrayTest&);      // Operator '=' Not Implemented
};
