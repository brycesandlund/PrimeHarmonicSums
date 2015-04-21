#include "special.cpp"

using namespace std;

#define EP 1e-10

int main() {
    
    for (int i = 10; i <= 1000; ++i) {
        int next_cube = (int)(pow((int)(pow(i, 1.0/3)+1-EP), 3) + EP);
        if (phi_s(i)-EP > phi_s(next_cube)) {
            cout << "ERROR" << endl;
            cout << i << " " << next_cube << endl;
            cout << phi_s(i) << endl;
            cout << phi_s(next_cube) << endl;
        }
    }

    //quad_float result = phi_s(300);
    //quad_float result2 = phi_s(343);
    //cout << result << endl;
}
