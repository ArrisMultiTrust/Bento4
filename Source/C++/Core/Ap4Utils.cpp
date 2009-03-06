/*****************************************************************
|
|    AP4 - Utilities
|
|    Copyright 2002-2008 Axiomatic Systems, LLC
|
|
|    This file is part of Bento4/AP4 (MP4 Atom Processing Library).
|
|    Unless you have obtained Bento4 under a difference license,
|    this version of Bento4 is Bento4|GPL.
|    Bento4|GPL is free software; you can redistribute it and/or modify
|    it under the terms of the GNU General Public License as published by
|    the Free Software Foundation; either version 2, or (at your option)
|    any later version.
|
|    Bento4|GPL is distributed in the hope that it will be useful,
|    but WITHOUT ANY WARRANTY; without even the implied warranty of
|    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|    GNU General Public License for more details.
|
|    You should have received a copy of the GNU General Public License
|    along with Bento4|GPL; see the file COPYING.  If not, write to the
|    Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
|    02111-1307, USA.
|
 ****************************************************************/

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "Ap4Utils.h"

/*----------------------------------------------------------------------
|   AP4_BytesToDoubleBE
+---------------------------------------------------------------------*/
double
AP4_BytesToDoubleBE(const unsigned char* bytes)
{
    AP4_UI64 i_value = AP4_BytesToUInt64BE(bytes);
    void*    v_value = reinterpret_cast<void*>(&i_value);
    double*  d_value = reinterpret_cast<double*>(v_value);
    
    return *d_value;
}

/*----------------------------------------------------------------------
|   AP4_BytesToUInt64BE
+---------------------------------------------------------------------*/
AP4_UI64
AP4_BytesToUInt64BE(const unsigned char* bytes)
{
    return 
        ( ((AP4_UI64)bytes[0])<<56 ) |
        ( ((AP4_UI64)bytes[1])<<48 ) |
        ( ((AP4_UI64)bytes[2])<<40 ) |
        ( ((AP4_UI64)bytes[3])<<32 ) |
        ( ((AP4_UI64)bytes[4])<<24 ) |
        ( ((AP4_UI64)bytes[5])<<16 ) |
        ( ((AP4_UI64)bytes[6])<<8  ) |
        ( ((AP4_UI64)bytes[7])     );    
}

/*----------------------------------------------------------------------
|   AP4_BytesFromDoubleBE
+---------------------------------------------------------------------*/
void
AP4_BytesFromDoubleBE(unsigned char* bytes, double value)
{
    void*     v_value = reinterpret_cast<void*>(&value);
    AP4_UI64* i_value = reinterpret_cast<AP4_UI64*>(v_value);
    
    AP4_BytesFromUInt64BE(bytes, *i_value);
}

/*----------------------------------------------------------------------
|   AP4_BytesFromUInt64BE
+---------------------------------------------------------------------*/
void
AP4_BytesFromUInt64BE(unsigned char* bytes, AP4_UI64 value)
{
    bytes[0] = (unsigned char)(value >> 56);
    bytes[1] = (unsigned char)(value >> 48);
    bytes[2] = (unsigned char)(value >> 40);
    bytes[3] = (unsigned char)(value >> 32);
    bytes[4] = (unsigned char)(value >> 24);
    bytes[5] = (unsigned char)(value >> 16);
    bytes[6] = (unsigned char)(value >>  8);
    bytes[7] = (unsigned char)(value      );
}

/*----------------------------------------------------------------------
|   AP4_MakePrefixString
+---------------------------------------------------------------------*/
static void
AP4_MakePrefixString(unsigned int indent, char* prefix, AP4_Size size)
{
    if (size == 0) return;
    if (indent >= size-1) indent = size-1;
    for (unsigned int i=0; i<indent; i++) {
        prefix[i] = ' ';
    }
    prefix[indent] = '\0';    
}

/*----------------------------------------------------------------------
|   AP4_DurationMsFromUnits
+---------------------------------------------------------------------*/
AP4_UI32
AP4_DurationMsFromUnits(AP4_UI64 units, AP4_UI32 units_per_second)
{
	if (units_per_second == 0) return 0;
	return (AP4_UI32)(((double)units*1000.0)/(double)units_per_second);
}

/*----------------------------------------------------------------------
|   AP4_ConvertTime
+---------------------------------------------------------------------*/
AP4_UI64 
AP4_ConvertTime(AP4_UI64 time_value,
                AP4_UI32 from_time_scale,
                AP4_UI32 to_time_scale)
{
    if (from_time_scale == 0) return 0;
    double ratio = (double)to_time_scale/(double)from_time_scale;
    return ((AP4_UI64)((double)time_value*ratio));
}

/*----------------------------------------------------------------------
|   AP4_FormatFourChars
+---------------------------------------------------------------------*/
void
AP4_FormatFourChars(char* str, AP4_UI32 value) {
    str[0] = (value >> 24) & 0xFF;
    str[1] = (value >> 16) & 0xFF;
    str[2] = (value >>  8) & 0xFF;
    str[3] = (value      ) & 0xFF;
    str[4] = '\0';
}

/*----------------------------------------------------------------------
|   AP4_FormatFourCharsPrintable
+---------------------------------------------------------------------*/
void
AP4_FormatFourCharsPrintable(char* str, AP4_UI32 value) {
    AP4_FormatFourChars(str, value);
    for (int i=0; i<4; i++) {
        if (str[i]<' ' || str[i] >= 127) {
            str[i] = '.';
        }
    }
}

/*----------------------------------------------------------------------
|   AP4_SplitArgs
+---------------------------------------------------------------------*/
AP4_Result
AP4_SplitArgs(char* arg, char*& arg0, char*& arg1)
{
    arg0 = arg;
    char* c = arg;
    while (*c != 0 && *c != ':') {
        c++;
    }
    if (*c == ':') {
        *c++ = '\0';
        arg1 = c;
        return AP4_SUCCESS;
    } else {
        return AP4_FAILURE;
    }
}

/*----------------------------------------------------------------------
|   AP4_SplitArgs
+---------------------------------------------------------------------*/
AP4_Result
AP4_SplitArgs(char* arg, char*& arg0, char*& arg1, char*& arg2)
{
    AP4_Result result = AP4_SplitArgs(arg, arg0, arg1);
    if (AP4_FAILED(result)) return result;
    return AP4_SplitArgs(arg1, arg1, arg2);
}

/*----------------------------------------------------------------------
|   AP4_HexNibble
+---------------------------------------------------------------------*/
static unsigned char
AP4_HexNibble(char c)
{
    switch (c) {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'a': case 'A': return 10;
        case 'b': case 'B': return 11;
        case 'c': case 'C': return 12;
        case 'd': case 'D': return 13;
        case 'e': case 'E': return 14;
        case 'f': case 'F': return 15;
        default: return 0;
    }
}

/*----------------------------------------------------------------------
|   AP4_ParseHex
+---------------------------------------------------------------------*/
AP4_Result
AP4_ParseHex(const char* hex, unsigned char* bytes, unsigned int count)
{
    if (AP4_StringLength(hex) != 2*count) return AP4_ERROR_INVALID_PARAMETERS;
    for (unsigned int i=0; i<count; i++) {
        bytes[i] = (AP4_HexNibble(hex[2*i]) << 4) | (AP4_HexNibble(hex[2*i+1]));
    }
    return AP4_SUCCESS;
}

/*----------------------------------------------------------------------
|   AP4_PrintInspector::AP4_PrintInspector
+---------------------------------------------------------------------*/
    AP4_PrintInspector::AP4_PrintInspector(AP4_ByteStream& stream, AP4_Cardinal indent) :
    m_Stream(&stream),
    m_Indent(indent)
{
    m_Stream->AddReference();
}

/*----------------------------------------------------------------------
|   AP4_PrintInspector::~AP4_PrintInspector
+---------------------------------------------------------------------*/
AP4_PrintInspector::~AP4_PrintInspector()
{
    m_Stream->Release();
}

/*----------------------------------------------------------------------
|   AP4_PrintInspector::StartElement
+---------------------------------------------------------------------*/
void
AP4_PrintInspector::StartElement(const char* name, const char* info)
{
    char prefix[256];
    AP4_MakePrefixString(m_Indent, prefix, sizeof(prefix));
    m_Stream->WriteString(prefix);
    m_Stream->WriteString(name);
    if (info) {
        m_Stream->Write(" ", 1);
        m_Stream->WriteString(info);
    }
    m_Stream->Write("\n", 1);

    m_Indent += 2;
}

/*----------------------------------------------------------------------
|   AP4_PrintInspector::EndElement
+---------------------------------------------------------------------*/
void
AP4_PrintInspector::EndElement()
{
    m_Indent -= 2;
}

/*----------------------------------------------------------------------
|   AP4_PrintInspector::AddField
+---------------------------------------------------------------------*/
void
AP4_PrintInspector::AddField(const char* name, const char* value, FormatHint)
{
    char prefix[256];
    AP4_MakePrefixString(m_Indent, prefix, sizeof(prefix));
    m_Stream->WriteString(prefix);
    
    m_Stream->WriteString(name);
    m_Stream->WriteString(" = ");
    m_Stream->WriteString(value);
    m_Stream->Write("\n", 1);
}

/*----------------------------------------------------------------------
|   AP4_PrintInspector::AddField
+---------------------------------------------------------------------*/
void
AP4_PrintInspector::AddField(const char* name, AP4_UI64 value, FormatHint hint)
{
    char prefix[256];
    AP4_MakePrefixString(m_Indent, prefix, sizeof(prefix));
    m_Stream->WriteString(prefix);

    char str[32];
    AP4_FormatString(str, sizeof(str), 
                     hint == HINT_HEX ? "%llx":"%lld", 
                     value);
    m_Stream->WriteString(name);
    m_Stream->WriteString(" = ");
    m_Stream->WriteString(str);
    m_Stream->Write("\n", 1);
}

/*----------------------------------------------------------------------
|   AP4_PrintInspector::AddFieldF
+---------------------------------------------------------------------*/
void
AP4_PrintInspector::AddFieldF(const char* name, float value, FormatHint /*hint*/)
{
    char prefix[256];
    AP4_MakePrefixString(m_Indent, prefix, sizeof(prefix));
    m_Stream->WriteString(prefix);

    char str[32];
    AP4_FormatString(str, sizeof(str), 
                     "%f", 
                     value);
    m_Stream->WriteString(name);
    m_Stream->WriteString(" = ");
    m_Stream->WriteString(str);
    m_Stream->Write("\n", 1);
}

/*----------------------------------------------------------------------
|   AP4_PrintInspector::AddField
+---------------------------------------------------------------------*/
void
AP4_PrintInspector::AddField(const char*          name, 
                             const unsigned char* bytes, 
                             AP4_Size             byte_count,
                             FormatHint           /* hint */)
{
    char prefix[256];
    AP4_MakePrefixString(m_Indent, prefix, sizeof(prefix));
    m_Stream->WriteString(prefix);

    m_Stream->WriteString(name);
    m_Stream->WriteString(" = [");
    unsigned int offset = 1;
    char byte[4];
    for (unsigned int i=0; i<byte_count; i++) {
        AP4_FormatString(byte, 4, " %02x", bytes[i]);
        m_Stream->Write(&byte[offset], 3-offset);
        offset = 0;
    }
    m_Stream->Write("]\n", 2);
}
