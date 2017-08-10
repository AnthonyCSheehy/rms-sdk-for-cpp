/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifndef _RMS_LIB_SERVICEDISCOVERYDETAILS_H_
#define _RMS_LIB_SERVICEDISCOVERYDETAILS_H_
#include <stdint.h>
#include <string>

namespace rmscore {
namespace restclients {
struct ServiceDiscoveryDetails
{
  std::string Domain;
  uint32_t    Ttl;

  //sdk 4.0/4.1 endpoints
  std::string EndUserLicensesUrl;
  std::string TemplatesUrl;
  std::string PublishingLicensesUrl;
  std::string CloudDiagnosticsServerUrl;

  //sdk 4.2 endpoints
  std::string PerformanceServerUrl;
  std::string DocTrackingLandingPageUrl;
  std::string PublishedPoliciesUrl;

  //sdk 4.3 endpoints
  std::string ClientLicensorCertificatesUrl;
  std::string OriginalInput;
};

} // namespace restclients
} // namespace rmscore
#endif // _RMS_LIB_SERVICEDISCOVERYDETAILS_H_
