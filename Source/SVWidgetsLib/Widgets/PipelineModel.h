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

#include <QtCore/QAbstractItemModel>
#include <QtCore/QModelIndex>
#include <QtCore/QVariant>

#include <QtWidgets/QAction>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/Filtering/FilterPipeline.h"

#include "SVWidgetsLib/Widgets/PipelineRootItem.h"

#include "SVWidgetsLib/SVWidgetsLib.h"

class QtSSettings;

class SVWidgetsLib_EXPORT PipelineModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  SIMPL_TYPE_MACRO(PipelineModel)

  PipelineModel(QObject* parent = 0);

  ~PipelineModel() override;

  enum Roles
  {
    FilterStateRole = Qt::UserRole + 1,
    ErrorStateRole,
    PipelineStateRole,
    ItemTypeRole
  };

  SIMPL_INSTANCE_PROPERTY(int, MaxNumberOfPipelines)
  
  /**
   * @brief Returns the data at the given index with the specified role
   * @param index
   * @param role
   * @return
   */
  QVariant data(const QModelIndex& index, int role) const override;

  /**
   * @brief Sets a target role data for the given index.
   * @param index
   * @param value
   * @param role
   * @return
   */
  bool setData(const QModelIndex& index, const QVariant& value, int role) override;
  //    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

  /**
   * @brief Returns the filter at the given index
   * @param index
   * @return
   */
  AbstractFilter::Pointer filter(const QModelIndex& index) const;

  /**
   * @brief Sets the filter at the given index
   * @param index
   * @param filter
   */
  void setFilter(const QModelIndex& index, AbstractFilter::Pointer filter);

  /**
   * @brief Returns the index of the given filter with specified parent
   * @param filter
   * @param parent
   * @return
   */
  QModelIndex indexOfFilter(AbstractFilter* filter, const QModelIndex& parent = QModelIndex());

  /**
   * @brief Returns the FilterPipeline at the given index
   * @param index
   * @return
   */
  FilterPipeline::Pointer pipeline(const QModelIndex& index) const;

  /**
   * @brief Returns the index of the given pipeline with specified parent
   * @param filter
   * @param parent
   * @return
   */
  QModelIndex indexOfPipeline(FilterPipeline::Pointer pipeline, const QModelIndex& parent = QModelIndex());

  /**
   * @brief Returns the FilterInputWidget for the target index
   * @param index
   * @return
   */
  FilterInputWidget* filterInputWidget(const QModelIndex& index);

  /**
   * @brief Returns the QModelIndex for an item at a given location
   * @param row
   * @param column
   * @param parent
   * @return
   */
  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

  /**
   * @brief Returns the parent index for the index specified
   * @param index
   * @return
   */
  QModelIndex parent(const QModelIndex& index) const override;

  /**
   * @brief Returns the number of rows under the given parent
   * @param parent
   * @return
   */
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  /**
   * @brief Returns the number of columns for the children of the given parent
   * @param parent
   * @return
   */
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  /**
   * @brief Attempts to add rows to the item at the given index.  Because PipelineFilterItems cannot have children,
   * and there is no default filter for PipelineItem, this will only succeed when the target item is the root node.
   * @param position
   * @param rows
   * @param parent
   * @return
   */
  bool insertRows(int position, int rows, const QModelIndex& parent = QModelIndex()) override;

  /**
   * @brief Attempts to remove rows from the item at the given location.  Because PiplineFilterItems cannot have children,
   * this only works for root and pipeline nodes.
   * @param position
   * @param rows
   * @param parent
   * @return
   */
  bool removeRows(int position, int rows, const QModelIndex& parent = QModelIndex()) override;

  /**
   * @param Attempts to move a given number of rows from one item to another
   * @param sourceParent
   * @param sourceRow
   * @param count
   * @param destinationParent
   * @param destinationChild
   * @return
   */
  bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild) override;

  /**
   * @brief Returns true if the model is empty
   * @return
   */
  bool isEmpty();

  /**
   * @brief Returns the Qt::ItemFlags for the item at the given index
   * @param index
   * @return
   */
  Qt::ItemFlags flags(const QModelIndex& index) const override;

  /**
   * @brief Returns the supported drop actions for the model
   * @return
   */
  Qt::DropActions supportedDropActions() const override;

  /**
   * @brief Returns the list of supported MIME types
   * @return
   */
  QStringList mimeTypes() const override;

  /**
   * @brief Returns true if the given MIME data can be dropped with the given DropAction at the given item.
   * @param data
   * @param action
   * @param row
   * @param column
   * @param parent
   * @return
   */
  bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const override;

  /**
   * @brief Returns true if the pipeline at the given index is saved
   * @param index
   * @return
   */
  bool isPipelineSaved(const QModelIndex& index);

  /**
   * @brief Returns the root item for the model
   * @return
   */
  PipelineRootItem* getRootItem();

  int getMaxFilterCount() const;

  QList<QObject*> getPipelineMessageObservers();

  /**
   * @brief Appends the given pipeline into the model
   * @param pipeline
   */
  void appendPipeline(FilterPipeline::Pointer pipeline);

  /**
   * @brief Inserts the given pipeline into the model
   * @param pipeline
   * @param insertIndex
   * @return
   */
  bool addPipeline(FilterPipeline::Pointer pipeline, int insertIndex);

  /**
   * @brief Connects the target PipelineItem
   * @param item
   */
  void connectPipelineItem(PipelineItem* item);

  int openPipeline(const QString& filePath, int insertIndex);
  int save(const QModelIndex& index);
  int saveAs(const QModelIndex& index, const QString& outputPath);

  void executePipeline(const QModelIndex& index);
  void cancelPipeline(const QModelIndex& index);

signals:
  void clearIssuesTriggered();

  void preflightTriggered(const QModelIndex& pipelineIndex, PipelineModel* model);

  void pipelineDataChanged(const QModelIndex& pipelineIndex);

  void statusMessageGenerated(const QString& msg);
  void standardOutputMessageGenerated(const QString& msg);

private:
  PipelineRootItem* m_RootItem;

  /**
   * @brief Returns the PipelineItem at the given index.  If the index is invalid, return the root item
   * @param index
   * @return
   */
  AbstractPipelineItem* getItem(const QModelIndex& index) const;
  PipelineItem* getPipelineItem(const QModelIndex& index) const;
  FilterPipeline::Pointer readPipelineFromFile(const QString& filePath);

  PipelineModel(const PipelineModel&);  // Copy Constructor Not Implemented
  void operator=(const PipelineModel&); // Operator '=' Not Implemented
};
