#include <magic.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <fstream>

#include "MapHandler.h"
/*Map Handler class*/
/*This class is used to load the maps from the directory and to check if they are valid for the game*/
MapHandler::MapHandler(){
  
}

MapHandler::~MapHandler(){
  std::cout<<"Deleting MapHandler"<<std::endl;
}

void MapHandler::print_map(){
    for(unsigned int i=0; i<map.size(); i++){
        std::cout<<map.at(i)<<std::endl;
    }
}
/*This function open the file and read the map into a vector*/
bool MapHandler::read_map(std::string filename){
    std::string line;
    std::ifstream FILEH;
    //std::cout<<filename<<std::endl;
    map.clear();
    FILEH.open (filename.c_str(),std::ifstream::in);
    if(!FILEH.is_open()){
        std::cout<<"File cannot be open!\n";
	return false;
    }else{
        while(getline(FILEH,line)){
            map.push_back(line);
        }
    }
    FILEH.close();
    return true;
}
/*This function read perform the fucll check, read map into vector, check if the map is valid. If it is, it returns true*/
bool MapHandler::map_is_valid(std::string filename){
  if(!read_map(filename)){
    return false;
  }else{
    if(check_regex(filename)){
      return true;
    }else{
      return false;
    }
  }
}
/*This fuction read the directory and check if the header file is correct. If it is, it check if the map is valid. If it is valid the map name is loaded into the vector containing the maps*/
bool MapHandler::loadMapsfromDir(std::vector<std::string> *maps){
    
    DIR * dir;
    struct dirent *dp;
    magic_t t = magic_open(MAGIC_MIME);
    std::string filen;
    std::string fileT;
    
    if (t == NULL) {
        printf("unable to initialize magic library\n");
        return false;
    }
    if (magic_load(t, NULL) != 0) {
        printf("cannot load magic database - %s\n", magic_error(t));
        magic_close(t);
        return false;
    }

    dir=opendir("maps");
    if(dir==NULL){
        std::cout<<"Error in opening the directory"<<std::endl;
        return false;
    }

    while((dp=readdir(dir)) != NULL){
        filen.assign(dp->d_name);
        filen="maps/" + filen;
        const char *ta= magic_file(t,filen.c_str());
        if(ta!=NULL){
            fileT.assign(ta);    
            if(fileT.substr(0,4)!="text"){
                continue;
            }else{
	      if(map_is_valid(filen)){
                maps->push_back(filen.substr(5,filen.size()-9));
	      }
            //std::cout<<"Filename: " + filen<<" FileType "<<fileT<<std::endl;
	    }
        }
    }
    magic_close(t);
    closedir(dir);
    dir=NULL;
    return true;
}
/*This function uses C++11 regex features to check if the map is valid. Returns true if the map is valid
 * The map that could potentiatially have various 0s 1s and 2s could result in  problem and it would still be valid 
 * for this fuction. However the game will bascally set the player position to the first digit encoutered for the respective player.. 
 * At the moment only 2 players are allowed, 0 and 1 there fore the player coodinates for player 1 will be set on the first 0
 * encountered, and not the last. This will reduce weird positiioning. */
bool MapHandler::check_regex(std::string filename){
    int linenumber=1;
    int sizemap=map.size();
    bool startpattern=false;
    bool stoppattern=false;
    bool success=false;
    int indexstart=0;
    int indexstop=0;
    int offset=0;

    std::regex re ("^\\*[0-3 \\*\\+\\-]{15}\\*$");
    std::regex restartstop("^\\*{17}$");
    std::regex renotvalid ("[\\*\\-]{17}");

    for(int i=1; i<sizemap; i++){
        linenumber++;
        if(startpattern){
            if(std::regex_match(map.at(i),restartstop)){
                stoppattern=true;
                indexstop=i;
                break;
            }
            if(std::regex_match(map.at(i), re)){
                continue;
            }else{
	      break;
	    }

        }else{
        
            if(i>=(sizemap/2)){
                break;
            }else{
                if(!std::regex_match(map.at(i),renotvalid)){
                    break;
                }else{
                    if(std::regex_match(map.at(i),restartstop)){
                        startpattern=true;
                        indexstart=i;
                    }
                }
            }
        
        }
    }
offset=indexstop-indexstart;
    if(startpattern && stoppattern){
        if(offset>=8 && offset<=12){
            std::cout<< "Map: "<<filename<<" is valid\n";
	    success=true;
            //std::string t(filename);
            //finalmaps.push_back(t);
        }
    }else{
      std::cout<<"Map: "<<filename<<" is not valid, error found at "<<linenumber<<"\n";
    }
    return success;
}
