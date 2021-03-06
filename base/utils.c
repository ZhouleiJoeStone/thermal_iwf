//Copyright ETH Zurich, IWF

//This file is part of iwf_thermal.

//iwf_thermal is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//iwf_thermal is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with iwf_thermal.  If not, see <http://www.gnu.org/licenses/>.

#include "utils.h"

void inv3x3(double A[9], double Ainv[9]) {
	double detA = A[0]*A[4]*A[8] + A[3]*A[7]*A[2] + A[6]*A[1]*A[5] - A[6]*A[4]*A[2] - A[7]*A[5]*A[0] - A[8]*A[3]*A[1];

	assert(detA != 0.f);

	Ainv[0] = (A[4]*A[8] - A[5]*A[7])/detA;
	Ainv[1] = (A[2]*A[7] - A[1]*A[8])/detA;
	Ainv[2] = (A[1]*A[5] - A[2]*A[4])/detA;
	Ainv[3] = (A[5]*A[6] - A[3]*A[8])/detA;
	Ainv[4] = (A[0]*A[8] - A[2]*A[6])/detA;
	Ainv[5] = (A[2]*A[3] - A[0]*A[5])/detA;
	Ainv[6] = (A[3]*A[7] - A[4]*A[6])/detA;
	Ainv[7] = (A[1]*A[6] - A[0]*A[7])/detA;
	Ainv[8] = (A[0]*A[4] - A[1]*A[3])/detA;
}

static double det4x4(double M[4*4]) {
	return M[0]*(M[5]*M[10]*M[15] + M[7]*M[9]*M[14] + M[13]*M[6]*M[11] - M[7]*M[10]*M[13] - M[5]*M[11]*M[14] - M[15]*M[6]*M[9])
		 - M[1]*(M[4]*M[10]*M[15] + M[7]*M[8]*M[14] + M[12]*M[6]*M[11] - M[7]*M[10]*M[12] - M[4]*M[11]*M[14] - M[15]*M[6]*M[8])
		 + M[2]*(M[4]*M[9] *M[15] + M[7]*M[8]*M[13] + M[12]*M[5]*M[11] - M[7]*M[9] *M[12] - M[4]*M[11]*M[13] - M[15]*M[5]*M[8])
		 - M[3]*(M[4]*M[9] *M[14] + M[6]*M[8]*M[13] + M[12]*M[5]*M[10] - M[6]*M[9] *M[12] - M[4]*M[10]*M[13] - M[14]*M[5]*M[8]);
}

static double det5x5(double M[5*5]) {
	double minor11[4*4] = {M[6 ],M[7 ],M[8 ],M[9 ],
						   M[11],M[12],M[13],M[14],
						   M[16],M[17],M[18],M[19],
						   M[21],M[22],M[23],M[24]};
	double minor12[4*4] = {M[5 ],M[7 ],M[8 ],M[9 ],
						   M[10],M[12],M[13],M[14],
						   M[15],M[17],M[18],M[19],
						   M[20],M[22],M[23],M[24]};
	double minor13[4*4] = {M[5 ],M[6 ],M[8 ],M[9 ],
						   M[10],M[11],M[13],M[14],
						   M[15],M[16],M[18],M[19],
						   M[20],M[21],M[23],M[24]};
	double minor14[4*4] = {M[5 ],M[6 ],M[7 ],M[9 ],
						   M[10],M[11],M[12],M[14],
						   M[15],M[16],M[17],M[19],
						   M[20],M[21],M[22],M[24]};
	double minor15[4*4] = {M[5 ],M[6 ],M[7 ],M[8 ],
						   M[10],M[11],M[12],M[13],
						   M[15],M[16],M[17],M[18],
						   M[20],M[21],M[22],M[23]};

	return M[0]*(det4x4(minor11))
		  -M[1]*(det4x4(minor12))
		  +M[2]*(det4x4(minor13))
		  -M[3]*(det4x4(minor14))
		  +M[4]*(det4x4(minor15));
}

static double det6x6(double M[6*6]) {
	double minor11[5*5] = {M[7 ],M[8 ],M[9 ],M[10],M[11],
						   M[13],M[14],M[15],M[16],M[17],
					       M[19],M[20],M[21],M[22],M[23],
						   M[25],M[26],M[27],M[28],M[29],
						   M[31],M[32],M[33],M[34],M[35]};
	double minor12[5*5] = {M[6 ],M[8 ],M[9 ],M[10],M[11],
						   M[12],M[14],M[15],M[16],M[17],
						   M[18],M[20],M[21],M[22],M[23],
						   M[24],M[26],M[27],M[28],M[29],
						   M[30],M[32],M[33],M[34],M[35]};
	double minor13[5*5] = {M[6 ],M[7 ],M[9 ],M[10],M[11],
						   M[12],M[13],M[15],M[16],M[17],
						   M[18],M[19],M[21],M[22],M[23],
						   M[24],M[25],M[27],M[28],M[29],
						   M[30],M[31],M[33],M[34],M[35]};
	double minor14[5*5] = {M[6 ],M[7 ],M[8 ],M[10],M[11],
						   M[12],M[13],M[14],M[16],M[17],
						   M[18],M[19],M[20],M[22],M[23],
						   M[24],M[25],M[26],M[28],M[29],
						   M[30],M[31],M[32],M[34],M[35]};
	double minor15[5*5] = {M[6 ],M[7 ],M[8 ],M[9 ],M[11],
						   M[12],M[13],M[14],M[15],M[17],
						   M[18],M[19],M[20],M[21],M[23],
						   M[24],M[25],M[26],M[27],M[29],
						   M[30],M[31],M[32],M[33],M[35]};
	double minor16[5*5] = {M[6 ],M[7 ],M[8 ],M[9 ],M[10],
						   M[12],M[13],M[14],M[15],M[16],
						   M[18],M[19],M[20],M[21],M[22],
						   M[24],M[25],M[26],M[27],M[28],
						   M[30],M[31],M[32],M[33],M[34]};

	return M[0]*(det5x5(minor11))
		  -M[1]*(det5x5(minor12))
		  +M[2]*(det5x5(minor13))
		  -M[3]*(det5x5(minor14))
		  +M[4]*(det5x5(minor15))
	      -M[5]*(det5x5(minor16));
}

void solve3x3(double M[3*3], double P[3], double C[3]) {
	double delta   =  M[0]*M[4]*M[8] + M[1]*M[5]*M[6] + M[2]*M[3]*M[7]
	                                                                - M[6]*M[4]*M[2] - M[7]*M[5]*M[0] - M[8]*M[3]*M[1];

	double delta_x =  P[0]*M[4]*M[8] + M[1]*M[5]*P[2] + M[2]*P[1]*M[7]
	                                                                - P[2]*M[4]*M[2] - M[7]*M[5]*P[0] - M[8]*P[1]*M[1];

	double delta_y =  M[0]*P[1]*M[8] + P[0]*M[5]*M[6] + M[2]*M[3]*P[2]
	                                                                - M[6]*P[1]*M[2] - P[2]*M[5]*M[0] - M[8]*M[3]*P[0];

	double delta_z =  M[0]*M[4]*P[2] + M[1]*P[1]*M[6] + P[0]*M[3]*M[7]
	                                                                - M[6]*M[4]*P[0] - M[7]*P[1]*M[0] - P[2]*M[3]*M[1];

	assert(delta != 0.f);

	C[0] = delta_x/delta;
	C[1] = delta_y/delta;
	C[2] = delta_z/delta;
}

void solve4x4(double M[4*4], double P[4], double C[4]) {
	double delta = + M[0]*(M[5]*M[10]*M[15] + M[7]*M[9]*M[14] + M[13]*M[6]*M[11] - M[7]*M[10]*M[13] - M[5]*M[11]*M[14] - M[15]*M[6]*M[9])
				   - M[1]*(M[4]*M[10]*M[15] + M[7]*M[8]*M[14] + M[12]*M[6]*M[11] - M[7]*M[10]*M[12] - M[4]*M[11]*M[14] - M[15]*M[6]*M[8])
				   + M[2]*(M[4]*M[9] *M[15] + M[7]*M[8]*M[13] + M[12]*M[5]*M[11] - M[7]*M[9] *M[12] - M[4]*M[11]*M[13] - M[15]*M[5]*M[8])
				   - M[3]*(M[4]*M[9] *M[14] + M[6]*M[8]*M[13] + M[12]*M[5]*M[10] - M[6]*M[9] *M[12] - M[4]*M[10]*M[13] - M[14]*M[5]*M[8]);

	double delta_x1 =  + P[0]*(M[5]*M[10]*M[15] + M[7]*M[9]*M[14] + M[13]*M[6]*M[11] - M[7]*M[10]*M[13] - M[5]*M[11]*M[14] - M[15]*M[6]*M[9])
															   - M[1]*(P[1]*M[10]*M[15] + M[7]*P[2]*M[14] + P[3] *M[6]*M[11] - M[7]*M[10]*P[3 ] - P[1]*M[11]*M[14] - M[15]*M[6]*P[2])
															   + M[2]*(P[1]*M[9] *M[15] + M[7]*P[2]*M[13] + P[3] *M[5]*M[11] - M[7]*M[9] *P[3 ] - P[1]*M[11]*M[13] - M[15]*M[5]*P[2])
															   - M[3]*(P[1]*M[9] *M[14] + M[6]*P[2]*M[13] + P[3] *M[5]*M[10] - M[6]*M[9] *P[3 ] - P[1]*M[10]*M[13] - M[14]*M[5]*P[2]);

	double delta_x2 = + M[0]*(P[1]*M[10]*M[15] + M[7]*P[2]*M[14] + P[3] *M[6]*M[11] - M[7]*M[10]*P[3 ] - P[1]*M[11]*M[14] - M[15]*M[6]*P[2])
															  - P[0]*(M[4]*M[10]*M[15] + M[7]*M[8]*M[14] + M[12]*M[6]*M[11] - M[7]*M[10]*M[12] - M[4]*M[11]*M[14] - M[15]*M[6]*M[8])
															  + M[2]*(M[4]*P[2] *M[15] + M[7]*M[8]*P[3 ] + M[12]*P[1]*M[11] - M[7]*P[2] *M[12] - M[4]*M[11]*P[3 ] - M[15]*P[1]*M[8])
															  - M[3]*(M[4]*P[2] *M[14] + M[6]*M[8]*P[3 ] + M[12]*P[1]*M[10] - M[6]*P[2] *M[12] - M[4]*M[10]*P[3 ] - M[14]*P[1]*M[8]);

	double delta_x3 = + M[0]*(M[5]*P[2]*M[15] + M[7]*M[9]*P[3 ] + M[13]*P[1]*M[11] - M[7]*P[2] *M[13] - M[5]*M[11]*P[3 ] - M[15]*P[1]*M[9])
															  - M[1]*(M[4]*P[2]*M[15] + M[7]*M[8]*P[3 ] + M[12]*P[1]*M[11] - M[7]*P[2] *M[12] - M[4]*M[11]*P[3 ] - M[15]*P[1]*M[8])
															  + P[0]*(M[4]*M[9]*M[15] + M[7]*M[8]*M[13] + M[12]*M[5]*M[11] - M[7]*M[9] *M[12] - M[4]*M[11]*M[13] - M[15]*M[5]*M[8])
															  - M[3]*(M[4]*M[9] *P[3] + P[1]*M[8]*M[13] + M[12]*M[5]*P[2 ] - P[1]*M[9] *M[12] - M[4]*P[2] *M[13] - P[3] *M[5]*M[8]);

	double delta_x4 = + M[0]*(M[5]*M[10]*P[3 ] + P[1]*M[9]*M[14] + M[13]*M[6]*P[2 ] - P[1]*M[10]*M[13] - M[5]*P[2] *M[14] - P[3] *M[6]*M[9])
															  - M[1]*(M[4]*M[10]*P[3 ] + P[1]*M[8]*M[14] + M[12]*M[6]*P[2 ] - P[1]*M[10]*M[12] - M[4]*P[2] *M[14] - P[3] *M[6]*M[8])
															  + M[2]*(M[4]*M[9] *P[3 ] + P[1]*M[8]*M[13] + M[12]*M[5]*P[2 ] - P[1]*M[9] *M[12] - M[4]*P[2] *M[13] - P[3] *M[5]*M[8])
															  - P[0]*(M[4]*M[9] *M[14] + M[6]*M[8]*M[13] + M[12]*M[5]*M[10] - M[6]*M[9] *M[12] - M[4]*M[10]*M[13] - M[14]*M[5]*M[8]);

	assert(delta != 0.f);

	C[0] = delta_x1/delta;
	C[1] = delta_x2/delta;
	C[2] = delta_x3/delta;
	C[3] = delta_x4/delta;

}

void solve6x6(double M[6*6], double P[6], double C[6]) {
	// detM
    double detM = det6x6(M);
	// minorM1
    double minorM1[6*6] = {P[0 ], M[1 ], M[2 ], M[3 ], M[4 ], M[5 ],
						   P[1 ], M[7 ], M[8 ], M[9 ], M[10], M[11],
						   P[2 ], M[13], M[14], M[15], M[16], M[17],
						   P[3 ], M[19], M[20], M[21], M[22], M[23],
						   P[4 ], M[25], M[26], M[27], M[28], M[29],
						   P[5 ], M[31], M[32], M[33], M[34], M[35]};
	// minorM2
    double minorM2[6*6] = {M[0 ], P[0 ], M[2 ], M[3 ], M[4 ], M[5 ],
						   M[6 ], P[1 ], M[8 ], M[9 ], M[10], M[11],
						   M[12], P[2 ], M[14], M[15], M[16], M[17],
						   M[18], P[3 ], M[20], M[21], M[22], M[23],
						   M[24], P[4 ], M[26], M[27], M[28], M[29],
						   M[30], P[5 ], M[32], M[33], M[34], M[35]};
	// minorM3
    double minorM3[6*6] = {M[0 ], M[1 ], P[0 ], M[3 ], M[4 ], M[5 ],
						   M[6 ], M[7 ], P[1 ], M[9 ], M[10], M[11],
						   M[12], M[13], P[2 ], M[15], M[16], M[17],
						   M[18], M[19], P[3 ], M[21], M[22], M[23],
						   M[24], M[25], P[4 ], M[27], M[28], M[29],
						   M[30], M[31], P[5 ], M[33], M[34], M[35]};
	// minorM4
    double minorM4[6*6] = {M[0 ], M[1 ], M[2 ], P[0 ], M[4 ], M[5 ],
						   M[6 ], M[7 ], M[8 ], P[1 ], M[10], M[11],
						   M[12], M[13], M[14], P[2 ], M[16], M[17],
						   M[18], M[19], M[20], P[3 ], M[22], M[23],
						   M[24], M[25], M[26], P[4 ], M[28], M[29],
						   M[30], M[31], M[32], P[5 ], M[34], M[35]};
	// minorM5
    double minorM5[6*6] = {M[0 ], M[1 ], M[2 ], M[3 ], P[0 ], M[5 ],
						   M[6 ], M[7 ], M[8 ], M[9 ], P[1 ], M[11],
						   M[12], M[13], M[14], M[15], P[2 ], M[17],
						   M[18], M[19], M[20], M[21], P[3 ], M[23],
						   M[24], M[25], M[26], M[27], P[4 ], M[29],
						   M[30], M[31], M[32], M[33], P[5 ], M[35]};
	// minorM6
    double minorM6[6*6] = {M[0 ], M[1 ], M[2 ], M[3 ], M[4 ], P[0 ],
						   M[6 ], M[7 ], M[8 ], M[9 ], M[10], P[1 ],
						   M[12], M[13], M[14], M[15], M[16], P[2 ],
						   M[18], M[19], M[20], M[21], M[22], P[3 ],
						   M[24], M[25], M[26], M[27], M[28], P[4 ],
						   M[30], M[31], M[32], M[33], M[34], P[5 ]};


	assert(detM != 0.f);

	C[0] = det6x6(minorM1)/detM;
	C[1] = det6x6(minorM2)/detM;
	C[2] = det6x6(minorM3)/detM;
	C[3] = det6x6(minorM4)/detM;
	C[4] = det6x6(minorM5)/detM;
	C[5] = det6x6(minorM6)/detM;

}
