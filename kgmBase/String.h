#include <string.h>
#include <stdlib.h>

namespace kgmEngine
{
  #ifndef null
    #define null 0L
  #endif

  class String
  {
    enum Type
    {
      ASCII,
      UNICODE
    };

    union
    {
      char*    ascii;
      wchat_t* unicode;
    };

    size_t length;

    Type type;

    String()
    {
      type   = ASCII;
      ascii  = null;
      length = 0;
    }

    ~String()
    {
      if( ascii != null)
      {
        delete [] ascii;
      }
    }

    String& operator = (char* s)
    {
      clear();

      if(s != null)
      {
        size_t len = strlen(s);

        ascii = new char[len + 1];
        strncpy(ascii, s, len);
        ascii[len] = '\0';
        type = ASCII;
      }

      return *this;
    }

    bool operator == (char* s)
    {
      if(ascii == null || s == null)
      {
        return false;
      }

      size_t len = strlen(s);

      if(len < 1 || len != length)
      {
        return false;
      }
    }

    void clear()
    {
      if( ascii != null)
      {
        delete [] ascii;
      }

      type = ASCII;
      ascii = null;
      length = 0;
    }
  };

};
