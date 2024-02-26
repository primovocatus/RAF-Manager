#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>

struct file {
    std::string name, path;
    int size;

    bool isDirectory;

    std::string humanSize() {
        int type = 0;

        double lsize = size;
        while(lsize >= 1024) {
            type++;
            lsize /= 1024.;
        }

        std::string humanSize = std::to_string((int)lsize) + " " + "BKMGTP"[type];
        while((int)humanSize.size() < 7) {
            humanSize = " " + humanSize;
        }

        return humanSize;
    }

    bool operator <(const file& X) const {
        if(isDirectory && X.isDirectory) {
            return name < X.name;
        }
        return isDirectory;
    }
};

void getFiles(const std::string& path, std::vector<file>& dir);