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

#include "PipelineItem.h"

#include <QtCore/QFileInfo>
#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtGui/QColor>
#include <QtWidgets/QMessageBox>

#include "SIMPLib/FilterParameters/H5FilterParametersWriter.h"
#include "SIMPLib/FilterParameters/JsonFilterParametersWriter.h"

#include "SVWidgetsLib/Widgets/FilterInputWidget.h"
#include "SVWidgetsLib/Widgets/PipelineFilterItem.h"
#include "SVWidgetsLib/Widgets/PipelineModel.h"
#include "SVWidgetsLib/Widgets/PipelineRootItem.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PipelineItem::PipelineItem(FilterPipeline::Pointer pipeline, PipelineRootItem* parent)
: AbstractPipelineItem(parent)
, m_SavedPipeline(pipeline)
, m_ItemTooltip("")
{
  m_TempPipeline = m_SavedPipeline->deepCopy();
  m_TempPipeline->setName(m_SavedPipeline->getName());
  updateFilterInputWidgets();
  connectPipeline();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PipelineItem::~PipelineItem()
{
  qDeleteAll(m_FilterItems);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractPipelineItem::ItemType PipelineItem::getItemType() const
{
  return ItemType::Pipeline;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString PipelineItem::getPipelineName() const
{
  return m_SavedPipeline->getName();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItem::setPipelineName(QString name)
{
  m_SavedPipeline->setName(name);
  m_TempPipeline->setName(name);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FilterPipeline::Pointer PipelineItem::getSavedPipeline() const
{
  return m_SavedPipeline;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FilterPipeline::Pointer PipelineItem::getCurrentPipeline() const
{
  return m_TempPipeline;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItem::connectPipeline()
{
  if(nullptr == m_TempPipeline)
  {
    return;
  }

  m_FilterItems.clear();
  for(AbstractFilter::Pointer filter : (*m_TempPipeline))
  {
    PipelineFilterItem* filterItem = new PipelineFilterItem(filter, this);
    m_FilterItems[filter] = filterItem;
  }

  connect(m_TempPipeline.get(), &FilterPipeline::filterAdded, this, &PipelineItem::addFilter);
  connect(m_TempPipeline.get(), &FilterPipeline::filterRemoved, this, &PipelineItem::removeFilter);
  connect(m_TempPipeline.get(), &FilterPipeline::pipelineFinished, this, &PipelineItem::pipelineUpdated);
  connect(m_TempPipeline.get(), &FilterPipeline::pipelineNameChanged, this, &PipelineItem::pipelineUpdated);
  //connect(m_TempPipeline.get(), &FilterPipeline::pipelineWasEdited, this, &PipelineItem::preflightPipeline);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineItem::save()
{
  if(m_CurrentPath.isEmpty())
  {
    emit statusMessage(tr("The pipeline was not written to file. File path required in order to save pipeline."));
    return -1;
  }

  return saveAs(m_CurrentPath);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineItem::saveAs(const QString& outputPath)
{
  bool newPath = (m_CurrentPath.compare(outputPath) != 0);

  QFileInfo fi(outputPath);
  QString ext = fi.completeSuffix();

  // If the filePath already exists - delete it so that we get a clean write to the file
  if(fi.exists() == true && (ext == "dream3d" || ext == "json"))
  {
    QFile f(outputPath);
    if(f.remove() == false)
    {
      QMessageBox::warning(nullptr, QString::fromLatin1("Pipeline Write Error"), QString::fromLatin1("There was an error removing the existing pipeline file. The pipeline was NOT saved."));
      return -1;
    }
  }

  // Create a Pipeline Object and fill it with the filters from this View
  FilterPipeline::Pointer pipeline = m_TempPipeline;

  int err = 0;
  if(ext == "dream3d")
  {
    QList<IObserver*> observers;
    for(int i = 0; i < m_PipelineMessageObservers.size(); i++)
    {
      observers.push_back(reinterpret_cast<IObserver*>(m_PipelineMessageObservers[i]));
    }

    H5FilterParametersWriter::Pointer dream3dWriter = H5FilterParametersWriter::New();
    err = dream3dWriter->writePipelineToFile(pipeline, fi.absoluteFilePath(), fi.fileName(), observers);
  }
  else if(ext == "json")
  {
    QList<IObserver*> observers;
    for(int i = 0; i < m_PipelineMessageObservers.size(); i++)
    {
      observers.push_back(reinterpret_cast<IObserver*>(m_PipelineMessageObservers[i]));
    }

    JsonFilterParametersWriter::Pointer jsonWriter = JsonFilterParametersWriter::New();
    jsonWriter->writePipelineToFile(pipeline, fi.absoluteFilePath(), fi.fileName(), observers);
  }
  else
  {
    emit statusMessage(tr("The pipeline was not written to file '%1'. '%2' is an unsupported file extension.").arg(fi.fileName()).arg(ext));
    return -1;
  }

  if(err < 0)
  {
    emit statusMessage(tr("There was an error while saving the pipeline to file '%1'.").arg(fi.fileName()));
    return -1;
  }
  else
  {
    emit statusMessage(tr("The pipeline has been saved successfully to '%1'.").arg(fi.fileName()));
  }

  // Store output path
  m_CurrentPath = outputPath;

  // Update temp pipeline
  if(newPath)
  {
    m_TempPipeline = m_TempPipeline->deepCopy();
    m_SavedPipeline = m_TempPipeline->deepCopy();
    // Set pipeline name based on the output path
    setPipelineName(fi.fileName());
  }
  else
  {
    m_SavedPipeline = m_TempPipeline->deepCopy();
  }

  // Update FilterItems
  updateFilterInputWidgets();
  return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItem::updateFromSaved()
{
  m_TempPipeline = FilterPipeline::FromJson(m_SavedPipeline->toJson());
  
  // Update FilterItems + num children
  updateFilterInputWidgets();

  // RemoveFiltersCommand
  // AddFiltersCommand
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineItem::pipelineHasChanges() const
{
  return m_TempPipeline->toJson() != m_SavedPipeline->toJson();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItem::preflightPipeline()
{
  clearIssues();

  // Preflight the pipeline
  qDebug() << "Preflight the Pipeline ... ";
  int err = m_TempPipeline->preflightPipeline();
  if(err < 0)
  {
    // TODO: Set FilterStates for errors and warnings
  }

  qDebug() << "----------- SVPipelineView::preflightPipeline End --------------";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItem::executePipeline()
{
  if(m_WorkerThread != nullptr)
  {
    if(false == m_WorkerThread->isFinished())
    {
      m_WorkerThread->wait(); // Wait until the thread is complete
    }
    if(m_WorkerThread->isFinished() == true)
    {
      delete m_WorkerThread;
      m_WorkerThread = nullptr;
    }
  }
  m_WorkerThread = new QThread(); // Create a new Thread Resource
  m_WorkerThread->setObjectName("Pipeline Thread");

  emit stdOutMessage("<b>Preflight Pipeline.....</b>");
  // Give the pipeline one last chance to preflight and get all the latest values from the GUI
  int err = m_TempPipeline->preflightPipeline();
  if(err < 0)
  {
    m_TempPipeline->clearDataContainerArray();
    return;
  }
  emit stdOutMessage("    Preflight Results: 0 Errors");

  // Store DataContainerArray from Preflight for post-Execute
  m_PreflightDataContainerArrays.clear();
  FilterPipeline::FilterContainerType filters = m_TempPipeline->getFilterContainer();
  for(FilterPipeline::FilterContainerType::size_type i = 0; i < filters.size(); i++)
  {
    m_PreflightDataContainerArrays.push_back(filters[i]->getDataContainerArray()->deepCopy(true));
  }

  // Move the FilterPipeline object into the thread that we just created.
  m_TempPipeline->moveToThread(m_WorkerThread);

  /* Connect the signal 'started()' from the QThread to the 'run' slot of the
   * PipelineBuilder object. Since the PipelineBuilder object has been moved to another
   * thread of execution and the actual QThread lives in *this* thread then the
   * type of connection will be a Queued connection.
   */
   // When the thread starts its event loop, start the PipelineBuilder going
  connect(m_WorkerThread, SIGNAL(started()), m_TempPipeline.get(), SLOT(run()));

  // When the PipelineBuilder ends then tell the QThread to stop its event loop
  m_PipelineConnection = connect(m_TempPipeline.get(), &FilterPipeline::pipelineFinished, [=]() { pipelineFinished(); });

  // When the QThread finishes, tell this object that it has finished.
  connect(m_WorkerThread, SIGNAL(finished()), this, SLOT(endPipelineThread()));

  m_WorkerThread->start();
  emit stdOutMessage("");
  emit stdOutMessage("<b>*************** PIPELINE STARTED ***************</b>");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItem::pipelineFinished()
{
  if((nullptr == m_TempPipeline) || (!m_PipelineConnection))
  {
    return;
  }

  if(m_TempPipeline->getCancel())
  {
    stdOutMessage("<b>*************** PIPELINE CANCELED ***************</b>");
  }
  else
  {
    stdOutMessage("<b>*************** PIPELINE FINISHED ***************</b>");
  }
  stdOutMessage("");

  // Put back the DataContainerArray for each filter at the conclusion of running
  // the pipeline. this keeps the data browser current and up to date.
  FilterPipeline::FilterContainerType filters = m_TempPipeline->getFilterContainer();
  for(FilterPipeline::FilterContainerType::size_type i = 0; i < filters.size(); i++)
  {
    filters[i]->setDataContainerArray(m_PreflightDataContainerArrays[i]);
  }

  m_TempPipeline->moveToThread(QApplication::instance()->thread());

  disconnect(m_PipelineConnection);
  m_WorkerThread->quit();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItem::endPipelineThread()
{
  if(nullptr == m_TempPipeline)
  {
    return;
  }

  if(!m_TempPipeline->getCancel())
  {
    // Emit the pipeline output if there were no errors during execution
    if(m_TempPipeline->getErrorCondition() == 0)
    {
      emit pipelineOutput(m_TempPipeline, m_TempPipeline->getDataContainerArray());
    }
  }
  m_TempPipeline->clearDataContainerArray();

  emit pipelineFinished();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItem::cancelPipeline()
{
  if(m_TempPipeline->getPipelineState() != FilterPipeline::PipelineState::Ready)
  {
    m_TempPipeline->cancelPipeline();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineItem::childIndex() const
{
  if(parent())
  {
    return parent()->indexOf(const_cast<PipelineItem*>(this));
  }

  return -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineItem::childCount() const
{
  return m_FilterItems.count();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineItem::insertChild(int position, AbstractPipelineItem* child)
{
  if(nullptr == dynamic_cast<PipelineFilterItem*>(child))
  {
    return false;
  }
  if(position < 0 || position > childCount())
  {
    return false;
  }

  AbstractFilter::Pointer filter = dynamic_cast<PipelineFilterItem*>(child)->getFilter();
  return m_TempPipeline->insert(position, filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineItem::insertChildren(int position, int count)
{
  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractPipelineItem* PipelineItem::child(int number) const
{
  if(number < 0 || number > m_FilterItems.count())
  {
    return nullptr;
  }

  AbstractFilter::Pointer filter = m_TempPipeline->getFilterContainer().at(number);
  if(m_FilterItems.contains(filter))
  {
    return m_FilterItems[filter];
  }
  return nullptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineItem::indexOf(AbstractPipelineItem* item) const
{
  PipelineFilterItem* filterItem = dynamic_cast<PipelineFilterItem*>(item);
  if(nullptr == filterItem)
  {
    return -1;
  }

  return m_TempPipeline->indexOf(filterItem->getFilter());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QModelIndex PipelineItem::pipelineIndex() const
{
  return model()->index(childIndex(), 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QModelIndex PipelineItem::firstFilterIndex() const
{
  if(childCount() > 0)
  {
    return model()->index(0, 0, pipelineIndex());
  }
  
  return QModelIndex();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QModelIndex PipelineItem::lastFilterIndex() const
{
  if(childCount() > 1)
  {
    return model()->index(childCount() - 1, 0, pipelineIndex());
  }
  
  return firstFilterIndex();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineItem::insertFilter(int position, AbstractFilter::Pointer filter)
{
  if(nullptr == filter || false == m_TempPipeline->getFilterContainer().contains(filter))
  {
    return false;
  }
  if(m_TempPipeline->insert(position, filter))
  {
    m_FilterItems[filter] = new PipelineFilterItem(filter, this);
    setupFilterInputWidget(filter);
    return true;
  }
  
  return false;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineItem::insertFilters(int position, QVector<AbstractFilter::Pointer> filters)
{
  if(position < 0 || position > m_TempPipeline->size())
  {
    return false;
  }

  for(AbstractFilter::Pointer filter : filters)
  {
    insertFilter(position, filter);
    position++;
  }
  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineItem::removeChild(int position)
{
  PipelineFilterItem* item = dynamic_cast<PipelineFilterItem*>(child(position));
  if(item && m_TempPipeline->erase(position))
  {
    delete item;
    return true;
  }

  return false;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineItem::removeChildren(int position, int count)
{
  if(position < 0 || position + count > m_TempPipeline->size())
  {
    return false;
  }

  for(int row = 0; row < count; ++row)
  {
    AbstractPipelineItem* item = dynamic_cast<PipelineFilterItem*>(child(position));
    if(item && m_TempPipeline->erase(position))
    {
      delete item;
    }
    else
    {
      return false;
    }
  }

  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItem::updateFilterInputWidgets()
{
  for(FilterInputWidget* fiw : m_FilterInputWidgets.values())
  {
    fiw->deleteLater();
  }
  m_FilterInputWidgets.clear();

  // Instantiate the filter input widget object
  for(AbstractFilter::Pointer filter : *m_TempPipeline)
  {
    setupFilterInputWidget(filter);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItem::setupFilterInputWidget(AbstractFilter::Pointer filter)
{
  FilterInputWidget* fiw = new FilterInputWidget(filter, nullptr);
  connect(fiw, &FilterInputWidget::filterParametersChanged, this, [=](bool preflight) { 
    if(preflight)
    {
      m_TempPipeline->preflightPipeline(); 
    }});
  m_FilterInputWidgets[filter] = fiw;
  m_FilterInputWidgets[filter]->displayFilterParameters(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FilterInputWidget* PipelineItem::getFilterInputWidget(AbstractFilter::Pointer filter) const
{
  if(m_FilterInputWidgets.contains(filter))
  {
    return m_FilterInputWidgets[filter];
  }

  return nullptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FilterPipeline::FilterState PipelineItem::getFilterState(AbstractFilter::Pointer filter) const
{
  return m_TempPipeline->getFilterState(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItem::addFilter(int index, AbstractFilter::Pointer filter)
{
  PipelineFilterItem* filterItem = new PipelineFilterItem(filter, this);
  m_FilterItems[filter] = filterItem;
  setupFilterInputWidget(filter);
  emit filterAdded(index, filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItem::removeFilter(int index, AbstractFilter::Pointer filter)
{
  auto iter = m_FilterItems.find(filter);
  if(nullptr != iter.value())
  {
    m_FilterItems.erase(iter);
    m_FilterInputWidgets.erase(m_FilterInputWidgets.find(filter));
    emit filterRemoved(index, filter);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QVariant PipelineItem::data(int role) const
{
  if(role == Qt::DisplayRole)
  {
    QString pipelineName = m_SavedPipeline->getName();
    if(pipelineName.isEmpty())
    {
      return "Untitled";
    }
    else
    {
      return pipelineName;
    }
  }
  else if(role == Qt::DecorationRole)
  {
    switch(m_TempPipeline->getErrorState())
    {
    case FilterPipeline::ErrorState::Ok:
      return QColor(Qt::GlobalColor::green);
    case FilterPipeline::ErrorState::Error:
      return QColor(Qt::GlobalColor::red);
    case FilterPipeline::ErrorState::Warning:
      return QColor(Qt::GlobalColor::darkYellow);
    }
  }
  else if(role == Qt::ToolTipRole)
  {
    return "";
    //return m_SavedPipeline->filePath();
  }
  else if(role == PipelineModel::Roles::ErrorStateRole)
  {
    return static_cast<int>(m_TempPipeline->getErrorState());
  }
  else if(role == PipelineModel::Roles::PipelineStateRole)
  {
    return static_cast<int>(m_TempPipeline->getPipelineState());
  }
  else if(role == PipelineModel::Roles::ItemTypeRole)
  {
    return static_cast<int>(ItemType::Pipeline);
  }
  else if(role == PipelineModel::Roles::ItemTypeRole)
  {
    return static_cast<int>(getItemType());
  }

  return QVariant();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineItem::setData(int role, const QVariant& value)
{
  return false;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Qt::ItemFlags PipelineItem::flags() const
{
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QList<QObject*> PipelineItem::getPipelineMessageObservers() const
{
  return m_PipelineMessageObservers;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItem::addPipelineMessageObserver(QObject* observer)
{
  m_PipelineMessageObservers.push_back(observer);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItem::clearIssues()
{
  // TODO: clear pipeline issues and FilterStates
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItem::stdOutMessage(const QString& msg)
{
  m_StdOutMsg += msg + "\n";
}