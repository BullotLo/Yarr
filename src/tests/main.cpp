#define CATCH_CONFIG_RUNNER

#include "catch.hpp"

#include "LoggingConfig.h"

int main( int argc, char* argv[] )
{
  Catch::Session session; // There must be exactly one instance
  
  std::string logging_config;
  
  // Build a new parser on top of Catch's
  using namespace Catch::clara;
  auto cli 
    = session.cli() // Get Catch's composite command line parser
    | Opt( logging_config, "log_config" ) // bind variable to a new option, with a hint string
    ["--logger-config"]    // the option names it will respond to
    ("Configure logging using json file");        // description string for the help output
        
  // Now pass the new composite back to Catch so it uses that
  session.cli( cli ); 

  // Let Catch (using Clara) parse the command line
  int returnCode = session.applyCommandLine( argc, argv );
  if( returnCode != 0 ) // Indicates a command line error
    return returnCode;

  json lj;
  if( logging_config.empty() ) {
    // default to logging errors and above
    lj["log_config"][0]["name"] = "all";
    lj["log_config"][0]["level"] = "error";
  } else {
    std::ifstream log_file(logging_config);
    lj = json::parse(log_file);
  }
  logging::setupLoggers(lj);

  return session.run();
}
