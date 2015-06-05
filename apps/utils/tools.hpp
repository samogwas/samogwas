/****************************************************************************************
 * File: tools.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 26/05/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_TOOLS_HPP
#define SAMOGWAS_TOOLS_HPP

namespace samogwas
{




} // namespace samogwasends here. 

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
#include <boost/filesystem.hpp> // to obtain the program's name
#include <string>
namespace samogwas
{

char* current_date()
{
  time_t rawtime;
  struct tm * timeinfo;
  char* buffer = new char[80];
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80,"%Y_%m_%d_%H_%M_%S",timeinfo);

  return buffer;
}

template<class Options>
boost::filesystem::path get_output_dir( Options& progOpt ) {
  auto path = boost::filesystem::absolute(progOpt.outputDir);
  path /= current_date();
  boost::filesystem::create_directories(path);
  return path;
}

} // namespace samogwas ends here. 

/****************************************************************************************/
#endif // SAMOGWAS_TOOLS_HPP
