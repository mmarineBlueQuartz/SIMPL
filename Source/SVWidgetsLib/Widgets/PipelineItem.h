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

class FilterInputWidget;
class PipelineFilterItem;
class PipelineRootItem;

class SVWidgetsLib_EXPORT PipelineItem : public AbstractPipelineItem
{
  Q_OBJECT

public:
  PipelineItem(QString filePath, FilterPipeline::Pointer pipeline, PipelineRootItem* parent);
  virtual ~PipelineItem();

  /**
   * @brief Returns the saved pipeline name;
   * @return
   */
  QString getPipelineName() const;

  /**
   * @brief Sets the pipeline name
   * @param name
   */
  void PipelineItem::setPipelineName(QString name);

  /**
   * @brief Returns the saved pipeline.
   */
  FilterPipeline::Pointer getSavedPipeline() const;

  /**
   * @brief Returns the current pipeline version to be edited.  Changes in this filter will be saved to getSavedPipeline()
   * @return
   */
  FilterPipeline::Pointer getCurrentPipeline() const;

  /**
   * @brief Updates the saved pipeline based on the current pipeline
   * @return
   */
  int save();

  /**
   * @brief Creates a new saved and current pipeline based on the current pipeline
   * @return
   */
  int saveAs(const QString& outputPath);

  /**
   * @brief Updates the current pipeline based on the saved pipeline
   */
  void updateFromSaved();

  /**
   * @brief Checks if there are changes to be saved
   * @return
   */
  bool pipelineHasChanges() const;

  void preflightPipeline();
  void executePipeline();
  void cancelPipeline();

  /**
  * @brief Returns the PipelineItem type
  * @return
  */
  ItemType getItemType() const override;

  int childIndex() const;
  AbstractPipelineItem* child(int number) const override;
  int indexOf(AbstractPipelineItem* item) const override;

  /**
   * @brief Returns the number of filters in the pipeline
   * @return
   */
  int childCount() const override;

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

  QVariant data(int column) const;
  bool setData(int column, const QVariant& value);
  Qt::ItemFlags flags() const override;
  FilterInputWidget* getFilterInputWidget(AbstractFilter::Pointer filter) const;
  FilterPipeline::FilterState getFilterState(AbstractFilter::Pointer filter) const;

  bool insertFilter(int position, AbstractFilter::Pointer child);
  bool insertFilters(int position, QVector<AbstractFilter::Pointer> filters);

  bool removeChild(int position);
  bool removeChildren(int position, int count);

  QModelIndex pipelineIndex() const;
  QModelIndex firstFilterIndex() const;
  QModelIndex lastFilterIndex() const;

  QList<QObject*> getPipelineMessageObservers() const;
  void addPipelineMessageObserver(QObject* observer);
  void stdOutMessage(const QString& msg);
  QString getFilePath() const;

  void updateDataStructure();

signals:
  void beginAddingFilter(int index);
  void filterAdded(int index, AbstractFilter::Pointer filter);
  void beginRemovingFilter(int index);
  void filterRemoved(int index, AbstractFilter::Pointer filter);
  void pipelineUpdated();
  void statusMessage(const QString& msg);
  void pipelineOutput(FilterPipeline::Pointer pipeline, DataContainerArray::Pointer dca);

private:
  FilterPipeline::Pointer m_TempPipeline;
  FilterPipeline::Pointer m_SavedPipeline;
  QMap<AbstractFilter::Pointer, PipelineFilterItem*> m_FilterItems;
  QMap<AbstractFilter::Pointer, FilterInputWidget*> m_FilterInputWidgets;
  QList<QObject*> m_PipelineMessageObservers;
  QString m_CurrentPath;
  QThread* m_WorkerThread = nullptr;
  QMetaObject::Connection m_PipelineConnection;
  QVector<DataContainerArray::Pointer> m_PreflightDataContainerArrays;
  QString m_StdOutMsg;

  void updateFilterInputWidgets();
  void setupFilterInputWidget(AbstractFilter::Pointer filter);
  void connectPipeline();
  void pipelineFinished();
  void endPipelineThread();
  void clearIssues();
  void addFilter(int index, AbstractFilter::Pointer filter);
  void removeFilter(int index, AbstractFilter::Pointer filter);

  PipelineItem(const PipelineItem&);   // Copy Constructor Not Implemented
  void operator=(const PipelineItem&); // Operator '=' Not Implemented
};
