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

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

#include "SIMPLib/Common/FilterFactory.hpp"
#include "SIMPLib/Common/FilterManager.h"
#include "SIMPLib/Common/FilterPipeline.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/CoreFilters/CreateStringArray.h"
#include "SIMPLib/DataArrays/StringDataArray.hpp"
#include "SIMPLib/DataContainers/DataContainer.h"
#include "SIMPLib/Plugin/ISIMPLibPlugin.h"
#include "SIMPLib/Plugin/SIMPLibPluginLoader.h"
#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Utilities/QMetaObjectUtilities.h"
#include "SIMPLib/Utilities/UnitTestSupport.hpp"

#include "SIMPLTestFileLocations.h"

class CreateStringArrayTest
{
public:
  CreateStringArrayTest()
  {
  }
  virtual ~CreateStringArrayTest()
  {
  }

  enum ErrorCodes
  {
    NO_ERROR = 0,
    INT8_ERROR = -4050
  };

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestFilterAvailability()
  {
    // Now instantiate the CreateDataArray Filter from the FilterManager
    QString filtName = "CreateStringArray";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
    if(nullptr == filterFactory.get())
    {
      std::stringstream ss;
      ss << "The CreateStringArrayTest Requires the use of the " << filtName.toStdString() << " filter which is found in the SIMPLib Plugin";
      DREAM3D_TEST_THROW_EXCEPTION(ss.str())
    }

    // Now instantiate the CreateDataContainer Filter from the FilterManager
    filtName = "CreateDataContainer";
    filterFactory = fm->getFactoryForFilter(filtName);
    if(nullptr == filterFactory.get())
    {
      std::stringstream ss;
      ss << "The CreateStringArrayTest Requires the use of the " << filtName.toStdString() << " filter which is found in the SIMPLib Plugin";
      DREAM3D_TEST_THROW_EXCEPTION(ss.str())
    }

    // Now instantiate the CreateDataContainer Filter from the FilterManager
    filtName = "CreateAttributeMatrix";
    fm = FilterManager::Instance();
    filterFactory = fm->getFactoryForFilter(filtName);
    if(nullptr == filterFactory.get())
    {
      std::stringstream ss;
      ss << "The CreateStringArrayTest Requires the use of the " << filtName.toStdString() << " filter which is found in the SIMPLib Plugin";
      DREAM3D_TEST_THROW_EXCEPTION(ss.str())
    }

    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestCreateDataContainerAndAttributeMatrix()
  {

    int err = 0;
    bool propWasSet = false;
    // bool ok = false;
    QVariant var;

    DataContainerArray::Pointer dca = DataContainerArray::New();

    // Now instantiate the CreateDataArray Filter from the FilterManager
    FilterManager* fm = FilterManager::Instance();

    QString filtName = "CreateDataContainer";
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
    if(nullptr != filterFactory.get())
    {
      // If we get this far, the Factory is good so creating the filter should not fail unless something has
      // horribly gone wrong in which case the system is going to come down quickly after this.
      AbstractFilter::Pointer filter = filterFactory->create();

      filter->setDataContainerArray(dca);

      // Test 1 set int32 array with an initialization of -5 and read value for comparison
      var.setValue(QString::fromLatin1("Test Data Container"));
      propWasSet = filter->setProperty("CreatedDataContainer", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true)

      filter->preflight();
      err = filter->getErrorCondition();
      DREAM3D_REQUIRED(err, >=, 0)
    }

    filtName = "CreateAttributeMatrix";
    filterFactory = fm->getFactoryForFilter(filtName);
    if(nullptr != filterFactory.get())
    {
      // If we get this far, the Factory is good so creating the filter should not fail unless something has
      // horribly gone wrong in which case the system is going to come down quickly after this.
      AbstractFilter::Pointer filter = filterFactory->create();

      filter->setDataContainerArray(dca);

      var.setValue(QString::fromLatin1("Test Data Container"));
      propWasSet = filter->setProperty("CreatedDataContainer", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true)

      var.setValue(QString::fromLatin1("Cell Attribute Matrix"));
      propWasSet = filter->setProperty("CreatedAttributeMatrix", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true)

      filter->preflight();
      err = filter->getErrorCondition();
      DREAM3D_REQUIRED(err, >=, 0)
    }

    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  DataContainerArray::Pointer CreateDataContainerArray()
  {
    DataContainerArray::Pointer dca = DataContainerArray::New();
    DataContainer::Pointer m = DataContainer::New(SIMPL::Defaults::DataContainerName);
    dca->addDataContainer(m);
    AttributeMatrix::Pointer attrMatrix = AttributeMatrix::New(QVector<size_t>(1, 1), SIMPL::Defaults::AttributeMatrixName, AttributeMatrix::Type::Generic);
    m->addAttributeMatrix(SIMPL::Defaults::AttributeMatrixName, attrMatrix);
    return dca;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  AbstractFilter::Pointer CreateFilter(QString initValue)
  {
    // Now instantiate the CreateDataArray Filter from the FilterManager
    QString filtName = "CreateStringArray";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
    if(nullptr != filterFactory.get())
    {
      // If we get this far, the Factory is good so creating the filter should not fail unless something has
      // horribly gone wrong in which case the system is going to come down quickly after this.
      AbstractFilter::Pointer filter = filterFactory->create();

      QString dsName = QString("StringArray");
      DataArrayPath path = DataArrayPath(SIMPL::Defaults::DataContainerName, SIMPL::Defaults::AttributeMatrixName, dsName);

      DataContainerArray::Pointer dca = CreateDataContainerArray();
      filter->setDataContainerArray(dca);

      QVariant var;
      int err = 0;
      bool propWasSet = false;

      //var.setValue(1);
      //propWasSet = filter->setProperty("NumberOfComponents", var); // 1 component
      //DREAM3D_REQUIRE_EQUAL(propWasSet, true)

      var.setValue(path);
      propWasSet = filter->setProperty("NewArray", var); // array path
      DREAM3D_REQUIRE_EQUAL(propWasSet, true)

      /* ==== Test The Maximum Value for the primitive type ===== */
      var.setValue(initValue);
      propWasSet = filter->setProperty("InitializationValue", var); // initialize with
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);

      return filter;
    }
    else
    {
      QString ss = QObject::tr("CreateStringArrayTest Error creating filter '%1'. Filter was not created/executed. Please notify the developers.").arg(filtName);
      DREAM3D_REQUIRE_EQUAL(0, 1)
    }

    return nullptr;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestCreateStringArray()
  {
    QString initValue = "Test";
    CreateStringArray::Pointer filter = std::static_pointer_cast<CreateStringArray>(CreateFilter(initValue));
    DataContainerArray::Pointer dca = CreateDataContainerArray();
    filter->setDataContainerArray(dca);

    int err;

    // Preflight
    filter->preflight();
    err = filter->getErrorCondition();
    DREAM3D_REQUIRE_EQUAL(err, NO_ERROR);

    dca = CreateDataContainerArray();
    filter->setDataContainerArray(dca);

    // Execute
    filter->execute();
    err = filter->getErrorCondition();
    DREAM3D_REQUIRE_EQUAL(err, NO_ERROR);

    // Created StringDataArray
    {
      DataArrayPath path = filter->getNewArray();
      AttributeMatrix::Pointer am = dca->getDataContainer(SIMPL::Defaults::DataContainerName)->getAttributeMatrix(path.getAttributeMatrixName());
      IDataArray::Pointer testArrayPtr = am->getAttributeArray(path.getDataArrayName());
      StringDataArray::Pointer inputArray = std::dynamic_pointer_cast<StringDataArray>(testArrayPtr);
      DREAM3D_REQUIRE_VALID_POINTER(inputArray.get());
      DREAM3D_REQUIRE_EQUAL(inputArray->getValue(0), initValue)
      DREAM3D_REQUIRE_EQUAL(inputArray->getNumberOfComponents(), 1)
      DREAM3D_REQUIRE_EQUAL(inputArray->getSize(), inputArray->getNumberOfTuples())
      DREAM3D_REQUIRE_EQUAL(inputArray->getSize(), am->getNumberOfTuples())
    }

    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    std::cout << "#### CreateStringArrayTest Starting ####" << std::endl;

    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(TestFilterAvailability());

    DREAM3D_REGISTER_TEST(TestCreateStringArray())
  }

private:
  CreateStringArrayTest(const CreateStringArrayTest&); // Copy Constructor Not Implemented
  void operator=(const CreateStringArrayTest&);      // Operator '=' Not Implemented
};
