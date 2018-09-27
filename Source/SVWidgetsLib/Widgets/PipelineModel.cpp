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

#include "PipelineModel.h"

#include <QtWidgets>

#include "SIMPLib/CoreFilters/DataContainerReader.h"
#include "SIMPLib/FilterParameters/H5FilterParametersReader.h"
#include "SIMPLib/FilterParameters/H5FilterParametersWriter.h"
#include "SIMPLib/FilterParameters/JsonFilterParametersReader.h"
#include "SIMPLib/FilterParameters/JsonFilterParametersWriter.h"

#include "SVWidgetsLib/Core/SVWidgetsLibConstants.h"
#include "SVWidgetsLib/QtSupport/QtSSettings.h"
#include "SVWidgetsLib/Widgets/PipelineFilterItem.h"
#include "SVWidgetsLib/Widgets/PipelineFilterMimeData.h"
#include "SVWidgetsLib/Widgets/PipelineItem.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PipelineModel::PipelineModel(QObject* parent)
: QAbstractItemModel(parent)
, m_MaxNumberOfPipelines(std::numeric_limits<int>::max())
{
  m_RootItem = new PipelineRootItem(this);
  connectItem(m_RootItem);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PipelineModel::~PipelineModel()
{
  delete m_RootItem;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineModel::columnCount(const QModelIndex& parent) const
{
  return m_RootItem->columnCount();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QVariant PipelineModel::data(const QModelIndex& index, int role) const
{
  if(!index.isValid())
  {
    return QVariant();
  }

  AbstractPipelineItem* item = getItem(index);
  return item->data(role);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  AbstractPipelineItem* item = getItem(index);
  if(nullptr == item)
  {
    return false;
  }

  return item->setData(role, value);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer PipelineModel::filter(const QModelIndex& index) const
{
  if(!index.isValid())
  {
    return AbstractFilter::NullPointer();
  }

  AbstractPipelineItem* item = getItem(index);
  PipelineFilterItem* filterItem = dynamic_cast<PipelineFilterItem*>(item);
  if(filterItem == nullptr)
  {
    return AbstractFilter::NullPointer();
  }

  return filterItem->getFilter();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineModel::setFilter(const QModelIndex& index, AbstractFilter::Pointer filter)
{
  if(!index.isValid())
  {
    return;
  }

  AbstractPipelineItem* item = getItem(index);
  PipelineFilterItem* filterItem = dynamic_cast<PipelineFilterItem*>(item);
  if(filterItem == nullptr)
  {
    return;
  }

  filterItem->setFilter(filter);
  emit dataChanged(index, index);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QModelIndex PipelineModel::indexOfFilter(AbstractFilter* filter, const QModelIndex& parent)
{
  for(int i = 0; i < rowCount(parent); i++)
  {
    QModelIndex childIndex = index(i, PipelineItem::Contents, parent);
    if(this->filter(childIndex).get() == filter)
    {
      return childIndex;
    }
  }

  return QModelIndex();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FilterPipeline::Pointer PipelineModel::pipeline(const QModelIndex& index) const
{
  if(!index.isValid())
  {
    return FilterPipeline::NullPointer();
  }

  AbstractPipelineItem* item = getItem(index);
  PipelineItem* pipelineItem = dynamic_cast<PipelineItem*>(item);
  if(nullptr == pipelineItem)
  {
    return FilterPipeline::NullPointer();
  }

  return pipelineItem->getCurrentPipeline();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FilterPipeline::Pointer PipelineModel::lastPipeline() const
{
  if(m_RootItem->childCount() == 0)
  {
    return nullptr;
  }

  PipelineItem* item = dynamic_cast<PipelineItem*>(m_RootItem->child(m_RootItem->childCount() - 1));
  return item->getCurrentPipeline();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QModelIndex PipelineModel::indexOfPipeline(FilterPipeline::Pointer pipeline, const QModelIndex& parent)
{
  for(int i = 0; i < rowCount(parent); i++)
  {
    QModelIndex childIndex = index(i, PipelineItem::Contents, parent);
    if(this->pipeline(childIndex).get() == pipeline.get())
    {
      return childIndex;
    }
  }

  return QModelIndex();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineModel::openPipeline(const QString& filePath, int insertIndex)
{
  QFileInfo fi(filePath);
  if(fi.exists() == false)
  {
    QMessageBox::warning(nullptr, QString::fromLatin1("Pipeline Read Error"), QString::fromLatin1("There was an error opening the specified pipeline file. The pipeline file does not exist."));
    return -1;
  }

  QString ext = fi.suffix();
  QString name = fi.fileName();
  QString baseName = fi.baseName();

  if(ext == "dream3d")
  {
#if 0
    QtSFileDragMessageBox* msgBox = new QtSFileDragMessageBox(this);
    msgBox->exec();
    msgBox->deleteLater();

    if(msgBox->cancelled())
    {
      return 0;
    }
    else if(msgBox->didPressOkBtn() == true)
    {
      if(msgBox->isExtractPipelineBtnChecked() == false)
#endif
    {
      DataContainerReader::Pointer reader = DataContainerReader::New();
      reader->setInputFile(filePath);

      FilterPipeline::Pointer pipeline = FilterPipeline::New();
      pipeline->pushBack(reader);
      addPipeline(pipeline, insertIndex);
      return 1;
    }
    //}
  }

  // Read the pipeline from the file
  FilterPipeline::Pointer pipeline = readPipelineFromFile(filePath);
  // Check that a valid extension was read...
  if(pipeline == FilterPipeline::NullPointer())
  {
    emit statusMessageGenerated(tr("The pipeline was not read correctly from file '%1'. '%2' is an unsupported file extension.").arg(name).arg(ext));
    emit standardOutputMessageGenerated(tr("The pipeline was not read correctly from file '%1'. '%2' is an unsupported file extension.").arg(name).arg(ext));
    return -1;
  }
  PipelineItem* item = m_RootItem->insertPipeline(insertIndex, pipeline->deepCopy());
  // Notify user of successful read
  emit statusMessageGenerated(tr("Opened \"%1\" Pipeline").arg(baseName));
  item->stdOutMessage(tr("Opened \"%1\" Pipeline").arg(baseName));

  return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FilterPipeline::Pointer PipelineModel::readPipelineFromFile(const QString& filePath)
{
  QFileInfo fi(filePath);
  QString ext = fi.suffix();

  FilterPipeline::Pointer pipeline;
  if(ext == "dream3d")
  {
    H5FilterParametersReader::Pointer dream3dReader = H5FilterParametersReader::New();
    pipeline = dream3dReader->readPipelineFromFile(filePath);
  }
  else if(ext == "json")
  {
    JsonFilterParametersReader::Pointer jsonReader = JsonFilterParametersReader::New();
    pipeline = jsonReader->readPipelineFromFile(filePath);
  }
  else
  {
    pipeline = FilterPipeline::NullPointer();
  }

  return pipeline;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineModel::save(const QModelIndex& index)
{
  PipelineItem* pipelineItem = getPipelineItem(index);
  if(pipelineItem)
  {
    return pipelineItem->save();
  }

  return -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineModel::saveAs(const QModelIndex& index, const QString& outputPath)
{
  PipelineItem* pipelineItem = getPipelineItem(index);
  if(pipelineItem)
  {
    return pipelineItem->saveAs(outputPath);
  }

  return -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineModel::executePipeline(const QModelIndex& index)
{
  PipelineItem* pipelineItem = getPipelineItem(index);
  if(pipelineItem)
  {
    pipelineItem->executePipeline();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineModel::cancelPipeline(const QModelIndex& index)
{
  PipelineItem* pipelineItem = getPipelineItem(index);
  if(pipelineItem)
  {
    pipelineItem->cancelPipeline();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Qt::ItemFlags PipelineModel::flags(const QModelIndex& index) const
{
  AbstractPipelineItem* item = getItem(index);
  return item->flags();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QStringList PipelineModel::mimeTypes() const
{
  QStringList types;
  types << SIMPLView::DragAndDrop::FilterPipelineItem;
  types << SIMPLView::DragAndDrop::FilterListItem;
  types << SIMPLView::DragAndDrop::BookmarkItem;
  types << SIMPLView::DragAndDrop::Url;
  return types;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const
{
  Q_UNUSED(row);
  Q_UNUSED(parent);

  if(action == Qt::IgnoreAction)
  {
    return false;
  }

  if(!data->hasFormat(SIMPLView::DragAndDrop::FilterPipelineItem) && !data->hasFormat(SIMPLView::DragAndDrop::FilterListItem) && !data->hasFormat(SIMPLView::DragAndDrop::BookmarkItem) &&
     !data->hasFormat(SIMPLView::DragAndDrop::Url))
  {
    return false;
  }

  if(column > 0)
  {
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractPipelineItem* PipelineModel::getItem(const QModelIndex& index) const
{
  if(index.isValid())
  {
    PipelineItem* item = static_cast<PipelineItem*>(index.internalPointer());
    if(item)
    {
      return item;
    }
  }
  return m_RootItem;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PipelineItem* PipelineModel::getPipelineItem(const QModelIndex& index) const
{
  if(!index.isValid())
  {
    return nullptr;
  }

  AbstractPipelineItem* item = getItem(index);
  if(dynamic_cast<PipelineItem*>(item))
  {
    return dynamic_cast<PipelineItem*>(item);
  }
  else if(dynamic_cast<PipelineFilterItem*>(item))
  {
    return dynamic_cast<PipelineFilterItem*>(item)->parentPipeline();
  }

  return nullptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QModelIndex PipelineModel::index(int row, int column, const QModelIndex& parent) const
{
  if(parent.isValid() && parent.column() != 0)
  {
    return QModelIndex();
  }

  AbstractPipelineItem* parentItem = getItem(parent);
  AbstractPipelineItem* childItem = parentItem->child(row);
  if(childItem)
  {
    return createIndex(row, column, childItem);
  }
  else
  {
    return QModelIndex();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineModel::insertRows(int position, int rows, const QModelIndex& parent)
{
  AbstractPipelineItem* parentItem = getItem(parent);
  bool success;

  beginInsertRows(parent, position, position + rows - 1);
  success = parentItem->insertChildren(position, rows);
  endInsertRows();

  return success;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineModel::removeRows(int position, int rows, const QModelIndex& parent)
{
  if(position < 0)
  {
    return false;
  }

  AbstractPipelineItem* parentItem = getItem(parent);
  bool success = true;

  beginRemoveRows(parent, position, position + rows - 1);
  success = parentItem->removeChildren(position, rows);
  endRemoveRows();

  return success;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineModel::moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild)
{
  beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild);

  AbstractPipelineItem* srcParentItem = getItem(sourceParent);
  AbstractPipelineItem* destParentItem = getItem(destinationParent);

  for(int i = sourceRow; i < sourceRow + count; i++)
  {
    QModelIndex srcIndex = index(i, PipelineItem::Contents, sourceParent);
    AbstractPipelineItem* srcItem = getItem(srcIndex);

    destParentItem->insertChild(destinationChild, srcItem);
    srcItem->setParent(destParentItem);
    srcParentItem->removeChild(i);
  }

  endMoveRows();

  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QModelIndex PipelineModel::parent(const QModelIndex& index) const
{
  if(!index.isValid())
  {
    return QModelIndex();
  }

  AbstractPipelineItem* childItem = getItem(index);
  AbstractPipelineItem* parentItem = childItem->parent();

  if(parentItem == m_RootItem)
  {
    return QModelIndex();
  }

  return createIndex(parentItem->childIndex(), 0, parentItem);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineModel::hasChildren(const QModelIndex& index) const
{
  AbstractPipelineItem* item = getItem(index);
  return item->childCount() > 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineModel::hasIndex(int row, int column, const QModelIndex &parent) const
{
  return getItem(parent)->hasIndex(row, column);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineModel::rowCount(const QModelIndex& parent) const
{
  AbstractPipelineItem* parentItem = getItem(parent);
  return parentItem->childCount();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Qt::DropActions PipelineModel::supportedDropActions() const
{
  return Qt::CopyAction | Qt::MoveAction;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineModel::getMaxFilterCount() const
{
  int numFilters = rowCount();
  int maxFilterCount = 0;
  if(numFilters < 10)
  {
    numFilters = 11;
    maxFilterCount = 99;
  }

  if(numFilters > 9)
  {
    int mag = 0;
    int max = numFilters;
    maxFilterCount = 1;
    while(max > 0)
    {
      mag++;
      max = max / 10;
      maxFilterCount *= 10;
    }
    maxFilterCount -= 1;
  }

  return maxFilterCount;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineModel::isPipelineSaved(const QModelIndex& index)
{
  AbstractPipelineItem* item = getItem(index);
  if(dynamic_cast<PipelineItem*>(item))
  {
    return false == dynamic_cast<PipelineItem*>(item)->pipelineHasChanges();
  }
  else if(dynamic_cast<PipelineFilterItem*>(item))
  {
    return false == dynamic_cast<PipelineFilterItem*>(item)->parentPipeline()->pipelineHasChanges();
  }

  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FilterInputWidget* PipelineModel::filterInputWidget(const QModelIndex& index)
{
  AbstractPipelineItem* item = getItem(index);
  if(dynamic_cast<PipelineFilterItem*>(item))
  {
    return dynamic_cast<PipelineFilterItem*>(item)->getFilterInputWidget();
  }
  return nullptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PipelineRootItem* PipelineModel::getRootItem()
{
  return m_RootItem;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineModel::isEmpty()
{
  if(rowCount(QModelIndex()) <= 0)
  {
    return true;
  }
  return false;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineModel::appendPipeline(FilterPipeline::Pointer pipeline)
{
   m_RootItem->insertPipeline(m_RootItem->childCount(), pipeline);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineModel::addPipeline(FilterPipeline::Pointer pipeline, int insertIndex)
{
  return nullptr != m_RootItem->insertPipeline(insertIndex, pipeline);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineModel::connectItem(AbstractPipelineItem* item)
{
  QModelIndex index = itemIndex(item);
  emit dataChanged(index, index);

  connect(item, &AbstractPipelineItem::modified, [=] {
    emit dataChanged(index, index);
  });
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineModel::connectPipelineItem(PipelineItem* item)
{
  if(nullptr == item)
  {
    return;
  }

  int itemNum = item->childIndex();
  beginInsertRows(QModelIndex(), itemNum, itemNum);
  endInsertRows();

  connect(item, &PipelineItem::filterAdded, [=](int index) {
    insertRows(index, index, item->pipelineIndex());
  });

  connect(item, &PipelineItem::filterRemoved, [=](int index) {
    removeRows(index, index, item->pipelineIndex());
  });

  connect(item, &PipelineItem::pipelineUpdated, [=] {
    emit dataChanged(item->firstFilterIndex(), item->lastFilterIndex());
    updateData(item);
  });

  connect(item, &PipelineItem::statusMessage, this, &PipelineModel::statusMessageGenerated);
  connect(item, &PipelineItem::pipelineOutput, this, &PipelineModel::pipelineOutput);

  QModelIndex pipelineIndex = item->pipelineIndex();
  QModelIndex rootIndex = pipelineIndex.parent();
  int count = item->getCurrentPipeline()->size();
  if(count > 0)
  {
    insertRows(0, count, item->pipelineIndex());
  }

  emit dataChanged(item->pipelineIndex(), item->pipelineIndex());
  emit dataChanged(item->firstFilterIndex(), item->lastFilterIndex());
  emit pipelineAdded(itemIndex(item));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineModel::updateData(AbstractPipelineItem* item)
{
  QModelIndex index = itemIndex(item);
  QVector<int> roles = { Qt::DisplayRole, Qt::DecorationRole, Qt::ToolTipRole };
  emit dataChanged(index, index, roles);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QModelIndex PipelineModel::itemIndex(AbstractPipelineItem* item)
{
  if(m_RootItem == item)
  {
    return QModelIndex();
  }
  return index(item->childIndex(), 0, itemIndex(item->parent()));
}