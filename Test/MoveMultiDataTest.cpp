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

#include "SIMPLib/CoreFilters/MoveData.h"
#include "SIMPLib/CoreFilters/MoveMultiData.h"
#include "SIMPLib/Utilities/UnitTestSupport.hpp"

#include "SIMPLTestFileLocations.h"

namespace
{
  static const int32_t k_MoveAttributeMatrix = 0;
  static const int32_t k_MoveMultiDataArray = 1;
}

class MoveMultiDataTest
{

public:
  MoveMultiDataTest()
  {
  }

  virtual ~MoveMultiDataTest()
  {
  }

  enum ErrorCodes
  {
    DC_SELECTED_NAME_EMPTY = -999,
    DC_NEW_NAME_EMPTY = -11001,
    DC_SELECTED_NOT_FOUND = -999,
    DCA_NOT_FOUND = -11003,
    AM_NEW_NAME_EMPTY = -11004,
    AM_SELECTED_PATH_EMPTY = -999,
    RENAME_ATTEMPT_FAILED = -999,
    DC_NOT_FOUND = -999,
    AM_NOT_FOUND = -307020,
    AA_NEW_NAME_EMPTY = -11009,
    AA_SELECTED_PATH_EMPTY = -999,
    DC_DEST_NOT_FOUND = -999,
    DC_SRC_NOT_FOUND = -999,
    AM_SRC_NOT_FOUND = -307020,
    AA_NOT_FOUND = -90002,
    AA_OLD_NAME_DOES_NOT_EXIST = -11016,
    AA_NEW_NAME_EXISTS = -11017,
    TUPLES_NOT_MATCH = -11019
  };

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  DataContainerArray::Pointer createDataContainerArray()
  {
    DataContainerArray::Pointer dca = DataContainerArray::New();
    DataContainer::Pointer dc1 = DataContainer::New("DataContainer1");
    DataContainer::Pointer dc2 = DataContainer::New("DataContainer2");
    DataContainer::Pointer dc3 = DataContainer::New("DataContainer3");
    AttributeMatrix::Pointer am1 = AttributeMatrix::New(QVector<size_t>(3, 2), "AttributeMatrix1", AttributeMatrix::Type::Vertex);
    AttributeMatrix::Pointer am2 = AttributeMatrix::New(QVector<size_t>(7, 2), "AttributeMatrix2", AttributeMatrix::Type::Vertex);
    AttributeMatrix::Pointer am3 = AttributeMatrix::New(QVector<size_t>(4, 3), "AttributeMatrix3", AttributeMatrix::Type::Vertex);
    AttributeMatrix::Pointer am4 = AttributeMatrix::New(QVector<size_t>(7, 2), "AttributeMatrix4", AttributeMatrix::Type::Vertex);
    AttributeMatrix::Pointer am5 = AttributeMatrix::New(QVector<size_t>(7, 2), "AttributeMatrix5", AttributeMatrix::Type::Vertex);
    IDataArray::Pointer da1 = DataArray<size_t>::CreateArray(8, "DataArray1");
    IDataArray::Pointer da2 = DataArray<size_t>::CreateArray(128, "DataArray2");
    IDataArray::Pointer da3 = DataArray<size_t>::CreateArray(128, "DataArray3");
    IDataArray::Pointer da4 = DataArray<size_t>::CreateArray(81, "DataArray4");
    IDataArray::Pointer da5 = DataArray<size_t>::CreateArray(81, "DataArray5");

    am1->addAttributeArray("DataArray1", da1);
    am2->addAttributeArray("DataArray2", da2);
    am2->addAttributeArray("DataArray3", da3);
    am3->addAttributeArray("DataArray4", da4);
    am3->addAttributeArray("DataArray5", da5);

    dc1->addAttributeMatrix("AttributeMatrix1", am1);
    dc1->addAttributeMatrix("AttributeMatrix2", am2);
    dc2->addAttributeMatrix("AttributeMatrix3", am3);
    dc2->addAttributeMatrix("AttributeMatrix4", am4);
    dc3->addAttributeMatrix("AttributeMatrix5", am5);

    dca->addDataContainer(dc1);
    dca->addDataContainer(dc2);
    dca->addDataContainer(dc3);

    return dca;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void setDeleteSelections(DataContainerArrayProxy& proxy)
  {
    proxy.setAllFlags(Qt::Unchecked);

    QMap<QString, DataContainerProxy>& dcProxies = proxy.dataContainers;

    for(QMap<QString, DataContainerProxy>::iterator dcIter = dcProxies.begin(); dcIter != dcProxies.end(); ++dcIter)
    {
      DataContainerProxy& dcProxy = dcIter.value();
      if(dcProxy.name == "DataContainer1")
      {
        dcProxy.flag = Qt::Checked;
      }

      QMap<QString, AttributeMatrixProxy>& amProxies = dcProxy.attributeMatricies;
      for(QMap<QString, AttributeMatrixProxy>::iterator amIter = amProxies.begin(); amIter != amProxies.end(); ++amIter)
      {
        AttributeMatrixProxy& amProxy = amIter.value();
        if(amProxy.name == "AttributeMatrix5")
        {
          amProxy.flag = Qt::Checked;
        }

        QMap<QString, DataArrayProxy>& daProxies = amProxy.dataArrays;
        for(QMap<QString, DataArrayProxy>::iterator daIter = daProxies.begin(); daIter != daProxies.end(); ++daIter)
        {
          DataArrayProxy& daProxy = daIter.value();
          if(daProxy.name == "DataArray4")
          {
            daProxy.flag = Qt::Checked;
          }
        }
      }
    }
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void MoveSingleAttributeMatrixTest()
  {
    DataContainerArray::Pointer dca = createDataContainerArray();

    MoveMultiData::Pointer moveDataPtr = MoveMultiData::New();
    moveDataPtr->setDataContainerArray(dca);
    moveDataPtr->setWhatToMove(::k_MoveAttributeMatrix);

    QVector<DataArrayPath> amSources;
    amSources.push_back(DataArrayPath("DataContainer1", "AttributeMatrix1", ""));

    // "Destination Data Container Does Not Exist" Test
    moveDataPtr->setDataContainerDestination("ThisDataContainerShouldNotExist");
    moveDataPtr->setAttributeMatrixSources(amSources);
    moveDataPtr->execute();
    DREAM3D_REQUIRE_EQUAL(moveDataPtr->getErrorCondition(), DC_DEST_NOT_FOUND)

    // "Source Data Container Does Not Exist" Test
    amSources.clear();
    amSources.push_back(DataArrayPath("ThisDataContainerShouldNotExist", "AttributeMatrix1", ""));
    moveDataPtr->setDataContainerDestination("DataContainer2");
    moveDataPtr->setAttributeMatrixSources(amSources);
    moveDataPtr->execute();
    DREAM3D_REQUIRE_EQUAL(moveDataPtr->getErrorCondition(), DC_SRC_NOT_FOUND)

    // "Source Attribute Matrix Does Not Exist" Test
    amSources.clear();
    amSources.push_back(DataArrayPath("DataContainer1", "ThisAttributeMatrixShouldNotExist", ""));
    moveDataPtr->setDataContainerDestination("DataContainer2");
    moveDataPtr->setAttributeMatrixSources(amSources);
    moveDataPtr->execute();
    DREAM3D_REQUIRE_EQUAL(moveDataPtr->getErrorCondition(), AM_SRC_NOT_FOUND)

    // "Move Attribute Matrix" Verification Test
    amSources.clear();
    amSources.push_back(DataArrayPath("DataContainer1", "AttributeMatrix1", ""));
    moveDataPtr->setAttributeMatrixSources(amSources);
    moveDataPtr->setDataContainerDestination("DataContainer2");
    moveDataPtr->execute();
    DREAM3D_REQUIRE_EQUAL(moveDataPtr->getErrorCondition(), 0)

  }
  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void MoveSingleDataArrayTest()
  {
    DataContainerArray::Pointer dca = createDataContainerArray();

    MoveMultiData::Pointer moveDataPtr = MoveMultiData::New();
    moveDataPtr->setDataContainerArray(dca);
    moveDataPtr->setWhatToMove(::k_MoveMultiDataArray);

    QVector<DataArrayPath> dataArrayPaths;
    dataArrayPaths.push_back(DataArrayPath("DataContainer1", "AttributeMatrix1", "ThisShouldNotExist"));

    // "Creation of Data Array Pointer" Test
    moveDataPtr->setDataArraySources(dataArrayPaths);
    moveDataPtr->execute();
    DREAM3D_REQUIRED(moveDataPtr->getErrorCondition(), < , 0)

    dataArrayPaths.clear();
    dataArrayPaths.push_back(DataArrayPath("DataContainer1", "ThisShouldNotExist", "DataArray1"));
    moveDataPtr->setDataArraySources(dataArrayPaths);
    moveDataPtr->execute();
    DREAM3D_REQUIRED(moveDataPtr->getErrorCondition(), <, 0)

    dataArrayPaths.clear();
    dataArrayPaths.push_back(DataArrayPath("ThisShouldNotExist", "AttributeMatrix1", "DataArray1"));
    moveDataPtr->setDataArraySources(dataArrayPaths);
    moveDataPtr->execute();
    DREAM3D_REQUIRED(moveDataPtr->getErrorCondition(), <, 0)

    // "Creation of Attribute Matrix Pointer" Test
    moveDataPtr->setAttributeMatrixDestination(DataArrayPath("DataContainer2", "ThisShouldNotExist", ""));
    moveDataPtr->execute();
    DREAM3D_REQUIRED(moveDataPtr->getErrorCondition(), <, 0)

    moveDataPtr->setAttributeMatrixDestination(DataArrayPath("ThisShouldNotExist", "AttributeMatrix3", ""));
    moveDataPtr->execute();
    DREAM3D_REQUIRED(moveDataPtr->getErrorCondition(), <, 0)

    dataArrayPaths.clear();
    dataArrayPaths.push_back(DataArrayPath("DataContainer1", "AttributeMatrix2", "DataArray3"));
    // "NumTuples Comparison" Test
    moveDataPtr->setAttributeMatrixDestination(DataArrayPath("DataContainer2", "AttributeMatrix3", ""));
    moveDataPtr->setDataArraySources(dataArrayPaths);
    moveDataPtr->execute();
    DREAM3D_REQUIRE_EQUAL(moveDataPtr->getErrorCondition(), TUPLES_NOT_MATCH)

    dataArrayPaths.clear();
    dataArrayPaths.push_back(DataArrayPath("DataContainer1", "AttributeMatrix2", "DataArray3"));
    // "Move Data Array" Verification Test
    moveDataPtr->setAttributeMatrixDestination(DataArrayPath("DataContainer2", "AttributeMatrix4", ""));
    moveDataPtr->setDataArraySources(dataArrayPaths);
    moveDataPtr->execute();
    DREAM3D_REQUIRE_EQUAL(moveDataPtr->getErrorCondition(), 0)
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void MoveMultiAttributeMatricesTest()
  {
    DataContainerArray::Pointer dca = createDataContainerArray();

    MoveMultiData::Pointer moveDataPtr = MoveMultiData::New();
    moveDataPtr->setDataContainerArray(dca);
    moveDataPtr->setWhatToMove(::k_MoveAttributeMatrix);

    QVector<DataArrayPath> amSources;

    // "Move Attribute Matrix" Verification Test
    amSources.push_back(DataArrayPath("DataContainer1", "AttributeMatrix1", ""));
    amSources.push_back(DataArrayPath("DataContainer1", "AttributeMatrix2", ""));
    moveDataPtr->setDataContainerDestination("DataContainer2");
    moveDataPtr->setAttributeMatrixSources(amSources);
    moveDataPtr->execute();
    DREAM3D_REQUIRE_EQUAL(moveDataPtr->getErrorCondition(), 0)

  }
  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void MoveMultiDataArraysTest()
  {
    DataContainerArray::Pointer dca = createDataContainerArray();

    MoveMultiData::Pointer moveDataPtr = MoveMultiData::New();
    moveDataPtr->setDataContainerArray(dca);
    moveDataPtr->setWhatToMove(::k_MoveMultiDataArray);

    QVector<DataArrayPath> dataArrayPaths;
    dataArrayPaths.push_back(DataArrayPath("DataContainer1", "AttributeMatrix2", "DataArray3"));
    dataArrayPaths.push_back(DataArrayPath("DataContainer1", "AttributeMatrix2", "DataArray2"));

    // "Move Data Array" Verification Test
    moveDataPtr->setAttributeMatrixDestination(DataArrayPath("DataContainer2", "AttributeMatrix4", ""));
    moveDataPtr->setDataArraySources(dataArrayPaths);
    moveDataPtr->execute();
    DREAM3D_REQUIRE_EQUAL(moveDataPtr->getErrorCondition(), 0)
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    std::cout << "#### MoveMultiDataTest Starting ####" << std::endl;

    int err = EXIT_SUCCESS;
    DREAM3D_REGISTER_TEST(MoveSingleAttributeMatrixTest())
    DREAM3D_REGISTER_TEST(MoveSingleDataArrayTest())

    DREAM3D_REGISTER_TEST(MoveMultiAttributeMatricesTest())
    DREAM3D_REGISTER_TEST(MoveMultiDataArraysTest())
  }

private:
  MoveMultiDataTest(const MoveMultiDataTest&); // Copy Constructor Not Implemented
  void operator=(const MoveMultiDataTest&); // Operator '=' Not Implemented
};
