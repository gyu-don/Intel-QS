import intelqs as simulator
import numpy
from numpy import random_intel

#------------------------------------------------
#- Quantum Simulation ---------------------------
#------------------------------------------------

print("Creation of the QuantumRegister object.");
num_qubits = 2;
qreg = simulator.QubitRegister(num_qubits, "base", 0, 0);

print("Initialize to |1>|0>.");
qreg.Initialize("base",1);

print("Apply X(0) to obtain state |0>|0>.");
qreg.ApplyPauliX(0);

print("Get probabilities :\n q(0) has prob. {} to be in |1>\n q(1) has prob. {} to be in |1>".format(qreg.GetProbability(0),qreg.GetProbability(1)));

print("Print state:");
qreg.Print("State should be |00>");

#------------------------------------------------
