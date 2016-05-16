#ifndef _MAPHANDLER_H
#define _MAPHANDLER_H

class MapHandler{
  public:
    MapHandler();
    ~MapHandler();
    bool loadMapsfromDir(std::vector<std::string>*);
    
    void print_map();
    bool check_regex(std::string);
    bool read_map(std::string);
    void print_finalmaps();
    bool map_is_valid(std::string);

  
  private:
    std::vector<std::string> map;
    
};
#endif
