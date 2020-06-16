#include "utils.hpp"
#include "common.hpp"

#include <string>
#include <cmath>
#include <algorithm>

namespace utils {
  std::string trim_tail(const std::string &str, const char *space){
    std::string::size_type tail = str.find_last_not_of(space);
    return tail == std::string::npos ? "" : str.substr(0, tail+1);
  }
  
  std::string trim_head(const std::string &str, const char *space){
    std::string::size_type head = str.find_first_not_of(space);
    return head == std::string::npos ? "" : str.substr(head);
  }
  
  int round(fltype val){
    return static_cast<int>(val+0.5);
  }
  
  fltype randf(fltype lower, fltype upper) {
    return ((fltype)rand() / INT_MAX) * (upper - lower) + lower;
  }

  std::string GetExtension(const std::string &path){
    std::string ext;
    std::size_t pos1 = path.rfind('.');
    if(pos1 != std::string::npos){
      ext = path.substr(pos1+1, path.size()-pos1);
      std::string::iterator itr = ext.begin();
      while(itr != ext.end()){
	*itr = tolower(*itr);
	itr++;
      }
      itr = ext.end()-1;
      while(itr != ext.begin()){ 
	if(*itr == 0 || *itr == 32){
	  ext.erase(itr--);
	}
	else{
	  itr--;
	}
      }
    }
    return ext;
  }

  std::string getDate(){
    time_t timer = time(NULL);
    tm* date = localtime(&timer);
    std::string dateStr = (boost::format("%02d_%02d_%02d_%02d_%02d") 
			   % date->tm_mon % date->tm_mday
			   % date->tm_hour % date->tm_min % date-> tm_sec).str();
    return dateStr;    
  }

  bool exist_in(const std::vector<int>& c, const int v){
    return (c.end() != std::find(c.begin(), c.end(), v) );
  }


} // namespace utils
