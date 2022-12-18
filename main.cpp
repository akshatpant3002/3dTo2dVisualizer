#include <iostream>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include "Calculations.h"
using namespace std;



int main() {
    Calculations h;

    // declare variables
    string line;
    int vertices;
    int faces;
    ifstream myfile("object.txt");
    getline (myfile,line, ',');
    vertices = stoi(line);
    getline(myfile,line);
    faces = stoi(line);

    map<int, sf::CircleShape> arb; //vertices
    map<int, vector<double> > container;
    vector<pair<sf::Vertex, sf::Vertex> > ab; //faces allow lines to be made
    vector<sf::ConvexShape> plane;

    int key;
    double val1;
    double val2;
    double val3;
    string abc;

    //reads the vertices for the file and stores them within a map called container
    for(int i = 0;i < vertices;i++){

        getline(myfile, line);
        stringstream linestream(line);
        getline(linestream, abc,',');
        key = stoi(abc);
        getline(linestream, abc,',');
        val1 = stod(abc);
        getline(linestream, abc,',');
        val2 = stod(abc);
        getline(linestream, abc,',');
        val3 = stod(abc);
        vector<double> xyz;
        xyz.push_back(val1);
        xyz.push_back(val2);
        xyz.push_back(val3);
        container[key] = xyz;
    }

    string test;
    int f1;
    int f2;
    int f3;
    int mouseOGx;
    int mouseOGy;
    bool press;
    map<int, vector<int> > faceCon;


    //reads the section defining the faces
    for(int i = 1;i < faces + 1;i++){

        getline(myfile, line);
        stringstream linestream(line);
        getline(linestream, test,',');
        f1 = stoi(test);
        getline(linestream, test, ',');
        f2 = stoi(test);
        getline(linestream, test, ',');
        f3 = stoi(test);



        vector<int> obj;
        obj.push_back(f1);
        obj.push_back(f2);
        obj.push_back(f3);

        //faceCon stores a vector of the 3 vertex that make up a face
        faceCon[i] = obj;
        pair<sf::Vertex, sf::Vertex> in; //stores corresponding x and y coordinates between two points to make line - does not actually make line yet. will be used to make the line later


        in.first = sf::Vertex(sf::Vector2f(arb.find(f1)->second.getPosition().x, arb.find(f1)->second.getPosition().y));
        in.second = sf::Vertex(sf::Vector2f(arb.find(f2)->second.getPosition().x, arb.find(f2)->second.getPosition().y));
        ab.push_back(in);
        in.first = sf::Vertex(sf::Vector2f(arb.find(f2)->second.getPosition().x, arb.find(f2)->second.getPosition().y));
        in.second = sf::Vertex(sf::Vector2f(arb.find(f3)->second.getPosition().x, arb.find(f3)->second.getPosition().y));
        ab.push_back(in);
        in.first = sf::Vertex(sf::Vector2f(arb.find(f1)->second.getPosition().x, arb.find(f1)->second.getPosition().y));
        in.second = sf::Vertex(sf::Vector2f(arb.find(f3)->second.getPosition().x, arb.find(f3)->second.getPosition().y));
        ab.push_back(in);

        //Creates plane to do part 2 of the project where the shades of the plane are dependent on the roation of the object
        sf::ConvexShape convex;

// resize it to 5 points
        convex.setPointCount(3);

// define the points
        convex.setPoint(0, sf::Vector2f(arb.find(f1)->second.getPosition().x, arb.find(f1)->second.getPosition().y));
        convex.setPoint(1, sf::Vector2f(arb.find(f2)->second.getPosition().x, arb.find(f2)->second.getPosition().y));
        convex.setPoint(2, sf::Vector2f(arb.find(f3)->second.getPosition().x, arb.find(f3)->second.getPosition().y));

        plane.push_back(convex);


    }



    //Creates window dimensions
    sf::RenderWindow window(sf::VideoMode(400, 400), "SFML Application");



    //while loop that runs as the window is open - where all functionality and visulas are implemented
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(0, 0, 0, 0));



        //iterates through all of the vertex values for each id
        for(auto i: container) {
            double result2[3][1];
            double result[3][1];

            //Create Matrix for both 3d to 2d projection and rotations about said axis - default rn
            double vec[3][1] = {{i.second[0]},
                                {i.second[1]},
                                {i.second[2]}};
            double matx[3][3] = { { 1, 0 , 0 },
                                  { 0, cos(0*3.14159/180) , -sin(0*3.14159/180)},
                                  {0,sin(0*3.14159/180),cos(0*3.14159/180)}};
            double maty[3][3] = { { cos(0*3.14159/180), 0 , sin(0*3.14159/180)},// according the wiki not neg mby so might cause error
                                  { 0, 1 , 0},
                                  {-sin(0*3.14159/180),0,cos(0*3.14159/180)}};
            double matP[2][3] = {{1,0,0},
                                 {0,1,0}};

            double projResult[2][1];

            //uses matrix multiplication methods to do rotations
            h.multRotation(matx, vec,result2);
            h.multRotation(maty, result2,result);

            double vecTransf[3][1] = {{result[0][0]},
                                {result[1][0]},
                                {result[2][0]}};

            //Project the 3D vertex onto a 2D window screen calculations
            h.multProjection(matP, vecTransf, projResult);

            vector<double> newVal;
            newVal.push_back(result[0][0]);//x
            newVal.push_back(result[1][0]);//y
            newVal.push_back(result[2][0]);//z



            container[i.first] = newVal;   // updates the container value with the new rotated vertex



            // Creates circle shape for vertex
            sf::CircleShape shape;
            shape.setRadius(2);
            double v1 = projResult[0][0];
            double v2 = projResult[1][0];

            //Scales the vertex to all the object to fill the window
            v1 *= 70.0;
            v2 *= 70.0;
            v1 += 200.0;
            v2 += 200.0;

            //set the x and y position for the vertex
            shape.setPosition(v1 , v2);
            shape.setFillColor(sf::Color::Blue);

            arb[i.first] = shape; // stores the vertex shape to later be drawn


        }



        //checks to see if the mouse has been unclicked
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) == false){
                press = false;
        }
        vector<pair<int,int> > mouseDragPosition;


        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            //if the mouse was unclicked previously then set new orginal x and y coordinates of the mouse
            if(press == false){
                mouseOGx = sf::Mouse::getPosition().x;
                mouseOGy = sf::Mouse::getPosition().y;
                press = true;
            }
            //stores the current x and y coordinates of the mouse
            int currMx = sf::Mouse::getPosition().x;
            int currMy = sf::Mouse::getPosition().y;

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                for(auto i: container) {
                    double result2[3][1];
                    double result[3][1];
                    double result3[3][1];
                    double vec[3][1] = {{i.second[0]},
                                        {i.second[1]},
                                        {i.second[2]}};

                    cout << "Curr: " << currMx << " MouseOGx: " << mouseOGx << endl;

                    //does rotation based on the the distance changed between the orginal and current x and y coordinates
                    double matx[3][3] = { { 1, 0 , 0 },
                                          { 0, cos((currMy-mouseOGy)*0.001*3.14159/180) , -sin((currMy-mouseOGy)*0.001*3.14159/180)},
                                          {0,sin((currMy-mouseOGy)*0.001*3.14159/180),cos((currMy-mouseOGy)*0.001*3.14159/180)}};

                    double maty[3][3] = { { cos((currMx-mouseOGx)*0.001*3.14159/180), 0 , -sin((currMx-mouseOGx)*0.001*3.14159/180)},// according the wiki not neg mby so might cause error
                                          { 0, 1 , 0},
                                          {sin((currMx-mouseOGx)*0.001*3.14159/180),0,cos((currMx-mouseOGx)*0.001*3.14159/180)}};

                    double matz[3][3] = { { cos(0*3.14159/180), -sin(0*3.14159/180) , 0},
                                          { sin(0*3.14159/180), cos(0*3.14159/180) , 0},
                                          {0,0,1}};

                    double matP[2][3] = {{1,0,0},
                                         {0,1,0}};

                    double projResult[2][1];

                    //does Matrix multiplcation for rotations and projecting from 3D to 2D
                    h.multRotation(matx, vec,result2);
                    h.multRotation(maty, result2,result3);
                    h.multRotation(matz, result3,result);
                    double vecTransf[3][1] = {{result[0][0]},
                                              {result[1][0]},
                                              {result[2][0]}};
                    h.multProjection(matP, vecTransf, projResult);



                    vector<double> newVal;
                    newVal.push_back(result[0][0]);//x
                    newVal.push_back(result[1][0]);//y
                    newVal.push_back(result[2][0]);//z

                    //updates container with new rotated values
                    container[i.first] = newVal;

                    //makes the vertex and sets is position based on the rotation
                    sf::CircleShape shape;
                    shape.setRadius(2);
                    double v1 = projResult[0][0];
                    double v2 = projResult[1][0];



                    //used for scaling the vertex
                    v1 *= 70.0;
                    v2 *= 70.0;
                    v1 += 200.0;
                    v2 += 200.0;

                    // sets the vertex x and y coordinates based on rotations conducted
                    shape.setPosition(v1 , v2);
                    shape.setFillColor(sf::Color::Blue);

                    arb[i.first] = shape; //stores new vertex and its locations for said id so that can be resued to conduct more changes later


                }
            }
        }





        //Actually draws all of the vertex on the window
        for(auto i: arb){
            window.draw(i.second);
        }

        ab.clear();
        plane.clear();

        //used to store vertex that connect into ab for it to be draw a line later
        for(auto i: faceCon){
            pair<sf::Vertex, sf::Vertex> in;
            in.first = sf::Vertex(sf::Vector2f(arb.find(i.second[0])->second.getPosition().x, arb.find(i.second[0])->second.getPosition().y));
            in.second = sf::Vertex(sf::Vector2f(arb.find(i.second[1])->second.getPosition().x, arb.find(i.second[1])->second.getPosition().y));
            ab.push_back(in);
            in.first = sf::Vertex(sf::Vector2f(arb.find(i.second[1])->second.getPosition().x, arb.find(i.second[1])->second.getPosition().y));
            in.second = sf::Vertex(sf::Vector2f(arb.find(i.second[2])->second.getPosition().x, arb.find(i.second[2])->second.getPosition().y));
            ab.push_back(in);
            in.first = sf::Vertex(sf::Vector2f(arb.find(i.second[0])->second.getPosition().x, arb.find(i.second[0])->second.getPosition().y));
            in.second = sf::Vertex(sf::Vector2f(arb.find(i.second[2])->second.getPosition().x, arb.find(i.second[2])->second.getPosition().y));
            ab.push_back(in);


            //container is a map that stores 3 vertex values
            // face con stores 3 ids that make a face

            // Below is the calculations for Part2 where the orthogonal vector is calculated and said planes angle to the orthogonal vector
            //Based on this said angle a shade is created for said plane
            double a = container.find(i.second[0])->second[0] - container.find(i.second[1])->second[0];
            double b = container.find(i.second[0])->second[1] - container.find(i.second[1])->second[1];
            double c = container.find(i.second[0])->second[2] - container.find(i.second[1])->second[2];
            double d = container.find(i.second[2])->second[0] - container.find(i.second[1])->second[0];
            double e = container.find(i.second[2])->second[1] - container.find(i.second[1])->second[1];
            double f = container.find(i.second[2])->second[2] - container.find(i.second[1])->second[2];
            double matv1[] = {a,b,c};
            double matv2[] = {d,e,f};
            double t[] = {0,0,1};
            double matv3[3];
            double angle;
            double finalVal;
            h.crossProduct(matv1,matv2, matv3);
            angle = abs(h.dotProduct(matv3,t)/ (h.vectorMagnitude(0,0,1) * h.vectorMagnitude(matv3[0],matv3[1],matv3[2])));
            finalVal = (angle * 160) + 95;
            //creates the plane
            sf::ConvexShape convex;
            convex.setPointCount(3);
            convex.setPoint(0, sf::Vector2f(arb.find(i.second[0])->second.getPosition().x, arb.find(i.second[0])->second.getPosition().y));
            convex.setPoint(1, sf::Vector2f(arb.find(i.second[1])->second.getPosition().x, arb.find(i.second[1])->second.getPosition().y));
            convex.setPoint(2, sf::Vector2f(arb.find(i.second[2])->second.getPosition().x, arb.find(i.second[2])->second.getPosition().y));
            //sets the planes color based on earlier relocations
            convex.setFillColor(sf::Color (0,0,finalVal)); // Sets color of the
            plane.push_back(convex);
        }

        //draws all of the planes
        for(int i = 0;i < plane.size();i++){
            window.draw(plane[i]);
        }

        //draws all of the lines
        for(int i= 0;i < ab.size();i++){ // makes line
            sf::Vertex rect[] =
                    {
                            ab[i].first,
                            ab[i].second
                    };
            rect->color = sf::Color::Blue;
            window.draw(rect, 2, sf::Lines);

        }
        window.display();
    }

}
