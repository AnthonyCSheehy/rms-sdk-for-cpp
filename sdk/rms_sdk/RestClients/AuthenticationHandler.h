/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
 */

#ifndef _RMS_LIB_AUTHENTICATIONHANDLER_H_
#define _RMS_LIB_AUTHENTICATIONHANDLER_H_

#include <atomic>
#include <memory>
#include <string>

#include "../ModernAPI/IAuthenticationCallbackImpl.h"
#include "../Common/CommonTypes.h"

namespace rmscore {
namespace restclients {


class AuthenticationHandler
{
public:
    // Optional Authentication Parameter struct.
    struct AuthenticationHandlerParameters
    {
        std::string m_ServerPublicCertificate; // Relevant When consuming PL. For EVO STS.
        std::string m_ServiceDiscoverUrl; // Relevant When doing service discovery. For EVO STS.
    };
    static std::string GetAccessTokenForUrl(const std::string& sUrl,
                                            const AuthenticationHandlerParameters &authParams,
                                            modernapi::IAuthenticationCallbackImpl& callback,
                                            std::shared_ptr<std::atomic<bool>> cancelState);

    static std::string GetAccessTokenForUrl(const std::string& sUrl,
                                            common::ByteArray&& requestBody,
                                            modernapi::IAuthenticationCallbackImpl& callback,
                                            std::shared_ptr<std::atomic<bool>> cancelState);



private:

    static modernapi::AuthenticationChallenge GetChallengeForUrl(const std::string& sUrl,
        const AuthenticationHandlerParameters& authParams,
        std::shared_ptr<std::atomic<bool>> cancelState);

    static modernapi::AuthenticationChallenge GetChallengeForUrl(const std::string& sUrl,
        common::ByteArray&& requestBody,
        std::shared_ptr<std::atomic<bool>> cancelState);

    static modernapi::AuthenticationChallenge ParseChallengeHeader(const std::string& header,
                                                                   const std::string& url);
};

} // namespace restclients
} // namespace rmscore
#endif // _RMS_LIB_AUTHENTICATIONHANDLER_H_
