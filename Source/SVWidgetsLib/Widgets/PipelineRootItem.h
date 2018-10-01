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
#include <QtCore/QSet>
#include <QtCore/QVariant>
#include <QtCore/QVector>

#include <QtGui/QIcon>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/Filtering/FilterPipeline.h"

#include "SVWidgetsLib/SVWidgetsLib.h"
#include "SVWidgetsLib/Widgets/AbstractPipelineItem.h"

class PipelineItem;
class PipelineModel;

class SVWidgetsLib_EXPORT PipelineRootItem : public AbstractPipelineItem
{
public:
  PipelineRootItem(PipelineModel* model);
  virtual ~PipelineRootItem();

  /**
  * @brief Returns the PipelineItem type
  * @return
  */
  ItemType getItemType() const override;

  int childIndex() const;
  AbstractPipelineItem* child(int number) const override;
  int indexOf(AbstractPipelineItem* item) const override;
  int indexOf(FilterPipeline::Pointer pipeline) const;

  int childCount() const override;
  bool insertChild(int position, AbstractPipelineItem* child) override;
  bool insertChildren(int position, int count) override;

  bool removeChild(int position) override;
  bool removeChildren(int position, int count) override;

  QVariant data(int role) const;
  bool setData(int role, const QVariant& value);
  Qt::ItemFlags flags() const override;

  PipelineItem* insertPipeline(int position, QString filePath, FilterPipeline::Pointer pipeline);
  PipelineItem* appendPipeline(QString filePath, FilterPipeline::Pointer pipeline);

  bool removePipelineAtIndex(int position);
  bool removePipeline(FilterPipeline::Pointer pipeline);

  static QString TopLevelString();

signals:

protected:
  PipelineModel* model() const override;

private:
  PipelineModel* m_Model = nullptr;
  QList<PipelineItem*> m_PipelineItems;

  PipelineRootItem(const PipelineRootItem&); // Copy Constructor Not Implemented
  void operator=(const PipelineRootItem&);   // Operator '=' Not Implemented
};
