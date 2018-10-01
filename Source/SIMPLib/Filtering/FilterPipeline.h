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

#pragma once

#include <QtCore/QJsonObject>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTextStream>

#include "SIMPLib/Common/Observer.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

class IObserver;

/**
 * @class FilterPipeline FilterPipeline.h DREAM3DLib/Common/FilterPipeline.h
 * @brief  This class holds the list of filters that will be run. This should be
 * the class (or a subclass) that you use to build up a pipeline in order to run
 * it.
 *
 * @date Sep 28, 2011
 * @version 1.0
 */
class SIMPLib_EXPORT FilterPipeline : public QObject
{
  Q_OBJECT
  
  PYB11_CREATE_BINDINGS(FilterPipeline)
  PYB11_PROPERTY(int ErrorCondition READ getErrorCondition WRITE setErrorCondition)
  PYB11_PROPERTY(AbstractFilter CurrentFilter READ getCurrentFilter WRITE setCurrentFilter)
  PYB11_PROPERTY(bool Cancel READ getCancel WRITE setCancel)
  PYB11_PROPERTY(QString Name READ getName WRITE setName)
  
  PYB11_METHOD(DataContainerArray::Pointer run)
  PYB11_METHOD(void preflightPipeline)
  PYB11_METHOD(void pushFront ARGS AbstractFilter)
  PYB11_METHOD(void pushBack ARGS AbstractFilter)
  PYB11_METHOD(void popFront)
  PYB11_METHOD(void popBack)
  PYB11_METHOD(void clear)
  PYB11_METHOD(size_t size)
  PYB11_METHOD(bool empty)
 
 
public:
  using EnumType = unsigned int;
  enum class PipelineState : EnumType
  {
    Ready = 0,
    Running = 1,
    Cancelling = 2,
    Paused = 3,
  };

  enum class ErrorState : EnumType
  {
    Ok = 0,
    Error = 1,
    Warning = 2,
  };

  enum class FilterState : EnumType
  {
    Ready = 0,      //!<
    Executing = 1, //!<
    Completed = 2, //!<
    Disabled = 3
  };


  SIMPL_SHARED_POINTERS(FilterPipeline)
  SIMPL_TYPE_MACRO(FilterPipeline)
  SIMPL_STATIC_NEW_MACRO(FilterPipeline)

  ~FilterPipeline() override;

  typedef QList<AbstractFilter::Pointer> FilterContainerType;

  SIMPL_INSTANCE_PROPERTY(int, ErrorCondition)
  SIMPL_INSTANCE_PROPERTY(AbstractFilter::Pointer, CurrentFilter)

  ErrorState getErrorState() const;

  /**
   * @brief Cancel the operation
   */
  virtual void setCancel(bool value);
  virtual bool getCancel();

  /**
   * @brief A pure virtual function that gets called from the "run()" method. Subclasses
   * are expected to create a concrete implementation of this method.
   */
  virtual DataContainerArray::Pointer execute();

  /**
   * @brief This will preflight the pipeline and report any errors that would occur during
   * execution of the pipeline
   */
  virtual int preflightPipeline();

  /**
   * @brief Returns the FilterState for the given filter
   * @param filter
   * @return
   */
  FilterState getFilterState(AbstractFilter::Pointer filter) const;

  /**
   * @brief
   */
  virtual bool pushFront(AbstractFilter::Pointer f);
  virtual bool popFront();
  virtual bool pushBack(AbstractFilter::Pointer f);
  virtual bool popBack();
  virtual bool insert(size_t index, AbstractFilter::Pointer f);
  virtual bool erase(size_t index);
  virtual bool clear();
  virtual size_t size() const;
  virtual bool empty() const;
  virtual int indexOf(AbstractFilter::Pointer filter) const;

  FilterContainerType::iterator begin();
  FilterContainerType::const_iterator begin() const;
  FilterContainerType::iterator end();
  FilterContainerType::const_iterator end() const;
  
  virtual FilterContainerType& getFilterContainer();

  /**
   * @brief Returns the DataContainerArray stored by the FilterPipeline
   * @return
   */
  virtual DataContainerArray::Pointer getDataContainerArray();

  /**
   * @brief Sets the stored DataContainerArray::Pointer to nullptr so that memory can be cleared.
   */
  void clearDataContainerArray();

  /**
   * @brief
   */
  virtual AbstractFilter::Pointer removeFirstFilterByName(const QString& name);

  virtual bool removeFilter(AbstractFilter::Pointer filter);

  virtual void printFilterNames(QTextStream& out);

  /**
   * @brief This method adds a QObject based class that is capable of being connected with the following signals from
   * AbstractFilter:
   * @li processPipelineMessage(PipelineMessage&)
   * @param obj Class that implements needed processPipelineMessage(PipelineMessage&) method
   */
  void addMessageReceiver(QObject* obj);

  void connectFilterNotifications(QObject* filter);
  void disconnectFilterNotifications(QObject* filter);

  /**
   * @brief Returns the FilterPipeline's name
   * @return
   */
  QString getName();

  /**
   * @brief Returns the PipelineState
   * @return
   */
  PipelineState getPipelineState() const;

  /**
  * @brief This method returns a deep copy of the FilterPipeline and all its filters
  * @return
  */
  virtual Pointer deepCopy();

  /**
  * @brief Returns the FilterPipeline contents as a JSon string
  * @return
  */
  virtual QJsonObject toJson() const;

  /**
  * @brief Sets the contents of the FilterPipeline to match the given JSon value.
  */
  virtual void fromJson(const QJsonObject& json, IObserver* obs = nullptr);

  /**
  * @brief Static version of fromJson that creates a FilterPipeline::Pointer.
  * If the meta data says there are less than zero filters, it returns a FilterPipeline::NullPointer
  * @return
  */
  static Pointer FromJson(const QJsonObject& json, IObserver* obs = nullptr);

public slots:

  /**
   * @brief This method is called to start the pipeline for a plugin
   */
  virtual DataContainerArray::Pointer run();

  /**
   * @brief cancelPipeline
   */
  virtual void cancelPipeline();

  /**
   * @brief Sets the FilterPipeline's name
   * @param name
   */
  void setName(QString name);

protected:
  FilterPipeline();

  void updatePrevNextFilters();

signals:
  void pipelineGeneratedMessage(const PipelineMessage& message);

  void preflightStarted();

  /**
  * @brief This method is emitted from the pipeline and signals the pipeline has started execution
  */
  void pipelineStarted();

  /**
  * @brief This method is emitted from the pipeline and signals a pipeline pause
  */
  void pipelineHasPaused();

  /**
  * @brief This method is emitted from the pipeline and signals that the pipeline is resuming
  */
  void pipelineIsResuming();

  /**
  * @brief This method is emitted from the pipeline and signals a pipeline cancel
  */
  void pipelineCanceled();

  /**
  * @brief This signal is emitted from the run() method just before exiting and
  * signals the end of the pipeline execution
  */
  void pipelineFinished();

  /**
  * @brief The signal is emitted when changes are applied to the FilterPipeline
  */
  void pipelineWasEdited();
  
  /**
  * @brief This signal is emitted before a filter is added
  * @param position
  */
  void beginAddingFilter(int position);

  /**
   * @brief This signal is emitted when a filter is added
   * @param position
   * @param filter
   */
  void filterAdded(int position, AbstractFilter::Pointer filter);

  /**
   * @brief This signal is called before a filter is removed
   * @return position
   */
  void beginRemovingFilter(int position);

  /**
   * @brief This signal is emitted when a filter is removed
   * @param position
   * @param filter
   */
  void filterRemoved(int position, AbstractFilter::Pointer filter);

  void beginClearingPipeline();

  /**
   * @brief This signal is emitted when the pipeline is cleared
   */
  void pipelineCleared();

  /**
  * @brief This signal is emitted when the pipeline name changes
  * @param oldName The FilterPipeline's previous name
  * @param newName The FilterPipeline's current name
  */
  void pipelineNameChanged(QString oldName, QString newName);

  /**
   * @brief Provides the DataContainerArray from an executed pipeline.
   * @param dca
   */
  void pipelineOutput(DataContainerArray::Pointer dca);

private:
  bool m_Cancel;
  FilterContainerType m_Pipeline;
  QString m_PipelineName;
  PipelineState m_PipelineState = PipelineState::Ready;
  ErrorState m_ErrorState = ErrorState::Ok;

  QVector<QObject*> m_MessageReceivers;

  DataContainerArray::Pointer m_Dca;

  void connectSignalsSlots();
  void disconnectSignalsSlots();

  FilterPipeline(const FilterPipeline&) = delete; // Copy Constructor Not Implemented
  void operator=(const FilterPipeline&) = delete; // Move assignment Not Implemented
};

