/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifndef IJSONPARSER
#define IJSONPARSER

#include <memory>
#include <string>

#include "../../Common/FrameworkSpecificTypes.h"
namespace rmscore { namespace platform { namespace json {

class IJsonObject;
class IJsonArray;

class IJsonParser
{
public:
    virtual std::shared_ptr<IJsonObject> Parse(const std::vector<uint8_t>& json) = 0;
    virtual std::shared_ptr<IJsonObject> Parse(const std::string& json) = 0;
    virtual std::shared_ptr<IJsonArray> ParseArray(const std::vector<uint8_t>& jsonArray) = 0;
    static std::shared_ptr<IJsonParser> Create();
};

}}} // namespace rmscore { namespace platform { namespace json {

#endif // IJSONPARSER

