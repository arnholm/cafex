#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
using namespace std;

#include "stringtokens.h"
#include "std_filename.h"


int export_template(const std::string& in_file, const std::string& out_root)
{
   std_filename fname(in_file);

   std::string out_dir = out_root + '/' + fname.GetName();
   std_filename::create_directory(out_dir,false);

   std::ifstream in(in_file);
   if(!in.is_open()) {
      std::cout << "Error: could not open '" << in_file << "'"<< std::endl;
      return 1;
   }

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

void write_file(const std::string& filename, std::ostream& out)
{
   std::ifstream in(filename);
   if(!in.is_open()) {
      std::cout << "Error, file does not exist : " << filename << std::endl;
   }

   std::string line;
   while(std::getline(in,line)) {
      out << line << std::endl;
   }
}

int import_files(const std::string& config_dir)
{
   if(!std_filename::Exists(config_dir)) {
      std::cout << "Error, directory does not exist : " << config_dir << std::endl;
      return 1;
   }

   std_filename fname_dir(config_dir);
   std::string example_name  = fname_dir.GetName();
   std::cout << "importing from :"<< example_name << std::endl;

   std::vector<std::string> file_names =
   {
       example_name
      ,"Connections"
      ,"Mappings"
      ,"Configuration"
   };

   for(auto file_name : file_names) {
      std::string file_path = config_dir + '/' + file_name + ".txt";
      if(!std_filename::Exists(file_path)) {
         std::cout << "Error, file does not exist : " << file_path << std::endl;
         return 1;
      }
   }

   // all files found
   std::ofstream out_file(example_name + ".template");
   out_file << "===Example" << std::endl;
   write_file( config_dir + '/' +file_names[0]+".txt",out_file);

   for(size_t i=1;i<4;i++) {
      out_file << "==="+file_names[i] << std::endl;
      write_file( config_dir + '/' +file_names[i]+".txt",out_file);
   }


   return 0;
}


int main(int argc, char **argv)
{
   if(argc < 3) {
      //                       1         2                 3
      std::cout << "Usage 1: export  <template_file>  [<out_root>]   : export config files from template" << std::endl;
      std::cout << "Usage 2: import  <config_dir>                    : create template from config_dir files (do not include final '/') "<< std::endl;
      return 1;
   }

   std::string command(argv[1]);
   if(command == "export") {

      // export files from template
      std::string in_file(argv[2]);
      std::string out_root = ".";
      if(argc > 3) out_root = std::string(argv[3]);
      return export_template(in_file,out_root);

   }
   else if(command == "import") {

      // import template from files
      std::string config_dir(argv[2]);
      return import_files(config_dir);
   }
   else {
      std::cout << "Error, unknown command : " << command << std::endl;
      return 1;
   }
   return 0;
}


