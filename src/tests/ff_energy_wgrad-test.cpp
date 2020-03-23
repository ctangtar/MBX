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

#include <cmath>
#include <cassert>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>

#include "io_tools/read_nrg.h"
#include "io_tools/write_nrg.h"
#include "io_tools/read_connectivity.h"
#include "io_tools/write_connectivity.h"

#include "bblock/system.h"

// #define PRINT_VIRIAL

namespace {

static std::vector<bblock::System> systems;

}  // namespace

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "usage: energy h2o_ion.nrg connectivity.txt" << std::endl;
        return 0;
    }

    try {
        std::ifstream ifs(argv[1]);

        if (!ifs) {
            throw std::runtime_error("could not open the NRG file");
        }

        tools::ReadNrg(argv[1], systems);
    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return 1;
    }

    try {
        std::ifstream ifs(argv[2]);

        if (!ifs) {
            throw std::runtime_error("could not open connectivity file");
        }
        tools::ReadConnectivity(argv[2], systems);
    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return 1;
    }

    // std::cout << "Energies without gradients:" << std::endl;
    // std::vector<double> g;
    // for (size_t i = 0; i < systems.size(); i++) {
    //     double energy = systems[i].Energy(false);
    //     std::cout << std::setprecision(10) << std::scientific << "system[" << std::setfill('.') << std::setw(5) << i
    //               << "]= " << std::setfill(' ') << std::setw(20) << std::right << energy << std::setw(12) <<
    //               std::right
    //               << " kcal/mol" << std::endl;
    // }

    for (size_t i = 0; i < systems.size(); i++) {
        double energy = systems[i].Energy(true);

        std::vector<double> grd = systems[i].GetGrads();
        std::vector<double> real_grd = systems[i].GetRealGrads();

        std::cout << std::setprecision(5) << std::scientific << "system[" << std::setfill('.') << std::setw(5) << i
                  << "]= " << std::setfill(' ') << std::setw(20) << std::right << energy << std::setw(12) << std::right
                  << " kcal/mol" << std::endl
                  << std::endl;

#ifdef PRINT_VIRIAL
        std::vector<double> virial = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        double E1b, E2b, E3b, Eff, Eelec, Edisp, Ebuck, Etotal;
        E1b = systems[0].OneBodyEnergy(true);
        std::cout << " 1B Energy [kcal/mol] " << std::setprecision(15) << E1b << std::endl;
        virial = systems[0].GetVirial();
        std::cout << " 1B Virial [kcal/mol] " << virial[0] << " , " << virial[1] << " , " << virial[2] << " , "
                  << virial[3] << " , " << virial[4] << " , " << virial[5] << " , " << virial[6] << " , " << virial[7]
                  << " , " << virial[8] << std::endl;
        E2b = systems[0].TwoBodyEnergy(true);
        std::cout << " 2B Energy [kcal/mol] " << std::setprecision(15) << E2b << std::endl;
        virial = systems[0].GetVirial();
        std::cout << " 2B Virial [kcal/mol] " << virial[0] << " , " << virial[1] << " , " << virial[2] << " , "
                  << virial[3] << " , " << virial[4] << " , " << virial[5] << " , " << virial[6] << " , " << virial[7]
                  << " , " << virial[8] << std::endl;
        E3b = systems[0].ThreeBodyEnergy(true);
        std::cout << " 3B Energy [kcal/mol] " << std::setprecision(15) << E3b << std::endl;
        virial = systems[0].GetVirial();
        std::cout << " 3B Virial [kcal/mol] " << virial[0] << " , " << virial[1] << " , " << virial[2] << " , "
                  << virial[3] << " , " << virial[4] << " , " << virial[5] << " , " << virial[6] << " , " << virial[7]
                  << " , " << virial[8] << std::endl;
        Eff = systems[0].ClassicPotential(true);
        std::cout << " FF Energy [kcal/mol] " << std::setprecision(15) << Eff << std::endl;
        virial = systems[0].GetVirial();
        std::cout << " FF Virial [kcal/mol] " << virial[0] << " , " << virial[1] << " , " << virial[2] << " , "
                  << virial[3] << " , " << virial[4] << " , " << virial[5] << " , " << virial[6] << " , " << virial[7]
                  << " , " << virial[8] << std::endl;
        Eelec = systems[0].Electrostatics(true);
        std::cout << " Elec Energy [kcal/mol] " << std::setprecision(15) << Eelec << std::endl;
        virial = systems[0].GetVirial();
        std::cout << " Elec Virial [kcal/mol] " << virial[0] << " , " << virial[1] << " , " << virial[2] << " , "
                  << virial[3] << " , " << virial[4] << " , " << virial[5] << " , " << virial[6] << " , " << virial[7]
                  << " , " << virial[8] << std::endl;
        Edisp = systems[0].Dispersion(true);
        std::cout << " Disp Energy [kcal/mol] " << std::setprecision(15) << Edisp << std::endl;
        virial = systems[0].GetVirial();
        std::cout << " Disp Virial [kcal/mol] " << virial[0] << " , " << virial[1] << " , " << virial[2] << " , "
                  << virial[3] << " , " << virial[4] << " , " << virial[5] << " , " << virial[6] << " , " << virial[7]
                  << " , " << virial[8] << std::endl;
        Ebuck = systems[0].Buckingham(true);
        std::cout << " Buck Energy [kcal/mol] " << std::setprecision(15) << Ebuck << std::endl;
        virial = systems[0].GetVirial();
        std::cout << " Buck Virial [kcal/mol] " << virial[0] << " , " << virial[1] << " , " << virial[2] << " , "
                  << virial[3] << " , " << virial[4] << " , " << virial[5] << " , " << virial[6] << " , " << virial[7]
                  << " , " << virial[8] << std::endl;
        Etotal = systems[0].Energy(true);
        std::cout << " Total Energy [kcal/mol] " << std::setprecision(15) << Etotal << std::endl;
        virial = systems[0].GetVirial();
        std::cout << " Total Virial [kcal/mol] " << virial[0] << " , " << virial[1] << " , " << virial[2] << " , "
                  << virial[3] << " , " << virial[4] << " , " << virial[5] << " , " << virial[6] << " , " << virial[7]
                  << " , " << virial[8] << std::endl;

#endif
#ifdef PRINT_GRADS

        std::vector<std::string> atn = systems[i].GetAtomNames();

        size_t n_sites = systems[i].GetNumSites();
        size_t n_atoms = systems[i].GetNumRealSites();

        std::cout << std::setw(6) << std::left << "Atom" << std::setw(20) << std::right << "GradientX" << std::setw(20)
                  << std::right << "GradientY" << std::setw(20) << std::right << "GradientZ" << std::endl;
        for (size_t j = 0; j < n_sites; j++) {
            if (atn[j] == "virt") continue;
            std::cout << std::setprecision(3) << std::scientific << std::setw(6) << std::left << atn[j] << std::setw(20)
                      << std::right << grd[3 * j] << std::setw(20) << std::right << grd[3 * j + 1] << std::setw(20)
                      << std::right << grd[3 * j + 2] << std::endl;
        }
#ifdef NUM_GRADS
        std::cout << std::endl
                  << std::setw(6) << std::left << "Atom" << std::setw(20) << std::right << "Analytical" << std::setw(20)
                  << std::right << "Numerical" << std::setw(20) << std::right << "Difference" << std::endl;
        // Comparing analytical and numerical
        std::vector<double> xyz;
        xyz = systems[i].GetRealXyz();
        std::vector<std::string> atms = systems[i].GetRealAtomNames();
        const double eps = 1.0e-6;
        for (size_t j = 0; j < n_atoms * 3; j++) {
            const double x_orig = xyz[j];
            xyz[j] = x_orig + eps;
            systems[i].SetRealXyz(xyz);
            const double Ep = systems[i].Energy(false);
            xyz[j] = x_orig + 2 * eps;
            systems[i].SetRealXyz(xyz);
            const double Epp = systems[i].Energy(false);
            xyz[j] = x_orig - eps;
            systems[i].SetRealXyz(xyz);
            const double Em = systems[i].Energy(false);
            xyz[j] = x_orig - 2 * eps;
            systems[i].SetRealXyz(xyz);
            const double Emm = systems[i].Energy(false);
            const double gfd = (8 * (Ep - Em) - (Epp - Emm)) / (12 * eps);
            xyz[j] = x_orig;
            systems[i].SetRealXyz(xyz);
            std::cout << std::setprecision(5) << std::scientific << std::setw(6) << std::left << atms[j / 3]
                      << std::setw(20) << std::right << real_grd[j] << std::setw(20) << std::right << gfd
                      << std::setw(20) << std::right << std::fabs(real_grd[j] - gfd) << std::endl;
        }
#endif  // NUM_GRADS
#endif  // PRINT_GRADS
    }
    return 0;
}
