/*
 * Your License or Copyright Information can go here
 */

#pragma once

#include <QString>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/IDataArray.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class SIMPLib_EXPORT Filt0 : public AbstractFilter
{
  Q_OBJECT
public:
  SIMPL_SHARED_POINTERS(Filt0)
  SIMPL_FILTER_NEW_MACRO(Filt0)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(Filt0, AbstractFilter)

  ~Filt0() override;
  SIMPL_INSTANCE_STRING_PROPERTY(DataContainerName)
  SIMPL_INSTANCE_STRING_PROPERTY(CellAttributeMatrixName)

  SIMPL_INSTANCE_PROPERTY(float, Filt0_Float)
  SIMPL_INSTANCE_PROPERTY(int32_t, Filt0_Integer)

  /**
  * @brief This returns the group that the filter belonds to. You can select
  * a different group if you want. The string returned here will be displayed
  * in the GUI for the filter
  */
  const QString getGroupName() const override
  {
    return SIMPL::FilterGroups::Generic;
  }

  /**
  * @brief This returns a string that is displayed in the GUI. It should be readable
  * and understandable by humans.
  */
  const QString getHumanLabel() const override
  {
    return "Filt0";
  }

  /**
  * @brief This returns a string that is displayed in the GUI and helps to sort the filters into
  * a subgroup. It should be readable and understandable by humans.
  */
  const QString getSubGroupName() const override
  {
    return "Misc";
  }

  /**
  * @brief This method will instantiate all the end user settable options/parameters
  * for this filter
  */
  void setupFilterParameters() override;

  /**
  * @brief This method will read the options from a file
  * @param reader The reader that is used to read the options from a file
  */
  void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

  /**
   * @brief Reimplemented from @see AbstractFilter class
   */
  void execute() override;

  /**
  * @brief This function runs some sanity checks on the DataContainer and inputs
  * in an attempt to ensure the filter can process the inputs.
  */
  void preflight() override;

signals:
  void updateFilterParameters(AbstractFilter* filter);
  void parametersChanged();
  void preflightAboutToExecute();
  void preflightExecuted();

protected:
  Filt0();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

private:
  Filt0(const Filt0&) = delete;          // Copy Constructor Not Implemented
  void operator=(const Filt0&) = delete; // Move assignment Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class SIMPLib_EXPORT Filt1 : public AbstractFilter
{
  Q_OBJECT
public:
  SIMPL_SHARED_POINTERS(Filt1)
  SIMPL_FILTER_NEW_MACRO(Filt1)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(Filt1, AbstractFilter)

  ~Filt1() override;
  SIMPL_INSTANCE_STRING_PROPERTY(DataContainerName)

  SIMPL_INSTANCE_PROPERTY(float, Filt1_Float)
  SIMPL_INSTANCE_PROPERTY(int32_t, Filt1_Integer)

  /**
  * @brief This returns the group that the filter belonds to. You can select
  * a different group if you want. The string returned here will be displayed
  * in the GUI for the filter
  */
  const QString getGroupName() const override
  {
    return "TestFilters";
  }

  /**
  * @brief This returns a string that is displayed in the GUI. It should be readable
  * and understandable by humans.
  */
  const QString getHumanLabel() const override
  {
    return "Filt1";
  }

  /**
  * @brief This returns a string that is displayed in the GUI and helps to sort the filters into
  * a subgroup. It should be readable and understandable by humans.
  */
  const QString getSubGroupName() const override
  {
    return "Test";
  }

  /**
  * @brief This method will instantiate all the end user settable options/parameters
  * for this filter
  */
  void setupFilterParameters() override;

  /**
  * @brief This method will read the options from a file
  * @param reader The reader that is used to read the options from a file
  */
  void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

  /**
   * @brief Reimplemented from @see AbstractFilter class
   */
  void execute() override;

  /**
  * @brief This function runs some sanity checks on the DataContainer and inputs
  * in an attempt to ensure the filter can process the inputs.
  */
  void preflight() override;

signals:
  void updateFilterParameters(AbstractFilter* filter);
  void parametersChanged();
  void preflightAboutToExecute();
  void preflightExecuted();

protected:
  Filt1();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

private:
  Filt1(const Filt1&) = delete;          // Copy Constructor Not Implemented
  void operator=(const Filt1&) = delete; // Move assignment Not Implemented
};

