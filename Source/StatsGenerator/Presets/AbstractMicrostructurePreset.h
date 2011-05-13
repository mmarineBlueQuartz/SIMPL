/* ============================================================================
 * Copyright (c) 2011, Michael A. Jackson (BlueQuartz Software)
 * All rights reserved.
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
 * Neither the name of Michael A. Jackson nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
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
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef MICROSTRUCTUREPRESET_H_
#define MICROSTRUCTUREPRESET_H_

#include "QtCore/QVector"

#include "MXA/Common/MXASetGetMacros.h"

class StatsGenPlotWidget;

/*
 *
 */
class AbstractMicrostructurePreset
{
  public:
    MXA_SHARED_POINTERS(AbstractMicrostructurePreset)
    MXA_TYPE_MACRO(AbstractMicrostructurePreset)
    virtual ~AbstractMicrostructurePreset() {};


    /**
     * @brief Displays a Dialog Box which gives the user a chance to customize
     * the preset with specific values. Although subclasses MUST implement the
     * method, nothing actually has to happen, Ie, one can simply have an empty
     * implementation if there are no customizations to be performed.
     */
    virtual void displayUserInputDialog() {};

    /**
     * @brief
     * @param tableModel
     * @param binNumbers
     */
    virtual void generateOmega3Data(StatsGenPlotWidget* plot, QVector<float> binNumbers) = 0;

    /**
     * @brief
     * @param tableModel
     * @param binNumbers
     */
    virtual void generateBOverAPlotData(StatsGenPlotWidget* plot, QVector<float> binNumbers) = 0;

    /**
     * @brief
     * @param tableModel
     * @param binNumbers
     */
    virtual void generateCOverAPlotData(StatsGenPlotWidget* plot, QVector<float> binNumbers) = 0;

    /**
     * @brief
     * @param tableModel
     * @param binNumbers
     */
    virtual void generateCOverBPlotData(StatsGenPlotWidget* plot, QVector<float> binNumbers) = 0;

    /**
     * @brief
     * @param tableModel
     * @param binNumbers
     */
    virtual void generateNeighborPlotData(StatsGenPlotWidget* plot, QVector<float> binNumbers) = 0;

  protected:
    AbstractMicrostructurePreset() {};

  private:
    AbstractMicrostructurePreset(const AbstractMicrostructurePreset&); // Copy Constructor Not Implemented
    void operator=(const AbstractMicrostructurePreset&); // Operator '=' Not Implemented
};

#endif /* MICROSTRUCTUREPRESET_H_ */
