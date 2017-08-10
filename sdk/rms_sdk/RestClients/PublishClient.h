/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
 */

#ifndef _RMS_LIB_PUBLISHCLIENT_H_
#define _RMS_LIB_PUBLISHCLIENT_H_

#include "IPublishClient.h"
#include "CLCCacheResult.h"
#include "RestClientCache.h"
#include "../Platform/Json/IJsonObject.h"
#include "../Platform/Json/IJsonArray.h"
#include "../Platform/Json/IJsonParser.h"
#include "../Platform/Json/JsonObjectQt.h"
#include "../../rmscrypto_sdk/CryptoAPI/CryptoAPI.h"
#include <functional>
#include <unordered_map>

using namespace rmscore::platform::json;

namespace rmscore {
namespace restclients {
class PublishClient : public IPublishClient {
public:

  virtual PublishResponse PublishUsingTemplate(
    const PublishUsingTemplateRequest     & request,
    modernapi::IAuthenticationCallbackImpl& authenticationCallback,
    const std::string                       sEmail,
    std::shared_ptr<std::atomic<bool> >     cancelState)
  override;
  virtual PublishResponse PublishCustom(
    const PublishCustomRequest            & request,
    modernapi::IAuthenticationCallbackImpl& authenticationCallback,
    const std::string                       sEmail,
    std::shared_ptr<std::atomic<bool> >     cancelState)
  override;

    //Offline Publishing Stuff
    virtual PublishResponse LocalPublishUsingTemplate(
        const PublishUsingTemplateRequest     & request,
        modernapi::IAuthenticationCallbackImpl& authenticationCallback,
        const std::string                       sEmail,
        std::shared_ptr<std::atomic<bool> >     cancelState,
        const std::function<std::string(std::string, std::string&)>& getCLCCallback = nullptr) override;

      virtual PublishResponse LocalPublishCustom(
        const PublishCustomRequest            & request,
        modernapi::IAuthenticationCallbackImpl& authenticationCallback,
        const std::string                       sEmail,
        std::shared_ptr<std::atomic<bool> >     cancelState,
        const std::function<std::string(std::string, std::string&)>& getCLCCallback = nullptr) override;

      std::shared_ptr<rmscrypto::api::IRSAKeyBlob> rsaKeyBlob;

      vector<uint8_t> EncryptBytesToBase64(
              std::vector<uint8_t> bytesToEncrypt,
              std::vector<uint8_t> key,
              rmscrypto::api::CipherMode cm);

protected:

  const size_t AES_KEY_SIZE = 256;
  const size_t AES_BLOCK_SIZE_BYTES = 16;

  std::string Unescape(std::string source, bool skipReformat = false);

  std::shared_ptr<platform::json::IJsonArray> ConvertUserRights(const PublishCustomRequest& request);

  std::shared_ptr<CLCCacheResult> GetCLCCache(std::shared_ptr<IRestClientCache> cache, const std::string &email);

  //retrives CLC from server or cache, depending on if the cache exists
  std::string RetrieveCLC(const std::string& sEmail,
                          modernapi::IAuthenticationCallbackImpl& authenticationCallback,
                          std::shared_ptr<std::atomic<bool>> cancelState,
                          std::string &outClcPubData);

  //fixes stray quotes and braces in stringified payload
  std::vector<uint8_t> Reformat(std::vector<uint8_t> source, int currentlevel = 2);

  //adds escaping. '\' -> '\\' '"' -> '\"'
  std::vector<uint8_t> Escape(std::vector<uint8_t> source);

  //creates and returns CLC as JsonObject. provide getCLCCallback only for testing purposes
  std::shared_ptr<IJsonObject> CreateCLC(std::string& outClcPubData, const std::function<std::string(const std::string, std::string&)>& getCLCCallback, const std::string sEmail, modernapi::IAuthenticationCallbackImpl& authenticationCallback, std::shared_ptr<std::atomic<bool>> cancelState);

  //creates and returns License as JsonObject
  std::shared_ptr<IJsonObject> CreateLicense(std::shared_ptr<IJsonObject> clcPayload, std::string clcPubData);

  //creates and returns ad-hoc policy as JsonObject
  std::shared_ptr<IJsonObject> CreatePolicyAdhoc(PublishCustomRequest request, std::string isstoEmail);

  //creates and returns template policy as JsonObject
  std::shared_ptr<IJsonObject> CreatePolicyTemplate(PublishUsingTemplateRequest request, std::string isstoEmail);

  //creates and returns signed applicationd data as JsonObject
  std::shared_ptr<IJsonObject> CreateSignedAppData(modernapi::AppDataHashMap appdata);

  //creates PublishResponse
  PublishResponse CreateResponse(std::vector<uint8_t> licenseNoBOM, modernapi::AppDataHashMap signedAppData, rmscrypto::api::CipherMode cm, std::vector<uint8_t> contentkey, string ownerName);

  //initializes RSAKeyBlob class with proper info from CLC
  void RSAInit(std::shared_ptr<IJsonObject> pClc);

  //creates and sets both content key and session key. returns the content key. session key is returned in out param.
  vector<uint8_t> SetSessionKey(std::shared_ptr<IJsonObject> pLicense, bool prefDeprecatedAlgs, vector<uint8_t> &outSK);

  //sets the header for the given payload
  void SetHeader(std::shared_ptr<IJsonObject> pld);

  //reformats, signs, and verifies the payload
  std::vector<uint8_t> SignPayload(std::vector<uint8_t> pld);

  PublishResponse LocalPublishCommon(bool                                    isAdhoc,
    void*                                   request, size_t cbRequest,
    modernapi::IAuthenticationCallbackImpl& authenticationCallback,
    const std::string                     & sEmail,
    std::shared_ptr<std::atomic<bool> >     cancelState,
    const std::function<std::string(std::string, std::string&)>& getCLCCallback);


private:

  PublishResponse PublishCommon(
    common::ByteArray                    && requestBody,
    modernapi::IAuthenticationCallbackImpl& authenticationCallback,
    const std::string                     & sEmail,
    std::shared_ptr<std::atomic<bool> >     cancelState);
};
} // namespace restclients
} // namespace rmscore
#endif // _RMS_LIB_PUBLISHCLIENT_H_
