
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

struct Values {
    char operation;
    short left;
    short right;
    int result;
};

struct Matrix3 {
    int mat[9];
};

struct Vector3 {
    int vec[3];
};
int gofunc(Values values[], int num_values);
void matmul(Vector3& result, const Matrix3& mat, const Vector3& vec);


int main()
{
 
   
    Values test[] = {
        {'|', 0b1100, 0b0101, 0},
        {'&', 0b1100, 0b0101, 0},
        {'^', 0b1100, 0b0101, 0},
        {'~', 0b1100, 0b0101, 0},

    };


    Vector3 vec = {
        1 , 2, 3
    };


    Vector3 RET = {
       0 , 0 ,0
    };

    Matrix3 mat = {
        1,2,3, 4,5,6, 7,8,9
   
    };

   

   (void) gofunc(test, 4);
   (void)matmul(RET, mat, vec);

   // cout << sizeof(Matrix3) << endl;
  //  cout << mat[4] << endl;


   return 0;
}

int gofunc(Values a0[], int a1) {
    Values* tmp = a0;
    for (int t0 = 0; t0 < a1; t0++) {

        short t1 = ((a0)->left); // a0 + 2
        short t2 = ((a0)->right); //a0 + 4
        char t3 = ((a0)->operation); // a0 + 0
        int t4 = -1;
        //short t4 = *((a0 + t0)->result); //a0 + 4=



        switch (t3) {
        case '&':
            // a0[t0].result => calculating offset (values + i) -> result
            // sw t4, 8(a0)
            t4 = t1 & t2;
           
            // ((a0)->result) = t4;
            break;
        case '|':
            t4 = t1 | t2;
            break;
        case '^':
            t4 = t1 ^ t2;
            break;
        case '~':
            t4 = ~t1;
            break;


        }
        // a0+=12
        ((a0)->result) = t4;

        a0 += 1;
    }

    for (int t0 = 0; t0 < a1; t0++) {
            printf("%d: %d %c %d = %d \n", t0, tmp[t0].left, tmp[t0].operation, tmp[t0].right, tmp[t0].result);
           

    }
     cout << endl;
      cout << endl;
   
    return -1;
}

  /*
 
  switch (a0[t0].operation) {
        case '&':
            // a0[t0].result => calculating offset (values + i) -> result
                a0[t0].result = a0[t0].left & a0[t0].right;
                break;
        case '|':
            a0[t0].result = a0[t0].left | a0[t0].right;
            break;
        case '^':
            a0[t0].result = a0[t0].left ^ a0[t0].right;
            break;
        case '~':
            a0[t0].result = ~a0[t0].left;
            break;
        }
  */

void matmul(Vector3& result, const Matrix3& mat, const Vector3& vec) {
    for (int t0 = 0; t0 < 3; t0++) {

        for (int t1 = 0; t1 < 3; t1++) {
       
            result.vec[t0] += mat.mat[t0*3+t1] *vec.vec[t1];
            


       
     }
       // cout << vec.vec[i] << endl;
        //result.vec[i+3] = mat.mat[i+3] * vec.vec[i+3];
       // result.vec[i+6] = mat.mat[i+6] * vec.vec[i+6];
    }
    for (int i = 0; i < 3; i++) {
        cout << result.vec[i]<< endl;
       // cout << result.vec[i+3];
        //cout << result.vec[i+6];
    }

}
