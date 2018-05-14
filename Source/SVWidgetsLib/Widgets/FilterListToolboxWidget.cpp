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
#include "FilterListToolboxWidget.h"

#include <QtCore/QBitArray>
#include <QtCore/QFileInfo>
#include <QtCore/QMapIterator>
#include <QtGui/QPainter>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTreeWidgetItem>

#include "SIMPLib/Filtering/FilterFactory.hpp"
#include "SIMPLib/Filtering/FilterManager.h"

#include "SVWidgetsLib/QtSupport/QtSStyles.h"
#include "SVWidgetsLib/Widgets/FilterListModel.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FilterListToolboxWidget::FilterListToolboxWidget(QWidget* parent)
: QWidget(parent)
, m_ContextMenu(new QMenu(this))
, m_Mapper(nullptr)
, m_SearchAnyWords(false)
, m_SearchExactPhrase(false)
, m_SearchAllWords(true)
{
  setupUi(this);

  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FilterListToolboxWidget::~FilterListToolboxWidget() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FilterListToolboxWidget::setupGui()
{
  setupSearchField();

//  QString css(" QToolTip {\
//              border: 2px solid #434343;\
//              padding: 2px;\
//              border-radius: 3px;\
//              opacity: 255;\
//              background-color: #FFFFFF;\
//              color: #000000;\
//              }");
  //filterListView->setStyleSheet(css);

  filterListView->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(filterListView, &FilterListView::filterItemDoubleClicked, this, &FilterListToolboxWidget::filterItemDoubleClicked);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FilterListToolboxWidget::setupSearchField()
{
  filterSearch->setAttribute(Qt::WA_MacShowFocusRect, false);
  filterSearch->setAcceptDrops(false);
  QMenu* lineEditMenu = new QMenu(filterSearch);
  filterSearch->setButtonMenu(QtSLineEdit::Left, lineEditMenu);
  filterSearch->setButtonVisible(QtSLineEdit::Left, true);
  filterSearch->setPlaceholderText("Search for filter");
  QPixmap pixmap(24, 24);
  pixmap.fill(Qt::transparent);
  QPainter painter(&pixmap);
  const QPixmap mag = QPixmap(QLatin1String(":/search_menu.png"));
  painter.drawPixmap(0, (pixmap.height() - mag.height()) / 2, mag);
  filterSearch->setButtonPixmap(QtSLineEdit::Left, pixmap);
  connect(filterSearch, SIGNAL(textChanged(QString)), this, SLOT(searchFilters(QString)));
  connect(filterSearch, &QtSLineEdit::keyPressed, filterListView, &FilterListView::listenKeyPressed);

  {
    m_ActionAllWords = new QAction(filterSearch);
    m_ActionAllWords->setObjectName(QString::fromUtf8("actionAllWords"));
    m_ActionAllWords->setText(QApplication::translate("SIMPLView_UI", "All Words", 0));
    m_ActionAllWords->setCheckable(true);
    m_ActionAllWords->setChecked(m_SearchAllWords);
    filterSearch->addAction(m_ActionAllWords);
    connect(m_ActionAllWords, SIGNAL(toggled(bool)), this, SLOT(searchFieldsChanged(bool)));
    lineEditMenu->addAction(m_ActionAllWords);
  }

  {
    m_ActionAnyWords = new QAction(filterSearch);
    m_ActionAnyWords->setObjectName(QString::fromUtf8("actionWordForWord"));
    m_ActionAnyWords->setText(QApplication::translate("SIMPLView_UI", "Any Words", 0));
    m_ActionAnyWords->setCheckable(true);
    m_ActionAnyWords->setChecked(m_SearchAnyWords);
    filterSearch->addAction(m_ActionAnyWords);
    connect(m_ActionAnyWords, SIGNAL(toggled(bool)), this, SLOT(searchFieldsChanged(bool)));
    lineEditMenu->addAction(m_ActionAnyWords);
  }

  {
    m_ActionExactPhrase = new QAction(filterSearch);
    m_ActionExactPhrase->setObjectName(QString::fromUtf8("actionExactPhrase"));
    m_ActionExactPhrase->setText(QApplication::translate("SIMPLView_UI", "Exact Phrase", 0));
    m_ActionExactPhrase->setCheckable(true);
    m_ActionExactPhrase->setChecked(m_SearchExactPhrase);
    filterSearch->addAction(m_ActionExactPhrase);
    connect(m_ActionExactPhrase, SIGNAL(toggled(bool)), this, SLOT(searchFieldsChanged(bool)));
    lineEditMenu->addAction(m_ActionExactPhrase);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FilterListToolboxWidget::loadFilterList()
{
  // Clear the list first
  filterListView->clear();

  // Get the FilterManager and loop over all the factories
  FilterManager* fm = FilterManager::Instance();
  m_LoadedFilters = fm->getFactories();
  QMapIterator<QString, IFilterFactory::Pointer> iter(m_LoadedFilters);

  QString countText = QObject::tr("Filter Count: %1").arg(m_LoadedFilters.size());
  filterCountLabel->setText(countText);

  while(iter.hasNext())
  {
    iter.next();
    IFilterFactory::Pointer factory = iter.value();
    if(nullptr == factory.get())
    {
      continue;
    }

    AbstractFilter::Pointer filter = factory->create();
    if(nullptr == filter.get())
    {
      continue;
    }

    filterListView->addFilter(filter);
  }

  filterListView->setSortingEnabled(true);
  filterListView->sortByColumn(FilterListModel::Column::Contents, Qt::AscendingOrder);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QList<QString> FilterListToolboxWidget::serializeString(QString string, char token)
{
  std::string stringString = string.toStdString();
  QList<QString> list;
  int currentIndex = 0;
  int spaceIndex = 0;
  QString strPart = "";

  while(spaceIndex >= 0 && string.isEmpty() == false)
  {
    spaceIndex = string.indexOf(token);
    strPart = string.left(spaceIndex);
    strPart = strPart.simplified();
    if(strPart != "")
    {
      list.push_back(strPart);
    }
    string = string.remove(currentIndex, spaceIndex + 1);
    stringString = string.toStdString();
  }

  return list;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FilterListToolboxWidget::deserializeString(QList<QString> list, char token)
{
  QString str = "";
  for(int i = 0; i < list.size(); i++)
  {
    str.append(list[i]);
    str.append(" ");
  }
  str.chop(1);

  return str;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int FilterListToolboxWidget::matchFiltersToSearchGroup(QMapIterator<QString, IFilterFactory::Pointer> iter, QString fullWord, FilterListView::SearchGroup searchGroup)
{
  QList<QString> wordList = serializeString(fullWord, ' ');
  QMap<AbstractFilter::Pointer, int> wordCountMap;
  QMultiMap<int, AbstractFilter::Pointer> relevanceMap;

  while(iter.hasNext())
  {
    iter.next();
    IFilterFactory::Pointer factory = iter.value();
    if(nullptr == factory)
    {
      continue;
    }

    AbstractFilter::Pointer filter = factory->create();
    if(nullptr == filter)
    {
      continue;
    }

    int wordCount = getMatchingWordCountForFilter(fullWord, filter, searchGroup);
    int relevance = getMatchingRelevanceForFilter(fullWord, filter, searchGroup);

    if(!wordCountMap.contains(filter) && wordCount > 0)
    {
      wordCountMap.insert(filter, wordCount);
      relevanceMap.insert(relevance, filter);
    }
  }

  int filterCount = 0;

  // Match according to "Exact Phrase"
  if(m_ActionExactPhrase->isChecked())
  {
    QList<AbstractFilter::Pointer> filterList = relevanceMap.values(wordList.size());
    for(QList<AbstractFilter::Pointer>::iterator iter = filterList.begin(); iter != filterList.end(); ++iter)
    {
      // Do not display results that have the exact phrase in the middle or end of the search phrase
      if((*iter)->getHumanLabel().startsWith(fullWord))
      {
        filterListView->addFilter(*iter, searchGroup);
        filterCount++;
      }
    }
  }
  // Match according to "All Words"
  else if(m_ActionAllWords->isChecked())
  {
    QList<AbstractFilter::Pointer> filterList = wordCountMap.keys(wordList.size());
    QMapIterator<int, AbstractFilter::Pointer> iter(relevanceMap);
    iter.toBack();
    while(iter.hasPrevious())
    {
      iter.previous();
      AbstractFilter::Pointer filter = iter.value();

      if(filterList.contains(filter))
      {
        filterListView->addFilter(filter, searchGroup);
        filterCount++;
      }
    }
  }
  // Match according to "Any Words"
  else if(m_ActionAnyWords->isChecked())
  {
    // QList<AbstractFilter::Pointer> filterListView = wordCountMap.keys();
    QMapIterator<int, AbstractFilter::Pointer> iter(relevanceMap);
    iter.toBack();
    while(iter.hasPrevious())
    {
      iter.previous();
      AbstractFilter::Pointer filter = iter.value();

      filterListView->addFilter(filter, searchGroup);
      filterCount++;
    }
  }

  return filterCount;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int FilterListToolboxWidget::getMatchingWordCountForFilter(const QString &searchPhrase, AbstractFilter::Pointer filter, FilterListView::SearchGroup searchGroup)
{
  QList<QString> wordList = serializeString(searchPhrase, ' ');

  QString searchGroupTerm = "";
  switch(searchGroup)
  {
    case FilterListView::SearchGroup::HumanLabel:
    {
      searchGroupTerm = filter->getHumanLabel();
      break;
    }
    case FilterListView::SearchGroup::ClassName:
    {
      searchGroupTerm = filter->getNameOfClass();
      break;
    }
    case FilterListView::SearchGroup::GroupName:
    {
      searchGroupTerm = filter->getGroupName();
      break;
    }
    case FilterListView::SearchGroup::SubgroupName:
    {
      searchGroupTerm = filter->getSubGroupName();
      break;
    }
    case FilterListView::SearchGroup::BrandingName:
    {
      searchGroupTerm = filter->getBrandingString();
      break;
    }
    case FilterListView::SearchGroup::CompiledLibraryName:
    {
      searchGroupTerm = filter->getCompiledLibraryName();
      break;
    }
    case FilterListView::SearchGroup::Keywords:
    {
      // Implement Keywords
      break;
    }
  }

  QBitArray bitArray(wordList.size(), false);

  for(int i = 0; i < wordList.size(); i++)
  {
    QString keyword = wordList[i];

    if(searchGroupTerm.contains(keyword, Qt::CaseInsensitive))
    {
      bitArray.setBit(i, true);
    }
  }

  return bitArray.count(true);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int FilterListToolboxWidget::getMatchingRelevanceForFilter(const QString &searchPhrase, AbstractFilter::Pointer filter, FilterListView::SearchGroup searchGroup)
{
  QList<QString> wordList = serializeString(searchPhrase, ' ');

  QString searchGroupTerm = "";
  switch(searchGroup)
  {
    case FilterListView::SearchGroup::HumanLabel:
    {
      searchGroupTerm = filter->getHumanLabel();
      break;
    }
    case FilterListView::SearchGroup::ClassName:
    {
      searchGroupTerm = filter->getNameOfClass();
      break;
    }
    case FilterListView::SearchGroup::GroupName:
    {
      searchGroupTerm = filter->getGroupName();
      break;
    }
    case FilterListView::SearchGroup::SubgroupName:
    {
      searchGroupTerm = filter->getSubGroupName();
      break;
    }
    case FilterListView::SearchGroup::BrandingName:
    {
      searchGroupTerm = filter->getBrandingString();
      break;
    }
    case FilterListView::SearchGroup::CompiledLibraryName:
    {
      searchGroupTerm = filter->getCompiledLibraryName();
      break;
    }
    case FilterListView::SearchGroup::Keywords:
    {
      // Implement Keywords
      break;
    }
  }

  QBitArray bitArray(wordList.size(), false);

  int consecutiveWordsCount = 0, maxConsecutiveWordsCount = 0, consecutiveWordsStartingIndex = 0;
  for(int i = 0; i < wordList.size(); i++)
  {
    QString keyword = wordList[i];

    if(searchGroupTerm.contains(keyword, Qt::CaseInsensitive)
          && filterListView->findIndexByName(searchGroupTerm).isValid() == false)
    {
      bitArray.setBit(i, true);

      QList<QString> phraseList;
      for(int j = consecutiveWordsStartingIndex; j <= i; j++)
      {
        phraseList.append(wordList[j]);
      }
      QString phrase = deserializeString(phraseList, ' ');

      if(searchGroupTerm.contains(phrase, Qt::CaseInsensitive) && consecutiveWordsCount < phraseList.size())
      {
        consecutiveWordsCount++;
      }
      else
      {
        if(consecutiveWordsCount > maxConsecutiveWordsCount)
        {
          maxConsecutiveWordsCount = consecutiveWordsCount;
        }
        consecutiveWordsCount = 1;
        consecutiveWordsStartingIndex = i;
      }
    }
  }

  if(consecutiveWordsCount > maxConsecutiveWordsCount)
  {
    maxConsecutiveWordsCount = consecutiveWordsCount;
  }

  return maxConsecutiveWordsCount;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FilterListToolboxWidget::searchFilters(QString text)
{
  // Set scroll bar back to the top
  filterListView->scrollToTop();

  if(text.isEmpty())
  {
    // Put back the entire list of Filters
    loadFilterList();
    return;
  }

  filterListView->clear();

  // The user is typing something in the search box so lets search the filter class name and human label
  // int listWidgetSize = m_LoadedFilters.size();
  QMapIterator<QString, IFilterFactory::Pointer> iter(m_LoadedFilters);

  matchFiltersToSearchGroup(iter, text, FilterListView::SearchGroup::HumanLabel);
  matchFiltersToSearchGroup(iter, text, FilterListView::SearchGroup::ClassName);
  matchFiltersToSearchGroup(iter, text, FilterListView::SearchGroup::GroupName);
  matchFiltersToSearchGroup(iter, text, FilterListView::SearchGroup::SubgroupName);
  matchFiltersToSearchGroup(iter, text, FilterListView::SearchGroup::BrandingName);
  matchFiltersToSearchGroup(iter, text, FilterListView::SearchGroup::CompiledLibraryName);
//  matchFiltersToSearchGroup(iter, text, FilterListView::SearchGroup::Keywords);

//  QString countText = QObject::tr("Filter Count: %1").arg(filterCount);
//  filterCountLabel->setText(countText);
  filterCountLabel->setText("");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FilterListToolboxWidget::searchFieldsChanged(bool isChecked)
{
  QAction* senderAction = qobject_cast<QAction*>(sender());

  if(isChecked)
  {
    m_ActionExactPhrase->blockSignals(true);
    m_ActionAllWords->blockSignals(true);
    m_ActionAnyWords->blockSignals(true);

    if(senderAction == m_ActionAnyWords)
    {
      m_ActionExactPhrase->setChecked(false);
      m_ActionAllWords->setChecked(false);
    }
    else if(senderAction == m_ActionExactPhrase)
    {
      m_ActionAnyWords->setChecked(false);
      m_ActionAllWords->setChecked(false);
    }
    else if(senderAction == m_ActionAllWords)
    {
      m_ActionExactPhrase->setChecked(false);
      m_ActionAnyWords->setChecked(false);
    }

    m_ActionExactPhrase->blockSignals(false);
    m_ActionAllWords->blockSignals(false);
    m_ActionAnyWords->blockSignals(false);

    searchFilters(filterSearch->text());
  }
  else
  {
    senderAction->setChecked(true);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QAction* FilterListToolboxWidget::getActiveSearchAction()
{
  if(m_ActionExactPhrase->isChecked())
  {
    return m_ActionExactPhrase;
  }
  else if(m_ActionAnyWords->isChecked())
  {
    return m_ActionAnyWords;
  }
  else
  {
    return m_ActionAllWords;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FilterListToolboxWidget::setActiveSearchAction(QAction* action)
{
  m_ActionExactPhrase->blockSignals(true);
  m_ActionAllWords->blockSignals(true);
  m_ActionAnyWords->blockSignals(true);

  m_ActionExactPhrase->setChecked(false);
  m_ActionAllWords->setChecked(false);
  m_ActionAnyWords->setChecked(false);
  action->setChecked(true);

  m_ActionExactPhrase->blockSignals(false);
  m_ActionAllWords->blockSignals(false);
  m_ActionAnyWords->blockSignals(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QList<QAction*> FilterListToolboxWidget::getSearchActionList()
{
  QList<QAction*> list;
  list.append(m_ActionAllWords);
  list.append(m_ActionAnyWords);
  list.append(m_ActionExactPhrase);
  return list;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QMap<QString, AbstractFilter::Pointer> FilterListToolboxWidget::getHumanNameMap(QList<AbstractFilter::Pointer> list)
{
  QMap<QString, AbstractFilter::Pointer> map;
  for(int i = 0; i < list.size(); i++)
  {
    map.insert(list[i]->getHumanLabel(), list[i]);
  }

  return map;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FilterListToolboxWidget::writeSettings(QtSSettings* prefs)
{
  prefs->beginGroup("DockWidgetSettings");
  prefs->beginGroup("Filter List Dock Widget");

  prefs->setValue(objectName(), isHidden());
  prefs->setValue("ActiveSearchAction", getActiveSearchAction()->objectName());

  prefs->endGroup();
  prefs->endGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FilterListToolboxWidget::readSettings(QtSSettings* prefs)
{
  prefs->beginGroup("DockWidgetSettings");
  prefs->beginGroup("Filter List Dock Widget");

  QString objectName = prefs->value("ActiveSearchAction", QString("")).toString();
  QList<QAction*> list = getSearchActionList();

  bool didCheck = false;
  for(int i = 0; i < list.size(); i++)
  {
    if(list[i]->objectName() == objectName)
    {
      list[i]->setChecked(true);
      didCheck = true;
    }
    else
    {
      list[i]->setChecked(false);
    }
  }

  if(didCheck == false && list.size() > 0)
  {
    // Set "All Words" as checked by default
    list[0]->setChecked(true);
  }

  prefs->endGroup();
  prefs->endGroup();
}
