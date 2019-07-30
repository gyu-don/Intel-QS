import intelqs as simulator
import numpy as np
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
#- Custom gates ---------------------------------
#------------------------------------------------

print("Define and apply a custom Y gate to qubit 0.");
Y = np.zeros((2,2),dtype=np.complex_);
Y[0,1] = -1j;
Y[1,0] =  1j;
qreg.Apply1QubitGate(0,Y);

print("Get probabilities :\n q(0) has prob. {} to be in |1>\n q(1) has prob. {} to be in |1>".format(qreg.GetProbability(0),qreg.GetProbability(1)));
qreg.Print("State should be i|10>");

print("qreg[0] = {}  ,   qreg[1] = {}".format(qreg[0],qreg[1]));

#------------------------------------------------
#------------------------------------------------
#------------------------------------------------

qreg2 = simulator.QubitRegister(num_qubits, "base", 0, 0);
qreg2.Initialize("base",1);
qreg2.ApplyHadamard(0);
qreg2.Print("State should be |-0>");

print("Overlap between i|10> and |-0> = {}".format(qreg.ComputeOverlap(qreg2)));

#------------------------------------------------
