/*
 * An intelligent file handling class that handles errors and takes care
 * about closing the file.
 */

#ifndef MEERKAT_FILE_MANAGER_H
#define MEERKAT_FILE_MANAGER_H

#include "stdlib.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include <vector>
#include <string>

namespace meerkat {

  class mk_file_manager
  {
  private:
    FILE *_filePointer;
    int _fileState;
    std::string _fileName;

    enum MK_FILE_STATE{ MK_EMPTY, MK_WRITE, MK_READ };
    void perror( const char *errorMessage_ ) const;
    void check_state( const char *function_, MK_FILE_STATE requiredState_ ) const;

  public:
    mk_file_manager();
    mk_file_manager( const std::string fileName_, const char *mode_ );
    ~mk_file_manager();

    bool open( const std::string fileName_, const char *mode_ );
    void close();
    void go_to_top();
    int rows() const;
    int columns() const;
    void put_data( std::vector<double> &data_ );
    void put_text( char *text_ );
    int get_data( std::vector<double> &data_ );
    void get_text( char *text_, int length_ );
  };

}

#endif // MEERKAT_FILE_MANAGER
