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
#include <QtCore/QSize>
#include <QtCore/QVariant>
#include <QtCore/QVector>

//#include <QtGui/QIcon>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

#include "SVWidgetsLib/SVWidgetsLib.h"
#include "SVWidgetsLib/Widgets/PipelineRootItem.h"

class FilterInputWidget;

class SVWidgetsLib_EXPORT PipelineRootItem : public AbstractPipelineItem
{
public:
  PipelineRootItem(const QVector<QVariant>& data);
  virtual ~PipelineRootItem();

  static const int MaxHeight = 28;

  PipelineItem* child(int number);
  AbstractPipelineItem* parent();

  int childCount() const;
  int columnCount() const;

  QVariant data(int column) const;
  bool setData(int column, const QVariant& value);

  bool insertChild(int position, PipelineRootItem* child);
  bool insertChildren(int position, int count, int columns);
  bool insertColumns(int position, int columns);

  bool removeChild(int position);
  bool removeChildren(int position, int count);
  bool removeColumns(int position, int columns);

  int childNumber() const;

  static QString TopLevelString();

private:
  QList<PipelineRootItem*> m_ChildItems;
  PipelineRootItem* m_ParentItem;

  void setupFilterInputWidget();

public:
  PipelineRootItem(const PipelineRootItem&) = delete;            // Copy Constructor Not Implemented
  PipelineRootItem(PipelineRootItem&&) = delete;                 // Move Constructor Not Implemented
  PipelineRootItem& operator=(const PipelineRootItem&) = delete; // Copy Assignment Not Implemented
  PipelineRootItem& operator=(PipelineRootItem&&) = delete;      // Move Assignment Not Implemented
};
