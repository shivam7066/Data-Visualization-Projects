/// \ingroup base
/// \class ttk::VectorFieldEuler
/// \author Your Name Here <Your Email Address Here>
/// \date The Date Here.
///
/// \brief TTK %vectorFieldEuler processing package.
///
/// %VectorFieldEuler is a TTK processing package that takes a scalar field on the input
/// and produces a scalar field on the output.
///
/// \sa ttk::Triangulation
/// \sa ttkVectorFieldEuler.cpp %for a usage example.

#pragma once

// base code includes
#include                  <Triangulation.h>
#include                  <Wrapper.h>

using namespace std;

namespace ttk{

  class VectorFieldEuler : public Debug {

  public:

      VectorFieldEuler();
      ~VectorFieldEuler();

      void coordsToVertex(pair<float,float> point, SimplexId &vertex);
      bool isValidPoint(pair<float, float> point);

      template<class dataType>
      bool pickNextVertex(pair<float, float> point, float dt, pair<float, float> &newPoint);

      template<class dataType>
      bool bilinearInterpolation(pair<float, float> point, pair<float, float> &vect);

      template<class dataType>
      int execute(int steps, float delta, vector<pair<float,float> >& line);


      inline int setInputDataPointer(void *datax, void *datay) {
          inputDatax_ = datax;
          inputDatay_ = datay;
          return 0;
      }


      inline int setupTriangulation(Triangulation *triangulation) {
          triangulation_ = triangulation;
          return 0;
      }

  protected:
      void *inputDatax_;
      void *inputDatay_;
      Triangulation *triangulation_;
      float Xmax,Ymax,Xmin,Ymin;
  };
}

template <class dataType> int ttk::VectorFieldEuler::execute(int steps, float delta, vector<pair<float,float> >& line) {
    //steps, number of steps for constructing the stream line
    //delta, increment used for the streamline
    //line, streamline to plot
    dataType *inputDatax = (dataType *)inputDatax_;
    dataType *inputDatay = (dataType *)inputDatay_;
    int vertexNumber = triangulation_->getNumberOfVertices();
    Xmax=(sqrt(vertexNumber)-1)/2;
    Ymax=(sqrt(vertexNumber)-1)/2;
    Xmin=(sqrt(vertexNumber)-1)/2*(-1);
    Ymin=(sqrt(vertexNumber)-1)/2*(-1);
    //initialize the stream line as collection of vertices
    line = vector<pair<float,float> >();
    
    //we define the first seed vertex as a pair of (x,y) coordinates
    pair<float,float> point;
    point.first = 0;
    point.second = -1;
    line.push_back(point);
    for(int i=0; i<steps; i++){
    	pair<float,float> new_point;        
        pickNextVertex<dataType>(point,delta,new_point);
        point=new_point;        
        if(!isValidPoint(point))
        {
            break;        
        }
        line.push_back(point);
    }

    //we implement the loop for the Euler method here

      //pick the new vertex at each iteration (function pickNextvertex(...))

      //implement isValidPoint to check if the new vertex is valid


  return 0;
}


template <class dataType> bool ttk::VectorFieldEuler::pickNextVertex(pair<float,float> point, float dt, pair<float,float>& newPoint) {

    dataType *inputDatax = (dataType *)inputDatax_; //this encodes the x component for each vector
    dataType *inputDatay = (dataType *)inputDatay_; //this encodes the y component for each vector
    SimplexId vertex;
    pair<float,float> vect;
    if(point.first == floor(point.first) && point.second == floor(point.second)){
        //this is easy since we are on one of the points on the grid,
        // we can just retrieve the value of the vector field

        coordsToVertex(point,vertex); // implement this

        //now that we have the vertex we can read it's components from inputDatax_ and inputDatay_
        vect.first = inputDatax[vertex];//save here the x component for the vector
        vect.second = inputDatay[vertex];//save herer the y component for the vector

    }
    else{
        //we are not on the grid, we have to compute the vector field by bilinear interpolation
        bilinearInterpolation<dataType>(point, vect);
    }

    //apply the euler formula here and store the coordinates in newPoint
    newPoint.first=point.first+(vect.first*dt);
    newPoint.second=point.second+(vect.second*dt);

    return true;
}

template <class dataType> bool ttk::VectorFieldEuler::bilinearInterpolation(pair<float,float> point, pair<float,float>& vect) {

    dataType *inputDatax = (dataType *)inputDatax_; //here is where the x component of each vector is stored
    dataType *inputDatay = (dataType *)inputDatay_; //here is where the y component of each vector is stored

    //retrieve all the values you need for implementing the formula
    //....
    float x,y,t1,t2;
    int x1,y1,x2,y2;
    x=point.first;
    y=point.second;
    x1=floor(x);
    y1=floor(y);
    x2=floor(x)+1;
    y2=floor(y)+1;
    dataType fvalx, fvaly;
    SimplexId vertex11,vertex12,vertex21,vertexp22;
    dataType tempx1,tempx2,tempy1,tempy2;
    
    pair<float,float> point11 = make_pair(x1,y1);
    coordsToVertex(point11,vertex11);

    pair<float,float> point12 = make_pair(x1,y2);
    coordsToVertex(point12,vertex12);
    
    pair<float,float> point21 = make_pair(x2,y1);
    coordsToVertex(point21,vertex21);

    pair<float,float> point22 = make_pair(x2,y2);
    coordsToVertex(point22,vertex22);


    

    if(x!=x1 && y==y1) /*we are on a horizontal line*/
    {
      //implement linear interpolation
      t1=(x1-x)/(x1-x2);
      t2=1;
    }
    else if(x==x1 && y!=y1) /*we are on a vertical line*/
    {
      //implement linear interpolation
      t1=(y1-y)/(y1-y2);
      t2=1;
    }
    else{
      //full bilinear interpolation
      t1=(x1-x)/(x1-x2);
      t2=(y1-y)/(y1-y2);
    }
    tempx1=(1-t1)*inputDatax[vertex11]+t1*inputDatax[vertex21];
    tempy1=(1-t1)*inputDatay[vertex11]+t1*inputDatay[vertex21];  

    tempx2=(1-t1)*inputDatax[vertex12]+t1*inputDatax[vertex22];
    tempy2=(1-t1)*inputDatay[vertex12]+t1*inputDatay[vertex22];  

    fvalx=(t2)*tempx1 + (1-t2)*tempx2;
    fvaly=(t2)*tempy1 + (1-t2)*tempy2;
    
    vect = pair<float,float>(fvalx,fvaly);
    return true;

}
