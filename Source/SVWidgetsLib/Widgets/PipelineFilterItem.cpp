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

#include "PipelineFilterItem.h"

#include <QtCore/QStringList>
#include <QtGui/QColor>

#include "SVWidgetsLib/Widgets/FilterInputWidget.h"
#include "SVWidgetsLib/Widgets/PipelineItem.h"
#include "SVWidgetsLib/Widgets/PipelineModel.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PipelineFilterItem::PipelineFilterItem(AbstractFilter::Pointer filter, PipelineItem* parent)
: AbstractPipelineItem(parent)
{
  setFilter(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractPipelineItem::ItemType PipelineFilterItem::getItemType() const
{
  return ItemType::Filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QVariant PipelineFilterItem::data(int role) const
{
  if(role == Qt::DisplayRole)
  {
    return m_Filter->getHumanLabel();
  }
  else if(role == Qt::DecorationRole)
  {
    if(m_Filter->getErrorCondition() < 0)
    {
      return QIcon(":/SIMPL/icons/images/bullet_ball_red.png");
      //return QColor(Qt::GlobalColor::red);
    }
    else if(m_Filter->getWarningCondition() < 0)
    {
      return QIcon(":/SIMPL/icons/images/warning.png");
      //return QColor(Qt::GlobalColor::darkYellow);
    }
    else
    {
      return QVariant();
    }
  }
  else if(role == Qt::ToolTipRole)
  {
    return getToolTip();
  }
  else if(role == PipelineModel::Roles::ItemTypeRole)
  {
    return static_cast<int>(getItemType());
  }
  else if(role == PipelineModel::Roles::FilterStateRole)
  {
    return static_cast<int>(parentPipeline()->getFilterState(getFilter()));
  }

  return QVariant();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineFilterItem::setData(int role, const QVariant& value)
{
  if(role == PipelineModel::Roles::FilterStateRole)
  {
    // Set enabled / disabled state through  the FilterStateRole
    FilterPipeline::FilterState state = static_cast<FilterPipeline::FilterState>(value.toInt());
    if(FilterPipeline::FilterState::Disabled == state)
    {
      getFilter()->setEnabled(false);
    }
    else
    {
      getFilter()->setEnabled(true);
    }
    return true;
  }

  return false;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Qt::ItemFlags PipelineFilterItem::flags() const
{
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer PipelineFilterItem::getFilter() const
{
  return m_Filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineFilterItem::setFilter(AbstractFilter::Pointer filter)
{
  m_Filter = filter;
  connect(filter.get(), &AbstractFilter::filterGeneratedMessage, this, &PipelineFilterItem::processPipelineMessage);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineFilterItem::processPipelineMessage(const PipelineMessage& msg)
{
  switch(msg.getType())
  {
    case PipelineMessage::MessageType::Error:
      m_ErrorMessages.push_back(msg);
      break;
    case PipelineMessage::MessageType::Warning:
      m_WarningMessages.push_back(msg);
      break;
    default:
      break;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FilterInputWidget* PipelineFilterItem::getFilterInputWidget() const
{
  return parentPipeline()->getFilterInputWidget(getFilter());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString PipelineFilterItem::getToolTip() const
{
  QString tooltip;
  QString divider = "::";
  tooltip += m_Filter->getGroupName() + divider + m_Filter->getSubGroupName() + divider + m_Filter->getHumanLabel();

  if(!m_ErrorMessages.empty() || !m_WarningMessages.empty())
  {
    tooltip += "\n";
  }

  // Begin Errors
  for(PipelineMessage msg : m_ErrorMessages)
  {
    tooltip += "\nError (" + QString::number(msg.getCode()) + "): " + msg.getText();
  }

  // Begin Warnings
  for(PipelineMessage msg : m_WarningMessages)
  {
    if(!tooltip.isEmpty())
    {
      tooltip += "\n";
    }
    tooltip += "Warning (" + QString::number(msg.getCode()) + "): " + msg.getText();
  }

  return tooltip;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineFilterItem::clearMessages()
{
  m_ErrorMessages.clear();
  m_WarningMessages.clear();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PipelineItem* PipelineFilterItem::parentPipeline() const
{
  return dynamic_cast<PipelineItem*>(parent());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineFilterItem::childIndex() const
{
  if(parentPipeline())
  {
    return parentPipeline()->indexOf(const_cast<PipelineFilterItem*>(this));
  }

  return -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineFilterItem::childCount() const
{
  return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractPipelineItem* PipelineFilterItem::child(int index) const
{
  return nullptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineFilterItem::indexOf(AbstractPipelineItem* item) const
{
  return -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineFilterItem::insertChild(int position, AbstractPipelineItem* child)
{
  return false;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineFilterItem::insertChildren(int position, int count)
{
  return false;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineFilterItem::removeChild(int position)
{
  return false;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineFilterItem::removeChildren(int position, int count)
{
  return false;
}
