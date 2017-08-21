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

#include "SIMPLib/CoreFilters/LinkFeatureMapToElementArray.h"
#include "SIMPLib/Utilities/UnitTestSupport.hpp"

#include "SIMPLTestFileLocations.h"


class LinkFeatureMapToElementArrayTest
{

public:
  LinkFeatureMapToElementArrayTest()
  {
  }

  virtual ~LinkFeatureMapToElementArrayTest()
  {
  }

  enum ErrorCodes
  {
    NO_ERROR = 0,
    ARRAY_TYPE_ERROR = -501,
    EXISTING_MATRIX_ERROR = -10014
  };

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestFilterAvailability()
  {
    // Now instantiate the CreateDataArray Filter from the FilterManager
    QString filtName = "CopyFeatureArrayToElementArray";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
    if(nullptr == filterFactory.get())
    {
      std::stringstream ss;
      ss << "The LinkFeatureMapToElementArrayTest Requires the use of the " << filtName.toStdString() << " filter which is found in the SIMPLib Plugin";
      DREAM3D_TEST_THROW_EXCEPTION(ss.str())
    }

    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  DataContainerArray::Pointer CreateDataContainerArray()
  {
    DataContainerArray::Pointer dca = DataContainerArray::New();
    DataContainer::Pointer dc = DataContainer::New("DataContainer");
    AttributeMatrix::Pointer am = AttributeMatrix::New(QVector<size_t>(1, 1), "AttributeMatrix", AttributeMatrix::Type::Vertex);
    DataArray<int32_t>::Pointer da1 = DataArray<int32_t>::CreateArray(1, "CellArray");
    IDataArray::Pointer da2 = DataArray<size_t>::CreateArray(1, "DataArray2");

    am->addAttributeArray("CellArray", da1);
    am->addAttributeArray("DataArray2", da2);

    dc->addAttributeMatrix("AttributeMatrix", am);

    dca->addDataContainer(dc);

    // Set the value of CellArray
    da1->setComponent(0, 0, 1);

    return dca;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  AbstractFilter::Pointer CreateFilter(QString cellFeatureAttributeMatrixName, DataArrayPath selectedCellArrayPath, QString activeArrayName)
  {
    // Now instantiate the CreateDataArray Filter from the FilterManager
    QString filtName = "LinkFeatureMapToElementArray";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
    if(nullptr != filterFactory.get())
    {
      // If we get this far, the Factory is good so creating the filter should not fail unless something has
      // horribly gone wrong in which case the system is going to come down quickly after this.
      AbstractFilter::Pointer filter = filterFactory->create();

      DataContainerArray::Pointer dca = CreateDataContainerArray();
      filter->setDataContainerArray(dca);

      QVariant var;
      int err = 0;
      bool propWasSet = false;

      var.setValue(cellFeatureAttributeMatrixName);
      propWasSet = filter->setProperty("CellFeatureAttributeMatrixName", var); // 1 component
      DREAM3D_REQUIRE_EQUAL(propWasSet, true)

      var.setValue(selectedCellArrayPath);
      propWasSet = filter->setProperty("SelectedCellArrayPath", var); // array path
      DREAM3D_REQUIRE_EQUAL(propWasSet, true)

      /* ==== Test The Maximum Value for the primitive type ===== */
      var.setValue(activeArrayName);
      propWasSet = filter->setProperty("ActiveArrayName", var); // initialize with
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);

      return filter;
    }
    else
    {
      QString ss = QObject::tr("LinkFeatureMapToElementArrayTest Error creating filter '%1'. Filter was not created/executed. Please notify the developers.").arg(filtName);
      DREAM3D_REQUIRE_EQUAL(0, 1)
    }

    return nullptr;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestDataCheck()
  {
    QString cellFeatureAttributeMatrixName = "FeatureMatrix";
    DataArrayPath selectedCellArrayPath = DataArrayPath(SIMPL::Defaults::DataContainerName, SIMPL::Defaults::AttributeMatrixName, "CellArray");
    QString activeArrayName = "Active";

    AbstractFilter::Pointer filter = CreateFilter(cellFeatureAttributeMatrixName, selectedCellArrayPath, activeArrayName);
    int err;

    // Valid Input
    filter->preflight();
    err = filter->getErrorCondition();
    DREAM3D_REQUIRE_EQUAL(err, NO_ERROR);

    // AttributeMatrix already exists
    filter->preflight();
    err = filter->getErrorCondition();
    DREAM3D_REQUIRE_EQUAL(err, EXISTING_MATRIX_ERROR);

    // Invalid DataArray type
    selectedCellArrayPath.setDataArrayName("DataArray2");
    filter = CreateFilter(cellFeatureAttributeMatrixName, selectedCellArrayPath, activeArrayName);
    filter->preflight();
    err = filter->getErrorCondition();
    DREAM3D_REQUIRE_EQUAL(err, ARRAY_TYPE_ERROR);
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestExecute()
  {
    QString cellFeatureAttributeMatrixName = "FeatureMatrix";
    DataArrayPath selectedCellArrayPath = DataArrayPath(SIMPL::Defaults::DataContainerName, SIMPL::Defaults::AttributeMatrixName, "CellArray");
    QString activeArrayName = "Active";

    AbstractFilter::Pointer filter = CreateFilter(cellFeatureAttributeMatrixName, selectedCellArrayPath, activeArrayName);
    int err;

    // Valid Input
    filter->execute();
    err = filter->getErrorCondition();
    DREAM3D_REQUIRE_EQUAL(err, NO_ERROR);

    DataArrayPath featureMatrixPath = DataArrayPath(selectedCellArrayPath.getDataContainerName(), cellFeatureAttributeMatrixName, "");
    QString daName = "CellArray";

    DataContainerArray::Pointer dca = filter->getDataContainerArray();
    AttributeMatrix::Pointer am = dca->getAttributeMatrix(selectedCellArrayPath);
    IDataArray::Pointer cellArray = am->getAttributeArray(daName);
    AttributeMatrix::Pointer featureMatrix = dca->getAttributeMatrix(featureMatrixPath);
    DataArray<bool>::Pointer activeArray = featureMatrix->getAttributeArrayAs<DataArray<bool>>(activeArrayName);

    int maxValue = 0;
    int32_t* cellPtr = std::static_pointer_cast<DataArray<int32_t>>(cellArray)->getPointer(0);
    int count = cellArray->getSize();
    for(int i = 0; i < count; i++)
    {
      if(cellPtr[i] > maxValue)
      {
        maxValue = cellPtr[i];
      }
    }

    // Check number of tuples.  +1 to correct zero-based array index
    DREAM3D_REQUIRE_EQUAL(activeArray->getNumberOfTuples(), maxValue + 1);

    // Check Active values
    bool* active = activeArray->getPointer(0);
    DREAM3D_REQUIRE_EQUAL(active[0], false);
    DREAM3D_REQUIRE_EQUAL(active[1], true);

  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    std::cout << "#### LinkFeatureMapToElementArrayTest Starting ####" << std::endl;

    int err = EXIT_SUCCESS;
    DREAM3D_REGISTER_TEST(TestFilterAvailability());
    DREAM3D_REGISTER_TEST(TestDataCheck());
    DREAM3D_REGISTER_TEST(TestExecute());
  }

private:
  LinkFeatureMapToElementArrayTest(const LinkFeatureMapToElementArrayTest&); // Copy Constructor Not Implemented
  void operator=(const LinkFeatureMapToElementArrayTest&); // Operator '=' Not Implemented
};
