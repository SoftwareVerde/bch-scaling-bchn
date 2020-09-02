// Copyright 2014 BitPay Inc.
// Copyright (c) 2020 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/licenses/mit-license.php.

#include <cstring>
#include <stdio.h>
#include "univalue.h"
#include "univalue_escapes.h"

/* static */
void UniValue::jsonEscape(Stream & ss, const std::string & inS)
{
    for (const auto ch : inS) {
        const char * const escStr = escapes[uint8_t(ch)];

        if (escStr)
            ss << escStr;
        else
            ss.put(ch);
    }
}

/* static */
inline void UniValue::startNewLine(Stream & ss, const unsigned int prettyIndent, const unsigned int indentLevel)
{
    if (prettyIndent) {
        ss.put('\n');
        ss.put(' ', indentLevel);
    }
}

/* static */
void UniValue::stringify(Stream& ss, const UniValue& value, const unsigned int prettyIndent, const unsigned int indentLevel)
{
    switch (value.typ) {
    case VNULL:
        ss.write("null", 4); // .write() is slightly faster than operator<<
        break;
    case VOBJ:
        stringify(ss, value.entries, prettyIndent, indentLevel);
        break;
    case VARR:
        stringify(ss, value.values, prettyIndent, indentLevel);
        break;
    case VSTR:
        stringify(ss, value.val, prettyIndent, indentLevel);
        break;
    case VNUM:
        ss << value.val;
        break;
    case VBOOL:
        stringify(ss, value.val == boolTrueVal, prettyIndent, indentLevel);
        break;
    }
}

/* static */
void UniValue::stringify(Stream& ss, const bool boolean, const unsigned int prettyIndent, const unsigned int indentLevel)
{
    if (boolean)
        ss.write("true", 4);
    else
        ss.write("false", 5);
}

/* static */
void UniValue::stringify(Stream& ss, const std::string& string, const unsigned int prettyIndent, const unsigned int indentLevel)
{
    ss.put('"');
    jsonEscape(ss, string);
    ss.put('"');
}

/* static */
void UniValue::stringify(Stream & ss, const UniValue::Array& array, const unsigned int prettyIndent, const unsigned int indentLevel)
{
    ss.put('[');
    if (!array.empty()) {
        const unsigned int internalIndentLevel = indentLevel + prettyIndent;
        for (auto value = array.begin(), end = array.end();;) {
            startNewLine(ss, prettyIndent, internalIndentLevel);
            stringify(ss, *value, prettyIndent, internalIndentLevel);
            if (++value == end) {
                break;
            }
            ss.put(',');
        }
    }
    startNewLine(ss, prettyIndent, indentLevel);
    ss.put(']');
}

/* static */
void UniValue::stringify(Stream & ss, const UniValue::Object& object, const unsigned int prettyIndent, const unsigned int indentLevel)
{
    ss.put('{');
    if (!object.empty()) {
        const unsigned int internalIndentLevel = indentLevel + prettyIndent;
        for (auto entry = object.begin(), end = object.end();;) {
            startNewLine(ss, prettyIndent, internalIndentLevel);
            ss.put('"');
            jsonEscape(ss, entry->first);
            ss.write("\":", 2);
            if (prettyIndent) {
                ss.put(' ');
            }
            stringify(ss, entry->second, prettyIndent, internalIndentLevel);
            if (++entry == end) {
                break;
            }
            ss.put(',');
        }
    }
    startNewLine(ss, prettyIndent, indentLevel);
    ss.put('}');
}
