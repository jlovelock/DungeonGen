
#ifndef DPG_H
#define DPG_H

#include <dungeon.h>

class DoorPlacementGuide {

    /*
     * Map between each wall (represented as an int-direction, eg NORTH) and its associated vector of pairs.
     * Each pair represents a continuous linear region -- (first, second) -- where doors can be placed.
     * A vector of such pairs is necessary to represent a discontinuous region.
     * For NORTH and SOUTH walls, the pairs are x-coordinates; for EAST and WEST, they are y-coordinates.
     */
    std::map< int, std::vector < std::pair<int,int> > >  preferred;
    std::map< int, std::vector < std::pair<int,int> > >  allowed;

    Room* r;
    void forbid(int wall, int LB, int UB);
    void remove_region(std::vector< std::pair<int, int> >& v, int LB, int UB);

    void downgrade(int wall, int position);

    int roll_wall();
    void spawn_door();
    void add_door_at(int wall, int x, int y);

    bool valid_position_exists();

public:
    DoorPlacementGuide(Room* rm, Dungeon* d);
    void spawn_doors();
};

#endif // DPG_H
