/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
 */

#ifdef QTFRAMEWORK
#include <QVariantMap>
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>

#include "JsonObjectQt.h"
#include "JsonArrayQt.h"
#include "JsonParserQt.h"

#include "../../ModernAPI/RMSExceptions.h"
#include "../Logger/Logger.h"

using namespace rmscore::platform::logger;

namespace rmscore {
namespace platform {
namespace json {
std::shared_ptr<IJsonObject>IJsonObject::Create()
{
  return std::make_shared<JsonObjectQt>();
}

bool JsonObjectQt::IsNull(const std::string& name)
{
  QJsonObject jo = this->impl_.toObject();

  return jo.contains(name.c_str())
         ? jo[name.c_str()].isNull()
         : throw exceptions::RMSInvalidArgumentException(
                 "JsonObjectQt::IsNull: Name doesn't exist");
}

std::shared_ptr<IJsonArray>JsonObjectQt::GetArray()
{
  if (!this->impl_.isArray())
  {
    throw exceptions::RMSInvalidArgumentException(
            "JsonObjectQt::GetArray: The object is not an array");
  }

  QJsonArray arr = this->impl_.toArray();

  return std::shared_ptr<IJsonArray>(new JsonArrayQt(arr));
}

bool JsonObjectQt::HasName(const std::string& name)
{
  QJsonObject jo = this->impl_.toObject();

  return jo.contains(name.c_str());
}

std::string JsonObjectQt::GetNamedString(const std::string& name,
                                         const std::string& defaultValue)
{
  QJsonObject jo = this->impl_.toObject();

  if (jo.contains(name.c_str()) && !jo[name.c_str()].isNull()) {
    auto obj = jo[name.c_str()];

    if (obj.isString()) {
      return obj.toString().toStdString();
    }

    throw exceptions::RMSInvalidArgumentException(
            "JsonObjectQt::GetNamedString: convertion error");
  }
  return defaultValue;
}

void JsonObjectQt::SetNamedString(const std::string& name,
                                  const std::string& value)
{
  QJsonObject jo = this->impl_.toObject();

  jo.insert(name.c_str(), QJsonValue(QString::fromStdString(value)));
  this->impl_ = QJsonValue(jo);
}

bool JsonObjectQt::GetNamedBool(const std::string& name, bool bDefaultValue)
{
  QJsonObject jo = this->impl_.toObject();

  if (jo.contains(name.c_str()) && !jo[name.c_str()].isNull()) {
    auto obj = jo[name.c_str()];

    if (obj.isBool()) {
      return obj.toBool();
    }

    throw exceptions::RMSInvalidArgumentException(
            "JsonObjectQt::GetNamedBool: convertion error");
  }
  return bDefaultValue;
}

void JsonObjectQt::SetNamedBool(const std::string& name, bool bValue)
{
  QJsonObject jo = this->impl_.toObject();

  jo.insert(name.c_str(), bValue);
  this->impl_ = QJsonValue(jo);
}

double JsonObjectQt::GetNamedNumber(const std::string& name, double fDefaultValue)
{
  QJsonObject jo = this->impl_.toObject();

  if (jo.contains(name.c_str())) {
    auto joVal = jo[name.c_str()];

    if (!joVal.isNull()) {
      if (jo[name.c_str()].isDouble()) {
        return jo[name.c_str()].toDouble();
      } else {
        throw exceptions::RMSInvalidArgumentException(
                "JsonObjectQt::GetNamedNumber: convertion error");
      }
    }
  }
  return fDefaultValue;
}

void JsonObjectQt::SetNamedNumber(const std::string& name, double fValue)
{
  QJsonObject jo = this->impl_.toObject();

  jo.insert(name.c_str(), fValue);
  this->impl_ = QJsonValue(jo);
}

std::shared_ptr<IJsonObject>JsonObjectQt::GetNamedObject(const std::string& name)
{
  QJsonObject jo = this->impl_.toObject();

  if (!jo.contains(name.c_str()))
  {
    return nullptr;
  }

  QJsonValue val = jo.value(name.data());

  if (!val.isObject())
  {
      if(val.isString()){
          QJsonDocument qJsonDocument = QJsonDocument::fromJson(val.toString().toUtf8());
          if(!qJsonDocument.isNull() && qJsonDocument.isObject()){
              return std::make_shared<JsonObjectQt>(qJsonDocument.object());
          }
      }
      throw exceptions::RMSInvalidArgumentException(
            "JsonObjectQt::GetNamedObject: convertion error");
  }

  return std::make_shared<JsonObjectQt>(val);
}

void JsonObjectQt::SetNamedObject(const std::string& name,
                                  const IJsonObject& jsonObject)
{
  QJsonObject jo = this->impl_.toObject();
  auto other     = static_cast<const JsonObjectQt&>(jsonObject);

  jo.insert(name.c_str(), other.impl());
  this->impl_ = QJsonValue(jo);
}

std::shared_ptr<IJsonArray>JsonObjectQt::GetNamedArray(const std::string& name)
{
  QJsonObject jo = this->impl_.toObject();

  if (!jo.contains(name.c_str()))
  {
    return nullptr;
  }

  QJsonValue val = jo.value(name.data());

  if (!val.isArray())
  {
    throw exceptions::RMSInvalidArgumentException("the value is not an array");
  }

  return std::shared_ptr<JsonArrayQt>(new JsonArrayQt(val.toArray()));
}

void JsonObjectQt::SetNamedArray(const std::string& name,
                                 const IJsonArray & jsonArray)
{
  QJsonObject jo = this->impl_.toObject();
  auto other     = static_cast<const JsonArrayQt&>(jsonArray);

  jo.insert(name.c_str(), QJsonValue(other.impl()));
  this->impl_ = QJsonValue(jo);
}

void JsonObjectQt::SetNamedValue(const std::string      & name,
                                 const vector<uint8_t>& value) {
  QJsonObject jo = this->impl_.toObject();

  QVariant jsonVar(QByteArray(reinterpret_cast<const char *>(value.data()),
                              static_cast<int>(value.size())));

  jo.insert(name.c_str(), QJsonValue::fromVariant(jsonVar));
  this->impl_ = QJsonValue(jo);
}

vector<uint8_t> JsonObjectQt::GetNamedValue(const std::string& name)
{
  QJsonObject jo = this->impl_.toObject();
  auto nameStr   = name.c_str();
  auto ret       = jo.contains(nameStr)
                   ? jo[nameStr].isString()
                   ? jo[nameStr].toString().toUtf8()
                   : throw exceptions::RMSInvalidArgumentException(
                           "JsonObjectQt::GetNamedValue: convertion error")
                         : QByteArray();

  return vector<uint8_t>(ret.begin(), ret.end());
}

StringArray JsonObjectQt::GetNamedStringArray(const std::string& name)
{
  QJsonObject jo = this->impl_.toObject();

  if (!jo.contains(name.c_str()))
  {
    Logger::Warning(
      "JsonObjectQt::GetNamedStringArray: Can't find the key named '%s'",
      name.c_str());
    return std::vector<std::string>();
  }

  QJsonValue val = jo[name.c_str()];

  if (!val.isArray())
  {
    throw exceptions::RMSInvalidArgumentException("the value is not an array");
  }

  StringArray list;
  QJsonArray  arr = val.toArray();

  for (int i = 0; i < arr.size(); ++i)
  {
    QJsonValue val = arr[i];
    list.push_back(val.toString().toStdString());
  }

  return list;
}

std::shared_ptr<IJsonObject> JsonObjectQt::GetNestedNamedObject(const string &name)
{
    QJsonObject jo = this->impl_.toObject();
    auto objstr = GetNamedString(name, "");
    string ret = "";
    uint32_t limit = objstr.size() - 1;

    uint32_t i;
    for (i = 0; i < limit; i++)
    {
        char currchar = objstr[i];
        char nextchar = objstr[i + 1];

        if (currchar != '\\')
            ret += currchar;
        else if (currchar == '\\' && nextchar == '\\')
        {
            ++i;
            while (objstr[i] == '\\')
            {
                ++i;
                ret += objstr[i];
            }
        }

    }

    return IJsonParser::Create()->Parse(vector<uint8_t>(ret.begin(), ret.end()));
}

modernapi::AppDataHashMap JsonObjectQt::ToStringDictionary()
{
  QVariantMap map =  this->impl_.toObject().toVariantMap();

  modernapi::AppDataHashMap result;

  for (QString& key : map.keys())
  {
    result.insert(std::make_pair(key.toStdString(),
                                 map[key].toString().toStdString()));
  }
  return result;
}

vector<uint8_t> JsonObjectQt::Stringify(bool withEscaping)
{
  QJsonDocument doc(this->impl_.toObject());
  QByteArray res;
  if (withEscaping)
    res = doc.toJson(QJsonDocument::Compact);
  else
    res = doc.toVariant().toByteArray();

  return vector<uint8_t>(res.begin(), res.end());
}
}
}
} // namespace rmscore { namespace platform { namespace json {
#endif // ifdef QTFRAMEWORK
