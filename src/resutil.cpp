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
#include <iterator>

using namespace std;
namespace evil_islands {
    typedef std::vector<uint8_t> MemData;
    typedef std::shared_ptr<MemData> PMemData;

    class ResArchive
    {
        private:
#pragma pack(push,1)
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

                std::uint32_t   last_change_time; // 4 байтовое время Unix

                std::uint16_t   name_length;
                std::uint32_t   name_offset; // смещение в массиве `names`
            };
#pragma pack(pop)
            struct file_info_name_s {
                std::uint32_t   next_index;
                std::uint32_t   file_size;
                std::uint32_t   file_offset;
                std::uint32_t   last_change_time; // 4 байтовое время Unix

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
            const std::vector<struct file_info_name_s>& GetFileList( ){ return files_; }
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
    //        std::cout << "magic in "<< filepath << " ";
    //cout << setw(8) << header_.table_size << " " ;
    //cout << setw(8) << std::hex << sizeof( struct file_info_s )<< " " ;
    //cout << setw(8) << std::hex << header_.table_offset << std::dec<< endl ;

        infile_.seekg( header_.table_offset);
        files_.resize(header_.table_size);
        file_info_s* finfo = new file_info_s[header_.table_size];

        infile_.read((char*)finfo, header_.table_size * sizeof(file_info_s));
        char* names= new char[ header_.names_size];
        infile_.read(names, header_.names_size);

        for( unsigned i= 0; i < header_.table_size; i++)
        {
            files_[i].next_index = finfo[i].next_index;
            files_[i].file_size = finfo[i].file_size;
            files_[i].file_offset = finfo[i].file_offset;
            files_[i].last_change_time = finfo[i].last_change_time;
            files_[i].name = std::string( names + finfo[i].name_offset, finfo[i].name_length);
            //std::transform(files_[i].name.begin(), files_[i].name.end(),files_[i].name.begin(),std::tolower);
            
            //cout << i << " " ;
            //cout << std::hex << setw(8) << files_[i].file_offset << " " ;
            //cout << setw(8) << finfo[i].name_offset << " ";
            //cout << setw(8) << finfo[i].name_length << " ";
            //cout << setw(8) << files_[i].file_size << std::dec <<" "<< files_[i].name << endl;
        }
        delete[] names;
        delete[] finfo;
    }
    ResArchive::~ResArchive()
    {
        if( infile_ ) infile_.close();
    }
    PMemData LoadFile( const std::string& fname)
    {
        return nullptr;
    }

}
int main(int argc, char *argv[])
{
    if( argc < 3 ) return 0;
    const char *archive_name = argv[2];
    evil_islands::ResArchive ra(archive_name);
    if( argv[1][0] == 'l' ){
        auto files = ra.GetFileList();
        cout <<  "-------------" << endl;
        for( auto it = files.begin(); it < files.end(); it++ ){
            cout << setw(8) << (*it).file_size << " ";
            cout <<  (*it).name << endl;
        }
        cout <<  "-------------" << endl;

    }
    if( argv[1][0] == 'x' && argc == 4 ){
        cout <<  "Extract " << argv[3] << endl;
        auto data = ra.LoadFile(argv[3]);
        if( data ){
            
        }
    }

    return 0;
}
