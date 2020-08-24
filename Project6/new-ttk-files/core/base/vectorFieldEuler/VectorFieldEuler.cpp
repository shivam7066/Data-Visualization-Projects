#include                  <VectorFieldEuler.h>

using namespace std;
using namespace ttk;

VectorFieldEuler::VectorFieldEuler(){
  Xmax=0;
  Xmin=0;
  Ymax=0;
  Ymin=0;
  inputDatax_ = NULL;
  inputDatay_ = NULL;
  triangulation_ = NULL;
}

VectorFieldEuler::~VectorFieldEuler(){

}

void VectorFieldEuler::coordsToVertex(pair<float,float> point, SimplexId& vertex){

  //from a pair of coordinates x and y return the index of said point
  int i;
  int vertexNumber;
  float x,y,z,x1,y1;
  vertexNumber=triangulation_->getNumberOfVertices();
  x1=point.first;
  y1=point.second;
  for(i=0;i<vertexNumber;i++){
  	triangulation_->getVertexPoint(i,x,y,z);
  	if(x==x1&& y==y1)
  	{
  		vertex=i;         
  		break;
  	}
  }
}

bool VectorFieldEuler::isValidPoint(pair<float, float> point) {

  //from a pair of coordinates x and y return true if the point is inside the dataset
  float x,y;
  x=point.first;
  y=point.second;
  if(x<Xmax && x>Xmin && y<Ymax && y>Ymin)
  {
  	return true;
  } 
  return false;
}
