#include "srgsim/world/ObjectDetection.h"
#include "srgsim/SRGEnums.h"
#include "srgsim/containers/FloatCoordinate.h"
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
    double calculateDistance(FloatCoordinate a, FloatCoordinate b){
        double x = (b.x - a.x)*(b.x - a.x);
        double y = (b.y - a.y)*(b.y - a.y);
        double z = x + y;
        return sqrt(z);
    }

    vector<FloatCoordinate> findNearestPoints(FloatCoordinate intersection, vector<FloatCoordinate> pointsToConsider) {
        vector<FloatCoordinate> nearestPoints;
        float smallestDistance = FLT_MAX;

        for (FloatCoordinate p : pointsToConsider) {
            float currentDistance = calculateDistance(p, intersection);

            if(currentDistance == smallestDistance){
                nearestPoints.emplace_back(p);
            } else if (currentDistance < smallestDistance){
                nearestPoints.clear();
                nearestPoints.emplace_back(p);
                smallestDistance = currentDistance;
            }
        }
        return nearestPoints;
    }

    std::vector<FloatCoordinate> findPointsToConsider(float x, float y) {
        std::vector<FloatCoordinate> resultPoints;

        bool xMin = x == X_MIN_VALUE;
        bool xMax = x == X_MAX_VALUE;
        bool yMin = y == Y_MIN_VALUE;
        bool yMax = y == Y_MAX_VALUE;

        if (!xMin) {
            resultPoints.emplace_back(FloatCoordinate(x-1,y));
            if (!yMin) {
                resultPoints.emplace_back(FloatCoordinate(x, y-1));
                resultPoints.emplace_back(FloatCoordinate(x-1, y-1));
            }

            if (!yMax) {
                resultPoints.emplace_back(FloatCoordinate(x, y+1));
                resultPoints.emplace_back(FloatCoordinate(x-1, y+1));
            }
        }

        if (!xMax) {
            resultPoints.emplace_back(FloatCoordinate(x+1, y));
            if (!yMin) {
                resultPoints.emplace_back(FloatCoordinate(x+1, y-1));
            }

            if (!yMax) {
                resultPoints.emplace_back(FloatCoordinate(x+1, y+1));
            }
        }

        return resultPoints;
    }


    std::vector<Coordinate> findNearestNeighbours(FloatCoordinate positionOfRobot, FloatCoordinate intersection) {
        vector<FloatCoordinate> pointsToConsider =
                findPointsToConsider(round(positionOfRobot.x - 0.5F) + 0.5F, round(positionOfRobot.y - 0.5F) + 0.5F);
        vector<FloatCoordinate> floatResult = findNearestPoints(intersection, pointsToConsider);
        vector<Coordinate> result;

        for(FloatCoordinate coordinate: floatResult){
            result.push_back(Coordinate(coordinate.x, coordinate.y));
        }

        return result;
    }

    std::vector<const Cell*> ObjectDetection::collectCells(Coordinate start, Coordinate end, World *world) {
        std::vector<const Cell*> cells;

        std::map<Coordinate, Cell *> grid = world->getGrid();
        const Cell* cell = world->getCell(start);

        // calculate function with start/end point
        Function* function = new Function(start, end);

        // find all transition points between cells
        std::set<FloatCoordinate> intersections;

        // TODO m = inf

        if (start.x <= end.x) {
            for (int i = start.x+1; i <= end.x; i++) {
                float resultY = function->calculateY(i);
                intersections.insert(FloatCoordinate((float) i, resultY));
                //System.out.println("P(" + i + "|" + resultY + ")");
            }
        } else {
            for (int i = start.x-1; i >= end.x; i--) {
                float resultY = function->calculateY(i);
                intersections.insert(FloatCoordinate((float) i, resultY));
                //System.out.println("P(" + i + "|" + resultY + ")");
            }
        }

        if(start.y <= end.y) {
            for (int i = start.y+1; i <= end.y; i++) {
                float resultX = function->calculateX(i);
                intersections.insert(FloatCoordinate(resultX, (float) i));
                //System.out.println("Q(" + resultX + "|" + i + ")");
            }
        } else {
            for (int i = start.y-1; i >= end.y; i--) {
                float resultX = function->calculateX(i);
                intersections.insert(FloatCoordinate(resultX, (float) i));
                //System.out.println("Q(" + resultX + "|" + i + ")");
            }
        }

        //TODO: Sort intersections

        vector<const Cell*> result;
        Coordinate currentCell = start;

        for (FloatCoordinate intersection: intersections){
            if(isinf(intersection.x)){
                // Ray is traveling directly upwards or downwards
                intersection.x = start.x;
            }

            FloatCoordinate currentCellFloat = FloatCoordinate(currentCell.x +0.5F, currentCell.y +0.5F);

            std::cout << "Intersection: (" << intersection.x << "," << intersection.y << ")" << std::endl;
            std::vector<Coordinate> nearestNeighbours = findNearestNeighbours(currentCellFloat, intersection);

            if(nearestNeighbours.size() == 1){
                Coordinate nearestNeighbour = nearestNeighbours.at(0);

                std::cout << "      Nearest Neighbor: (" << nearestNeighbour.x << "," << nearestNeighbour.y << ")" << std::endl;

                result.push_back(world->getCell(nearestNeighbour));

                currentCell = nearestNeighbour;
            }

            if (nearestNeighbours.size() == 3){
                // Diagonal Direction

                for (Coordinate nearestNeighbour: nearestNeighbours){
                    if(nearestNeighbour.x != currentCell.x && nearestNeighbour.y != currentCell.y){
                        std::cout << "      Nearest Neighbor: (" << nearestNeighbour.x << "," << nearestNeighbour.y << ")" << std::endl;
                        currentCell = nearestNeighbour;
                    }
                }
            }

        }

        return result;
    }
} // namespace srgsim
