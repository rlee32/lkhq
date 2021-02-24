#include "NanoTimer.h"
#include "config.hh"
#include "fileio.hh"
#include "hill_climb.hh"
#include "point_quadtree/point_quadtree.h"
#include "tour.hh"

#include <filesystem>
#include <iostream>
#include <optional>
#include <string>

int main(int argc, const char** argv)
{
    if (argc == 1) {
        std::cout << "Arguments: config_file_path" << std::endl;
    }
    // Read config file.
    const std::string config_path = (argc == 1) ? "config.txt" : argv[1];
    std::cout << "Reading config file: " << config_path << std::endl;
    Config config(config_path);

    // Read input files.
    const std::optional<std::string> tsp_file_path_string = config.get("tsp_file_path");
    if (not tsp_file_path_string) {
        std::cout << "tsp_file_path not specified.\n";
        return EXIT_FAILURE;
    }
    const std::optional<std::filesystem::path> tsp_file_path(*tsp_file_path_string);
    const auto [x, y] = fileio::read_coordinates(*tsp_file_path_string);
    const auto initial_tour = fileio::initial_tour(x.size(), config.get("tour_file_path"));

    // Initial tour length calculation.
    point_quadtree::Domain domain(x, y);
    std::cout << "domain aspect ratio: " << domain.xdim(0) / domain.ydim(0) << std::endl;
    std::cout << "bounding x, y dim: "
        << domain.xdim(0) << ", " << domain.ydim(0)
        << std::endl;

    Tour tour(&domain, initial_tour);
    const auto initial_tour_length = tour.length();
    std::cout << "Initial tour length: " << initial_tour_length << std::endl;

    // Quad tree.
    NanoTimer timer;
    timer.start();

    std::cout << "\nquadtree stats:\n";
    const auto root {point_quadtree::make_quadtree(x, y, domain)};
    std::cout << "node ratio: "
        << static_cast<double>(point_quadtree::count_nodes(root))
            / point_quadtree::count_points(root)
        << std::endl;
    std::cout << "Finished quadtree in " << timer.stop() / 1e9 << " seconds.\n\n";

    auto best_length = initial_tour_length;

    const auto &save_prefix = tsp_file_path->stem().string();
    const auto &save_dir_string = config.get("save_dir");
    std::optional<std::filesystem::path> save_dir;
    if (save_dir_string) {
        save_dir = std::filesystem::path(*save_dir_string);
        if (not std::filesystem::exists(*save_dir)) {
            std::filesystem::create_directory(*save_dir);
        }
    }
    auto write_if_better = [&](primitives::length_t new_length)
    {
        if (new_length < best_length)
        {
            if (save_dir) {
                const auto &save_path = *save_dir / (save_prefix + '_' + std::to_string(new_length) + ".tour");
                fileio::write_ordered_points(tour.order(), save_path);
                std::cout << "saved tour to " << save_path << std::endl;
            }
            best_length = new_length;
        }
    };

    PointSet point_set(root, x, y);

    // hill climb from initial tour.
    HillClimber hill_climber(point_set);
    const auto &kmax = config.get<size_t>("kmax", 3);
    std::cout << "kmax: " << kmax << std::endl;
    auto new_length = hill_climb::hill_climb(hill_climber, tour, kmax);
    if (new_length < best_length) {
        best_length = new_length;
        std::cout << "new improved length: " << new_length << std::endl;
    }
    write_if_better(new_length);

    return EXIT_SUCCESS;
}
