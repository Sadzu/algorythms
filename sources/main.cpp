#include "../headers/Graph.h"
#include "../headers/ShortestPathUnweighted.h"

int main() {
    // Создаем ориентированный граф
    Graph<std::string, int> g(5, Graph<std::string, int>::GraphType::DIRECTED,
                             Graph<std::string, int>::RepresentationType::LIST);

    // Добавляем ребра
    g.InsertE(0, 1);
    g.InsertE(1, 2);
    g.InsertE(2, 3);
    g.InsertE(3, 4);
    g.InsertE(0, 4);
    g.InsertE(1, 3);

    // Решаем задачу
    ShortestPathsUnweighted<std::string> solver(g);

    // Получаем результаты
    auto results = solver.Result();

    // Выводим расстояния
    for (const auto& [from, dists] : results) {
        for (const auto& [to, dist] : dists) {
            if (dist != -1) {
                std::cout << "Distance from " << from << " to " << to
                          << " is " << dist << std::endl;
            }
        }
    }

    // Пример получения конкретного расстояния
    std::cout << "Distance from 0 to 4: " << solver.GetDistance(0, 4) << std::endl;
    std::cout << "Distance from 1 to 4: " << solver.GetDistance(1, 4) << std::endl;

    return 0;
}
