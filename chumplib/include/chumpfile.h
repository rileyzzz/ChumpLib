
#ifndef _CHUMPFILE_H_
#define _CHUMPFILE_H_

class IOArchive;

class ChumpFile
{
private:

    ChumpFile() { }

    bool Serialize(IOArchive& Ar);
public:
    static ChumpFile read(const char* path);
};
#endif // _CHUMPFILE_H_
