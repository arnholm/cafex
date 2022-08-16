#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
using namespace std;

#include "stringtokens.h"
#include "std_filename.h"

int main(int argc, char **argv)
{
   if(argc < 2) {
      std::cout << "Usage <template_file>  [<out_root>]" << std::endl;
      return 1;
   }

   std::string in_file(argv[1]);
   std::ifstream in(in_file);
   if(!in.is_open()) {
      std::cout << "Error: could not open '" << in_file << "'"<< std::endl;
      return 1;
   }
   std_filename fname(in_file);

   std::string out_root = ".";
   if(argc > 2) {
      out_root = std::string(argv[2]);
   }
   std::string out_dir = out_root + '/' + fname.GetName();
   std_filename::create_directory(out_dir,false);

   std::shared_ptr<std::ofstream> out_file;
   std::string line;
   while(std::getline(in,line)) {

      if(line.substr(0,3) == "===") {

         std::vector<std::string> tokens;
         char delims[] = {' ','=',char(13)};
         tokenize(line,delims,tokens);
         if(tokens.size() > 0) {
            std::string command = tokens[0];

            std::string out_name;
            if     (command == "Example")       out_name = fname.GetName() + ".txt";
            else if(command == "Connections")   out_name = command + ".txt";
            else if(command == "Mappings")      out_name = command + ".txt";
            else if(command == "Configuration") out_name = command + ".txt";

            if(out_name.length() > 0) {
               out_file = std::make_shared<std::ofstream>(out_dir + '/' +  out_name);
               std::cout << "Writing: " << out_dir + '/' + out_name << std::endl;
            }
         }
      }
      else if(out_file.get()) {
         *out_file << line << std::endl;
      }
      else {
         std::cout << "nullptr" << std::endl;
      }
   }

   out_file = nullptr;

   return 0;
}


