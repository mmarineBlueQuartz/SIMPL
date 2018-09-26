/* ============================================================================
 * Copyright (c) 2017 BlueQuartz Software, LLC
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
 * Neither the name of BlueQuartz Software nor the names of its
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
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#pragma once

#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/QVector>

#include <QtGui/QIcon>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"

#include "SVWidgetsLib/SVWidgetsLib.h"
#include "SVWidgetsLib/Widgets/AbstractPipelineItem.h"

class PipelineItem;

class SVWidgetsLib_EXPORT PipelineFilterItem : public AbstractPipelineItem
{
public:
  PipelineFilterItem(AbstractFilter::Pointer filter, PipelineItem* parent = nullptr);
  virtual ~PipelineFilterItem() = default;

  /**
   * @brief Returns the PipelineItem that this belongs to
   * @return
   */
  PipelineItem* parentPipeline() const;

  /**
  * @brief Returns the PipelineItem type
  * @return
  */
  ItemType getItemType() const override;
  QVariant data(int column) const;
  bool setData(int column, const QVariant& value);
  Qt::ItemFlags flags() const override;

  /**
   * @brief Returns the filter pointer this item operates on
   * @return
   */
  AbstractFilter::Pointer getFilter() const;

  /**
   * @brief Sets the filter this item operates on and updates the FilterInputWidget accordingly
   * @param filter
   */
  void setFilter(AbstractFilter::Pointer filter);

  /**
   * @brief Returns the FilterInputWidget for this item
   * @return
   */
  FilterInputWidget* getFilterInputWidget() const;

  /**
   * @brief Returns the index of this item in its parent's children
   * @return
   */
  int childIndex() const override;

  /**
   * @brief Returns the number of child items
   * @return
   */
  int childCount() const override;

  /**
   * @brief Returns the child item at the given index
   * @param index
   */
  AbstractPipelineItem* child(int index) const override;

  /**
   * @brief Returns the index at which to find child item
   * @return
   */
  int indexOf(AbstractPipelineItem* item) const override;

  /**
   * @brief insertChild
   * @param position
   * @param child
   */
  bool insertChild(int position, AbstractPipelineItem* child) override;

  /**
   * @brief insertChildren
   * @param position
   * @param count
   */
  bool insertChildren(int position, int count) override;

  bool removeChild(int position) override;
  bool removeChildren(int position, int count) override;

private:
  AbstractFilter::Pointer m_Filter = nullptr;

  PipelineFilterItem(const PipelineFilterItem&); // Copy Constructor Not Implemented
  void operator=(const PipelineFilterItem&);     // Operator '=' Not Implemented
};
