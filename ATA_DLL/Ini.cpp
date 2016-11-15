#include "stdafx.h"
#include "Ini.h"
#include "common_api.h"
// due to the unsafe usage of sprintf in conversion function
// TODO: modify the unsafe usage and replace the conversion function
#pragma warning (disable : 4996)
bool TIniFile::ValueExists(const char* section, const char* key)
{
    int count;
    count = GetPrivateProfileString( section, key, NULL, m_buffer, sizeof(m_buffer), m_IniFileName);
    if(count)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool TIniFile::WriteString(const char* section, const char* key, const char* write_buffer)
{
    int count;
    count = WritePrivateProfileString( section, key, write_buffer, m_IniFileName);
    if(!count)
    {
        return false;
    }
    return true;
}
std::string TIniFile::ReadString(const char* section, const char* key, const char* default_str)
{
    int count;
    std::string ret;
    count = GetPrivateProfileString( section, key, default_str, m_buffer, sizeof(m_buffer), m_IniFileName);
    ret = m_buffer;
    return ret;
}
bool TIniFile::ReadBool(const char* section, const char* key, bool default_value)
{
    int ret = ReadInteger(section, key, (default_value == true) ? 1 : 0);
    if(ret == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}
int TIniFile::ReadInteger(const char* section, const char* key, int default_value)
{
    int ret;
    int count;
    std::string default_str;
    sprintf_s(m_buffer, sizeof(m_buffer), "%d", default_value);
    default_str = m_buffer;
    count = GetPrivateProfileString( section, key, default_str.c_str(), m_buffer, sizeof(m_buffer), m_IniFileName);
    ret = atoi(m_buffer);
    return ret;
}
bool TIniFile::WriteInteger(const char* section, const char* key, int write_value)
{
    int count;
    sprintf_s(m_buffer, sizeof(m_buffer), "%d", write_value);
    count = WritePrivateProfileString( section, key, m_buffer, m_IniFileName);
    if(!count)
    {
        return false;
    }
    return true;
}

double TIniFile::ReadFloat(const char* section, const char* key, double default_value)
{
    double ret;
    int count;
    std::string default_str;
    sprintf_s(m_buffer, sizeof(m_buffer), "%lf", default_value);
    default_str = m_buffer;
    count = GetPrivateProfileString( section, key, default_str.c_str(), m_buffer, sizeof(m_buffer), m_IniFileName);
    ret = atof(m_buffer);
    return ret;
}

bool TIniFile::WriteFloat(const char* section, const char* key, double write_value)
{
    int count;
    sprintf_s(m_buffer, sizeof(m_buffer), "%lf", write_value);
    count = WritePrivateProfileString( section, key, m_buffer, m_IniFileName);
    if(!count)
    {
        return false;
    }
    return true;
}
bool TIniFile::ReadArrayInteger(const char* section, const char* key, int *default_value, int max)
{
#if 0
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
#endif
    return true;
}
bool TIniFile::WriteArrayInteger(const char* section, const char* key, int *write_value, int max)
{
#if 0
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
#endif
    return true;
}
bool TIniFile::ReadArrayFloat(const char* section, const char* key, double *default_value, int max)
{
#if 0
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
#endif
    return true;
}
bool TIniFile::WriteArrayFloat(const char* section, const char* key, double *write_value, int max)
{
#if 0
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
    /* under construction !*/
#endif
    return true;
}
TIniFile::TIniFile(const char* filename)
{
    strcpy_s(m_IniFileName, sizeof(m_IniFileName), filename);
}
TIniFile::TIniFile()
{
}

bool TIniFile::SectionExists(const char *section)
{
    int count = GetPrivateProfileSection(section, m_buffer, sizeof(m_buffer),m_IniFileName);
    if(count >0) // at least copy one character into buffer,
    {
        return true;
    }
    else
        return false;
}
//--------------------------------------------------------------------------
int TIniFile::Array_To_String_Int( char **str, int* array, int count, char separate_char )
{
    char  *ptr = *str = m_buffer;
    int   i;

    for( i=0; i<count-1; i++ )
    {
        ptr += sprintf( ptr, "%d%c", array[i], separate_char );
    }
    ptr += sprintf( ptr, "%d", array[i] );
    i = (int)((unsigned long)ptr - (unsigned long)str);

    return( i );
}

//--------------------------------------------------------------------------
int TIniFile::Array_To_String_Short( char **str, const short* array, int count, char separate_char )
{
    int   i;
    char  *ptr = *str = m_buffer;
    for( i=0; i<count-1; i++ )
    {
        ptr += sprintf( ptr, "%d%c", (int)array[i], separate_char );
    }
    ptr += sprintf( ptr, "%d", (int)array[i] );
    i = (int)((unsigned long)ptr - (unsigned long)*str);
    return( i );
}
//--------------------------------------------------------------------------
int TIniFile::Array_To_String_UnsignedShort( char **str, const unsigned short* array, int count, char separate_char)
{
    int   i;
    char  *ptr = *str = m_buffer;
    for( i=0; i<count-1; i++ )
    {
        ptr += sprintf( ptr, "%d%c", (unsigned short)array[i], separate_char );
    }
    ptr += sprintf( ptr, "%d", (unsigned short)array[i] );
    i = (int)((unsigned long)ptr - (unsigned long)*str);
    return( i );
}
//--------------------------------------------------------------------------
int TIniFile::Array_To_String_Double(char **str, const double* array, int count, char separate_char, int precision)
{
    int i;
    char  *ptr = *str = m_buffer;
    char format[128];
    sprintf(format, "%%.%dlf%%c", precision);
    for (i=0; i<count-1; i++)
    {
        ptr += sprintf( ptr, format, array[i], separate_char);
    }
    ptr += sprintf( ptr, format, array[i], '\0' );
    i = (int)((unsigned long)ptr - (unsigned long)*str);
    return i;
}
//--------------------------------------------------------------------------
int TIniFile::Array_To_String_Float(char **str, const float* array, int count, char separate_char, int precision)
{
    int i;
    char  *ptr = *str = m_buffer;
    char format[128];
    sprintf(format, "%%.%dlf%%c", precision);
    for (i=0; i<count-1; i++)
    {
        ptr += sprintf( ptr, format, array[i], separate_char);
    }
    ptr += sprintf( ptr, format, array[i], '\0' );
    i = (int)((unsigned long)ptr - (unsigned long)*str);
    return i;
}
//---------------------------------------------------------------------------
int TIniFile::String_To_Array_Int(const char *src, int* array, int max_count )
{
    int   i, count;
    count = _ArrayString_Parse( src, m_StrParse, m_StrParsePos );
    if( count>max_count )
    {
        count = max_count;
    }
    for(i=0; i<count; i++)
    {
        array[i] = (int)atoi( &m_StrParse[m_StrParsePos[i]] );
    }
    return( count );
}
//---------------------------------------------------------------------------
int TIniFile::String_To_Array_Short(const char *src, short* array, int max_count )
{
    int   i, count;
    count = _ArrayString_Parse( src, m_StrParse, m_StrParsePos );
    if( count>max_count )
    {
        count = max_count;
    }
    for(i=0; i<count; i++)
    {
        array[i] = (short)atoi( &m_StrParse[m_StrParsePos[i]] );
    }
    return( count );
}
//---------------------------------------------------------------------------
int TIniFile::String_To_Array_UnsignedShort(const char *src, unsigned short* array, int max_count )
{
    int   i, count;
    count = _ArrayString_Parse( src, m_StrParse, m_StrParsePos );
    if( count>max_count )
    {
        count = max_count;
    }
    for(i=0; i<count; i++)
    {
        array[i] = (unsigned short) atoi( &m_StrParse[m_StrParsePos[i]] );
    }
    return( count );
}
//---------------------------------------------------------------------------
int TIniFile::String_To_Array_Float(const char *src, float* array, int max_count )
{
    int   i, count;
    count = _ArrayString_Parse( src, m_StrParse, m_StrParsePos );
    if( count>max_count )
    {
        count = max_count;
    }
    for(i=0; i<count; i++)
    {
        array[i] = (float)atof( &m_StrParse[m_StrParsePos[i]] );
    }
    return count;
}
//---------------------------------------------------------------------------
int TIniFile::String_To_Array_Double(const char *src, double* array, int max_count )
{
    int   i, count;
    count = _ArrayString_Parse( src, m_StrParse, m_StrParsePos );
    if( count>max_count )
    {
        count = max_count;
    }
    for(i=0; i<count; i++)
    {
        array[i] = atof( &m_StrParse[m_StrParsePos[i]] );
    }
    return count;
}
//---------------------------------------------------------------------------
int TIniFile::_ArrayString_Parse(const char *src, char* dst, short* dst_pos )
{
    char  ch = src[0];
    char  last_char_is_null = 1;
    int   dst_idx = 0;
    int   i;
    for( i=1; (ch!=0)&&(i<32767); i++ )
    {
        if(  ((ch>='0')&&(ch<='9'))||(ch=='-')||(ch=='+')||(ch=='.')||(ch=='E') ||(ch=='e'))
        {
            if( last_char_is_null )
            {
                dst_pos[dst_idx] = i;
                dst_idx++;
            }
            dst[i] = ch;
            last_char_is_null = 0;
        }
        else
        {
            dst[i] = 0;
            last_char_is_null = 1;
        }
        ch = src[i];
    }
    dst[i] = 0;
    dst_pos[dst_idx] = -1;
    return( dst_idx );
}
int TIniFile::_ArrayString_ParseWithLetter(const char *src, char* dst, short* dst_pos )
{
    char  ch = src[0];
    char  last_char_is_null = 1;
    int   dst_idx = 0;
    int   i;
    for( i=1; (ch!=0)&&(i<32768-1); i++ )
    {
        if(  ((ch>='0')&&(ch<='9')) ||
                (ch=='-')||
                (ch=='+')||
                (ch=='.')||
                (ch=='E')
                ||(ch=='e') ||
                (ch >='A' && ch <='Z') ||
                (ch >='a' && ch <='z')
          )
        {
            if( last_char_is_null )
            {
                dst_pos[dst_idx] = i;
                dst_idx++;
            }
            dst[i] = ch;
            last_char_is_null = 0;
        }
        else
        {
            dst[i] = 0;
            last_char_is_null = 1;
        }
        ch = src[i];
    }
    dst[i] = 0;
    dst_pos[dst_idx] = -1;
    return( dst_idx );
}
int TIniFile::Array_To_String_Char(char **str, char* array, int count, char separate_char)
{
    char  *ptr = *str = m_buffer;
    int   i;

    for( i=0; i<count-1; i++ )
    {
        ptr += sprintf( ptr, "%d%c", (char)array[i], separate_char );
    }
    ptr += sprintf( ptr, "%d", (char)array[i] );
    i = (int)((unsigned long)ptr - (unsigned long)str);

    return i;
}
int TIniFile::Array_To_String_ASCIIChar( char **str, const char* array, int count, char separate_char )
{
    char  *ptr = *str = m_buffer;
    int   i;

    for( i=0; i<count-1; i++ )
    {
        ptr += sprintf( ptr, "%c%c", (char)array[i], separate_char );
    }
    ptr += sprintf( ptr, "%c", (char)array[i] );
    i = (int)((unsigned long)ptr - (unsigned long)str);

    return i;
}
int TIniFile::Array_To_String_UnsignedChar( char **str, unsigned char* array, int count, char separate_char )
{
    char  *ptr = *str = m_buffer;
    int   i;

    for( i=0; i<count-1; i++ )
    {
        ptr += sprintf( ptr, "%d%c", (unsigned char)array[i], separate_char );
    }
    ptr += sprintf( ptr, "%d", (unsigned char)array[i] );
    i = (int)((unsigned long)ptr - (unsigned long)str);

    return( i );
}
int TIniFile::String_To_Array_ASCIIChar(const char *src, char* array, int max_count )
{
    int   i, count;
    count = _ArrayString_ParseWithLetter( src, str, str_pos );
    if( count>max_count )  count = max_count;
    for(i=0; i<count; i++)
    {
        array[i] = str[str_pos[i]];
    }
    return( count );
}
int TIniFile::String_To_Array_UnsignedChar(const char *src, unsigned char* array, int max_count )
{
    int   i, count;
    count = _ArrayString_Parse( src, str, str_pos );
    if( count>max_count )  count = max_count;

    for(i=0; i<count; i++)
    {
        array[i] = (unsigned char) atoi( &str[str_pos[i]] );
    }
    return( count );
}
int TIniFile::String_To_Array_Char(const char *src, char* array, int max_count )
{
    int   i, count;
    count = _ArrayString_Parse( src, str, str_pos );
    if( count>max_count )  count = max_count;
    for(i=0; i<count; i++)
    {
        array[i] = (char) atoi( &str[str_pos[i]] );
    }
    return( count );
}
int TIniFile::String_To_Array_AnsiString(const char *src, std::string* array, int max_count )
{
    int   i, count;
    count = _ArrayString_ParseWithLetter( src, str, str_pos );
    if( count>max_count )  count = max_count;
    for(i=0; i<count; i++)
    {
        std::string tempstr( &str[str_pos[i]] );
        array[i] = tempstr;
    }
    return( count );
}
void TIniFile::DeleteKey(const char* section, const char* key)
{
    WritePrivateProfileString( section, key, NULL, m_IniFileName);
}
bool getSubStrFromStrByRightBasedIndex(std::string asStr, std::string asDelimiter, int rindex, int total_num_delimiter, std::string &asResult)
{

    if(rindex >total_num_delimiter)
        return false;


    int iLD = 0;
    std::string astmp = asStr;

    std::string astmpPrev;

    // locate the sub string

    for( int i =0; (i<=rindex && i< total_num_delimiter); i++)
    {
        iLD = astmp.find_last_of(asDelimiter);
        if(iLD<=0)
            return false;
        astmpPrev = astmp;
        astmp = astmp.substr(0, iLD);
    }

    if(rindex == total_num_delimiter)  // get the left most valid string
    {
        asResult =  astmp;
    }
    else
    {
        asResult = astmpPrev.substr(iLD+1, astmpPrev.length());
    }


    return true;
}
