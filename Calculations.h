//
// Created by Akshat Pant on 11/24/22.
//

#ifndef HELLOSFML_CALCUATIONS_H
#define HELLOSFML_CALCUATIONS_H

#endif //HELLOSFML_CALCUATIONS_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

class Calculations{
public:
    void multProjection(double arr1[2][3], double arr2[3][1], double arr3[2][1]);
    double vectorMagnitude(double x, double y, double z);
    double dotProduct(double vect_A[3], double vect_B[3]);
    void crossProduct(double vect_A[], double vect_B[], double cross_P[]);
    void multRotation(double arr1[][3], double arr2[][1], double arr3[3][1]);

};

void Calculations::multProjection(double arrA[2][3], double arrB[3][1], double arrC[2][1]){
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 1; j++){
            arrC[i][j] = 0;
            for(int k = 0; k < 3; k++){
                arrC[i][j] += arrA[i][k] * arrB[k][j];
            }
        }
    }
}

//The vector magnitude method calculates the magnatiude of a vector - used for plane shade calculations
double Calculations::vectorMagnitude(double a, double b, double c)
{

    double sum = a * a + b * b + c * c;


    return sqrt(sum);
}

//Dot product calculates the dot product between vectors - used for plane shade calculation
double Calculations::dotProduct(double A[3], double B[3])
{

    double output = 0;


    for (int i = 0; i < 3; i++)

        output = output + A[i] * B[i];
    return output;
}

//crossproduct method calculates the crossProduct between vectors
void Calculations::crossProduct(double A[], double B[], double C[])

{

    C[0] = A[1] * B[2] - A[2] * B[1];
    C[1] = A[2] * B[0] - A[0] * B[2];
    C[2] = A[0] * B[1] - A[1] * B[0];
}

//MultRotations does matric multiplication intended for rotation functionality
void Calculations::multRotation(double arrA[][3], double arrB[][1], double arrC[3][1]){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 1; j++){
            arrC[i][j] = 0;
            for(int k = 0; k < 3; k++){
                arrC[i][j] += arrA[i][k] * arrB[k][j];
            }
        }
    }


}