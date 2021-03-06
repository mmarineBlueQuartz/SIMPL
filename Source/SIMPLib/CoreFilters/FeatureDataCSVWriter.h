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


#pragma once

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

#include "SIMPLib/Common/Constants.h"

/**
 * @brief The FeatureDataCSVWriter class. See [Filter documentation](@ref featuredatacsvwriter) for details.
 */
class  SIMPLib_EXPORT FeatureDataCSVWriter : public AbstractFilter
{
    Q_OBJECT
    PYB11_CREATE_BINDINGS(FeatureDataCSVWriter SUPERCLASS AbstractFilter)
    PYB11_PROPERTY(DataArrayPath CellFeatureAttributeMatrixPath READ getCellFeatureAttributeMatrixPath WRITE setCellFeatureAttributeMatrixPath)
    PYB11_PROPERTY(QString FeatureDataFile READ getFeatureDataFile WRITE setFeatureDataFile)
    PYB11_PROPERTY(bool WriteNeighborListData READ getWriteNeighborListData WRITE setWriteNeighborListData)
    PYB11_PROPERTY(int DelimiterChoice READ getDelimiterChoice WRITE setDelimiterChoice)
    PYB11_PROPERTY(bool WriteNumFeaturesLine READ getWriteNumFeaturesLine WRITE setWriteNumFeaturesLine)

  public:
    SIMPL_SHARED_POINTERS(FeatureDataCSVWriter)
    SIMPL_FILTER_NEW_MACRO(FeatureDataCSVWriter)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(FeatureDataCSVWriter, AbstractFilter)

    ~FeatureDataCSVWriter() override;

    SIMPL_FILTER_PARAMETER(DataArrayPath, CellFeatureAttributeMatrixPath)
    Q_PROPERTY(DataArrayPath CellFeatureAttributeMatrixPath READ getCellFeatureAttributeMatrixPath WRITE setCellFeatureAttributeMatrixPath)

    SIMPL_FILTER_PARAMETER(QString, FeatureDataFile)
    Q_PROPERTY(QString FeatureDataFile READ getFeatureDataFile WRITE setFeatureDataFile)

    SIMPL_FILTER_PARAMETER(bool, WriteNeighborListData)
    Q_PROPERTY(bool WriteNeighborListData READ getWriteNeighborListData WRITE setWriteNeighborListData)

    SIMPL_FILTER_PARAMETER(SIMPL::DelimiterTypes::Type, DelimiterChoice)
    Q_PROPERTY(SIMPL::DelimiterTypes::Type DelimiterChoice READ getDelimiterChoice WRITE setDelimiterChoice)
    Q_PROPERTY(int DelimiterChoiceInt READ getDelimiterChoiceInt WRITE setDelimiterChoiceInt)

    SIMPL_FILTER_PARAMETER(bool, WriteNumFeaturesLine)
    Q_PROPERTY(bool WriteNumFeaturesLine READ getWriteNumFeaturesLine WRITE setWriteNumFeaturesLine)

    SIMPL_INSTANCE_PROPERTY(char, Delimiter)

    /**
     * @brief getDelimiterChoiceInt Returns the corresponding int from the enum SIMPL::DelimiterTypes::Type for DelimiterChoice
     */
    int getDelimiterChoiceInt() const;

    /**
     * @brief setDelimiterChoiceInt Sets DelimiterChoice to the enum type from SIMPL::DelimiterTypes::Type corresponding to the int parameter
     */
    void setDelimiterChoiceInt(const int& value);

    /**
     * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
     */
    const QString getCompiledLibraryName() const override;

    /**
     * @brief getBrandingString Returns the branding string for the filter, which is a tag
     * used to denote the filter's association with specific plugins
     * @return Branding string
    */
    const QString getBrandingString() const override;

    /**
     * @brief getFilterVersion Returns a version string for this filter. Default
     * value is an empty string.
     * @return
     */
    const QString getFilterVersion() const override;

    /**
     * @brief newFilterInstance Reimplemented from @see AbstractFilter class
     */
    AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

    /**
     * @brief getGroupName Reimplemented from @see AbstractFilter class
     */
    const QString getGroupName() const override;

    /**
     * @brief getSubGroupName Reimplemented from @see AbstractFilter class
     */
    const QString getSubGroupName() const override;

    /**
     * @brief getUuid Return the unique identifier for this filter.
     * @return A QUuid object.
     */
    const QUuid getUuid() override;

    /**
     * @brief getHumanLabel Reimplemented from @see AbstractFilter class
     */
    const QString getHumanLabel() const override;

    /**
     * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
     */
    void setupFilterParameters() override;

    /**
     * @brief readFilterParameters Reimplemented from @see AbstractFilter class
     */
    void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

    /**
     * @brief execute Reimplemented from @see AbstractFilter class
     */
    void execute() override;

    /**
    * @brief preflight Reimplemented from @see AbstractFilter class
    */
    void preflight() override;

  signals:
    /**
     * @brief updateFilterParameters Emitted when the Filter requests all the latest Filter parameters
     * be pushed from a user-facing control (such as a widget)
     * @param filter Filter instance pointer
     */
    void updateFilterParameters(AbstractFilter* filter);

    /**
     * @brief parametersChanged Emitted when any Filter parameter is changed internally
     */
    void parametersChanged();

    /**
     * @brief preflightAboutToExecute Emitted just before calling dataCheck()
     */
    void preflightAboutToExecute();

    /**
     * @brief preflightExecuted Emitted just after calling dataCheck()
     */
    void preflightExecuted();

  protected:
    FeatureDataCSVWriter();
    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    void dataCheck();

    /**
     * @brief Initializes all the private instance variables.
     */
    void initialize();

  public:
    FeatureDataCSVWriter(const FeatureDataCSVWriter&) = delete; // Copy Constructor Not Implemented
    FeatureDataCSVWriter(FeatureDataCSVWriter&&) = delete;      // Move Constructor Not Implemented
    FeatureDataCSVWriter& operator=(const FeatureDataCSVWriter&) = delete; // Copy Assignment Not Implemented
    FeatureDataCSVWriter& operator=(FeatureDataCSVWriter&&) = delete;      // Move Assignment Not Implemented
};
