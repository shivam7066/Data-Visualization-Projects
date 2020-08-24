/// \author Your Name Here <Your Email Address Here>.
/// \date The Date Here.
///
/// \brief dummy program example.

// include the local headers
#include                  <ttkLevelSets.h>
#include                  <ttkProgramBase.h>

using namespace std;
using namespace ttk;

int main(int argc, char **argv) {

    vtkProgram<ttkLevelSets> program;


    int ret = 0;
    ret = program.init(argc, argv);

    program.ttkObject_->SetScalarField("XFiltering")
    program.ttkObject_->SetLevel(0.4);


    if(ret != 0)
    return ret;


    // execute data processing
    ret = program.run();

    if(ret != 0)
    return ret;


    return ret;
}
