#include "meerkat_argument_manager.hpp"

/**
 * Desc: Empty constructor.
*/
meerkat::mk_argument_manager::mk_argument_manager()
{
  _numArgs = 0;
  _description = "";
  _usage = "";
}


/**
 * Desc: Constructor with initial settings.
 *
 * @description_  description of the program
 * @usage_        how to use the program
*/
meerkat::mk_argument_manager::mk_argument_manager( const char *description_, const char *usage_ )
{
  _numArgs = 0;
  _description = std::string( description_ );
  _usage = std::string( usage_ );
}


/**
 * Desc: Prints help menu.
 */
void meerkat::mk_argument_manager::print_help() const
{
  // determine max keyword length
  int key_length = 0;
  int max_key_length = 0;
  for(int i=0; i<_numArgs; i++)
  {
    key_length = (int)_argLongKey[i].length() + (int)_argShortKey[i].length() + 5;
    if( key_length > max_key_length )
      max_key_length = key_length;
  }

  // print help menu
  printf( "\n" );
  printf( "%s\n", _description.c_str() );
  printf( "%s\n", _usage.c_str() );
  if( _numArgs > 0 )
  {
    printf( "Options:\n" );
    for(int i=0; i<_numArgs; i++)
    {
      key_length = (int)_argLongKey[i].length() + (int)_argShortKey[i].length();
      printf( "  %s,%s", _argLongKey[i].c_str(), _argShortKey[i].c_str() );
      for(int j=0; j<max_key_length-key_length; j++)
        printf( " " );
      printf( "%s.\n", _argHelpEntry[i].c_str() );
    }
  }
  printf( "\n" );
  exit(0);
}


/**
 * Desc: Adds an argument entry.
 *
 * @name_       string identifier of the argument
 * @type_       whether argument is a value (MK_VALUE) or flag (MK_FLAG)
 * @longKey_    long key for the argument
 * @shortKey_   short key for the argument
 * @helpEntry_  description of the argument in the help menu
 * @default_    default value
 * @priority_   whether the argument is required (MK_REQUIRED) or optional (MK_OPTIONAL)
 */
void meerkat::mk_argument_manager::add_argument_entry( const char *name_,
                                                       int type_,
                                                       const char *longKey_,
                                                       const char *shortKey_,
                                                       const char *helpEntry_,
                                                       const char *default_,
                                                       int priority_ )
{
    _argName.push_back( std::string(name_) );
    _argType.push_back( type_ );
    _argHelpEntry.push_back( std::string(helpEntry_) );
    _argLongKey.push_back( std::string(longKey_) );
    _argShortKey.push_back( std::string(shortKey_) );
    _argValue.push_back( "" );
    if( type_ == MK_VALUE )
      _argDefaultValue.push_back( std::string(default_) );
    else
        _argDefaultValue.push_back( std::string("0") );
    _argStatus.push_back( MK_UNSET );
    _argPriority.push_back( priority_ );
    _numArgs++;
}


/**
 * Desc: Adds a defualt argument entry for the help menu with the default keys '--help' and '-h'.
 */
void meerkat::mk_argument_manager::add_help()
{
  add_argument_entry( "help", MK_FLAG, "--help", "-h", "Prints help menu", "0", MK_OPTIONAL );
}


/**
 * Desc: Reads arguments from the argument values array.
 *
 * @argc_  number of arguments
 * @argv_  array of arguments
 */
void meerkat::mk_argument_manager::read_arguments( int argc_, char **argv_ )
{
  if( argc_ == 1 )
  {
    print_help();
    exit(0);
  }

  // Set arguments
  for(int i=0; i<argc_; i++)
  {
    for(int j=0; j<_numArgs; j++)
    {
      if( strcmp( argv_[i], _argLongKey[j].c_str() ) == 0 ||
          strcmp( argv_[i], _argShortKey[j].c_str() ) == 0 )
      {
        if( _argName[j] == "help" )
          print_help();

				if( _argType[j] == MK_VALUE )
	        _argValue[j] = std::string( argv_[i+1] );
				else
					_argValue[j] = std::string( "1" );
        _argStatus[j] = MK_SET;
      }
    }
  }

  // Check required arguments
  int req_not_given = 0;
  for(int i=0; i<_numArgs; i++)
  {
    if( _argPriority[i] == MK_REQUIRED && _argStatus[i] == MK_UNSET )
    {
      printf( "read_arguments error: a required argument is not given (\"%s\").\n", _argName[i].c_str() );
      req_not_given++;
    }
  }

  if( req_not_given > 0 )
    exit(0);
}


/**
 * Desc: Displays setting parameter values.
 */
void meerkat::mk_argument_manager::show_settings() const
{
  // Determine max name length
  int name_length = 0;
  int max_name_length = 0;
  for(int i=0; i<_numArgs; i++)
  {
    name_length = (int)_argName[i].length() + 5;
    if( name_length > max_name_length )
      max_name_length = name_length;
  }

  // Print settings
  printf( "\n" );
  printf( "Settings:\n---------\n" );
  if( _numArgs > 0 )
  {
    for(int i=0; i<_numArgs; i++)
    {
      if( _argName[i] != "help" )
      {
        name_length = (int)_argName[i].length();
        printf( "%s:", _argName[i].c_str() );
        for(int j=0; j<max_name_length-name_length; j++)
          printf( " " );
        if( _argStatus[i] == MK_UNSET )
          printf( "\"%s\"\n", _argDefaultValue[i].c_str() );
        else
          printf( "\"%s\"\n", _argValue[i].c_str() );
      }
    }
  }
  printf( "\n" );
}

bool meerkat::mk_argument_manager::is_set( const char *argName_ )
{
    for(int i=0; i<_numArgs; i++)
    {
      if( strcmp( _argName[i].c_str(), argName_ ) == 0 )
        return _argStatus[i] == MK_SET;
    }

    printf( "is_set error: no such argument.\n" );
    exit(0);
}


/**
 * Desc: Queries an integer argument.
 *
 * @argName_  name of the queried argument
 * @return    argument integer value
 */
int meerkat::mk_argument_manager::get_int_argument( const char *argName_ )
{
  for(int i=0; i<_numArgs; i++)
  {
    if( strcmp( _argName[i].c_str(), argName_ ) == 0 )
    {
      int arg_int;
      if( _argStatus[i] == MK_SET )
        sscanf( _argValue[i].c_str(), "%i", &arg_int );
      else
        sscanf( _argDefaultValue[i].c_str(), "%i", &arg_int );
      return arg_int;
      break;
    }
  }

  printf( "get_int_argument error: no such argument.\n" );
  exit(0);
}


/**
 * Desc: Queries a double argument.
 *
 * @argName_  name of the queried argument
 * @return    argument double value
 */
double meerkat::mk_argument_manager::get_double_argument( const char *argName_ )
{
  for(int i=0; i<_numArgs; i++)
  {
    if( strcmp( _argName[i].c_str(), argName_ ) == 0 )
    {
      double arg_double;
      if( _argStatus[i] == MK_SET )
        sscanf( _argValue[i].c_str(), "%lf", &arg_double );
      else
        sscanf( _argDefaultValue[i].c_str(), "%lf", &arg_double );
      return arg_double;
      break;
    }
  }

  printf( "get_double_argument error: no such argument.\n" );
  exit(0);
}


/**
 * Desc: Queries a string argument.
 *
 * @argName_  name of the queried argument
 * @return    argument string value
 */
std::string meerkat::mk_argument_manager::get_string_argument( const char *argName_ )
{
  for(int i=0; i<_numArgs; i++)
  {
    if( strcmp( _argName[i].c_str(), argName_ ) == 0 )
    {
      std::string arg_string;
      if( _argStatus[i] == MK_SET )
        arg_string = _argValue[i];
      else
        arg_string = _argDefaultValue[i];
      return arg_string;
      break;
    }
  }

  printf( "get_string_argument error: no such argument.\n" );
  exit(0);
}

