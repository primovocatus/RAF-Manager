#include "lib/directory.h"

void getFiles(const std::string& path, std::vector<file>& dir) {
    dir.clear();

    for (const auto& files: std::filesystem::directory_iterator(path)) {
        file currentFile;
            
        currentFile.path = files.path();
        currentFile.name = files.path().filename();
        bool isFolder = files.is_directory();
        currentFile.isDirectory = isFolder;

        try {
            currentFile.size = std::filesystem::file_size(files.path());
        } catch(std::filesystem::filesystem_error& e) {
            currentFile.size = 0;
        }

        dir.push_back(currentFile);
    }

    sort(dir.begin(), dir.end());
}