#include <iostream>
#include "../include/Session.h"
using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "usage splflix input_file" << endl;
        return 0;
    }
   // Session s(argv[1]);

    Session* s = new Session(argv[1]);
    s->start();
    Session s2 = *s;
    delete(s);
    s2.start();
    Session s3 = Session(argv[1]);
    s3 = s2;
    s3.start();
    return 0;

}