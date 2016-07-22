#pragma once

#include <boost/interprocess/exceptions.hpp>

#include <string>
#include <sstream>


namespace Logger {
    void SetError(std::string msg);
    //PrintError can also set errors
    void PrintError(std::string msg = "Unknown"); //Can also set errors
    void PrintError(boost::interprocess::interprocess_exception &ex,
                    std::string msg = "Unknown");
    void SetLog(bool logging); //Toggles redirecting error messages to log file
    
    extern std::stringstream error_msg;
    extern bool error_set;
	extern bool logging;
}
