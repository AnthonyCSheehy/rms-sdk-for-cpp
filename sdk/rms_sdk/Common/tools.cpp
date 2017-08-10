/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include <QUuid>
#include "tools.h"

#define TIME_CONVERSION_MS_TO_100NS 10000

using namespace std;

namespace rmscore {
namespace common {
uint64_t timeToWinFileTime(const QDateTime& dateTime) {
  // Definition of FILETIME from MSDN:
  // Contains a 64-bit value representing the number of 100-nanosecond intervals
  // since January 1, 1601 (UTC).
  QDateTime origin(QDate(1601, 1, 1), QTime(0, 0, 0, 0), Qt::UTC);

  // Get offset - note we need 100-nanosecond intervals, hence we multiply by
  // 10000.
  return TIME_CONVERSION_MS_TO_100NS * origin.msecsTo(dateTime);
}

vector<uint8_t> ConvertBase64ToBytes(const vector<uint8_t>& base64str) {
  QByteArray ba;

  ba.append(QByteArray(reinterpret_cast<const char *>(base64str.data()),
                       static_cast<int>(base64str.size())));

  auto convArray = QByteArray::fromBase64(ba);

  return vector<uint8_t>(convArray.begin(), convArray.end());
}

string timeToString(const QDateTime& dateTime) {
  if (!dateTime.isNull())
  {
    //    return dateTime.toString("yyyy-MM-DDThh:mm:ssZ").toStdString();
    return dateTime.toString(Qt::ISODate).toStdString();
  }
  else
  {
    return string();
  }
}

std::string ReplaceString(std::string subject, const std::string& search,
                          const std::string& replace, int occurrences)
{
    size_t pos = 0;
    int found = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos)
    {
         subject.replace(pos, search.length(), replace);
         if (occurrences != 0)
         {
            ++found;
            if (found >= occurrences)
                break;
         }
         pos += replace.length();
    }
    return subject;
}

vector<uint8_t> ConvertBytesToBase64(const vector<uint8_t>& bytes) {
  return ConvertBytesToBase64(bytes.data(), bytes.size());
}

vector<uint8_t> ConvertBytesToBase64(const void *bytes, const size_t size)
{
  QByteArray ba;

  ba.append(reinterpret_cast<const char *>(bytes), static_cast<int>(size));

  auto convArray = ba.toBase64();

  return vector<uint8_t>(convArray.begin(), convArray.end());
}

vector<uint8_t> HashString(const vector<uint8_t>& bytes, size_t *size, bool isSHA256)
{
    if (isSHA256)
    {
        vector<uint8_t> hash(SHA256_DIGEST_LENGTH);
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, &bytes[0], bytes.size());
        SHA256_Final(&hash[0], &sha256);
        *size = SHA256_DIGEST_LENGTH;
        return hash;
    }
    else
    {
        //sha1
        vector<uint8_t> hash(SHA_DIGEST_LENGTH);
        SHA1(&bytes[0], bytes.size(), &hash[0]);
        *size = SHA_DIGEST_LENGTH;
        return hash;
    }
}

std::string Hash(const std::string& str, size_t *size)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.length());
    SHA256_Final(hash, &sha256);
    *size = SHA256_DIGEST_LENGTH;
    char mdString[SHA256_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)hash[i]);
    return std::string(mdString);
}

string GenerateAGuid()
{
  return QUuid::createUuid().toString().toStdString();
}

string Unescape(string source, bool skipReformat){
    stringstream ss;
    for (size_t i = 0; i < source.length(); i++)
        if (source[i] != '\\')
            ss << source[i];
    auto ret = ss.str();
    if (!skipReformat)
    {
        ret = common::ReplaceString(ret, "\"{\"", "{\"");
        ret = common::ReplaceString(ret, "}}\"", "}}");
    }
    return ret;
}

} // namespace common
} // namespace rmscore
