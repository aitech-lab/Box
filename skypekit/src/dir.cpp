#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

void getNextSms(string dir, string& phone, string& sms) {
    
    DIR *dp;
    struct dirent *dirp;
    phone = "";
    sms   = "";
    
    string file_name = "";
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error opening " << dir << endl;
        return;
    }
    
    while ((dirp = readdir(dp)) != NULL) {
        file_name = string(dirp->d_name);
        if(file_name!=".." && file_name!=".") {
            string path = dir+"/"+file_name;
            ifstream ifs(path.c_str(), ifstream::in );
            cout << path << "\n";
            getline(ifs, phone);
            getline(ifs, sms);
            ifs.close();
            remove(path.c_str());
            closedir(dp);
            return; 
        }
    }
    
    closedir(dp);
    return;
}



int main(int argc, char** argv) {
    
    string dir;
    if(argc<2) dir= string(".");
    else dir = string(argv[1]);
    string phone;
    string sms;
    getNextSms(dir, phone, sms);
    cout << dir <<": " << phone << " << " << sms << "\n";
    return 0;
}
