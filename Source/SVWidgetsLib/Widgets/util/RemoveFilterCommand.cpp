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

#include "RemoveFilterCommand.h"

//#include "SVWidgetsLib/Animations/PipelineItemSlideAnimation.h"
#include "SVWidgetsLib/Widgets/FilterInputWidget.h"
#include "SVWidgetsLib/Widgets/PipelineModel.h"
#include "SVWidgetsLib/Widgets/SVPipelineView.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
RemoveFilterCommand::RemoveFilterCommand(AbstractFilter::Pointer filter, PipelineModel* model, QString actionText, bool useAnimationOnFirstRun, QUndoCommand* parent)
: QUndoCommand(parent)
, m_Model(model)
, m_UseAnimationOnFirstRun(useAnimationOnFirstRun)
{
  if(nullptr == filter || nullptr == model)
  {
    return;
  }

  setText(QObject::tr("\"%1 '%2'\"").arg(actionText).arg(filter->getHumanLabel()));

  m_Filters.push_back(filter);

  QModelIndex modelIndex = m_Model->indexOfFilter(filter);
  FilterPipeline::Pointer pipeline = m_Model->getPipelineContaining(modelIndex);
  m_Pipelines.push_back(pipeline);
  int index = pipeline->indexOf(filter);
  m_FilterRows.push_back(index);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
RemoveFilterCommand::RemoveFilterCommand(std::vector<AbstractFilter::Pointer> filters, PipelineModel* model, QString actionText, bool useAnimationOnFirstRun, QUndoCommand* parent)
: QUndoCommand(parent)
, m_Model(model)
, m_Filters(filters)
, m_UseAnimationOnFirstRun(useAnimationOnFirstRun)
{
  if(nullptr == model)
  {
    return;
  }

  setText(QObject::tr("\"%1 %2 Filters\"").arg(actionText).arg(filters.size()));

  for(AbstractFilter::Pointer filter : filters)
  {
    QModelIndex modelIndex = m_Model->indexOfFilter(filter);
    FilterPipeline::Pointer pipeline = m_Model->getPipelineContaining(modelIndex);
    m_Pipelines.push_back(pipeline);
    int index = pipeline->indexOf(filter);
    m_FilterRows.push_back(index);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
RemoveFilterCommand::~RemoveFilterCommand() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveFilterCommand::undo()
{
  for(size_t i = 0; i < m_FilterRows.size(); i++)
  {
    int insertIndex = m_FilterRows[i];
    AbstractFilter::Pointer filter = m_Filters[i];
    FilterPipeline::Pointer pipeline = m_Pipelines[i];

    addFilter(filter, pipeline, insertIndex);
  }

  std::set<FilterPipeline::Pointer> pipelines = getPipelines();
  for(FilterPipeline::Pointer pipeline : pipelines)
  {
    pipeline->preflightPipeline();
    emit pipeline->pipelineWasEdited();
  }

  QString statusMessage;
  if(m_Filters.size() > 1)
  {
    QString indexesString = QObject::tr("%1").arg(m_FilterRows[0] + 1);
    for(size_t i = 1; i < m_FilterRows.size(); i++)
    {
      indexesString.append(", ");
      indexesString.append(QObject::tr("%1").arg(m_FilterRows[i] + 1));
    }
    statusMessage = QObject::tr("Undo \"Removed %1 filters at indexes %2\"").arg(m_Filters.size()).arg(indexesString);
  }
  else
  {
    statusMessage = QObject::tr("Undo \"Removed '%1' filter at index %2\"").arg(m_Filters[0]->getHumanLabel()).arg(m_FilterRows[0] + 1);
  }

  //emit m_PipelineView->statusMessage(statusMessage);
  //emit m_PipelineView->stdOutMessage(statusMessage);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool variantCompare(const QVariant& v1, const QVariant& v2)
{
  return v1.toInt() > v2.toInt();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveFilterCommand::redo()
{
  for(size_t i = 0; i < m_Filters.size(); i++)
  {
    removeFilter(m_Filters[i]);
  }

  QString statusMessage;
  if(m_Filters.size() > 1)
  {
    QString indexesString = QObject::tr("%1").arg(m_FilterRows[0] + 1);
    for(size_t i = 1; i < m_FilterRows.size(); i++)
    {
      indexesString.append(", ");
      indexesString.append(QObject::tr("%1").arg(m_FilterRows[i] + 1));
    }
    statusMessage = QObject::tr("Removed %1 filters at indexes %2").arg(m_Filters.size()).arg(indexesString);
  }
  else
  {
    statusMessage = QObject::tr("Removed '%1' filter at index %2").arg(m_Filters[0]->getHumanLabel()).arg(m_FilterRows[0] + 1);
  }

  if(m_FirstRun == false)
  {
    statusMessage.prepend("Redo \"");
    statusMessage.append('\"');
  }
  else
  {
    m_FirstRun = false;
  }

  std::set<FilterPipeline::Pointer> pipelines = getPipelines();
  for(FilterPipeline::Pointer pipeline : pipelines)
  {
    pipeline->preflightPipeline();
    emit pipeline->pipelineWasEdited();
  }

  /*emit m_PipelineView->statusMessage(statusMessage);
  emit m_PipelineView->stdOutMessage(statusMessage);*/
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveFilterCommand::addFilter(AbstractFilter::Pointer filter, FilterPipeline::Pointer pipeline, int insertionIndex)
{
  filter->setRemoving(false);

  pipeline->insert(insertionIndex, filter);

#if 0
  QRect filterRect = m_PipelineView->visualRect(filterIndex);

  PipelineItemSlideAnimation* animation = new PipelineItemSlideAnimation(model, QPersistentModelIndex(filterIndex), filterRect.width(), PipelineItemSlideAnimation::AnimationDirection::EnterRight);
  model->setData(QPersistentModelIndex(filterIndex), PipelineItem::AnimationType::Add, PipelineModel::Roles::AnimationTypeRole);

  QObject::connect(animation, &PipelineItemSlideAnimation::finished,
                   [=] { model->setData(QPersistentModelIndex(filterIndex), PipelineItem::AnimationType::None, PipelineModel::Roles::AnimationTypeRole); });
  animation->start(QAbstractAnimation::DeleteWhenStopped);
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveFilterCommand::removeFilter(AbstractFilter::Pointer filter)
{
  // Check if the given filter is already being removed before removing it again
  // Multiple calls to remove the same object causes crashes.
  if(filter->getRemoving())
  {
    return;
  }
  filter->setRemoving(true);

  QModelIndex index = m_Model->indexOfFilter(filter);
  FilterPipeline::Pointer pipeline = m_Model->getPipelineContaining(index);
  pipeline->removeFilter(filter);

#if 0
  QModelIndex index = model->indexOfFilter(filter.get());
  QPersistentModelIndex persistentIndex = index;
  QRect filterRect = m_PipelineView->visualRect(index);

  if(m_UseAnimationOnFirstRun == false && m_FirstRun == true)
  {
    model->removeRow(persistentIndex.row());
  }
  else
  {
    PipelineItemSlideAnimation* animation = new PipelineItemSlideAnimation(model, persistentIndex, filterRect.width(), PipelineItemSlideAnimation::AnimationDirection::ExitRight);
    model->setData(persistentIndex, PipelineItem::AnimationType::Remove, PipelineModel::Roles::AnimationTypeRole);

    QObject::connect(animation, &PipelineItemSlideAnimation::finished, [=] { model->removeRow(persistentIndex.row()); });
    animation->start(QAbstractAnimation::DeleteWhenStopped);
  }
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::set<FilterPipeline::Pointer> RemoveFilterCommand::getPipelines() const
{
  return std::set<FilterPipeline::Pointer> (m_Pipelines.begin(), m_Pipelines.end());
}
