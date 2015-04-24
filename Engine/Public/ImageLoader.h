#include "Engine.h"
#include <map>

class ImageLoader
{
public:
  
  typedef uvec2 loadImageFun(std::vector&, std::string);
  
  static void addLoader(std::string extension, loadImageFun* function)
  {
    if (loadFunctions.find(extension) != loadFunctions.end())
    {
      ENG_LOG("Dulplicate loader for " << extension << ". using newest loader.")
      
    }
    loadFunctions[extension] = function;
    
    
  }
  
  static uvec2 load(std::string filename, std::vector& data)
  {
    std::string extension;
    
    for (uint8 i = 1; filename[filename.size() - i] != '.' && i <= filename.size(); i++)
    {
      extension.push_front(filename[filename.size() - i]);
    }
    
    if (loadFunctions.find(extension) == loadFunctions.end())
    {
      FATAL_ERR("no loader for extension")
    }

    auto fun = loadFunctions[extension];
    
    check(fun);
    
    return fun(data, filename);
  }
  
private:
  static std::map<std::string, loadImageFun*> loadFunctions;
}