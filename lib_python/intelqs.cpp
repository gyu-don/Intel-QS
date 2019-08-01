#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/numpy.h>

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "qureg.hpp"

#include <mpi.h>

//////////////////////////////////////////////////////////////////////////////

namespace py = pybind11;

//////////////////////////////////////////////////////////////////////////////
// PYBIND CODE for the QubitRegister class
//////////////////////////////////////////////////////////////////////////////

PYBIND11_MODULE(intelqs, m)
{
    m.doc() = "pybind11 wrap for the Intel Quantum Simulator";

//////////////////////////////////////////////////////////////////////////////
// Intel-QS
//////////////////////////////////////////////////////////////////////////////

    // Intel Quantum Simulator
    // Notice that to use std::cout in the C++ code, one needs to redirect the output streams.
    // https://pybind11.readthedocs.io/en/stable/advanced/pycpp/utilities.html
//    py::class_<QubitRegister<ComplexDP>, shared_ptr< QubitRegister<ComplexDP> >>(m, "QubitRegister")
    py::class_< QubitRegister<ComplexDP> >(m, "QubitRegister", py::buffer_protocol(), py::dynamic_attr())
        .def(py::init<> ())
        .def(py::init<std::size_t , std::string , std::size_t, std::size_t> ())
        // Element access:
        .def("__getitem__", [](const QubitRegister<ComplexDP> &a, std::size_t index) {
             return a[index];
             }, py::is_operator())
        // One-qubit gates:
        .def("ApplyRotationX", &QubitRegister<ComplexDP>::ApplyRotationX)
        .def("ApplyRotationY", &QubitRegister<ComplexDP>::ApplyRotationY)
        .def("ApplyRotationZ", &QubitRegister<ComplexDP>::ApplyRotationZ)
        .def("ApplyPauliX", &QubitRegister<ComplexDP>::ApplyPauliX)
        .def("ApplyPauliY", &QubitRegister<ComplexDP>::ApplyPauliY)
        .def("ApplyPauliZ", &QubitRegister<ComplexDP>::ApplyPauliZ)
        .def("ApplyPauliSqrtX", &QubitRegister<ComplexDP>::ApplyPauliSqrtX)
        .def("ApplyPauliSqrtY", &QubitRegister<ComplexDP>::ApplyPauliSqrtY)
        .def("ApplyPauliSqrtZ", &QubitRegister<ComplexDP>::ApplyPauliSqrtZ)
        .def("ApplyT", &QubitRegister<ComplexDP>::ApplyT)
        .def("ApplyHadamard", &QubitRegister<ComplexDP>::ApplyHadamard)
        // Two-qubit gates:
        .def("ApplySwap", &QubitRegister<ComplexDP>::ApplySwap)
        .def("ApplyCRotationX", &QubitRegister<ComplexDP>::ApplyCRotationX)
        .def("ApplyCRotationY", &QubitRegister<ComplexDP>::ApplyCRotationY)
        .def("ApplyCRotationZ", &QubitRegister<ComplexDP>::ApplyCRotationZ)
        .def("ApplyCPauliX", &QubitRegister<ComplexDP>::ApplyCPauliX)
        .def("ApplyCPauliY", &QubitRegister<ComplexDP>::ApplyCPauliY)
        .def("ApplyCPauliZ", &QubitRegister<ComplexDP>::ApplyCPauliZ)
        .def("ApplyCPauliSqrtZ", &QubitRegister<ComplexDP>::ApplyCPauliSqrtZ)
        .def("ApplyCHadamard", &QubitRegister<ComplexDP>::ApplyCHadamard)
        // Custom 1-qubit gate and controlled 2-qubit gates:
        .def("Apply1QubitGate",
             [](QubitRegister<ComplexDP> &a, unsigned qubit,
                py::array_t<ComplexDP, py::array::c_style | py::array::forcecast> matrix ) {
               py::buffer_info buf = matrix.request();
               if (buf.ndim != 2)
                   throw std::runtime_error("Number of dimensions must be two.");
               if (buf.shape[0] != 2 || buf.shape[1] != 2)
                   throw std::runtime_error("Input shape is not 2x2.");
               // Create and initialize the custom tiny-matrix used by Intel QS.
               ComplexDP *ptr = (ComplexDP *) buf.ptr;
               TM2x2<ComplexDP> m;
               m(0,0)=ptr[0];
               m(0,1)=ptr[1];
               m(1,0)=ptr[2];
               m(1,1)=ptr[3];
               a.Apply1QubitGate(qubit, m);
             }, "Apply custom 1-qubit gate.")
        .def("ApplyControlled1QubitGate",
             [](QubitRegister<ComplexDP> &a, unsigned control, unsigned qubit,
                py::array_t<ComplexDP, py::array::c_style | py::array::forcecast> matrix ) {
               py::buffer_info buf = matrix.request();
               if (buf.ndim != 2)
                   throw std::runtime_error("Number of dimensions must be two.");
               if (buf.shape[0] != 2 || buf.shape[1] != 2)
                   throw std::runtime_error("Input shape is not 2x2.");
               // Create and initialize the custom tiny-matrix used by Intel QS.
               ComplexDP *ptr = (ComplexDP *) buf.ptr;
               TM2x2<ComplexDP> m;
               m(0,0)=ptr[0];
               m(0,1)=ptr[1];
               m(1,0)=ptr[2];
               m(1,1)=ptr[3];
               a.ApplyControlled1QubitGate(control, qubit, m);
             }, "Apply custom controlled-1-qubit gate.")
        // Three-qubit gates:
        .def("ApplyToffoli", &QubitRegister<ComplexDP>::ApplyToffoli)
        // State initialization:
        .def("Initialize",
               (void (QubitRegister<ComplexDP>::*)(std::string, std::size_t ))
                 &QubitRegister<ComplexDP>::Initialize)
        // State measurement and collapse:
        .def("GetProbability", &QubitRegister<ComplexDP>::GetProbability)
        .def("CollapseQubit", &QubitRegister<ComplexDP>::CollapseQubit)
          // Recall that the collapse selects: 'false'=|0> , 'true'=|1>
        .def("Normalize", &QubitRegister<ComplexDP>::Normalize)
        .def("ExpectationValue", &QubitRegister<ComplexDP>::ExpectationValue)
        // Other quantum operations:
        .def("ComputeNorm", &QubitRegister<ComplexDP>::ComputeNorm)
        .def("ComputeOverlap", &QubitRegister<ComplexDP>::ComputeOverlap)
        // Utility functions:
        .def("Print",
             [](QubitRegister<ComplexDP> &a, std::string description) {
               py::scoped_ostream_redirect stream(
               std::cout,                               // std::ostream&
               py::module::import("sys").attr("stdout") // Python output
               );
               std::vector<size_t> qubits = {};
               std::cout << "<<the output has been redirected to the terminal>>\n";
               a.Print(description, qubits);
             }, "Print the quantum state with an initial description.");

}

//////////////////////////////////////////////////////////////////////////////
