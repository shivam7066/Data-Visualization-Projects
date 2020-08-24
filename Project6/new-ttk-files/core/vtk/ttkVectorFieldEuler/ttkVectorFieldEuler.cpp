#include                  <ttkVectorFieldEuler.h>

using namespace std;
using namespace ttk;

vtkStandardNewMacro(ttkVectorFieldEuler)

int ttkVectorFieldEuler::doIt(vector<vtkDataSet *> &inputs, vector<vtkDataSet *> &outputs){

    Memory m;

    vtkDataSet *input = inputs[0];
    vtkDataSet *output = outputs[0];

    Triangulation *triangulation = ttkTriangulation::getTriangulation(input);

    if(!triangulation)
        return -1;

    vectorFieldEuler_ = new VectorFieldEuler();

    triangulation->setWrapper(this);
    vectorFieldEuler_->setupTriangulation(triangulation);
    vectorFieldEuler_->setWrapper(this);


    vtkDataArray *inputVectorFieldx = NULL;
    vtkDataArray *inputVectorFieldy = NULL;

    inputVectorFieldx = input->GetPointData()->GetArray(VectorField[0].data());
    inputVectorFieldy = input->GetPointData()->GetArray(VectorField[1].data());

    vectorFieldEuler_->setInputDataPointer(inputVectorFieldx->GetVoidPointer(0),inputVectorFieldy->GetVoidPointer(0));
    vector<pair<float,float> > lines;
    switch(inputVectorFieldx->GetDataType()) {
        ttkTemplateMacro(vectorFieldEuler_->execute<VTK_TT>(Steps,Delta,lines));
    }

    vtkUnstructuredGrid* outputline=vtkUnstructuredGrid::SafeDownCast(output);
    //TODO:
    //here we are preparing the output
    //look at the code from last assignmnet, also there we were creating lines given a list of points

    vtkSmartPointer<vtkPoints> linePoints=vtkSmartPointer<vtkPoints>::New();
    int i;
    for(i=0; i<lines.size();i++)
    {
        linePoints->InsertNextPoint(lines[i].first,lines[i].second,0.0);
    }

    //take each single point from lines and add it to the oputput
    
    outputline->SePoints(linePoints);
    outputline->Allocate(lines.size()/2)
    //define the lines, to be drawn
    tkIdType line[2];
    for(int i =0; i< lines.size();i=i+2){
        line[0] = i;
        line[1] = i+1;
        outputline->InsertNextCell(VTK_LINE,2,line);
    }

    return 0;
}
