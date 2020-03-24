/******************************************************************************
Copyright 2019 The Regents of the University of California.
All Rights Reserved.

Permission to copy, modify and distribute any part of this Software for
educational, research and non-profit purposes, without fee, and without
a written agreement is hereby granted, provided that the above copyright
notice, this paragraph and the following three paragraphs appear in all
copies.

Those desiring to incorporate this Software into commercial products or
use for commercial purposes should contact the:
Office of Innovation & Commercialization
University of California, San Diego
9500 Gilman Drive, Mail Code 0910
La Jolla, CA 92093-0910
Ph: (858) 534-5815
FAX: (858) 534-7345
E-MAIL: invent@ucsd.edu

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE, EVEN IF THE UNIVERSITY
OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS. THE UNIVERSITY OF CALIFORNIA MAKES NO
REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER IMPLIED OR
EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE
SOFTWARE WILL NOT INFRINGE ANY PATENT, TRADEMARK OR OTHER RIGHTS.
******************************************************************************/

#ifndef ENERGYFF_H
#define ENERGYFF_H

#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
// #include <unordered_map>
#include "potential/force_field/connectivity.h"
#include "potential/force_field/angles.h"
#include "potential/force_field/bond.h"
// #include "buckingham.h"
#include "potential/force_field/calculators.h"
// #include "definitions.h"
#include "potential/force_field/dihedral.h"
// #include "dispersion.h"
// #include "electrostatics.h"
#include "potential/force_field/inversion.h"
// #include "reader_func.h"
// #include "xyz_frame.h"
#include "tools/definitions.h"

/**
 * @file energyff.h
 * @brief Contains the calls to the classic energy with and without gradients
 */

/**
 * @namespace eff
 * @brief Defines the namespace for the classical potential
 */
namespace eff {

/**
 * @brief Gets the classical force field potential energy for a given set of
 * monomers of the same monomer type.
 *
 * Given the monomer id and the number of monomers, will return the total sum
 * of the classical potential energy of those monomers.
 * @param[in] connectivity Connectivity for the given monomer id
 * @param[in] nm number of monomers of monomer type "mon"
 * @param[in] xyz coordinates of the monomer
 * @param[in,out] good Boolean that will be set to false if any of the monomers
 * @param[in] nat Number of atoms in one monomer
 * has an energy larger than the value set in definitions.h (EMAX1B)
 * (TO_BE_FIXED )
 * @return Sum of the classical potential energies of all the monomers passed as arguments
 */
double get_ff_energy(eff::Conn &connectivity, size_t nm, std::vector<double> xyz1, bool &good, int nat);

/**
 * @brief Gets the classical force field potential energy for a given set of monomers of
 * the same monomer type, calculating the gradients.
 *
 * Given the monomer id and the number of monomers, will return the total sum
 * of the classical potential energy of those monomers and their corresponding gradients.
 * @param[in] connectivity Connectivity for the given monomer id
 * @param[in] nm number of monomers of monomer type "mon"
 * @param[in] xyz coordinates of the monomer
 * @param[in,out] grad Gradients of the one-body energy
 * @param[in,out] good Boolean that will be set to false if any of the monomers
 * @param[in] nat Number of atoms in one monomer
 * has an energy larger than the value set in definitions.h (EMAX1B)
 * @return Sum of the classical potential energies of all the monomers passed as arguments
 */
double get_ff_energy(eff::Conn &connectivity, size_t nm, std::vector<double> xyz1, std::vector<double> &grad1,
                     bool &good, int nat, std::vector<double> *virial = 0);
}  // namespace eff
#endif
