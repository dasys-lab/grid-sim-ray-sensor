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
        std::map<Coordinate, Cell*> grid = world->getGrid();
        std::vector<CellPerceptions> cellPerceptionsList;

        std::map<Coordinate, Cell *>::iterator it = grid.begin();
        while(it != grid.end()) {
            CellPerceptions cellPerceptions;

            cellPerceptions.x = it->first.x;
            cellPerceptions.y = it->first.y;
            std::vector<Object *> objects = it->second->getObjects();
            Perception *perception = new Perception();

            if(objects.empty()){
                it++;
                continue;
            } else {
                Object *object = objects.at(0);
                perception->state = object->getState();
                perception->type = object->getType();
                perception->objectID = object->getID();
            }

            perception->x = cellPerceptions.x;
            perception->y = cellPerceptions.y;
            cellPerceptions.perceptions.push_back(*perception);

            cellPerceptionsList.push_back(cellPerceptions);
            it++;
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
        vector<FloatCoordinate> pointsToConsider = findPointsToConsider(positionOfRobot.x, positionOfRobot.y);
        vector<FloatCoordinate> floatResult = findNearestPoints(intersection, pointsToConsider);
        vector<Coordinate> result;

        for(FloatCoordinate coordinate: floatResult){
            result.push_back(Coordinate(coordinate.x, coordinate.y));
        }

        return result;
    }

    std::vector<const Cell*> ObjectDetection::collectCells(Coordinate start, Coordinate end, World *world) {
        FloatCoordinate startFloat = FloatCoordinate(start.x +0.5F, start.y +0.5F);
        FloatCoordinate endFloat = FloatCoordinate(end.x +0.5F, end.y +0.5F);
        std::vector<const Cell*> cells;

        // calculate function with start/end point
        Function* function = new Function(startFloat, endFloat);

        // find all transition points between cells
        std::vector<FloatCoordinate> intersections;

        // m = 1 means that our function is diagonal, so we must not cover both axis.
        bool isDiagonal = function->m == 1 || function->m == -1;

        if (start.x <= end.x) {
            for (int i = start.x+1; i <= end.x; i++) {
                float resultY = function->calculateY(i);
                intersections.emplace_back(FloatCoordinate((float) i, resultY));
                //System.out.println("P(" + i + "|" + resultY + ")");
            }
        } else {
            for (int i = start.x; i >= end.x+1; i--) {
                float resultY = function->calculateY(i);
                intersections.emplace_back(FloatCoordinate((float) i, resultY));
                //System.out.println("P(" + i + "|" + resultY + ")");
            }
        }

        if(!isDiagonal){
            if(start.y <= end.y) {
                for (int i = start.y+1; i <= end.y; i++) {
                    float resultX = function->calculateX(i);
                    if(isinf(resultX)){
                        resultX = startFloat.x;
                    }
                    intersections.emplace_back(FloatCoordinate(resultX, (float) i));
                    //System.out.println("Q(" + resultX + "|" + i + ")");
                }
            } else {
                for (int i = start.y; i >= end.y+1; i--) {
                    float resultX = function->calculateX(i);
                    if(isinf(resultX)){
                        resultX = startFloat.x;
                    }
                    intersections.emplace_back(FloatCoordinate(resultX, (float) i));
                    //System.out.println("Q(" + resultX + "|" + i + ")");
                }
            }
        }

        if(start.x < end.x){
            std::sort(intersections.begin(), intersections.end());
        } else if (start.x > end.x){
            std::sort(intersections.rbegin(), intersections.rend());
        }

        vector<const Cell*> result;
        FloatCoordinate currentCell = startFloat;

        for (FloatCoordinate intersection: intersections){

            //std::cout << "Intersection: (" << intersection.x << "," << intersection.y << ")" << std::endl;
            std::vector<Coordinate> nearestNeighbours = findNearestNeighbours(currentCell, intersection);

            if(nearestNeighbours.size() == 1){
                Coordinate nearestNeighbour = nearestNeighbours.at(0);
                //std::cout << "      Nearest Neighbor: (" << nearestNeighbour.x << "," << nearestNeighbour.y << ")" << std::endl;
                result.push_back(world->getCell(nearestNeighbour));
                currentCell = FloatCoordinate(nearestNeighbour.x +0.5F, nearestNeighbour.y +0.5F);
            }

            if (nearestNeighbours.size() == 3){
                // Diagonal Direction

                for (Coordinate nearestNeighbour: nearestNeighbours){
                    if(nearestNeighbour.x != (int) currentCell.x && nearestNeighbour.y != (int) currentCell.y){
                        //std::cout << "      Nearest Neighbor: (" << nearestNeighbour.x << "," << nearestNeighbour.y << ")" << std::endl;
                        result.push_back(world->getCell(nearestNeighbour));
                        currentCell = FloatCoordinate(nearestNeighbour.x +0.5F, nearestNeighbour.y +0.5F);
                    }
                }
            }

        }

        return result;
    }

    Coordinate ObjectDetection::getRayEndpoint(float angle, Coordinate start) {
        float radians = (fmod(angle, 360)) * M_PI / 180;

        float vec0 = cos(radians);
        float vec1 = sin(radians);

        float magnitude = sqrt(vec0 * vec0 + vec1 * vec1);

        vec0 = vec0 * magnitude * sightLimit;
        vec1 = vec1 * magnitude * sightLimit;

        return Coordinate(start.x + vec0, start.y + vec1);
    }
} // namespace srgsim
