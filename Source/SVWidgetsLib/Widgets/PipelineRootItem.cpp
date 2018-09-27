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

#include "PipelineRootItem.h"

#include "SVWidgetsLib/Widgets/PipelineFilterItem.h"
#include "SVWidgetsLib/Widgets/PipelineModel.h"
#include "SVWidgetsLib/Widgets/PipelineItem.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PipelineRootItem::PipelineRootItem(PipelineModel* parent)
: AbstractPipelineItem(nullptr)
, m_Model(parent)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PipelineRootItem::~PipelineRootItem()
{
  qDeleteAll(m_PipelineItems);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString PipelineRootItem::TopLevelString()
{
  return QString::fromLatin1("[Top Level]");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractPipelineItem::ItemType PipelineRootItem::getItemType() const
{
  return ItemType::Root;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineRootItem::childIndex() const
{
  return -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineRootItem::childCount() const
{
  return m_PipelineItems.count();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineRootItem::insertChild(int position, AbstractPipelineItem* child)
{
  FilterPipeline::Pointer pipeline = FilterPipeline::New();
  return insertPipeline(position, pipeline);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineRootItem::insertChildren(int position, int count)
{
  for(int i = 0; i < count; i++)
  {
    FilterPipeline::Pointer pipeline = FilterPipeline::New();
    if(false == insertPipeline(position + count, pipeline))
    {
      return false;
    }
  }

  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineRootItem::removeChild(int position)
{
  if(position < 0 || position >= childCount())
  {
    return false;
  }

  m_PipelineItems.removeAt(position);
  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineRootItem::removeChildren(int position, int count)
{
  if(position < 0 || position + count > childCount())
  {
    return false;
  }

  for(int index = position; index < position + count; index++)
  {
    removeChild(index);
  }
  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractPipelineItem* PipelineRootItem::child(int number) const
{
  if(number < 0 || number >= m_PipelineItems.count())
  {
    return nullptr;
  }

  return m_PipelineItems[number];
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineRootItem::indexOf(AbstractPipelineItem* item) const
{
  PipelineItem* pipelineItem = dynamic_cast<PipelineItem*>(item);
  if(nullptr == pipelineItem)
  {
    return -1;
  }

  return m_PipelineItems.indexOf(pipelineItem);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineRootItem::indexOf(FilterPipeline::Pointer pipeline) const
{
  if(nullptr == pipeline)
  {
    return -1;
  }

  int index = 0;
  for(auto iter = m_PipelineItems.begin(); iter != m_PipelineItems.end(); iter++, index++)
  {
    if((*iter)->getSavedPipeline() == pipeline)
    {
      return index;
    }
  }

  return -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PipelineItem* PipelineRootItem::insertPipeline(int position, FilterPipeline::Pointer pipeline)
{
  if(nullptr == pipeline)
  {
    return false;
  }

  if(position < childCount())
  {
    position = childCount();
  }

  PipelineItem* pipelineItem = new PipelineItem(pipeline, this);
  m_PipelineItems.insert(position, pipelineItem);
  model()->connectPipelineItem(pipelineItem);
  emit modified();
  return pipelineItem;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PipelineItem* PipelineRootItem::appendPipeline(FilterPipeline::Pointer pipeline)
{
  return insertPipeline(childCount(), pipeline);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineRootItem::removePipelineAtIndex(int position)
{
  if(position < 0 || position >= childCount())
  {
    return false;
  }

  PipelineItem* item = m_PipelineItems.takeAt(position);
  m_PipelineItems.removeAt(position);
  delete item;
  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineRootItem::removePipeline(FilterPipeline::Pointer pipeline)
{
  return removePipelineAtIndex(indexOf(pipeline));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PipelineModel* PipelineRootItem::model() const
{
  return m_Model;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QVariant PipelineRootItem::data(int role) const
{
  if(role == Qt::DisplayRole)
  {
    return TopLevelString();
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
bool PipelineRootItem::setData(int role, const QVariant& value)
{
  return false;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Qt::ItemFlags PipelineRootItem::flags() const
{
  return Qt::ItemIsDropEnabled;
}
