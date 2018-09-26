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
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QVector>
#include <QtCore/QModelIndex>

#include <QtGui/QIcon>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"

#include "SVWidgetsLib/SVWidgetsLib.h"

class FilterInputWidget;
class PipelineModel;

class SVWidgetsLib_EXPORT AbstractPipelineItem : public QObject
{
public:
  enum class ItemType : unsigned int
  {
    Root,
    Pipeline,
    Filter
  };

  enum PipelineItemData
  {
    Contents
  };

  AbstractPipelineItem(AbstractPipelineItem* parent = nullptr);
  virtual ~AbstractPipelineItem() = default;

  virtual int indexOf(AbstractPipelineItem* item) const = 0;
  virtual AbstractPipelineItem* child(int number) const = 0;
  AbstractPipelineItem* parent() const;
  virtual ItemType getItemType() const = 0;

  virtual int childCount() const = 0;
  int rowCount() const;
  int columnCount() const;

  virtual bool insertChild(int position, AbstractPipelineItem* child) = 0;
  virtual bool insertChildren(int position, int count) = 0;

  virtual bool removeChild(int position) = 0;
  virtual bool removeChildren(int position, int count) = 0;

  virtual int childIndex() const = 0;

  void setParent(AbstractPipelineItem* parent);

  virtual QVariant data(int role) const = 0;
  virtual bool setData(int role, const QVariant& value) = 0;
  virtual Qt::ItemFlags flags() const = 0;

protected:
  virtual PipelineModel* model() const;

private:
  AbstractPipelineItem* m_ParentItem;

  AbstractPipelineItem(const AbstractPipelineItem&); // Copy Constructor Not Implemented
  void operator=(const AbstractPipelineItem&);       // Operator '=' Not Implemented
};
