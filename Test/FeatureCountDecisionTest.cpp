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

#include "SIMPLib/CoreFilters/FeatureCountDecision.h"
#include "SIMPLib/Geometry/VertexGeom.h"
#include "SIMPLib/Utilities/UnitTestSupport.hpp"

#include "SIMPLTestFileLocations.h"

class FeatureCountDecisionTest
{
public:
  FeatureCountDecisionTest()
  {
  }

  virtual ~FeatureCountDecisionTest()
  {
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  DataContainerArray::Pointer createDataContainerArray()
  {
    DataContainerArray::Pointer dca = DataContainerArray::New();
    DataContainer::Pointer dc1 = DataContainer::New("DataContainer1");
    DataContainer::Pointer dc2 = DataContainer::New("DataContainer2");
    DataContainer::Pointer dc3 = DataContainer::New("DataContainer3");
    AttributeMatrix::Pointer am1 = AttributeMatrix::New(QVector<size_t>(1, 1), "AttributeMatrix1", AttributeMatrix::Type::CellEnsemble);
    AttributeMatrix::Pointer am2 = AttributeMatrix::New(QVector<size_t>(1, 1), "AttributeMatrix2", AttributeMatrix::Type::Vertex);
    AttributeMatrix::Pointer am3 = AttributeMatrix::New(QVector<size_t>(1, 1), "AttributeMatrix3", AttributeMatrix::Type::CellEnsemble);
    AttributeMatrix::Pointer am4 = AttributeMatrix::New(QVector<size_t>(1, 1), "AttributeMatrix4", AttributeMatrix::Type::CellEnsemble);
    IDataArray::Pointer da1 = DataArray<int32_t>::CreateArray(1, "DataArray1");
    IDataArray::Pointer da2 = DataArray<size_t>::CreateArray(1, "DataArray2");
    IDataArray::Pointer da3 = DataArray<int32_t>::CreateArray(1, "DataArray3");
    IDataArray::Pointer da4 = DataArray<int32_t>::CreateArray(1, "DataArray4");
    IDataArray::Pointer da5 = DataArray<int32_t>::CreateArray(1, "DataArray5");
    
    am1->addAttributeArray("DataArray1", da1);
    am1->addAttributeArray("DataArray2", da2);
    am2->addAttributeArray("DataArray3", da3);
    am3->addAttributeArray("DataArray4", da4);
    am4->addAttributeArray("DataArray5", da5);

    dc1->addAttributeMatrix("AttributeMatrix1", am1);
    dc1->addAttributeMatrix("AttributeMatrix2", am2);
    dc2->addAttributeMatrix("AttributeMatrix3", am3);
    dc3->addAttributeMatrix("AttributeMatrix4", am4);

    // Image Geometry
    ImageGeom::Pointer imageGeom = ImageGeom::New();
    size_t dims[3] = { 1,1,1 };
    imageGeom->setDimensions(dims);
    float res[3] = { 1.0f, 1.0f, 1.0f };
    imageGeom->setResolution(res);
    float origin[3] = { 0.0f, 0.0f, 0.0f };
    imageGeom->setOrigin(origin);

    // Vertex Geometry
    VertexGeom::Pointer vertexGeom = VertexGeom::New();

    dc1->setGeometry(imageGeom);
    dc3->setGeometry(vertexGeom);

    dca->addDataContainer(dc1);
    dca->addDataContainer(dc2);
    dca->addDataContainer(dc3);

    return dca;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestDataType()
  {
    DataContainerArray::Pointer dca = createDataContainerArray();
    FeatureCountDecision::Pointer featureCountPtr = FeatureCountDecision::New();
    featureCountPtr->setDataContainerArray(dca);
    DataArrayPath featureIdsPath;
    
    // Empty path
    featureIdsPath = DataArrayPath("", "", "");
    featureCountPtr->setFeatureIdsArrayPath(featureIdsPath);
    featureCountPtr->setMaxGrains(1);
    featureCountPtr->execute();
    DREAM3D_REQUIRE_EQUAL(featureCountPtr->getErrorCondition(), -80000)

    // Invalid Matrix Type
    featureIdsPath = DataArrayPath("DataContainer1", "AttributeMatrix2", "DataArray3");
    featureCountPtr->setFeatureIdsArrayPath(featureIdsPath);
    featureCountPtr->setMaxGrains(1);
    featureCountPtr->execute();
    DREAM3D_REQUIRE(featureCountPtr->getErrorCondition() != 0)

    // Invalid Array Type
    featureIdsPath = DataArrayPath("DataContainer1", "AttributeMatrix1", "DataArray2");
    featureCountPtr->setFeatureIdsArrayPath(featureIdsPath);
    featureCountPtr->setMaxGrains(1);
    featureCountPtr->execute();
    DREAM3D_REQUIRE_EQUAL(featureCountPtr->getErrorCondition(), -501)

    // Valid Input
    featureIdsPath = DataArrayPath("DataContainer1", "AttributeMatrix1", "DataArray1");
    featureCountPtr->setFeatureIdsArrayPath(featureIdsPath);
    featureCountPtr->setMaxGrains(1);
    featureCountPtr->execute();
    DREAM3D_REQUIRE_EQUAL(featureCountPtr->getErrorCondition(), 0)
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestGeometry()
  {
    DataContainerArray::Pointer dca = createDataContainerArray();
    FeatureCountDecision::Pointer featureCountPtr = FeatureCountDecision::New();
    featureCountPtr->setDataContainerArray(dca);
    DataArrayPath featureIdsPath;

    // Missing Geometry
    featureIdsPath = DataArrayPath("DataContainer2", "AttributeMatrix3", "DataArray4");
    featureCountPtr->setFeatureIdsArrayPath(featureIdsPath);
    featureCountPtr->setMaxGrains(1);
    featureCountPtr->execute();
    DREAM3D_REQUIRE_EQUAL(featureCountPtr->getErrorCondition(), -385)

    // Invalid Geometry
    featureIdsPath = DataArrayPath("DataContainer3", "AttributeMatrix4", "DataArray5");
    featureCountPtr->setFeatureIdsArrayPath(featureIdsPath);
    featureCountPtr->setMaxGrains(1);
    featureCountPtr->execute();
    DREAM3D_REQUIRE_EQUAL(featureCountPtr->getErrorCondition(), -384)

    // Valid Geometry
    featureIdsPath = DataArrayPath("DataContainer1", "AttributeMatrix1", "DataArray1");
    featureCountPtr->setFeatureIdsArrayPath(featureIdsPath);
    featureCountPtr->setMaxGrains(1);
    featureCountPtr->execute();
    DREAM3D_REQUIRE_EQUAL(featureCountPtr->getErrorCondition(), 0)
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestDecision()
  {
    DREAM3D_REQUIRE(false)
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestTargetValue()
  {
    DREAM3D_REQUIRE(false)
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    std::cout << "#### FeatureCountDecisionTest Starting ####" << std::endl;

    int err = EXIT_SUCCESS;
    DREAM3D_REGISTER_TEST(TestDataType())
    DREAM3D_REGISTER_TEST(TestGeometry())
    DREAM3D_REGISTER_TEST(TestDecision())
    DREAM3D_REGISTER_TEST(TestTargetValue())
  }

private:
  FeatureCountDecisionTest(const FeatureCountDecisionTest&); // Copy Constructor Not Implemented
  void operator=(const FeatureCountDecisionTest&); // Operator '=' Not Implemented
};
