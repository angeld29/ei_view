#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <ctime>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cctype>
#include <clocale>

using namespace std;
namespace evil_islands {
    typedef std::vector<uint8_t> MemData;
    typedef std::shared_ptr<memdata_t> PMemData;

    class ResArchive
    {
        private:
            struct header_s
            {
                std::uint32_t magic; // 3C E2 9C 01

                std::uint32_t table_size; // число файлов
                std::uint32_t table_offset;

                std::uint32_t names_size;
            };
            struct file_info_s
            {
                std::uint32_t   next_index;

                std::uint32_t   file_size;
                std::uint32_t   file_offset;

                time_t last_change_time; // 4 байтовое время Unix

                std::uint16_t   name_length;
                std::uint32_t   name_offset; // смещение в массиве `names`
            };
            struct file_info_name_s {
                struct file_info_s info;
                std::string name;
            };
            std::string filepath_;
            ifstream infile_;
            struct header_s header_;
            std::vector<struct file_info_name_s> files_;

            //pak_item_t FindFile(const std::string& fname);
        public:
            ResArchive(const std::string filepath);
            ~ResArchive();
            PMemData LoadFile( const std::string& fname); //данные как они есть в lod файле
            std::vector<struct file_info_name_s> GetFileList( ){ return files_; }
        protected:

    };
    ResArchive::ResArchive( const std::string filepath )
    {
        infile_.open( filepath, ios::in|ios::binary );
        if( !infile_ ) return;
        infile_.read((char*)&header_, sizeof(header_));
        if( header_.magic != 0x019CE23C )
        {
            cout << "Invalid magic in "<< filepath << endl ;
            infile_.close();
            return;
        }

        infile_.seekg( header_.table_offset);
        files_.resize(header_.table_size);
        file_info_s* finfo = new file_info_s[header_.table_size];

        infile_.read((char*)finfo, header_.table_size * sizeof(file_info_s));
        char* names= new char[ header_.names_size];
        infile_.read(names, header_.names_size);

        for( int i= 0; i < header_.table_size; i++)
        {
            
            memcpy( &files_[i].info, finfo[i], sizeof(file_info_s) );
            std::transform(names + finfo[i].name_offset, names + finfo[i].name_offset + finfo[i].name_length, files_[i].name.begin(), tolower);
            std::cout << files_[i].name << endl;
        }
        delete[] names;
        delete[] finfo;
    }
    ResArchive::~ResArchive()
    {
        if( infile_ ) infile_.close();
    }

}
int main(int argc, char *argv[])
{
    cout << "Test " ;
    cout << setw(8) << 5 << " " ;
    cout << setw(6) << 0x3E << endl;

    return 0;
}
