/* ARGUMENT_MANAGER:
 * An intelligent class for managing argument list of a program.
 */

#ifndef MEERKAT_ARGUMENT_MANAGER_H
#define MEERKAT_ARGUMENT_MANAGER_H

#define MK_VALUE 0
#define MK_FLAG 1
#define MK_UNSET 0
#define MK_SET 1
#define MK_REQUIRED 0
#define MK_OPTIONAL 1

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <string>
#include <vector>

namespace meerkat {

  class mk_argument_manager
  {
  // private members
  private:
    int _numArgs;
    std::string _description;
    std::string _usage;
    std::vector<std::string> _argName;
    std::vector<std::string> _argHelpEntry;
    std::vector<std::string> _argLongKey;
    std::vector<std::string> _argShortKey;
    std::vector<std::string> _argDefaultValue;
    std::vector<std::string> _argValue;
	  std::vector<int> _argType;
    std::vector<int> _argStatus;
    std::vector<int> _argPriority;

  // functions
  public:
    // constructor
    mk_argument_manager();
    mk_argument_manager( const char *description_, const char *usage_ );

    // add an erntry
    void add_argument_entry( const char *name_, int type_, const char *keyLong_, const char *keyShort_, const char *helpEntry_, const char *defaultValue_, int priority_ );

    // default entries
    void add_help();

    // help menu
    void print_help() const;

    // read arguments from argument list
    void read_arguments( int argc_, char **argv_ );

    // print arguments
    void show_settings() const;

    // check if argument is set
    bool is_set( const char* argName_ );

    // query argument
    int get_int_argument( const char *argName_ );
    double get_double_argument( const char *argName_ );
    std::string get_string_argument( const char *argName_ );
  };

}

#endif // MEERKAT_ARGUMENT_MANAGER_H
