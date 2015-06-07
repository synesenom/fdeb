/***********************************************************************************************\
* This header defines mk_log class which is a simply controllable warning and error mk_log    *
* handler class. The class is designed to be a singleton as we only one usually.                *
*                                                                                               *
* Requires:                                                                                     *
*   STL                                                                                         *
*                                                                                               *
* Author: Enys Mones                                                                            *
\***********************************************************************************************/


#ifndef _mk_log_HPP_
#define _mk_log_HPP_


#include "stdlib.h"
#include "stdio.h"
#include <string>
#include <time.h>
#include <stdarg.h>


namespace meerkat {

  class mk_log
  {
  private:
      // Member variables
      std::string _tag;
      static int _severityLevel;
      static clock_t _startTime;

      // Internal methods
      std::string _get_time_str() const;

  public:
      // Severity levels
      enum SeverityLevel { SLInfo, SLWarning, SLError };

      // Constructors
      mk_log();
      mk_log( const std::string tag_ );
      mk_log( std::string tag_, SeverityLevel severityLevel_ );

      /**
       * Sets logger tag.
       *
       * @param tag_ tag of the logger
       */
      void tag( std::string tag_ );


      /**
       * Sets severity level.
       *
       * @param policy_  policy to set
       */
      static void severity_level( SeverityLevel severityLevel_ );


      /**
       * Prints out an information mk_log.
       *
       * @param message_  message to show
       */
      void i( const char *functionName_, const char *message_, ... ) const;


      /**
       * Prints out a warning mk_log.
       *
       * @param message_  message to show
       */
      void w( const char *functionName_, const char *message_, ... ) const;


      /**
       * Prints out an error mk_log.
       *
       * @param message_  message to show
       */
      void e( const char *functionName_, const char *message_, ... ) const;
  };

}

#endif // _mk_log_HPP_
