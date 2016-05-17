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

#include <QtCore/QObject>

#include "SVWidgetsLib/Widgets/PipelineFilterObject.h"
#include "SVWidgetsLib/Widgets/PipelineView.h"

#include "SIMPLib/FilterParameters/JsonFilterParametersReader.h"
#include "SIMPLib/FilterParameters/JsonFilterParametersWriter.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
RemoveFilterCommand::RemoveFilterCommand(PipelineFilterObject* fw, PipelineView *pipelineView, QString actionText, QUndoCommand* parent) :
QUndoCommand(parent),
m_PipelineView(pipelineView)
{
  if (NULL == fw || NULL == pipelineView)
  {
    return;
  }

  m_RemovalValues.push_back(pipelineView->valueOfFilterWidget(fw));

  FilterPipeline::Pointer pipeline = FilterPipeline::New();
  PipelineFilterObject* filterWidget = m_PipelineView->filterObjectAt(m_RemovalValues[0]);
  pipeline->pushBack(filterWidget->getFilter());

  m_JsonString = JsonFilterParametersWriter::WritePipelineToString(pipeline, "Pipeline");

  setText(QObject::tr("\"%1 '%2'\"").arg(actionText).arg(pipeline->getFilterContainer()[0]->getHumanLabel()));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
RemoveFilterCommand::RemoveFilterCommand(QList<PipelineFilterObject*> filterWidgets, PipelineView* pipelineView, QString actionText, QUndoCommand* parent) :
QUndoCommand(parent),
m_PipelineView(pipelineView)
{
  if (filterWidgets.size() <= 0 || NULL == pipelineView)
  {
    return;
  }

  for (int i=0; i<filterWidgets.size(); i++)
  {
    m_RemovalValues.push_back(pipelineView->valueOfFilterWidget(filterWidgets[i]));
  }

  FilterPipeline::Pointer pipeline = FilterPipeline::New();
  for (int i=0; i<m_RemovalValues.size(); i++)
  {
    PipelineFilterObject* filterWidget = m_PipelineView->filterObjectAt(m_RemovalValues[i]);
    pipeline->pushBack(filterWidget->getFilter());
  }

  if (m_RemovalValues.size() == 1)
  {
    setText(QObject::tr("\"%1 '%2'\"").arg(actionText).arg(pipeline->getFilterContainer()[0]->getHumanLabel()));
  }
  else
  {
    setText(QObject::tr("\"%1 %2 Filter Widgets\"").arg(actionText).arg(pipeline->getFilterContainer().size()));
  }

  m_JsonString = JsonFilterParametersWriter::WritePipelineToString(pipeline, "Pipeline");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
RemoveFilterCommand::~RemoveFilterCommand()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveFilterCommand::undo()
{
  FilterPipeline::Pointer pipeline = JsonFilterParametersReader::ReadPipelineFromString(m_JsonString);
  QList<AbstractFilter::Pointer> container = pipeline->getFilterContainer();

  m_PipelineView->clearSelectedFilterObjects();

  for (int i=0; i<container.size(); i++)
  {
    SVPipelineFilterWidget* filterWidget = new SVPipelineFilterWidget(container[i], NULL, NULL);
    m_PipelineView->addFilterWidget(filterWidget, m_RemovalValues[i], false);
    m_PipelineView->setSelectedFilterObject(filterWidget, Qt::ShiftModifier);
  }

  m_PipelineView->preflightPipeline();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveFilterCommand::redo()
{
  QList<PipelineFilterObject*> filterWidgets;
  for (int i=0; i<m_RemovalValues.size(); i++)
  {
    filterWidgets.push_back(m_PipelineView->filterObjectAt(m_RemovalValues[i]));
  }

  for (int i=0; i<filterWidgets.size(); i++)
  {
    PipelineFilterObject* filterWidget = filterWidgets[i];
    m_PipelineView->removeFilterWidget(filterWidget, false);
  }

  m_PipelineView->preflightPipeline();
}



