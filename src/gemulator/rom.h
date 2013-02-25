#ifndef _ROM_H

#define _ROM_H

typedef char byte;
typedef short halfword;
typedef int word;
typedef unsigned int uword;

using namespace std;

class ROM {
  private:
    char* file_name;
    byte* bytes;
    word address;
  public:
    ROM(const char* file_name);
    const byte* get_bytes();
    const word get_word();
    const halfword get_halfword();
    void set_offset(word address);
    void inc_offset(word inc);
    void dec_offset(word dec);
    word get_offset();
    ~ROM();
};
#endif
