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

#include "AbstractPipelineItem.h"

#include <QtCore/QStringList>
#include <QtGui/QColor>

#include "SVWidgetsLib/Widgets/FilterInputWidget.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractPipelineItem::AbstractPipelineItem(AbstractPipelineItem* parent)
: QObject(parent)
, m_ParentItem(parent)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractPipelineItem* AbstractPipelineItem::parent() const
{
  return m_ParentItem;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AbstractPipelineItem::setParent(AbstractPipelineItem* parent)
{
  QObject::setParent(parent);
  m_ParentItem = parent;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int AbstractPipelineItem::rowCount() const
{
  return childCount();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int AbstractPipelineItem::columnCount() const
{
  return 1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PipelineModel* AbstractPipelineItem::model() const
{
  if(parent())
  {
    return parent()->model();
  }

  return nullptr;
}
