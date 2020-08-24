/// \ingroup vtk
/// \class ttkVectorFieldEuler
/// \author Your Name Here <Your Email Address Here>
/// \date The Date Here.
///
/// \brief TTK VTK-filter that wraps the vectorFieldEuler processing package.
///
/// VTK wrapping code for the @VectorFieldEuler package.
///
/// \param Input Input scalar field (vtkDataSet)
/// \param Output Output scalar field (vtkDataSet)
///
/// This filter can be used as any other VTK filter (for instance, by using the
/// sequence of calls SetInputData(), Update(), GetOutput()).
///
/// See the related ParaView example state files for usage examples within a
/// VTK pipeline.
///
/// \sa ttk::VectorFieldEuler
#pragma once

// VTK includes -- to adapt
#include                  <vtkCharArray.h>
#include                  <vtkDataArray.h>
#include                  <vtkDataSet.h>
#include                  <vtkDataSetAlgorithm.h>
#include                  <vtkDoubleArray.h>
#include                  <vtkFiltersCoreModule.h>
#include                  <vtkFloatArray.h>
#include                  <vtkInformation.h>
#include                  <vtkIntArray.h>
#include                  <vtkObjectFactory.h>
#include                  <vtkPointData.h>
#include                  <vtkSmartPointer.h>

// ttk code includes
#include                  <VectorFieldEuler.h>
#include                  <ttkWrapper.h>

// in this example, this wrapper takes a data-set on the input and produces a
// data-set on the output - to adapt.
// see the documentation of the vtkAlgorithm class to decide from which VTK
// class your wrapper should inherit.
#ifndef TTK_PLUGIN
class VTKFILTERSCORE_EXPORT ttkVectorFieldEuler
#else
class ttkVectorFieldEuler
#endif
  : public vtkDataSetAlgorithm, public ttk::Wrapper{

  public:

    static ttkVectorFieldEuler* New();
    vtkTypeMacro(ttkVectorFieldEuler, vtkDataSetAlgorithm)

    void SetVectorField(std::string name){
        if(vectorFieldEuler_ != NULL){
            vectorFieldEuler_ = NULL;
            VectorField.clear();
        }
        VectorField.push_back(name);
        Modified();
    }

    void SetSteps(int val){
      Steps = val;
      Modified();
    }

    void SetDelta(double val){
        Delta = val;
        Modified();
    }

    int FillInputPortInformation(int port, vtkInformation *info) override {

        switch(port){
            case 0:
                info->Set(vtkDataObject::DATA_TYPE_NAME(),
                          "vtkDataSet"); break;
            default:
                break;
        }

        return 1;
    }


    int FillOutputPortInformation(int port, vtkInformation *info) override
    {

        switch(port){
            case 0:
                info->Set(vtkDataObject::DATA_TYPE_NAME(),
                          "vtkUnstructuredGrid"); break;
            default:
                break;
        }

        return 1;
    }

protected:
    ttkVectorFieldEuler() {
        SetNumberOfInputPorts(1);
        SetNumberOfOutputPorts(1);

        vectorFieldEuler_ = NULL;
    }

    ~ttkVectorFieldEuler(){};

    TTK_SETUP();


  private:

    std::vector<std::string>               VectorField;
    ttk::VectorFieldEuler*           vectorFieldEuler_;
    int                                          Steps;
    double                                       Delta;

};
