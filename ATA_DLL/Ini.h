#ifndef _ATA_DLL_INI_H_
#define _ATA_DLL_INI_H_
#include <string>

class TIniFile
{
private:
    char m_IniFileName[4096];
    char m_buffer[4096];
    char  m_StrParse[32768];
    short m_StrParsePos[32768];
    char  str[32768];
    short str_pos[32768];
    int _ArrayString_Parse(const char *src, char* dst, short* dst_pos );
    int _ArrayString_ParseWithLetter(const char *src, char* dst, short* dst_pos );
public:
    TIniFile();
    TIniFile(const char* filename);
    bool ValueExists(const char* section, const char* key);
    bool WriteString(const char* section, const char* key, const char* write_buffer);
    std::string ReadString(const char* section, const char* key, const char* default_str);
    bool   ReadBool(const char* section, const char* key, bool default_value);
    int    ReadInteger(const char* section, const char* key, int default_value);
    double ReadFloat(const char* section, const char* key, double default_value);
    bool   WriteFloat(const char* section, const char* key, double write_value);
    bool   WriteInteger(const char* section, const char* key, int write_value);
    bool   ReadArrayInteger(const char* section, const char* key, int *default_value, int max);
    bool   ReadArrayFloat(const char* section, const char* key, double *default_value, int max);
    bool   WriteArrayInteger(const char* section, const char* key, int *write_value, int max);
    bool   WriteArrayFloat(const char* section, const char* key, double *write_value, int max);
    void DeleteKey(const char* section, const char* key);
    bool SectionExists(const char *section);
    int Array_To_String_Int( char **str, int* array, int count, char separate_char = ',');
    int Array_To_String_Char(char **str, char* array, int count, char separate_char = ',');
    int Array_To_String_UnsignedChar( char **str, unsigned char* array, int count, char separate_char = ',');
    int Array_To_String_ASCIIChar(char **str, const char* array, int count, char separate_char = ',' );
    int Array_To_String_Short(char **str, const short* array, int count, char separate_char = ',' );
    int Array_To_String_Double(char **str, const double* array, int count, char separate_char, int precision = 5);
    int Array_To_String_Float(char **str, const float* array, int count, char separate_char, int precision = 5);
    int Array_To_String_UnsignedShort(char **str, const unsigned short* array, int count, char separate_char = ',');
    int String_To_Array_Char(const char *src, char* array, int max_count );
    int String_To_Array_UnsignedChar(const char *src, unsigned char* array, int max_count );
    int String_To_Array_Int(const char *src, int* array, int max_count );
    int String_To_Array_Short(const char *src, short* array, int max_count );
    int String_To_Array_Float(const char *src, float* array, int max_count );
    int String_To_Array_Double(const char *src, double* array, int max_count );
    int String_To_Array_UnsignedShort(const char *src, unsigned short* array, int max_count );
    int String_To_Array_ASCIIChar(const char *src, char* array, int max_count );
    int String_To_Array_AnsiString(const char *src, std::string* array, int max_count );
};

#endif
