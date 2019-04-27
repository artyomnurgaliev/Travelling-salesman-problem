#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include "tsp_mst.h"
const int INF = 10000;
const double PI = 3.141592653589793238462643383279;
const int M = 10;


void experiment(int N, int M){
    std::uniform_real_distribution<double> uniDist(0, 2*PI);
    std::normal_distribution<double> normDist;
    double sum = 0;
    std::vector<double> results;
    for (int k = 0; k<M; ++k){
        std::vector<std::pair<double ,double>> nodes;

        std::random_device rd{};
        std::mt19937 generator{rd()};
        //std::default_random_engine generator((unsigned)std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

        for (int i=0; i<N; ++i) {
            double r = normDist(generator);
            double phi = uniDist(generator);
            double x = r * (cos(phi));
            double y = r * (sin(phi));
            nodes.emplace_back(x, y);
        }
        Graph graph = Graph(nodes);
        std::vector<int> vec;

        double min = INF;
        graph.find_tsp_with_bust(vec, min, 0);
        double real_res = min;

        double near_res = graph.find_tsp_with_mst();
        //double near_res = graph.find_tsp_with_busting_couples_and_eulerian_cycle();
        double approximation_quality = near_res/real_res;
        sum += approximation_quality;
        results.push_back(approximation_quality);
    }
    double avg = sum/M;
    //std::cout.width(4);

    //std::cout<<N<<"      |";
    sum = 0;
    for (int k = 0; k<M; ++k){
        sum += (results[k] - avg)*(results[k] - avg);
    }
    //std::cout<<" "<<avg<<"   |";
    double standard_deviation_of_approximation_quality = sqrt(sum/M);
    //std::cout<<"  "<<standard_deviation_of_approximation_quality<<"\n";

}

int main() {
    //std::cout<<"   N      |   avg     |  st_dev  "<<std::endl;
    //std::cout<<"----------+-----------+----------"<<std::endl;
    for (int i=2; i<11; ++i){
        experiment(i, M);
    }
    return 0;
}