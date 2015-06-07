#include "meerkat_file_manager.hpp"


/**
 * Desc: Empty constructor.
 */
meerkat::mk_file_manager::mk_file_manager()
{
  _filePointer = NULL;
  _fileState = MK_EMPTY;
  _fileName = "_UNDEFINED_";
}


/**
 * Desc: Complete constructor. Checks if the file exists with the given name and opens it.
 *
 * @fileName_ : name of the file to open.
 * @mode_ : mode of file operation ('r', 'w' or else).
 */
meerkat::mk_file_manager::mk_file_manager( const std::string fileName_, const char *mode_ )
{
  // set file name
  _fileName = std::string( fileName_ );

  // set state
  _fileState = MK_READ;
  if( strcmp( mode_, "w" ) == 0 )
    _fileState = MK_WRITE;

  // try to open depending on the state
  _filePointer = NULL;
  switch( _fileState )
  {
    case MK_READ:
    {
      _filePointer = fopen( _fileName.c_str(), "r" );
      break;
    }
    case MK_WRITE:
    {
      _filePointer = fopen( _fileName.c_str(), "w" );
      break;
    }
  }

  // if failed, print error
  if( _filePointer == NULL )
    perror( "constructor error: file not found" );
}


/**
 * Desc: Destructor, which closes the file if it is not null.
 */
meerkat::mk_file_manager::~mk_file_manager()
{
  if( _fileState != MK_EMPTY && _filePointer != NULL )
    fclose( _filePointer );
}


/**
 * Desc: Prints an error message, closes file and exits.
 */
void meerkat::mk_file_manager::perror( const char *errorMessage_ ) const
{
  if( _fileName == "_UNDEFINED_" )
    printf( "%s\n", errorMessage_ );
  else
    printf( "%s (\"%s\").\n", errorMessage_, _fileName.c_str() );

  if( _filePointer != NULL )
    fclose( _filePointer );

  exit( 0 );
}


/**
 * Desc: Checks if file is in the required state. If state is not correct, exits with an error message.
 *
 * @functionName_ : name of the current function where the check is carried out.
 * @requiredState_ : state to be checked against.
 */
void meerkat::mk_file_manager::check_state( const char *functionName_, MK_FILE_STATE requiredState_ ) const
{
  if( _fileState == requiredState_ )
    return;
  else
  {
    std::string err_msg = std::string( functionName_ ) + " error: ";
    switch( _fileState )
    {
      case MK_EMPTY:
      {
        err_msg += "file is not open";
        break;
      }
      case MK_WRITE:
      {
        err_msg += "file is open for writing";
        break;
      }
      case MK_READ:
      {
        err_msg += "file is open for reading";
        break;
      }
    }
    perror( err_msg.c_str() );
  }
}


/**
 * Desc: Opens a file with the given file name and in the given mode.
 *
 * @fileName_ : name of the file to open.
 * @mode_ : mode of the file operations ('r', 'w', etc).
 */
bool meerkat::mk_file_manager::open( const std::string fileName_, const char *mode_ )
{
  // set state
  _fileState = MK_READ;
  if( strcmp( mode_, "w" ) == 0 )
    _fileState = MK_WRITE;

  // try to open depending on the file state
  _filePointer = NULL;
  switch( _fileState )
  {
    case MK_READ:
    {
      _filePointer = fopen( fileName_.c_str(), "r" );
      break;
    }
    case MK_WRITE:
    {
      _filePointer = fopen( fileName_.c_str(), "w" );
      break;
    }
  }

  // if failed, print message and exit
  if( _filePointer == NULL )
    return false;
  else
    return true;
}


/**
 * Desc: Closes file.
 */
void meerkat::mk_file_manager::close()
{
  switch( _fileState )
  {
    case MK_EMPTY:
      perror( "close error: file is not opened" );

    case MK_READ:
    case MK_WRITE:
    {
      if( _filePointer != NULL )
        fclose( _filePointer );
      _fileState = MK_EMPTY;
      _fileName = "_UNDEFINED_";
    }
  }
}


/**
 * Desc: Goes to the top of the file.
 */
void meerkat::mk_file_manager::go_to_top()
{
  check_state( "go_to_top", MK_READ );

  fseek( _filePointer, 0, SEEK_SET );
}


/**
 * Desc: Retrieves number of rows in the file.
 *
 * return : number of rows.
 */
int meerkat::mk_file_manager::rows() const
{
  check_state( "rows", MK_READ );

  char line[1024] = {""};
  int _rows = 0;
  fseek( _filePointer, 0, SEEK_SET );
  while( !feof(_filePointer) )
  {
    fgets( line, sizeof(line), _filePointer );
    _rows++;
  }
  fseek( _filePointer, 0, SEEK_SET );

  return _rows-1;
}


/**
 * Desc: Retrieves number of columns in the file. Requires a white-space separated values format.
 *
 * return : number of columns in the file.
 */
int meerkat::mk_file_manager::columns() const
{
  check_state( "columns", MK_READ );

  char line[1024] = {""};
  char *line_ptr = fgets( line, sizeof(line), _filePointer );
  char col[1024] = {""};
  int _columns = 0, shift = 0;
  fseek( _filePointer, 0, SEEK_SET );
  while( sscanf(line_ptr, "%s%n", col, &shift) == 1 )
  {
    _columns++;
    line_ptr += shift;
  }
  fseek( _filePointer, 0, SEEK_SET );

  return _columns;
}


/**
 * Desc: Prints a data line into the file.
 *
 * @data_ : vector of values to print in the file.
 */
void meerkat::mk_file_manager::put_data( std::vector<double> &data_ )
{
  check_state( "put_data", MK_WRITE );

  int len = (int)data_.size();
  fprintf( _filePointer, "%lg", data_[0] );
  for(int i=1; i<len; i++)
    fprintf( _filePointer, " %lg", data_[i] );
  fprintf( _filePointer, "\n" );
}


/**
 * Desc: Prints a text line into the file.
 *
 * @text_ : the text line to print in the file.
 */
void meerkat::mk_file_manager::put_text( char *text_ )
{
  check_state( "put_text", MK_WRITE );

  fprintf( _filePointer, "%s\n", text_ );
}


/**
 * Desc: Reads a row of data from the file. Requires a white-space separated values format.
 *
 * @data_ : vector that will contain the values in the row.
 * return : number of successfully read values.
 */
int meerkat::mk_file_manager::get_data( std::vector<double> &data_ )
{
  check_state( "get_data", MK_READ );

  int num_data_element = 0;
  int shift = 0;
  double data_element = 0.0;
  char *data_line, line[1024] = {""};

  data_.clear();
  data_line = fgets( line, sizeof(line), _filePointer );
  while( sscanf(data_line, "%lf%n", &data_element, &shift) == 1 )
  {
    data_.push_back( data_element );
    num_data_element++;
    data_line += shift;
  }

  return num_data_element;
}


/**
 * Desc: Reads a text line from the file.
 *
 * @text_ : char array that will contain the read line.
 * @length_ : length of the char array.
 */
void meerkat::mk_file_manager::get_text( char *text_, int length_ )
{
  check_state( "get_text", MK_READ );

  fgets( text_, length_-1, _filePointer );
  text_[strlen(text_)-1] = '\0';
}
