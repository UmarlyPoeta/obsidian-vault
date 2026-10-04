#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Node { double x, y; bool bc = false; };
struct Element { int id[4]; };                  // numery węzłów od 1, jak w pliku

struct GlobalData {
    double simulationTime, simulationStepTime, conductivity, alfa,
           tot, initialTemp, density, specificHeat;
    int nN, nE;
};

struct Grid {
    int nN, nE;
    std::vector<Node> nodes;
    std::vector<Element> elements;
};

static std::string zamienPrzecinki(std::string s) {
    for (char& c : s) if (c == ',') c = ' ';
    return s;
}

bool wczytaj(const std::string& sciezka, GlobalData& gd, Grid& grid) {
    std::ifstream f(sciezka);
    if (!f) return false;
    std::string linia, klucz;
    while (std::getline(f, linia)) {
        if (linia.empty()) continue;
        std::istringstream ss(linia);
        ss >> klucz;
        if      (klucz == "SimulationTime")     ss >> gd.simulationTime;
        else if (klucz == "SimulationStepTime") ss >> gd.simulationStepTime;
        else if (klucz == "Conductivity")       ss >> gd.conductivity;
        else if (klucz == "Alfa")               ss >> gd.alfa;
        else if (klucz == "Tot")                ss >> gd.tot;
        else if (klucz == "InitialTemp")        ss >> gd.initialTemp;
        else if (klucz == "Density")            ss >> gd.density;
        else if (klucz == "SpecificHeat")       ss >> gd.specificHeat;
        else if (klucz == "Nodes")   { std::string n; ss >> n >> gd.nN; }   // "Nodes number 16"
        else if (klucz == "Elements"){ std::string n; ss >> n >> gd.nE; }
        else if (klucz == "Nodes_number")       ss >> gd.nN;                // wariant z podkreślnikiem
        else if (klucz == "Elements_number")    ss >> gd.nE;
        else if (klucz == "*Node") {
            grid.nodes.resize(gd.nN);
            for (int i = 0; i < gd.nN && std::getline(f, linia); ++i) {
                std::istringstream w(zamienPrzecinki(linia));
                int id; w >> id >> grid.nodes[i].x >> grid.nodes[i].y;
            }
        } else if (klucz.rfind("*Element", 0) == 0) {
            grid.elements.resize(gd.nE);
            for (int i = 0; i < gd.nE && std::getline(f, linia); ++i) {
                std::istringstream w(zamienPrzecinki(linia));
                int id; w >> id;
                for (int& n : grid.elements[i].id) w >> n;
            }
        } else if (klucz == "*BC") {
            std::getline(f, linia);
            std::istringstream w(zamienPrzecinki(linia));
            for (int n; w >> n;) grid.nodes[n - 1].bc = true;
        }
    }
    grid.nN = gd.nN; grid.nE = gd.nE;
    return true;
}

int main(int argc, char** argv) {
    GlobalData gd{}; Grid grid{};
    if (!wczytaj(argc > 1 ? argv[1] : "Test1_4_4.txt", gd, grid)) { std::cerr << "Brak pliku\n"; return 1; }
    std::cout << "nN = " << grid.nN << ", nE = " << grid.nE << ", k = " << gd.conductivity
              << ", alfa = " << gd.alfa << ", tot = " << gd.tot << "\n";
    for (int i = 0; i < grid.nN; ++i)
        std::cout << "wezel " << i + 1 << ": x = " << grid.nodes[i].x << "  y = " << grid.nodes[i].y
                  << "  BC = " << grid.nodes[i].bc << "\n";
    for (int i = 0; i < grid.nE; ++i) {
        const int* id = grid.elements[i].id;
        std::cout << "element " << i + 1 << ": ID[" << id[0] << "," << id[1] << "," << id[2] << "," << id[3] << "]\n";
    }
}
