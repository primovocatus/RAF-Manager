#include "lib/directory.h"

bool sortName(const file& a, const file& b) {
    if(!(a.isDirectory ^ b.isDirectory)) {
        return a.name < b.name;
    }
    return a.isDirectory;
}

void getFiles(const std::string& path, std::vector<file>& dir) {
    dir.clear();

    if(std::filesystem::path(path).parent_path() != path) {
        file upDir;

        upDir.isDirectory = 1;
        upDir.name = "..";
        upDir.path = std::filesystem::path(path).parent_path();

        dir.push_back(upDir);
    }

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

    sort(dir.begin(), dir.end(), sortName);
}