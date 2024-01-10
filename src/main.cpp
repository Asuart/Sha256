#include "sha256.h"

class Debug {
     template <class T>
     static void PrintAsBin(const T val) {
          uint32_t size = sizeof(T) * 8;
          for (uint32_t i = 0; i < size; i++)
               std::cout << (uint32_t)((val >> (size - i)) & 1);
          std::cout << std::endl;
     }
};

int main(int argc, char* argv[]) {
     if (argc < 2) { std::cout << "No arguments specified. Use -help for more information." << std::endl; return 0; }

     std::string arg1 = std::string(argv[1]);

     if (arg1 == "-h" || arg1 == "-help" || arg1 == "?" || arg1 == "-?" || arg1 == "--help") {
          std::cout << "This command can get any number of parameters and will compute hash for each of them. The format if following:" << std::endl;
          std::cout << "1) use \"string\" to compute hash of a string" << std::endl;
          std::cout << "2) use -f \"path\" to specify file." << std::endl;
          std::cout << "use -m flag to mute all warning after it." << std::endl;
          std::cout << "sha256 \"string\"" << std::endl;
          std::cout << "sha256 -f \"path\"" << std::endl;
          std::cout << "sha256 \"string\" -f \"path\" \"string\" -f \"path\" -f \"path\"" << std::endl;
          std::cout << std::endl;
          return 0;
     }

     bool awaitFile = false;
     bool muteWarnings = false;
     for (uint32_t i = 1; i < argc; i++) {
          std::string arg = std::string(argv[i]);
          if (arg == "-m") { muteWarnings = true; continue; }
          if (awaitFile && arg == "-f") { if (!muteWarnings) std::cout << "Warning: no file path specified for argument -f at index: " << i - 1 << std::endl; continue; }
          if (arg == "-f") { awaitFile = true; continue; }
          if (awaitFile) {
               std::fstream reader(arg.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
               if (!reader.is_open()) { if (!muteWarnings) std::cout << "Could not open file: " << arg << std::endl; return 1; }
               uint32_t inputSize = reader.tellg();
               reader.seekg(reader.beg);
               uint8_t* fileData = new uint8_t[inputSize];
               reader.read((char*)fileData, inputSize);
               reader.close();
               Sha256::GetHash(fileData, inputSize).Print();
               delete[] fileData;
          }
          else {
               uint8_t* fileData = (uint8_t*)argv[i];
               uint32_t inputSize = std::string(argv[i]).size();
               Sha256::GetHash(fileData, inputSize).Print();
          }
          awaitFile = false;
     }
     if (awaitFile && !muteWarnings) std::cout << "Warning: no file path specified for last -f argument." << std::endl;

     return 0;
}