// -*- mode: C++; coding: utf-8; -*-
#pragma once

#include <vector>
#include "rbSolver.h"
#include "rbTypes.h"

class rbEnvironment
{
public:

    using BodyPtrContainer = std::vector<rbRigidBody*>;
    using ContactContainer = std::vector<rbContact>;

    struct Config
    {
        rbs32 RigidBodyCapacity = 10;
        rbs32 ContactCapacty = 20;
        rbReal NearThreshold = rbReal(0.02);
    };

    rbEnvironment();
    rbEnvironment( const Config& config );
    ~rbEnvironment();


    BodyPtrContainer RigidBodies()
        { return bodies; }

    rbRigidBody* RigidBody( rbu32 index )
        { return bodies.at( index ); }

    size_t RigidBodyCount()
        { return bodies.size(); }

    size_t RigidBodyCapacity()
        { return bodies.capacity(); }


    ContactContainer Contacts()
        { return contacts; }

    rbContact* Contact( rbu32 index )
        { return &contacts.at( index ); }

    size_t ContactCount()
        { return contacts.size(); }

    size_t ContactCapacity()
        { return contacts.capacity(); }

    void ClearContacts()
        { contacts.clear(); }

    bool Register( rbRigidBody* box );
    bool Unregister( rbRigidBody* box );

    void Update( rbReal dtime, int div );

private:

    BodyPtrContainer bodies;
    ContactContainer contacts;
    rbSolver solver;
    Config config;
};

// RigidBox : A Small Library for 3D Rigid Body Physics Tutorial
// Copyright (c) 2011-2020 vaiorabbit <http://twitter.com/vaiorabbit>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//     1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//
//     2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//
//     3. This notice may not be removed or altered from any source
//     distribution.
