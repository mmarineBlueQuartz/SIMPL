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

#include "SIMPLib/CoreFilters/FeatureDataCSVWriter.h"
#include "SIMPLib/Utilities/UnitTestSupport.hpp"

#include "SIMPLTestFileLocations.h"


class FeatureDataCSVWriterTest
{

public:
  FeatureDataCSVWriterTest()
  {
  }

  virtual ~FeatureDataCSVWriterTest()
  {
  }

  enum ErrorCodes
  {
    NO_ERROR = 0
  };

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  QString getOutputPath()
  {
    return UnitTest::TestTempDir + "FeatureDataCSVWriter / FeatureData.csv";
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestFilterAvailability()
  {
    // Now instantiate the CreateDataArray Filter from the FilterManager
    QString filtName = "FeatureDataCSVWriter";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
    if(nullptr == filterFactory.get())
    {
      std::stringstream ss;
      ss << "The FeatureDataCSVWriterTest Requires the use of the " << filtName.toStdString() << " filter which is found in the SIMPLib Plugin";
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
    AttributeMatrix::Pointer am = AttributeMatrix::New(QVector<size_t>(1, 2), "FeatureMatrix", AttributeMatrix::Type::CellFeature);

    QVector<size_t> cDims(1, 3);
    DataArray<float>::Pointer da1 = DataArray<float>::CreateArray(2, cDims, "Feature1");
    DataArray<int32_t>::Pointer da2 = DataArray<int32_t>::CreateArray(2, cDims, "Feature2");

    da1->initializeWithZeros();
    for(int i = 0; i < 2; i++)
    {
      for(int j = 0; j < 3; j++)
      {
        da2->setComponent(i, j, i + j);
      }
    }

    am->addAttributeArray("Feature1", da1);
    am->addAttributeArray("Feature2", da2);

    dc->addAttributeMatrix("FeatureMatrix", am);

    dca->addDataContainer(dc);

    return dca;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  AbstractFilter::Pointer CreateFilter(DataArrayPath cellFeatureAttributeMatrixPath, QString featureDataFile, bool writeNeighborListData, int delimiterChoice, bool writeNumFeaturesLine)
  {
    // Now instantiate the CreateDataArray Filter from the FilterManager
    QString filtName = "FeatureDataCSVWriter";
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

      var.setValue(cellFeatureAttributeMatrixPath);
      propWasSet = filter->setProperty("CellFeatureAttributeMatrixPath", var); // 1 component
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);

      var.setValue(featureDataFile);
      propWasSet = filter->setProperty("FeatureDataFile", var); // array path
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);

      var.setValue(writeNeighborListData);
      propWasSet = filter->setProperty("WriteNeighborListData", var); // initialize with
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);

      var.setValue(delimiterChoice);
      propWasSet = filter->setProperty("DelimiterChoice", var); // initialize with
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);

      var.setValue(writeNumFeaturesLine);
      propWasSet = filter->setProperty("WriteNumFeaturesLine", var); // initialize with
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);

      return filter;
    }
    else
    {
      QString ss = QObject::tr("FeatureDataCSVWriterTest Error creating filter '%1'. Filter was not created/executed. Please notify the developers.").arg(filtName);
      DREAM3D_REQUIRE_EQUAL(0, 1)
    }

    return nullptr;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestWriteCSVFile()
  {
    DataArrayPath featureMatrixPath("DataContainer", "FeatureMatrix", "");
    bool writeNeighborListData = true;
    int delimiterChoice = 0;
    bool writeNumFeaturesLine = true;
    AbstractFilter::Pointer filter = CreateFilter(featureMatrixPath, getOutputPath(), writeNeighborListData, delimiterChoice, writeNumFeaturesLine);

    // Valid Input
    filter->execute();
    int err = filter->getErrorCondition();
    DREAM3D_REQUIRE_EQUAL(err, NO_ERROR);
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestFileOutput()
  {
    // DataContainer
    DataContainerArray::Pointer dca = CreateDataContainerArray();
    AttributeMatrix::Pointer am = dca->getAttributeMatrix(DataArrayPath("DataContainer", "FeatureMatrix", ""));
    DataArray<float>::Pointer feature1 = am->getAttributeArrayAs<DataArray<float>>("Feature1");
    DataArray<int32_t>::Pointer feature2 = am->getAttributeArrayAs<DataArray<int32_t>>("Feature2");

    // File
    QFile file(getOutputPath());
    DREAM3D_REQUIRE(file.exists());

    if(!file.open(QIODevice::ReadOnly))
    {
      DREAM3D_REQUIRE_EQUAL(0, -1);
    }

    QTextStream in(&file);

    QString line = in.readLine();
    int numTuples = line.toInt();
    int numValues = in.readLine().split(",").count();
    for(int i = 0; i < numTuples; i++)
    {
      line = in.readLine();
      QStringList values = line.split(",");
      DREAM3D_REQUIRE_EQUAL(values.count(), numValues);

      // Item number
      DREAM3D_REQUIRE_EQUAL(values[0].toInt(), i + 1);

      // Feature1
      DREAM3D_REQUIRE_EQUAL(values[1].toFloat(), feature1->getComponent(i+1, 0));
      DREAM3D_REQUIRE_EQUAL(values[2].toFloat(), feature1->getComponent(i+1, 1));
      DREAM3D_REQUIRE_EQUAL(values[3].toFloat(), feature1->getComponent(i+1, 2));

      // Feature2
      DREAM3D_REQUIRE_EQUAL(values[4].toInt(), feature2->getComponent(i+1, 0));
      DREAM3D_REQUIRE_EQUAL(values[5].toInt(), feature2->getComponent(i+1, 1));
      DREAM3D_REQUIRE_EQUAL(values[6].toInt(), feature2->getComponent(i+1, 2));
    }
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    std::cout << "#### FeatureDataCSVWriterTest Starting ####" << std::endl;

    int err = EXIT_SUCCESS;
    DREAM3D_REGISTER_TEST(TestFilterAvailability());
    DREAM3D_REGISTER_TEST(TestWriteCSVFile());
    DREAM3D_REGISTER_TEST(TestFileOutput());
  }

private:
  FeatureDataCSVWriterTest(const FeatureDataCSVWriterTest&); // Copy Constructor Not Implemented
  void operator=(const FeatureDataCSVWriterTest&); // Operator '=' Not Implemented
};
