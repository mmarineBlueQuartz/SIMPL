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
#include <QtCore/QSize>
#include <QtCore/QVariant>
#include <QtCore/QVector>

//#include <QtGui/QIcon>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

#include "SVWidgetsLib/SVWidgetsLib.h"

class FilterInputWidget;

class SVWidgetsLib_EXPORT AbstractPipelineItem
{
public:
  AbstractPipelineItem(const QVector<QVariant>& data, AbstractPipelineItem* parent = nullptr);
  virtual ~AbstractPipelineItem();

  //SIMPL_POINTER_PROPERTY(FilterInputWidget, FilterInputWidget)
  //SIMPL_INSTANCE_PROPERTY(bool, FilterEnabled)
  //SIMPL_BOOL_PROPERTY(ActivePipeline)
  //SIMPL_BOOL_PROPERTY(PipelineSaved)
  //SIMPL_INSTANCE_PROPERTY(QIcon, Icon)
  //SIMPL_INSTANCE_PROPERTY(bool, Expanded)
  //SIMPL_INSTANCE_PROPERTY(QString, ItemTooltip)
  //SIMPL_INSTANCE_PROPERTY(QString, DropIndicatorText)
  SIMPL_INSTANCE_PROPERTY(int, BorderSize)
  SIMPL_INSTANCE_PROPERTY(QSize, Size)
  SIMPL_INSTANCE_PROPERTY(int, Height)
  SIMPL_INSTANCE_PROPERTY(int, Width)
  SIMPL_INSTANCE_PROPERTY(int, XOffset)
  SIMPL_INSTANCE_PROPERTY(int, YOffset)

  static const int MaxHeight = 28;

  enum AnimationType
  {
    None,
    Add,
    Remove
  };

  SIMPL_INSTANCE_PROPERTY(AnimationType, CurrentAnimationType)

  enum AbstractPipelineItemData
  {
    Contents
  };

  using EnumType = unsigned int;

#if 0
  enum class WidgetState : EnumType
  {
    Ready = 0,     //!<
    Executing = 1, //!<
    Completed = 2, //!<
    Disabled = 3
  };
  SIMPL_INSTANCE_PROPERTY(WidgetState, WidgetState)

  enum class PipelineState : EnumType
  {
    Running = 0,
    Stopped = 1,
    Paused = 4,
  };
  SIMPL_INSTANCE_PROPERTY(PipelineState, PipelineState)

  enum class ErrorState : EnumType
  {
    Ok = 0,
    Error = 1,
    Warning = 2,
  };
  SIMPL_INSTANCE_PROPERTY(ErrorState, ErrorState)
#endif


  virtual AbstractPipelineItem* child(int number) = 0;
  virtual AbstractPipelineItem* parent() = 0;

  virtual int childCount() const = 0;
  virtual int columnCount() const = 0;

  virtual QVariant data(int column) const = 0;
  virtual bool setData(int column, const QVariant& value) = 0;

  virtual bool insertChild(int position, AbstractPipelineItem* child) = 0;
  virtual bool insertChildren(int position, int count, int columns) = 0;

  virtual bool removeChild(int position) = 0;
  virtual bool removeChildren(int position, int count) = 0;

  virtual int childNumber() const = 0;

  virtual void setParent(AbstractPipelineItem* parent);

  static QString TopLevelString();

private:
  AbstractPipelineItem* m_ParentItem;

public:
  AbstractPipelineItem(const AbstractPipelineItem&) = delete;            // Copy Constructor Not Implemented
  AbstractPipelineItem(AbstractPipelineItem&&) = delete;                 // Move Constructor Not Implemented
  AbstractPipelineItem& operator=(const AbstractPipelineItem&) = delete; // Copy Assignment Not Implemented
  AbstractPipelineItem& operator=(AbstractPipelineItem&&) = delete;      // Move Assignment Not Implemented
};
