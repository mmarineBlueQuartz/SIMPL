/* ============================================================================
 * Copyright (c) 2017-2019 BlueQuartz Software, LLC
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
 * Neither the names of any of the BlueQuartz Software contributors
 * may be used to endorse or promote products derived from this software without
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
 *
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#pragma once

#include "QtWebApp/httpserver/httprequest.h"
#include "QtWebApp/httpserver/httprequesthandler.h"
#include "QtWebApp/httpserver/httpresponse.h"

#include "SIMPLib/SIMPLib.h"

/**
  @brief This class responds to REST API endpoint NamesOfFilters

  The returned JSON is the following on success

{
  "ErrorCode": 0,
  "Filters":[
  {"ClassName":"ArrayCalculator","HumanLabel":"Attribute Array Calculator"},
  {"ClassName":"ArraySelectionExample","HumanLabel":"DataContainerArrayProxy Example"},
  ....
  ]
  }
  On Error the following JSON is returned.
  {
    "ErrorMessage": "Error Message ...."
  }
*/

class SIMPLib_EXPORT NamesOfFiltersController : public HttpRequestHandler
{
  Q_OBJECT
  Q_DISABLE_COPY(NamesOfFiltersController)
public:
  /** Constructor */
  NamesOfFiltersController(const QHostAddress& hostAddress, const int hostPort);

  /** Generates the response */
  void service(HttpRequest& request, HttpResponse& response);

  /**
   * @brief Returns the name of the end point that is controller uses
   * @return
   */
  static QString EndPoint();
};

