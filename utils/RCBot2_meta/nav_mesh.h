/*
 *    This file is part of RCBot.
 *
 *    RCBot by Paul Murphy adapted from Botman's HPB Bot 2 template.
 *
 *    RCBot is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    RCBot is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RCBot; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */
 
#ifndef NAV_MESH_H
#define NAV_MESH_H

#include <vector>
#include "vector.h" // Replace "vector3.h" with your actual vector math library/header

// Define your NavMeshNode structure to represent a node in the Nav Mesh
struct NavMeshNode {
    int id;
    Vector position;
    std::vector<int> neighbors;
};

// Define your NavMesh class to handle Nav Mesh functionality
class NavMesh {
public:
    NavMesh();
    ~NavMesh();

    // Add a new node to the Nav Mesh
    void AddNode(int id, const Vector& position);

    // Connect two nodes in the Nav Mesh (Create an edge between them)
    void ConnectNodes(int nodeID1, int nodeID2);

    // Calculate the route between two nodes using pathfinding algorithm (e.g., A* algorithm)
    void CalculateRoute(Vector startNodeID, Vector goalNodeID);

    // Get the next point in the calculated route
    Vector GetNextRoutePoint();

    // Update the bot's position during navigation
    void botPosition(const Vector& botPosition);

    // Check if a route has been successfully calculated
    bool IsRouteFound();

    // Check if there are more points in the calculated route
    bool HasNextRoutePoint();

    // Reset the Nav Mesh data
    void Reset();

    // Free memory related to Nav Mesh map data
    void FreeMapMemory();

    // Free all allocated memory related to Nav Mesh
    void FreeAllMemory();
	
    void Init();

private:
    // Helper functions for pathfinding algorithms
    // Implement your pathfinding algorithm (e.g., A*) using these functions

    // ... Add your pathfinding algorithm implementation here ...

    // Data members to store Nav Mesh nodes and other information
    std::vector<NavMeshNode> nodes;
    int startNodeID; // ID of the starting node for the current route
    int goalNodeID; // ID of the goal node for the current route
    bool routeFound; // Flag indicating whether a route has been found
    std::vector<Vector> calculatedRoute; // The calculated route points

    // ... Add any other data members you need ...
};

#endif // NAV_MESH_H
