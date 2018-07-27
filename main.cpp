#include <igl/opengl/glfw/Viewer.h>

#include <igl/decimate.h>
#include <igl/upsample.h>

void resample(Eigen::MatrixXd& V, Eigen::MatrixXi& F, int targetResolution)
{
	Eigen::MatrixXd Vcurr = V;
	Eigen::MatrixXi Fcurr = F;
	while (Fcurr.rows() < targetResolution * 2)
	{
		Eigen::MatrixXd Vtmp = Vcurr;
		Eigen::MatrixXi Ftmp = Fcurr;
		igl::upsample(Vtmp, Ftmp, Vcurr, Fcurr);
	}
	Eigen::VectorXi J;

	igl::decimate(Vcurr, Fcurr, targetResolution, V, F, J);
	igl::writeOBJ("resampled.obj", V, F);
}

int main(int argc, char *argv[])
{
  // Inline mesh of a cube
  Eigen::MatrixXd V= (Eigen::MatrixXd(8,3)<<
    0.0,0.0,0.0,
    0.0,0.0,1.0,
    0.0,1.0,0.0,
    0.0,1.0,1.0,
    1.0,0.0,0.0,
    1.0,0.0,1.0,
    1.0,1.0,0.0,
    1.0,1.0,1.0).finished();
  Eigen::MatrixXi F = (Eigen::MatrixXi(12,3)<<
    1,7,5,
    1,3,7,
    1,4,3,
    1,2,4,
    3,8,7,
    3,4,8,
    5,7,8,
    5,8,6,
    1,5,6,
    1,6,2,
    2,6,8,
    2,8,4).finished().array()-1;

  resample(V, F, 1000);

  // Plot the mesh
  igl::opengl::glfw::Viewer viewer;
  viewer.data().set_mesh(V, F);
  viewer.data().set_face_based(true);
  viewer.launch();
}
