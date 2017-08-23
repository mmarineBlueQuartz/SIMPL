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

#include "SIMPLib/CoreFilters/WriteTriangleGeometry.h"
#include "SIMPLib/Utilities/UnitTestSupport.hpp"

#include "SIMPLTestFileLocations.h"


class WriteTriangleGeometryTest
{

public:
  WriteTriangleGeometryTest()
  {
  }

  virtual ~WriteTriangleGeometryTest()
  {
  }

  enum ErrorCodes
  {
    NO_ERROR = 0,
    WRONG_GEOMETRY_ERROR = -384,
    NO_GEOM_ERROR = -385,
    NO_TRIANGLE_ERROR = -387,
    NO_NODE_ERROR = -386
  };

  const QString OUTPUT_NODE_PATH = UnitTest::TestTempDir + "/WriteTriangleGeom/OutputNodes.txt";
  const QString OUTPUT_TRIANGLE_PATH = UnitTest::TestTempDir + "/WriteTriangleGeom/OutputTriangles.txt";

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestFilterAvailability()
  {
    // Now instantiate the CreateDataArray Filter from the FilterManager
    QString filtName = "WriteTriangleGeometry";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
    if(nullptr == filterFactory.get())
    {
      std::stringstream ss;
      ss << "The WriteTriangleGeometryTest Requires the use of the " << filtName.toStdString() << " filter which is found in the SIMPLib Plugin";
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
    DataContainer::Pointer dc2 = DataContainer::New("NoGeomContainer");
    DataContainer::Pointer dcTri = DataContainer::New("NoNodeContainer");
    DataContainer::Pointer dcNode = DataContainer::New("NoTriangleContainer");
    DataContainer::Pointer dcImage = DataContainer::New("ImageContainer");
    AttributeMatrix::Pointer am = AttributeMatrix::New(QVector<size_t>(1, 1), "AttributeMatrix", AttributeMatrix::Type::Vertex);

    QVector<size_t> cDims(1, 3);
    DataArray<float>::Pointer vertArray = DataArray<float>::CreateArray(1, cDims, "VertArray");
    DataArray<int64_t>::Pointer triArray = DataArray<int64_t>::CreateArray(1, cDims, "TriArray");

    vertArray->initializeWithZeros();
    for(int i = 0; i < 3; i++)
    {
      triArray->setValue(i, i);
    }

    am->addAttributeArray("VertArray", vertArray);
    am->addAttributeArray("TriArray", triArray);

    dc->addAttributeMatrix("AttributeMatrix", am);

    dca->addDataContainer(dc);
    dca->addDataContainer(dc2);
    dca->addDataContainer(dcTri);
    dca->addDataContainer(dcNode);
    dca->addDataContainer(dcImage);

    // Geometry
    TriangleGeom::Pointer triGeom = TriangleGeom::New();
    triGeom->setVertices(vertArray);
    triGeom->setTriangles(triArray);
    dc->setGeometry(triGeom);

    TriangleGeom::Pointer noTriGeom = TriangleGeom::New();
    noTriGeom->setVertices(vertArray);
    noTriGeom->setTriangles(nullptr);
    dcNode->setGeometry(noTriGeom);

    TriangleGeom::Pointer noVertGeom = TriangleGeom::New();
    noVertGeom->setVertices(nullptr);
    noVertGeom->setTriangles(triArray);
    dcTri->setGeometry(noVertGeom);

    ImageGeom::Pointer imageGeom = ImageGeom::New();
    imageGeom->setDimensions(1, 1, 1);
    imageGeom->setResolution(1.0f, 1.0f, 1.0f);
    dcImage->setGeometry(imageGeom);

    return dca;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  AbstractFilter::Pointer CreateFilter(QString dataContainerSelection, QString outputNodesFile, QString outputTrianglesFile)
  {
    // Now instantiate the CreateDataArray Filter from the FilterManager
    QString filtName = "WriteTriangleGeometry";
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

      var.setValue(dataContainerSelection);
      propWasSet = filter->setProperty("DataContainerSelection", var); // 1 component
      DREAM3D_REQUIRE_EQUAL(propWasSet, true)

        var.setValue(outputNodesFile);
      propWasSet = filter->setProperty("OutputNodesFile", var); // array path
      DREAM3D_REQUIRE_EQUAL(propWasSet, true)

        /* ==== Test The Maximum Value for the primitive type ===== */
        var.setValue(outputTrianglesFile);
      propWasSet = filter->setProperty("OutputTrianglesFile", var); // initialize with
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);

      return filter;
    }
    else
    {
      QString ss = QObject::tr("WriteTriangleGeometryTest Error creating filter '%1'. Filter was not created/executed. Please notify the developers.").arg(filtName);
      DREAM3D_REQUIRE_EQUAL(0, 1)
    }

    return nullptr;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestHasTriangleGeom()
  {
    int err;
    AbstractFilter::Pointer filter = CreateFilter("DataContainer", OUTPUT_NODE_PATH, OUTPUT_TRIANGLE_PATH);
    
    // Valid Input
    filter->preflight();
    err = filter->getErrorCondition();
    DREAM3D_REQUIRE_EQUAL(err, NO_ERROR);

    // Missing triangle geometry
    filter = CreateFilter("NoGeomContainer", OUTPUT_NODE_PATH, OUTPUT_TRIANGLE_PATH);
    filter->preflight();
    err = filter->getErrorCondition();
    DREAM3D_REQUIRE_EQUAL(err, NO_GEOM_ERROR);

    // Incorrect geometry type
    filter = CreateFilter("ImageContainer", OUTPUT_NODE_PATH, OUTPUT_TRIANGLE_PATH);
    filter->preflight();
    err = filter->getErrorCondition();
    DREAM3D_REQUIRE_EQUAL(err, WRONG_GEOMETRY_ERROR);

    // Missing triangles
    filter = CreateFilter("NoTriangleContainer", OUTPUT_NODE_PATH, OUTPUT_TRIANGLE_PATH);
    filter->preflight();
    err = filter->getErrorCondition();
    DREAM3D_REQUIRE_EQUAL(err, NO_TRIANGLE_ERROR);

    // Missing triangle geometry
    filter = CreateFilter("NoNodeContainer", OUTPUT_NODE_PATH, OUTPUT_TRIANGLE_PATH);
    filter->preflight();
    err = filter->getErrorCondition();
    DREAM3D_REQUIRE_EQUAL(err, NO_NODE_ERROR);
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestWriteGeom()
  {
    int err;
    AbstractFilter::Pointer filter = CreateFilter("DataContainer", OUTPUT_NODE_PATH, OUTPUT_TRIANGLE_PATH);

    // Valid Input
    filter->execute();
    err = filter->getErrorCondition();
    DREAM3D_REQUIRE_EQUAL(err, NO_ERROR);
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestNodeOutput()
  {
    QFile file(OUTPUT_NODE_PATH);
    DREAM3D_REQUIRE(file.exists());

    if(!file.open(QIODevice::ReadOnly))
    {
      DREAM3D_REQUIRE_EQUAL(0, -1);
    }

    QTextStream in(&file);

    QString line = in.readLine();
    DREAM3D_REQUIRE_EQUAL(line, "# All lines starting with '#' are comments");
    line = in.readLine();
    DREAM3D_REQUIRE_EQUAL(line, "# DREAM.3D Nodes file");
    line = in.readLine(); // Skip DREAM.3D version
    line = in.readLine();
    DREAM3D_REQUIRE_EQUAL(line, "# Node Data is X Y Z space delimited.");
    line = in.readLine();
    DREAM3D_REQUIRE_EQUAL(line, "Node Count: 1");
    line = in.readLine();
    DREAM3D_REQUIRE_EQUAL(line, " 0.00000  0.00000  0.00000");
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestTriangleOutput()
  {
    QFile file(OUTPUT_TRIANGLE_PATH);
    DREAM3D_REQUIRE(file.exists());

    if(!file.open(QIODevice::ReadOnly))
    {
      DREAM3D_REQUIRE_EQUAL(0, -1);
    }

    QTextStream in(&file);

    QString line = in.readLine();
    DREAM3D_REQUIRE_EQUAL(line, "# All lines starting with '#' are comments");
    line = in.readLine();
    DREAM3D_REQUIRE_EQUAL(line, "# DREAM.3D Triangle file");
    line = in.readLine(); // Skip DREAM.3D version
    line = in.readLine();
    DREAM3D_REQUIRE_EQUAL(line, "# Each Triangle consists of 3 Node Ids.");
    line = in.readLine();
    DREAM3D_REQUIRE_EQUAL(line, "# NODE IDs START AT 0.");
    line = in.readLine();
    DREAM3D_REQUIRE_EQUAL(line, "Geometry Type: TriangleGeometry");
    line = in.readLine();
    DREAM3D_REQUIRE_EQUAL(line, "Node Count: 1");
    line = in.readLine();
    DREAM3D_REQUIRE_EQUAL(line, "Max Node Id: 0");
    line = in.readLine();
    DREAM3D_REQUIRE_EQUAL(line, "Triangle Count: 1");
    line = in.readLine();
    DREAM3D_REQUIRE_EQUAL(line, "0 1 2");
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    std::cout << "#### WriteTriangleGeometryTest Starting ####" << std::endl;

    int err = EXIT_SUCCESS;
    DREAM3D_REGISTER_TEST(TestFilterAvailability());
    DREAM3D_REGISTER_TEST(TestHasTriangleGeom());
    DREAM3D_REGISTER_TEST(TestWriteGeom());
    DREAM3D_REGISTER_TEST(TestNodeOutput());
    DREAM3D_REGISTER_TEST(TestTriangleOutput());
  }

private:
  WriteTriangleGeometryTest(const WriteTriangleGeometryTest&); // Copy Constructor Not Implemented
  void operator=(const WriteTriangleGeometryTest&); // Operator '=' Not Implemented
};
