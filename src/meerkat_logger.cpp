#include "meerkat_logger.hpp"

// static members
int meerkat::mk_log::_severityLevel = meerkat::mk_log::SLInfo;
clock_t meerkat::mk_log::_startTime = clock();

std::string meerkat::mk_log::_get_time_str() const
{
    char timestr[128] = {""};
    float fsec = (float)(clock() - _startTime)/CLOCKS_PER_SEC;
    int isec = (int)fsec;
    sprintf(timestr, "[%02d:%02d:%02d.%03d]",
          (int)((isec%86400)/3600),
          (int)((isec%3600)/60),
          (int)(isec%60),
          (int)((fsec-isec)*1000));
    return std::string(timestr);
}


meerkat::mk_log::mk_log()
{
    _tag = "";
    _severityLevel = SLInfo;
}


meerkat::mk_log::mk_log( const std::string tag_ )
{
    _tag = tag_;
    _severityLevel = SLInfo;
}


meerkat::mk_log::mk_log( const std::string tag_, SeverityLevel severityLevel_ )
{
    _tag = tag_;
    _severityLevel = severityLevel_;
}


void meerkat::mk_log::tag( std::string tag_ )
{
    _tag = tag_;
}


void meerkat::mk_log::severity_level( SeverityLevel severityLevel_ )
{
    _severityLevel = severityLevel_;
}


void meerkat::mk_log::i( const char *functionName_, const char *message_, ... ) const
{
    char timestr[32];
    va_list arg;
    if( _severityLevel == SLInfo ) {
        va_start(arg, message_);
        printf( "INFO %s  %s::%s: ", _get_time_str().c_str(), _tag.c_str(), functionName_);
        vprintf(message_, arg);
        va_end(arg);
        printf("\n");
    }
}


void meerkat::mk_log::w( const char *functionName_, const char *message_, ... ) const
{
    char timestr[32];
    va_list arg;
    if( _severityLevel == SLInfo ||_severityLevel == SLWarning ) {
        va_start(arg, message_);
        printf( "WARN %s  %s::%s: ", _get_time_str().c_str(), _tag.c_str(), functionName_);
        vprintf(message_, arg);
        va_end(arg);
        printf("\n");
    }
}


void meerkat::mk_log::e( const char *functionName_, const char *message_, ... ) const
{
    char timestr[32];
    va_list arg;
    if( _severityLevel == SLInfo || _severityLevel == SLWarning || _severityLevel == SLError ) {
        va_start(arg, message_);
        printf( "ERRO %s %s::%s: ", _get_time_str().c_str(), _tag.c_str(), functionName_);
        vprintf(message_, arg);
        va_end(arg);
        printf("\n");
    }
}
