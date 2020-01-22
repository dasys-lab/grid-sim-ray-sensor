#include "srgsim/world/ObjectDetection.h"
#include "srgsim/SRGEnums.h"
#include "srgsim/containers/Coordinate.h"
#include "srgsim/world/Cell.h"
#include "srgsim/world/ServiceRobot.h"
#include "srgsim/world/World.h"
#include "srgsim/containers/CellPerceptions.h"

#include <SystemConfig.h>
#include <cnc_geometry/Calculator.h>
#include <srgsim/containers/Function.h>

#include <math.h>

namespace srgsim {
    const float X_MIN_VALUE = 0.5F;
    const float X_MAX_VALUE = 121.5F;
    const float Y_MIN_VALUE = 0.5F;
    const float Y_MAX_VALUE = 38.5F;

    ObjectDetection::ObjectDetection(srgsim::ServiceRobot *robot)
            : Sensor(robot) {
        this->sc = essentials::SystemConfig::getInstance();
        this->sightLimit = (*sc)["ObjectDetection"]->get<uint32_t>("sightLimit", NULL);
    }

    std::vector<CellPerceptions> ObjectDetection::createPerceptions(World *world) {
        std::map<Coordinate, Cell *> grid = world->getGrid();
        std::vector<CellPerceptions> cellPerceptionsList;

        std::map<Coordinate, Cell *>::iterator it;
        for (it = grid.begin(); it != grid.end(); it++) {
            CellPerceptions cellPerceptions;

            std::cout << it->first  // string (key)
                      << ':'
                      << it->second   // string's value
                      << std::endl;

            cellPerceptions.x = it->first.x;
            cellPerceptions.y = it->first.y;
            std::vector<Object *> objects = it->second->getObjects();

            for (int i = 0; i < objects.size(); i++) {
                Perception *perception = new Perception();
                Object *object = objects.at(i);
                perception->state = object->getState();
                perception->type = object->getType();
                perception->objectID = object->getID();
                perception->x = cellPerceptions.x;
                perception->y = cellPerceptions.y;
                cellPerceptions.perceptions.push_back(*perception);
            }

            cellPerceptionsList.push_back(cellPerceptions);
        }

        return cellPerceptionsList;
    }

    //Finding nearest points
    double calculateDistance(Coordinate* a, Coordinate* b){
        double x = (b->x - a->x)^2;
        double y = (b->y - a->y)^2;
        double z = x + y;
        return sqrt(z);
    }

    vector<Coordinate*> findNearestPoints(Coordinate* point, vector<Coordinate*> points) {
        vector<Coordinate*> nearestPoints;
        float smallestDistance = FLT_MAX;

        for (Coordinate* p : points) {
            float currentDistance = calculateDistance(p, point);

            if(currentDistance == smallestDistance){
                nearestPoints.emplace_back(p);
            } else if (currentDistance < smallestDistance){
                nearestPoints.empty();
                nearestPoints.emplace_back(p);
                smallestDistance = currentDistance;
            }
        }
        return nearestPoints;
    }

    std::vector<Coordinate*> findPointsToConsider(Coordinate* positionOfRobot) {
        std::vector<Coordinate*> resultPoints;
        float x = positionOfRobot->x;
        float y = positionOfRobot->y;

        bool xMin = positionOfRobot->x == X_MIN_VALUE;
        bool xMax = positionOfRobot->x == X_MAX_VALUE;
        bool yMin = positionOfRobot->y == Y_MIN_VALUE;
        bool yMax = positionOfRobot->y == Y_MAX_VALUE;

        if (!xMin) {
            resultPoints.emplace_back(Coordinate(x-1, y));
            if (!yMin) {
                resultPoints.emplace_back(Coordinate(x, y-1));
                resultPoints.emplace_back(Coordinate(x-1, y-1));
            }

            if (!yMax) {
                resultPoints.emplace_back(Coordinate(x, y+1));
                resultPoints.emplace_back(Coordinate(x-1, y+1));
            }
        }

        if (!xMax) {
            resultPoints.emplace_back(Coordinate(x+1, y));
            if (!yMin) {
                resultPoints.emplace_back(Coordinate(x+1, y-1));
            }

            if (!yMax) {
                resultPoints.emplace_back(Coordinate(x+1, y+1));
            }
        }

        return resultPoints;
    }


    std::vector<Coordinate*> findNearestNeighbours(Coordinate* point) {
        Coordinate* convertedPoint = new Coordinate(round(point->x - 0.5F) + 0.5F, round(point->y - 0.5F) + 0.5F);
        vector<Coordinate*> pointsToConsider = findPointsToConsider(convertedPoint);
        vector<Coordinate*> result = findNearestPoints(point, pointsToConsider);
        return result;
    }

    std::vector<const Cell *> ObjectDetection::collectCells(Coordinate start, Coordinate end, World *world) {
        std::vector<const Cell *> cells;

        std::map<Coordinate, Cell *> grid = world->getGrid();
        const Cell* cell = world->getCell(start);

        // calculate function with start/end point
        Function* function = new Function(start, end);

        // find all transition points between cells
        std::vector<Coordinate*> intersections;

        int x;
        int y;

        if (start.x <= end.x) {
            x = (int) start.x + 1;
            for (int i = x; i <= end.x; i++) {
                float resultY = function->calculateY(i);
                intersections.emplace_back(Coordinate(x, resultY));
                //System.out.println("P(" + i + "|" + resultY + ")");
            }
        } else {
            x = (int) end.x + 1;
            for (int i = x; i <= start.x; i++) {
                float resultY = function->calculateY(i);
                intersections.emplace_back(Coordinate(x, resultY));
                //System.out.println("P(" + i + "|" + resultY + ")");
            }
        }

        if(start.y <= end.y) {
            y = (int) start.y + 1;
            for (int i = y; i <= end.y; i++) {
                float resultX = function->calculateX(i);
                intersections.emplace_back(Coordinate(resultX, i));
                //System.out.println("Q(" + resultX + "|" + i + ")");
            }
        } else {
            y = (int) end.y + 1;
            for (int i = y; i <= start.y; i++) {
                float resultX = function->calculateX(i);
                intersections.emplace_back(Coordinate(resultX, i));
                //System.out.println("Q(" + resultX + "|" + i + ")");
            }
        }

        for (Coordinate* intersection: intersections){
            if(isnan(intersection->x)){
                // Ray is traveling directly upwards or downwards
                intersection->x = start.x;
            }

            std::cout << "Intersection: (" << intersection->x << "," << intersection->y << ")" << std::endl;
            std::vector<Coordinate*> nearestNeighbours = findNearestNeighbours(intersection);
            for (Coordinate* nearestNeighbour : nearestNeighbours) {
                std::cout << "    Nearest Neighbor: (" << (nearestNeighbour->x -0.5F) << "," << (nearestNeighbour->x + 0.5F) << ")";
            }
        }

        return intersections;
    }
} // namespace srgsim
