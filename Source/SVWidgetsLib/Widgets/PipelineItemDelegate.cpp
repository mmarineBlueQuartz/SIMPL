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

#include <QtCore/QFileInfo>

#include <QtWidgets/QLineEdit>

#include <QtGui/QIntValidator>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

#include <QtWidgets/QStylePainter>

#include "SVWidgetsLib/Widgets/PipelineItem.h"
#include "SVWidgetsLib/Widgets/PipelineItemDelegate.h"
#include "SVWidgetsLib/Widgets/PipelineModel.h"
#include "SVWidgetsLib/Widgets/SVPipelineView.h"
#include "SVWidgetsLib/Widgets/SVStyle.h"

namespace
{
const int k_ButtonSize = 24;
const int k_TextMargin = 4;

const QColor k_DropIndicatorWidgetBackgroundColor = QColor(150, 150, 150);
const QColor k_DropIndicatorIndexBackgroundColor = QColor(48, 48, 48);
const QColor k_DropIndicatorLabelColor = QColor(242, 242, 242);
} // namespace

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PipelineItemDelegate::PipelineItemDelegate(SVPipelineView* view)
: QStyledItemDelegate(nullptr)
, m_View(view)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PipelineItemDelegate::~PipelineItemDelegate() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PipelineItemDelegate::itemHeight() const
{
  return 48;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  painter->save();
  painter->setRenderHint(QPainter::Antialiasing);

  PipelineModel* model = m_View->getPipelineModel();
  if(nullptr != model->filter(index))
  {
    paintFilter(painter, option, index);
  }
  else if(nullptr != model->pipeline(index))
  {
    paintPipeline(painter, option, index);
  }

  painter->restore();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItemDelegate::paintFilterIndex(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  PipelineModel* model = m_View->getPipelineModel();

  FilterPipeline::FilterState wState = static_cast<FilterPipeline::FilterState>(model->data(index, PipelineModel::FilterStateRole).toInt());
  FilterPipeline::PipelineState pState = static_cast<FilterPipeline::PipelineState>(model->data(index, PipelineModel::PipelineStateRole).toInt());
  FilterPipeline::ErrorState eState = static_cast<FilterPipeline::ErrorState>(model->data(index, PipelineModel::ErrorStateRole).toInt());

  QColor indexBackgroundColor;
#if 1
  QColor selectedBgColor = SVStyle::Instance()->GetFilterSelectionColor();
#else
  QColor selectedBgColor = m_View->palette().color(QPalette::Highlight);
#endif
  QColor disabledBgColor = QColor(124, 124, 124);
  QColor indexFontColor(242, 242, 242);

  switch(wState)
  {
  case FilterPipeline::FilterState::Ready:
    indexBackgroundColor = QColor(48, 48, 48);
    break;
  case FilterPipeline::FilterState::Executing:
    indexBackgroundColor = QColor(6, 140, 190);
    break;
  case FilterPipeline::FilterState::Completed:
    indexBackgroundColor = QColor(6, 118, 6);
    break;
  case FilterPipeline::FilterState::Disabled:
    indexBackgroundColor = QColor(96, 96, 96);
    break;
  }

  QFont font = SVStyle::Instance()->GetHumanLabelFont();

#if defined(Q_OS_MAC)
  font.setPointSize(font.pointSize() - 4);
#elif defined(Q_OS_WIN)
  font.setPointSize(font.pointSize() - 3);
#else
  font.setPointSize(font.pointSize() - 1);
#endif

  QFontMetrics fontMetrics(font);
  int fontHeight = fontMetrics.height();
  int fontMargin = ((option.rect.height() - fontHeight) / 2) - 1;

  int indexFontWidth = fontMetrics.width(QString::number(model->getMaxFilterCount()));

  painter->setFont(font);

  const int textMargin = 6;
  const int indexBoxWidth = 35;
  int xOffset = 0;
  int yOffset = 0;

  QRect indexRect = option.rect;
  indexRect.setX(indexRect.x() + xOffset);
  indexRect.setY(indexRect.y() + yOffset);
  indexRect.setHeight(itemHeight());
  indexRect.setWidth(2 * textMargin + indexFontWidth);

  painter->fillRect(indexRect, indexBackgroundColor);

  // Draw the Index number
  painter->setPen(QPen(indexFontColor));
  QString number = getFilterIndexString(index); // format the index number with a leading zero
  if(fontHeight <= indexRect.height())
  {
    painter->drawText(option.rect.x() + textMargin + xOffset, option.rect.y() + fontMargin + fontHeight + yOffset, number);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItemDelegate::paintBackground(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  PipelineModel* model = m_View->getPipelineModel();
  AbstractFilter::Pointer filter = model->filter(index);

  FilterPipeline::FilterState wState = static_cast<FilterPipeline::FilterState>(model->data(index, PipelineModel::FilterStateRole).toInt());
  FilterPipeline::PipelineState pState = static_cast<FilterPipeline::PipelineState>(model->data(index, PipelineModel::PipelineStateRole).toInt());
  FilterPipeline::ErrorState eState = static_cast<FilterPipeline::ErrorState>(model->data(index, PipelineModel::ErrorStateRole).toInt());

  QColor grpColor;
  if(nullptr != filter)
  {
    QString grpName = filter->getGroupName();
    grpColor = SVStyle::Instance()->GetFilterBackgroundColor();
  }

  QColor widgetBackgroundColor;
  QColor bgColor = grpColor;
#if 1
  QColor selectedBgColor = SVStyle::Instance()->GetFilterSelectionColor();
#else
  QColor selectedBgColor = m_View->palette().color(QPalette::Highlight);
#endif
  QColor disabledBgColor = QColor(124, 124, 124);

  if(option.state & QStyle::State_Selected)
  {
    bgColor = selectedBgColor;
  }

  if((option.state & QStyle::State_MouseOver))
  {
    if((option.state & QStyle::State_Selected) == false)
    {
      QColor hoveredColor = bgColor;
      hoveredColor.setRedF((hoveredColor.redF() * 1.10 > 1.0) ? 1.0 : hoveredColor.redF() * 1.10);
      hoveredColor.setGreenF((hoveredColor.greenF() * 1.10 > 1.0) ? 1.0 : hoveredColor.greenF() * 1.10);
      hoveredColor.setBlueF((hoveredColor.blueF() * 1.10 > 1.0) ? 1.0 : hoveredColor.blueF() * 1.10);
      bgColor = hoveredColor;
    }
  }

  switch(wState)
  {
  case FilterPipeline::FilterState::Ready:
    widgetBackgroundColor = bgColor;
    break;
  case FilterPipeline::FilterState::Executing:
    widgetBackgroundColor = QColor(130, 130, 130);
    break;
  case FilterPipeline::FilterState::Completed:
    widgetBackgroundColor = bgColor.name();
    break;
  case FilterPipeline::FilterState::Disabled:
    bgColor = disabledBgColor;
    widgetBackgroundColor = disabledBgColor.name();
    break;
  }
  QColor selectedColor = QColor::fromHsv(bgColor.hue(), 100, 120);

  // Do not change the background color if the widget is disabled.
  if(wState != FilterPipeline::FilterState::Disabled)
  {
    switch(pState)
    {
    case FilterPipeline::PipelineState::Running:
      widgetBackgroundColor = selectedColor.name();
      break;
    case FilterPipeline::PipelineState::Ready:
      widgetBackgroundColor = bgColor.name();
      break;
    case FilterPipeline::PipelineState::Paused:
      widgetBackgroundColor = QColor(160, 160, 160);
      break;
    }
  }

  QFont font = SVStyle::Instance()->GetHumanLabelFont();

#if defined(Q_OS_MAC)
  font.setPointSize(font.pointSize() - 4);
#elif defined(Q_OS_WIN)
  font.setPointSize(font.pointSize() - 3);
#else
  font.setPointSize(font.pointSize() - 1);
#endif

  QFontMetrics fontMetrics(font);
  int fontHeight = fontMetrics.height();
  int fontMargin = ((option.rect.height() - fontHeight) / 2) - 1;

  int indexFontWidth = fontMetrics.width(QString::number(model->getMaxFilterCount()));
  const int textMargin = 6;
  const int xOffset = 0;
  const int yOffset = 0;

  // Draw the Title area
  QRect coloredRect(2 * textMargin + indexFontWidth + xOffset, option.rect.y() + yOffset, option.rect.width() - (2 * textMargin + indexFontWidth),
                    itemHeight()); // +4? without it it does not paint to the edge
  painter->fillRect(coloredRect, widgetBackgroundColor);

  // Draw the border that separates the Index area and the Title area
  painter->setPen(QPen(QBrush(QColor(Qt::black)), m_BorderSize));
  //  painter->setPen(QPen(QBrush(QColor(48, 48, 48)), m_BorderSize));
  painter->drawLine(2 * textMargin + indexFontWidth + xOffset, option.rect.y() + yOffset + 1, 2 * textMargin + indexFontWidth + xOffset, option.rect.y() + yOffset + itemHeight() - 0.5);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItemDelegate::paintFilter(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  paintBackground(painter, option, index);
  paintFilterIndex(painter, option, index);

  PipelineModel* model = m_View->getPipelineModel();

  FilterPipeline::FilterState wState = static_cast<FilterPipeline::FilterState>(model->data(index, PipelineModel::FilterStateRole).toInt());
  FilterPipeline::PipelineState pState = static_cast<FilterPipeline::PipelineState>(model->data(index, PipelineModel::PipelineStateRole).toInt());
  FilterPipeline::ErrorState eState = static_cast<FilterPipeline::ErrorState>(model->data(index, PipelineModel::ErrorStateRole).toInt());

  AbstractFilter::Pointer filter = model->filter(index);
  QColor grpColor;
  if(filter.get() != nullptr)
  {
    QString grpName = filter->getGroupName();
    grpColor = SVStyle::Instance()->GetFilterBackgroundColor();
  }

  QColor labelColor = SVStyle::Instance()->GetFilterFontColor();

  bool drawButtons = false;

  if((option.state & QStyle::State_MouseOver))
  {
    drawButtons = true;
  }

  // Do not change the background color if the widget is disabled.
  if(wState != FilterPipeline::FilterState::Disabled)
  {
    switch(pState)
    {
    case FilterPipeline::PipelineState::Running:
      labelColor = QColor(20, 20, 20);
      break;
    case FilterPipeline::PipelineState::Ready:
      break;
    case FilterPipeline::PipelineState::Paused:
      labelColor = QColor(0, 0, 0);
      break;
    }
  }

  if(option.state & QStyle::State_Selected)
  {
    labelColor = m_View->palette().color(QPalette::HighlightedText);
  }

  PipelineItem::ItemType itemType = static_cast<PipelineItem::ItemType>(model->data(index, PipelineModel::ItemTypeRole).toInt());
#if 0
  if(itemType == PipelineItem::ItemType::DropIndicator)
  {
    indexBackgroundColor = k_DropIndicatorIndexBackgroundColor;
    widgetBackgroundColor = k_DropIndicatorWidgetBackgroundColor;
    labelColor = k_DropIndicatorLabelColor;
    indexFontColor = k_DropIndicatorLabelColor;

    drawButtons = false;
  }
#endif
  if(m_View->getPipelineState() == SVPipelineView::PipelineViewState::Running)
  {
    drawButtons = false;
  }

  QFont font = SVStyle::Instance()->GetHumanLabelFont();

#if defined(Q_OS_MAC)
  font.setPointSize(font.pointSize() - 4);
#elif defined(Q_OS_WIN)
  font.setPointSize(font.pointSize() - 3);
#else
  font.setPointSize(font.pointSize() - 1);
#endif

  QFontMetrics fontMetrics(font);
  int fontHeight = fontMetrics.height();
  int fontMargin = ((option.rect.height() - fontHeight) / 2) - 1;

  int indexFontWidth = fontMetrics.width(QString::number(model->getMaxFilterCount()));

  painter->setFont(font);

  // back fill with RED so we know if we missed something
  // painter->fillRect(rect(), QColor(255, 0, 0));

  const int textMargin = 6;
  const int indexBoxWidth = 35;
  int xOffset = 0;
  int yOffset = 0;

  // Draw the Index area
  QRect rect = option.rect;
  QRect indexRect = option.rect;
  indexRect.setX(indexRect.x() + xOffset);
  indexRect.setY(indexRect.y() + yOffset);
  indexRect.setHeight(itemHeight());
  indexRect.setWidth(2 * textMargin + indexFontWidth);

  // Compute the Width to draw the text based on the visibility of the various buttons
  int fullWidth = rect.width() - indexBoxWidth;
  int allowableWidth = fullWidth;

  if(drawButtons == true)
  {
    // Draw the "delete" button
    QRectF deleteBtnRect;
    deleteBtnRect.setX(option.rect.width() - ::k_ButtonSize - ::k_TextMargin);
    deleteBtnRect.setY(option.rect.y() + ((option.rect.height() / 2) - (::k_ButtonSize / 2)));
    deleteBtnRect.setWidth(::k_ButtonSize);
    deleteBtnRect.setHeight(::k_ButtonSize);

    QPoint mousePos = QCursor::pos();
    mousePos = m_View->viewport()->mapFromGlobal(mousePos);

    QPixmap deleteBtnPixmap;
    if(deleteBtnRect.contains(mousePos))
    {
      deleteBtnPixmap = m_View->getDeleteBtnHoveredPixmap(option.state & QStyle::State_Selected);
      if(painter->device()->devicePixelRatio() == 2)
      {
        deleteBtnPixmap = m_View->getHighDPIDeleteBtnHoveredPixmap(option.state & QStyle::State_Selected);
      }
    }
    else
    {
      deleteBtnPixmap = m_View->getDeleteBtnPixmap(option.state & QStyle::State_Selected);
      if(painter->device()->devicePixelRatio() == 2)
      {
        deleteBtnPixmap = m_View->getHighDPIDeleteBtnPixmap(option.state & QStyle::State_Selected);
      }
    }

    painter->drawPixmap(deleteBtnRect.center().x() - (deleteBtnRect.width() / 2) + xOffset, deleteBtnRect.center().y() - (deleteBtnRect.height() / 2 + 1) + yOffset,
                        deleteBtnPixmap); // y is 1px offset due to how the images were cut

    // Draw the "disable" button
    QRectF disableBtnRect;
    disableBtnRect.setX(deleteBtnRect.x() - ::k_TextMargin - ::k_ButtonSize);
    disableBtnRect.setY(option.rect.y() + ((option.rect.height() / 2) - (::k_ButtonSize / 2)));
    disableBtnRect.setWidth(::k_ButtonSize);
    disableBtnRect.setHeight(::k_ButtonSize);

    QPixmap disableBtnPixmap;
    FilterPipeline::FilterState wState = static_cast<FilterPipeline::FilterState>(model->data(index, PipelineModel::FilterStateRole).toInt());
    if(wState == FilterPipeline::FilterState::Disabled)
    {
      disableBtnPixmap = m_View->getDisableBtnActivatedPixmap(option.state & QStyle::State_Selected);
      if(painter->device()->devicePixelRatio() == 2)
      {
        disableBtnPixmap = m_View->getHighDPIDisableBtnActivatedPixmap(option.state & QStyle::State_Selected);
      }
    }
    else if(disableBtnRect.contains(mousePos))
    {
      disableBtnPixmap = m_View->getDisableBtnHoveredPixmap(option.state & QStyle::State_Selected);
      if(painter->device()->devicePixelRatio() == 2)
      {
        disableBtnPixmap = m_View->getHighDPIDisableBtnHoveredPixmap(option.state & QStyle::State_Selected);
      }
    }
    else
    {
      disableBtnPixmap = m_View->getDisableBtnPixmap(option.state & QStyle::State_Selected);
      if(painter->device()->devicePixelRatio() == 2)
      {
        disableBtnPixmap = m_View->getHighDPIDisableBtnPixmap(option.state & QStyle::State_Selected);
      }
    }

    allowableWidth -= deleteBtnRect.width();
    allowableWidth -= disableBtnRect.width();

    painter->drawPixmap(disableBtnRect.center().x() - (disableBtnRect.width() / 2) + xOffset, disableBtnRect.center().y() - (disableBtnRect.height() / 2 + 1) + yOffset,
                        disableBtnPixmap); // y is 1px offset due to how the images were cut
  }

  //  QString elidedHumanLabel = fontMetrics.elidedText(m_FilterHumanLabel, Qt::ElideRight, allowableWidth);

  int humanLabelWidth = 0;
  if(filter)
  {
    humanLabelWidth = fontMetrics.width(model->data(index, Qt::DisplayRole).toString());
  }
#if 0
  if(itemType == PipelineItem::ItemType::DropIndicator)
  {
    QString dropIndicatorText = model->dropIndicatorText(index);
    humanLabelWidth = fontMetrics.width(dropIndicatorText);
  }
  else
  {
    humanLabelWidth = fontMetrics.width(filter->getHumanLabel());
  }
#endif

  QRect coloredRect(2 * textMargin + indexFontWidth + xOffset, option.rect.y() + yOffset, option.rect.width() - (2 * textMargin + indexFontWidth),
                    itemHeight()); // +4? without it it does not paint to the edge

  // Draw the filter human label
  painter->setPen(QPen(labelColor));
  font.setWeight(QFont::Normal);
  painter->setFont(font);

  // Compute a Fade out of the text if it is too long to fit in the widget
  if(humanLabelWidth > allowableWidth)
  {
    QRect fadedRect = coloredRect;
    fadedRect.setWidth(fullWidth);
    if(option.state & QStyle::State_MouseOver)
    {
      fadedRect.setWidth(allowableWidth);
    }

    QLinearGradient gradient(fadedRect.topLeft(), fadedRect.topRight());
    gradient.setColorAt(0.8, labelColor);
    gradient.setColorAt(1.0, QColor(0, 0, 0, 10));

    QPen pen;
    pen.setBrush(QBrush(gradient));
    painter->setPen(pen);
  }

  if(fontHeight <= indexRect.height())
  {
#if 0
    if(itemType == PipelineItem::ItemType::DropIndicator)
    {
      QString text = model->dropIndicatorText(index);
      painter->drawText(rect.x() + indexBoxWidth + textMargin + xOffset, rect.y() + fontMargin + fontHeight + yOffset, text);
    }
    else
#endif
    if(itemType == PipelineItem::ItemType::Filter)
    {
      painter->drawText(rect.x() + indexBoxWidth + textMargin + xOffset, rect.y() + fontMargin + fontHeight + yOffset, model->data(index, Qt::DisplayRole).toString());
    }
  }

  QPen pen(QBrush(QColor(Qt::black)), m_BorderSize);
  //  QPen pen(QBrush(QColor(48, 48, 48)), m_BorderSize);
  painter->setPen(pen);

  // Draw inside option.rect to avoid painting artifacts
  qreal x = option.rect.x() + (m_BorderSize / 2);
  qreal y = option.rect.y() + (m_BorderSize / 2);
  painter->drawRoundedRect(QRectF(x + xOffset, y + yOffset, option.rect.width() - m_BorderSize, option.rect.height() - m_BorderSize), 1, 1);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PipelineItemDelegate::paintPipeline(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  PipelineModel* model = m_View->getPipelineModel();

  FilterPipeline::FilterState wState = static_cast<FilterPipeline::FilterState>(model->data(index, PipelineModel::FilterStateRole).toInt());
  FilterPipeline::PipelineState pState = static_cast<FilterPipeline::PipelineState>(model->data(index, PipelineModel::PipelineStateRole).toInt());
  FilterPipeline::ErrorState eState = static_cast<FilterPipeline::ErrorState>(model->data(index, PipelineModel::ErrorStateRole).toInt());

  FilterPipeline::Pointer pipeline = model->pipeline(index);
  QRect pipelineTextRect = option.rect;

  painter->setPen(Qt::GlobalColor::darkRed);
  painter->drawText(pipelineTextRect, model->data(index, Qt::DisplayRole).toString());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PipelineItemDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
  PipelineModel* pipelineModel = dynamic_cast<PipelineModel*>(model);

  QRect deleteBtnRect;
  deleteBtnRect.setX(option.rect.width() - ::k_ButtonSize - ::k_TextMargin);
  deleteBtnRect.setY(option.rect.y() + (option.rect.height() / 2 - ::k_ButtonSize / 2));
  deleteBtnRect.setWidth(::k_ButtonSize);
  deleteBtnRect.setHeight(::k_ButtonSize);

  QRect disableBtnRect = deleteBtnRect;
  disableBtnRect.setX(disableBtnRect.x() - ::k_TextMargin - ::k_ButtonSize);

  // Looking for click in the delete button area
  QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
  if(mouseEvent != nullptr)
  {
    if(event->type() == QEvent::MouseMove)
    {
      if(deleteBtnRect.contains(mouseEvent->pos()))
      {
        // We are inside the delete button
        if(m_CurrentlyHoveredItem != HoverItem::DeleteButton)
        {
          // We were not inside the delete button before, so set the current hovered item to DeleteButton and schedule a repaint
          m_CurrentlyHoveredItem = HoverItem::DeleteButton;
          return true;
        }
      }
      else if(disableBtnRect.contains(mouseEvent->pos()))
      {
        // We are inside the disable button
        if(m_CurrentlyHoveredItem != HoverItem::DisableButton)
        {
          // We were not inside the disable button before, so set the current hovered item to DisableButton and schedule a repaint
          m_CurrentlyHoveredItem = HoverItem::DisableButton;
          return true;
        }
      }
      else if(m_CurrentlyHoveredItem != HoverItem::Widget)
      {
        // Otherwise, we have to be inside the main widget.
        // We were not inside the main widget before, so set the current hovered item to Widget and schedule a repaint
        m_CurrentlyHoveredItem = HoverItem::Widget;
        return true;
      }
    }

    if(event->type() == QEvent::MouseButtonPress)
    {
      if(deleteBtnRect.contains(mouseEvent->pos()) || disableBtnRect.contains(mouseEvent->pos()))
      {
        m_MousePressIndex = index.row();
        return true; // don't call the base class, we handled the event here
      }
    }

    if(event->type() == QEvent::MouseButtonRelease)
    {
      if(deleteBtnRect.contains(mouseEvent->pos()) || disableBtnRect.contains(mouseEvent->pos()))
      {
        m_MousePressIndex = -1;
        // qDebug() << "Clicked the Pipeline Filter delete button on: " << index.data(Qt::DisplayRole).toString();

        if(deleteBtnRect.contains(mouseEvent->pos()) && m_View->getPipelineState() != SVPipelineView::PipelineViewState::Running)
        {
          AbstractFilter::Pointer filter = pipelineModel->filter(index);
          m_View->removeFilter(filter);
          return true;
        }
        else if(disableBtnRect.contains(mouseEvent->pos()) && m_View->getPipelineState() != SVPipelineView::PipelineViewState::Running)
        {
          AbstractFilter::Pointer filter = pipelineModel->filter(index);
          bool enabled = filter->getEnabled();
          if(enabled)
          {
            filter->setEnabled(false);
            model->setData(index, static_cast<int>(FilterPipeline::FilterState::Disabled), PipelineModel::FilterStateRole);
          }
          else
          {
            filter->setEnabled(true);
            model->setData(index, static_cast<int>(FilterPipeline::FilterState::Ready), PipelineModel::FilterStateRole);
          }

          m_View->preflightPipeline();
          return true;
        }
      }
    }
  }

  return QStyledItemDelegate::editorEvent(event, model, option, index);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString PipelineItemDelegate::getFilterIndexString(const QModelIndex& index) const
{
  const PipelineModel* model = getPipelineModel(index);
  int numFilters = model->rowCount();
  int i = index.row() + 1;

  if(numFilters < 10)
  {
    numFilters = 11;
  }
  QString numStr = QString::number(i);

  if(numFilters > 9)
  {
    int mag = 0;
    int max = numFilters;
    while(max > 0)
    {
      mag++;
      max = max / 10;
    }
    numStr = "";             // Clear the string
    QTextStream ss(&numStr); // Create a QTextStream to set up the padding
    ss.setFieldWidth(mag);
    ss.setPadChar('0');
    ss << i;
  }
  QString paddedIndex = numStr;

  return paddedIndex;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QPixmap PipelineItemDelegate::createPixmap(const QModelIndex& index) const
{
  QRect indexRect = m_View->visualRect(index);

  QPixmap pixmap(indexRect.width(), indexRect.height());
  QPainter painter;

  QStyleOptionViewItem option;
  indexRect.setHeight(indexRect.height() - indexRect.y());
  indexRect.setY(0);
  indexRect.setX(0);
  option.rect = indexRect;
  option.state = QStyle::State_None;

  painter.begin(&pixmap);
  paint(&painter, option, index);
  painter.end();

  return pixmap;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const PipelineModel* PipelineItemDelegate::getPipelineModel(const QModelIndex& index) const
{
  return dynamic_cast<const PipelineModel*>(index.model());
}
