/// \ingroup base
/// \class ttk::LevelSets
/// \author Your Name Here <Your Email Address Here>
/// \date The Date Here.
///
/// \brief TTK %levelSets processing package.
///
/// %LevelSets is a TTK processing package that takes a scalar field on the input
/// and produces a scalar field on the output.
///
/// \sa ttk::Triangulation
/// \sa ttkLevelSets.cpp %for a usage example.

#pragma once

// base code includes
#include                  <Triangulation.h>
#include                  <Wrapper.h>

using namespace std;

namespace ttk{

  class LevelSets : public Debug{

    public:

        LevelSets();

        ~LevelSets();


        template <class dataType>
        int execute(dataType level, vector<vector<float> >& minima, vector<vector<float> >& maxima, vector<vector<float> >&lines);

        template <class dataType>
        bool findPoint(float f, SimplexId v1, SimplexId v2, dataType f1, dataType f2, vector<float>& point);

        inline int setInputDataPointer(void *data){
            inputData_ = data;
            return 0;
        }


        inline int setupTriangulation(Triangulation *triangulation){
            triangulation_ = triangulation;

            if(triangulation_){
                triangulation_->preprocessVertexNeighbors();
            }

            return 0;
        }

    protected:

      void                  *inputData_;
      Triangulation         *triangulation_;
  };
}


// template functions
template <class dataType> int ttk::LevelSets::execute(dataType level, vector<vector<float> >& minima, vector<vector<float> >& maxima, vector<vector<float> >& lines){

    //here we need to populate the list of mimina, maxima and the counter lines
    //minima will store a list of points encoded by their x,y,z coordinates
    //maxima will store a list of points encoded by their x,y,z coordinates
    //lines will store a list of points encoded by their x,y,z coordinates, each pair of points will correspond to a line forming the contour

    //the following line is how we read the input scalar field,
    //we recall that the scalar field here is defined on the vertices of our triangulation
    //so inputData[i] will correspond to the scalar value associated to the vertex of index i
    dataType *inputData = (dataType *) inputData_;

    //STEP 1 - Extract the minima and maxima from your dataset
    //TODO: you need to cycle on the vertices of your triangulation
    //TIP: you can retrieve the total number of vertices with the function
    //triangulation_->getNumberOfVertices();
    SimplexId NumberOfVertices,neighbours;
    int flagmax=1, flagmin=1,i,j;
    float x,y,z,normal;
    dataType parent,child;
    int max,min;
    NumberOfVertices=triangulation_->getNumberOfVertices();
        //Then, for each vertex you should retrieve its neighbors
        //you can use the function getVertexNeighborNumber() and getVertexNeighbor() in Triangulation
        //(https://topology-tool-kit.github.io/doc/html/classttk_1_1Triangulation.html#a2755bb69b06a1c82595e3bbe8ce97570)
        for(i=0;i<NumberOfVertices;i++)
        {
          neighbours=triangulation_->getVertexNeighborNumber(i);
          parent=inputData[i];
        //Given the set of neighbors (adjacent vertices) identify if the vertex is a maximum or a minimum and update
        //the vectors minima/maxima accordingly
        //....
            for(j=0;j<neighbours;j++)
            {
              flagmax=1;
              flagmin=1;
              SimplexId neighbour{};
              triangulation_->getVertexNeighbor(i,j,neighbour); 
              child=inputData[neighbour];
              if(parent<child)
              {
                flagmax=0;
                break;
              }
              else
              {
                flagmin=0;
                break;
              }
            }
            if(flagmax==1){
                vector<float> maximapoint;
                triangulation_->getVertexPoint(i,x,y,z);
                maximapoint.push_back(x);
                maximapoint.push_back(y);
                maximapoint.push_back(z);
                maxima.push_back(maximapoint);
            }
            if(flagmin==1){
                vector<float> minimapoint;
                triangulation_->getVertexPoint(i,x,y,z);
                minimapoint.push_back(x);
                minimapoint.push_back(y);
                minimapoint.push_back(z);
                minima.push_back(minimapoint);
            }

        }
    max=min=inputData[0]; 
    for(i=1;i<NumberOfVertices;i++)
    {
       if(inputData[i]>max)
       {
          max=inputData[i];
       }
       else if(inputData[i]<min)
       {
          min=inputData[i];  
       }
    }

    //STEP 2 - Compute the isocontour given a level set

    //TODO: First thing. Our input value (level) is between [0,1]. We need to find what is the real
    //level set value for the input scalar field
    //Find the maximum and minimum values of the input scalar field and find out at which value the level corresponds
    //...
    normal=((level)*(max-min))+min;


    //Now that we know what is the real value for the level set, we are ready to compute it
    //The marching triangle algorithm could be implemented with a lookup table
    //For this assignment I suggest a strategy that is easier to implement (but less efficient)

    //We still work independently on each triangle (cycle on them similarly to what you did with the vertices)
        //For each triangle extract its vertices (search the proper function in
        //https://topology-tool-kit.github.io/doc/html/classttk_1_1Triangulation.html)
        //..
    
    int NumberOfTriangles,v1,v2,v3;
    NumberOfTriangles=triangulation_->getNumberOfTriangles();
    for(i=0;i<NumberOfTriangles;i++)
    {
        triangulation_->getTriangleVertex(i,0,v1);
        triangulation_->getTriangleVertex(i,1,v2);
        triangulation_->getTriangleVertex(i,2,v3);
        vector<float> point1,point2, point3;
        bool v12, v23, v31;
        v12= findPoint(normal,v1,v2,inputData[v1],inputData[v2],point1);
        v23= findPoint(normal,v2,v3,inputData[v2],inputData[v3],point2);
        v31= findPoint(normal,v3,v1,inputData[v3],inputData[v1],point3);
        
        if(v12 && v23){
            lines.push_back(point1);
            lines.push_back(point2);
        }
         else if(v23 && v31){
            lines.push_back(point2);
            lines.push_back(point3);
        }
        if(v31 && v12){
            lines.push_back(point3);
            lines.push_back(point1);
        }
    }
        //for each edge (pair of vertices (v1,v2)) we need to check if the level set hits that edge, i.e. the level set is between v1 and v2
        //and, if this is the case, we need find the corresponding point by interpolation.
        //Implement and use the function findPoint to this aim. Save the point you have found for later.
        //..

        //here we have computed all the points where the level set intersects the triangle
        //if you have found 2 points in the above loop, then you have found a new edge for your contour, save it in `lines`
        //..

  return 0;
}

template<class dataType>
bool ttk::LevelSets::findPoint(float lvlSet, SimplexId v1, SimplexId v2, dataType f1, dataType f2, vector<float> &point) {

    //This function will compute the x,y,z coordinates of the point by interpolation know the level set (function value) of such point
    //lvlSet is the function value for the contour
    //v1 is the index of the first vertex composing the edge
    //v2 is the index of the second vertex composing the edge
    //f1 is the function value of the first vertex
    //f2 is the function value of the second vertex
    //point is the vector were we will save the new point computed

    point = vector<float>(3);

    //TODO: check if the level set falls in between v1 and v2.
    //If not, we have nothing to compute and we just return false
    //TODO: otherwise, initialize the point with the correct coordinates
        //get the vertex coordinates for v1 and v2 with the function getVertexPoint in Triangulation
        //...
    
    float x,y,z,x1,x2,y1,y2,z1,z2,t;
    if(((lvlSet>f1 && lvlSet<f2) || (lvlSet<f1 && lvlSet>f2)))
    {
     //getVertexPoint (const SimplexId &vertexId, float &x, float &y, float &z)
        triangulation_->getVertexPoint(v1,x1,y1,z1);
        triangulation_->getVertexPoint(v2,x2,y2,z2);
        //we can use an helper function that express the interpolation along one axis as
        // f = (1-t)*f1 + t*f2 -> t=(f-f1)/(f2-f1)
        // (https://en.wikipedia.org/wiki/Linear_interpolation)
        //notice that f = lvlSet, so we know all the parameters except t
        //find out the value of t
        //...
        t=(lvlSet-f1)/(f2-f1);
        x=(1-t)*x1 + t*x2;
        point.push_back(x);
        y=(1-t)*y1 + t*y2;
        point.push_back(y);
        z=(1-t)*z1 + t*z2;
        point.push_back(z);
        return true;
    }



        //now that you know t, use the previous function to find out the values of x, y and z for our new point
        //work independently on each coordinate and save those in `point`.
        //...

}
