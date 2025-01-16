//
// Copyright (c) 1993-2022 Robert McNeel & Associates. All rights reserved.
// OpenNURBS, Rhinoceros, and Rhino3D are registered trademarks of Robert
// McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete openNURBS copyright information see <http://www.opennurbs.org>.
//
////////////////////////////////////////////////////////////////

#if !defined(OPENNURBS_PLUS) || !defined(OPENNURBS_PLUS_INC_) || defined(OPENNURBS_PUBLIC)
#error This file should not be distributed with the public opennurbs source code toolkit.
#endif

#if !defined(OPENNURBS_PLUS_PARTICLE_INC_)
#define OPENNURBS_PLUS_PARTICLE_INC_

#define ON_PARTICLE_SYSTEM_WIP

#if defined(ON_PARTICLE_SYSTEM_WIP)

class ON_CLASS ON_ParticleSystem
{
public:
  ON_ParticleSystem();

  class ON_CLASS ON_ParticleState
  {
  public:

    // state
    double      m_mass;
    double      m_time;
    ON_3dPoint  m_position;
    ON_3dVector m_velocity;
    ON_3dVector m_acceleration; 

    // force accumulator 
    ON_3dVector m_force;

    bool IsSet() const;

    bool IsUnset() const;

    // Unset.m_time = ON_UNSET_VALUE
    static const ON_ParticleSystem::ON_ParticleState Unset;
  };

  class ON_CLASS ON_Particle
  {
  public:

    /*
      The default constructor does not initialize the values of the m_mass,
      m_position m_velocity and m_force.  
      It sets m_particle_id = 0 and m_context = 0;
    */
    ON_Particle() ON_NOEXCEPT;

    /*
    Remarks:
      The copy constructor copies the values of every field including
      m_context and m_particle_id.
    */
    ON_Particle(const ON_Particle& src) ON_NOEXCEPT;

    /*
    Remarks:
      The operator= copies the values of every field except
      m_particle_id, which is left unchanged.
    */
    ON_Particle& operator=(const ON_Particle& src) ON_NOEXCEPT;

    // current mass, position, and whatever else this system cares about
    ON_ParticleState m_current_state;

    // previous mass, position, and whatever else this system cares about
    ON_ParticleState m_previous_state;

    /*
    Returns:
      The distance from the previous position to the current position.
      If either the previous or current positions are unset, then
      ON_UNSET_VALUE is returned.
    */
    double DistanceMoved() const;

    ON__UINT_PTR m_context; // each application can use m_context as it sees fit.

    // 0 != (0x80000000 & m_particle_id) indicates the particle will not be moved
    // by ON_ParticleSystem::IncrementTime().
    const unsigned int m_particle_id;

    bool IsFixed() const;

  private:
    friend class ON_ParticleSystem;
    ON_Particle(
      unsigned int particle_id,
      const class ON_ParticleSystem::ON_ParticleState& initial_state
      ) ON_NOEXCEPT;
  };

  class ON_CLASS ON_ParticleIterator
  {
  public:
    ON_ParticleIterator(ON_ParticleSystem& particle_system);

    const class ON_ParticleSystem::ON_Particle* First();
    const class ON_ParticleSystem::ON_Particle* Next();
    void Reset();
  private:
    ON_FixedSizePoolIterator m_particle_pool_it;
  };

  /*
  Description:
    Add a particle to the system.
  Parameters:
    mass - [in]
    initial_position - [in]
    initial_velocity - [in]
      You may pass zero if you are using an integrator that
      does not need this value.
    initial_acceleration - [in]
      You may pass zero if you are using an integrator that
      does not need this value.
  Remarks:
    The constructors that omit initial_velocity or initial_acceleration
    assume those values are zero
  */
  class ON_ParticleSystem::ON_Particle* AddParticle(
    double mass,
    ON_3dPoint initial_position
    );

  class ON_ParticleSystem::ON_Particle* AddParticle(
    double mass,
    ON_3dPoint initial_position,
    ON_3dVector initial_velocity
    );

  class ON_ParticleSystem::ON_Particle* AddParticle(
    double mass,
    ON_3dPoint initial_position,
    ON_3dVector initial_velocity,
    ON_3dVector initial_acceleration
    );

  class ON_ParticleSystem::ON_Particle* AddFixedParticle(
    double mass,
    ON_3dPoint initial_position
    );



  class ON_CLASS ON_Force
  {
  public:
    ON_Force() ON_NOEXCEPT;
    ~ON_Force() ON_NOEXCEPT;

    /*
    Remarks:
      The copy constructor copies the values of the m_mass, m_position
      m_velocity, m_force and m_context fields and sets m_force_id = 0.
    */
    ON_Force(const ON_Force& src) ON_NOEXCEPT;

    /*
    Remarks:
      The operator= copies the values of the m_mass, m_position
      m_velocity m_force and m_context fields and leaves the value
      of the m_force_id unchanged.
    */
    ON_Force& operator=(const ON_Force& src) ON_NOEXCEPT;

#if defined(ON_HAS_RVALUEREF)
    ON_Force(ON_Force&&) ON_NOEXCEPT;
    ON_Force& operator=(ON_Force&&) ON_NOEXCEPT;
#endif

    // If m_particle_count is zero, then m_particles is null and  
    // m_unary_force_func() points to a unary force evaluation
    // function that acts on every particle in the system.
    // If m_particle_count greater than zero, then m_particles[]
    // is an array of pointers to n particles and m_nary_force_func()
    // points to a N-ary force evaluation function that acts on
    // those particles.
    unsigned int m_particle_count;
    unsigned int m_particle_capacity;
    class ON_ParticleSystem::ON_Particle** m_particles;


    union
    {
      ON__UINT_PTR m_func;
      bool (*m_unary_force_func)(ON__UINT_PTR, class ON_ParticleSystem::ON_Particle*);
      bool (*m_nary_force_func)(ON__UINT_PTR, class ON_ParticleSystem::ON_Particle**);
    } m_force_func;

    struct
    {
      ON__UINT_PTR m_ptr; // first parameter value for force_func()
      double m_x[4];      // buffer for values to minimize need for heap management
    } m_context;

    const unsigned int m_force_id;

  private:
    friend class ON_ParticleSystem;

    ON_Force(
      unsigned int force_id,
      bool (*unary_force_func)(ON__UINT_PTR, class ON_ParticleSystem::ON_Particle*),
      ON__UINT_PTR context
      ) ON_NOEXCEPT;

    ON_Force(
      unsigned int force_id,
      bool (*nary_force_func)(ON__UINT_PTR, class ON_ParticleSystem::ON_Particle**),
      ON__UINT_PTR context,
      unsigned int particle_count,
      class ON_ParticleSystem::ON_Particle** particles
      ) ON_NOEXCEPT;

    void DeleteParticleList();
    void AllocateParticleList( size_t particle_capacity );

    void CopyParticleList(
      unsigned int particle_count,
      class ON_ParticleSystem::ON_Particle** particles
      );
    class ON_ParticleSystem::ON_Particle* m_particle_buffer[4];
  };

  /*
  Description:
    Adds a Hook's law spring force between particles that is proportional
    to (distance - rest_length).
  Parameters:
    particle1 - [in]
    particle2 - [in]
    rest_length - [in]
      rest length of the spring.
    spring_constant - [in]
      Spring force magnitude = spring_constant*(distance - rest_length).
      When spring_constant > 0 and distance > rest_length, the force 
      attracts.
    damping_constant - [in]
      Damping force magnitude = damping_constant*|(delta velocity) o D|,
      where D is a unit vector points from one particle to the other.
      When damping_constant > 0 and the particles are moving towards
      each other, the damping force repels.
      When damping_constant > 0 and the particles are moving away from
      each other, the damping force attracts.
  */
  class ON_ParticleSystem::ON_Force* AddHooksSpringForce(
    class ON_ParticleSystem::ON_Particle* particle1,
    class ON_ParticleSystem::ON_Particle* particle2,
    double rest_length,
    double spring_constant,
    double damping_constant
    );

  /*
  Description:
    Adds binary force between particles that is proportional
    to 1/(distance squared).
  Parameters:
    particle1 - [in]
    particle2 - [in]
    c - [in]
      force = c/d^2 
      c > 0 for attractive forces
      c < 0 for repulsive forces
    minimum_distance - [in]
      If the distance between the particles is < minimum_distance,
      then the force evaluator fails.
  Example:
    Gravity:
      double g = 6.672e-11; // N m^2/kg^2
      double c = g*mass1*mass2;
      double min_distance = (radius1+radius2);
      AddInverseDistanceSquaredForce(p1,p2,c,min_distance);
    Coulumb's Force
      double k = 8.9875e9; // N m^2/C^2
      double c = -k*charge1*charge2;
      double min_distance = (radius1+radius2);
      AddInverseDistanceSquaredForce(p1,p2,c,min_distance);      
  */
  class ON_ParticleSystem::ON_Force* AddInverseDistanceSquaredForce(
    class ON_ParticleSystem::ON_Particle* particle1,
    class ON_ParticleSystem::ON_Particle* particle2,
    double c,
    double minimum_distance
    );

  /*
  Description:
    A unary force acts on every particle in the system.

  Parameters:
    context - [in]
      First parameter passed to force_function.

    force_function - [in]
      A function that calculates the force on the particle.
      If force_function() returns false, the solver will
      stop.

  Example:
    If you had 8, or perhaps 9, particles representing the planets in our
    solar system, you were comfortable neglecting the effects the planets'
    mass have on the sun's position and you chose a coordinate system
    with the sun at the origin, then you could add a solar gravity force 
    doing something like the following.

        bool GetSolarGravity( 
          void*, // no context required
          const class ON_ParticleSystem::ON_Particle* planet, 
          ON_3dVector* SolarGravityForce
          )
        {
          // masses in kilograms and distances in meters
          const double solar_mass = 1.9891e30; // kilograms
          const double g = 6.67384e-11; // meters^3/(kilogram seconds^2)
          ON_3dVector V = ON_3dPoint::Origin - planet->m_position;
          double r = V.Length();  
          if ( !(r > 6.955e8) )
          {
            // Planet crashed into the sun!
            // The sun's radius is 6.955e8 meters. This calculation 
            // ignores planets melting as they get too close to the sun.
            return false;
          }
          *SolarGravityForce = (g*planet->m_mass*solar_mass/(r*r*r))*V; // r^3 because input V is not unitized
          return true;
        }

        ....
        AddUnaryForce(0,GetSolarGravity);

  Remarks:
    To add a global "gravity", it is more efficient to use SetGlobalGravityForce().
    To add a global "drag" force, it is more efficient to use SetGlobalDragForce().
  */
  class ON_ParticleSystem::ON_Force* AddUnaryForce(
    bool (*force_function)(ON__UINT_PTR, class ON_ParticleSystem::ON_Particle*),
    ON__UINT_PTR context
    );

  /*
  Description:
    A N-ary force acts on a fixed set of particles.

  Parameters:
    force_function - [in]
      A function that calculates the force on the particles
      listed in the particles[] array parameter.
      If force_function() returns false, the solver will
      stop.

    context - [in]
      First parameter passed to force_function.  
      If the information in your context consists of up to 4 doubles,
      then you may use the ON_Force.m_context member as shown in 
      the example.

    particle_count - [in]
      Number of particles in particles[] array.

    particles[] - [in]
      List of particles this force acts on.

  Example:
    // Hook's law spring 
    // This example if for illustration purposes.
    // In practice it is more efficient to call ON_ParticleSystem::AddHooksSpringForce().

          struct HooksSpringForceContext
          {
            double m_rest_length;
            double m_spring_constant;
            double m_damping_constant;
          };

          static bool HooksSpringForceFunction(
            ON__UINT_PTR context,
            class ON_ParticleSystem::ON_Particle** particles
            )
          {
            const ON_3dVector dx = particles[1]->m_position - particles[0]->m_position;
            const double length = dx.Length();
            if ( length > 0.0 )
            {
              const ON_3dVector dv = particles[1]->m_velocity - particles[0]->m_velocity;
              const struct HooksSpringForceContext* spring_context = (const struct HooksSpringForceContext*)context;
              double f = spring_context->m_spring_constant*(1.0 - spring_context->m_rest_length/length)
                        + spring_context->m_damping_constant*((dv.x*dx.x + dv.y*dx.y + dv.z*dx.z)/(length*length));
              ON_3dVector F = f*dx;
              particles[0]->m_force += F;
              particles[1]->m_force -= F;
            }
            return true;
          }

          class ON_ParticleSystem::ON_Particle* particle1 = ...;
          class ON_ParticleSystem::ON_Particle* particle2 = ...;
          double rest_length = ...;
          double spring_constant = ...;
          double damping_constant  = ...;
          ON_ParticleSystem::ON_Particle* particles[2] = {particle1,particle2};
          ON_Force* F = AddNaryForce(HooksSpringForceFunction,0,2,particles);
          if ( 0 != F )
          {
            F->m_context.m_ptr = (ON__UINT_PTR)(&F->m_context.m_x[0]);
            struct HooksSpringForceContext* spring_context = (struct HooksSpringForceContext* )F->m_context.m_ptr;
            spring_context->m_rest_length = rest_length;
            spring_context->m_spring_constant = spring_constant;
            spring_context->m_damping_constant = damping_constant;
          }

  Remarks:
    To add the common damped spring force that is calculated using Hook's law, it is
    easier to call AddHooksSpringForce().
  */
  class ON_ParticleSystem::ON_Force* AddNaryForce(
    bool (*force_function)(ON__UINT_PTR, class ON_ParticleSystem::ON_Particle**),
    ON__UINT_PTR context,
    unsigned int particle_count,
    class ON_Particle** particles
    );

  /*
  Description:
    Applies a force of ON_Particle.m_mass*G to every particle.
  Parameters:
    G - [in]
      Vector acceleration of gravity.
  */
  void SetGlobalGravityForce(
    ON_3dVector G
    );

  ON_3dVector GlobalGravityForce() const;

  /*
  Description:
    Applies a cubic drag force of 
      -(drag_coefficient1 + drag_coefficient2*|v| + drag_coefficient3*|v|^2)*v, 
    where "v" is the particle's velocity.
  Parameters:
    drag_coefficient1 - [in]
    drag_coefficient2 - [in]
    drag_coefficient3 - [in]
  */
  void SetGlobalDragForce(
    double drag_coefficient1,
    double drag_coefficient2,
    double drag_coefficient3
    );

  double GlobalDragForceCoefficient(
    int i
    ) const;

  bool AccumulateForces();

  /*
  Description:
    Sets the starting time.
  Parameters:
    start_time - [in]
  Remarks:
    The default starting time is zero.
    Once time has been incremented, the starting
    time cannot be set.
  */
  bool SetStartTime(double start_time);

  /*
  Returns:
    Current particle system time.
  */
  double CurrentTime() const;

  /*
  Returns:
    Particle system time at the previous iteration.
    ON_UNSET_VALUE is returned if time has never been
    incremented.
  */
  double PreviousTime() const;

  class ON_CLASS ON_IntegrateContext
  {
  public:
    ON_IntegrateContext(
      const class ON_ParticleSystem& particle_system,
      double delta_time
      ) ON_NOEXCEPT;

    ON_IntegrateContext(
      const ON_IntegrateContext&
      ) ON_NOEXCEPT;

    // Input to particle system integration functions.
    const class ON_ParticleSystem& m_particle_system;
    const class ON_ParticleSystem::ON_Particle* m_particle;

    const double m_prev_time;
    const double m_current_time;
    const double m_destination_time; // = m_current_time + m_delta_time

    const double m_delta_time;
    const double m_delta_time_squared;
    const double m_half_delta_time_squared;

    ON__UINT_PTR m_integrate_context;

    // Integration functions receive m_state = m_particle->m_current_state
    // and update position and any other values they care to modify.
    ON_ParticleSystem::ON_ParticleState* m_state;

  private:
    // prohibit use - no implementation
    const ON_IntegrateContext& operator=(const ON_IntegrateContext&);
  };

  /*
  Description:
    Increment the current state of the particle system using 
    the Euler integration algorithm.
  Parameters:
    delta_time - [in]
  Remarks:
    Calls IncrementTime with ON_ParticleSystem::IntegrateEuler
    as the integration function.
  */  
  bool IncrementTimeEuler(
    double delta_time
    );

  /*
  Description:
    Increment the current state of the particle system using 
    the semi-implicitly Euler integration algorithm.
  Parameters:
    delta_time - [in]
  Remarks:
    Calls IncrementTime with ON_ParticleSystem::IntegrateSemiImplicitEuler
    as the integration function.
  */  
  bool IncrementTimeSemiImplicitEuler(
    bool (*integrate_func)(class ON_ParticleSystem::ON_IntegrateContext&),
    ON__UINT_PTR integrate_context,
    double delta_time
    );

  /*
  Description:
    Increment the current state of the particle system using 
    the Verlet integration algorithm.
  Parameters:
    delta_time - [in]
  Remarks:
    Calls IncrementTime with ON_ParticleSystem::IntegrateVerlet
    as the integration function. Note that the Verlet algorithm
    does not calculate velocity values.
  */  
  bool IncrementTimeVerlet(
    bool (*integrate_func)(class ON_ParticleSystem::ON_IntegrateContext&),
    ON__UINT_PTR integrate_context,
    double delta_time
    );

  /*
  Description:
    Increment the current state of the particle system using 
    the velocity Verlet integration algorithm.
  Parameters:
    delta_time - [in]
  Remarks:
    Calls IncrementTime with ON_ParticleSystem::IntegrateVerlet
    as the integration function. Note that the Verlet algorithm
    does not calculate velocity values.
  */  
  bool IncrementTimeVelocityVeret(
    bool (*integrate_func)(class ON_ParticleSystem::ON_IntegrateContext&),
    ON__UINT_PTR integrate_context,
    double delta_time
    );

  /*
  Description:
    Increment the current state of the particle system using a custom
    integration algorithm.
  Parameters:
    integrate_func - [in]
      An integration function.  
      You may use one of the canned methods listed below or provide your own.
        ON_ParticleSystem::IntegrateEuler()
        ON_ParticleSystem::IntegrateSemiImplicitEuler()
        ...

    integrate_context - [in]
      value passed as context.m_integration_context.
    delta_time - [in]
      value passed as contextm_delta_time.
  */
  bool IncrementTime(
    bool (*integrate_func)(class ON_ParticleSystem::ON_IntegrateContext&),
    ON__UINT_PTR integrate_context,
    double delta_time
    );

  /*
  Returns:
    A pointer to the particle that moved the farthest during the
    most recent call to IncrementTime(). This information is 
    useful for adjusting time increments.
  */
  const class ON_ParticleSystem::ON_Particle* MaximumIncrementalChange() const;

  /*
  Description:
    Updates context.m_state.m_position and context.m_state.m_velocity
    using Euler integration.

  Parameters:
    context - [in / out]
      The returned position and velocity are incremented
      using the following calculation.
        x1 = current position
        v1 = current velocity
        a1 = (current accumulated force)/(current mass)
        // incremented position and velocity
        x = x1 + v1*dt + 0.5*a1*dt*dt;
        v = v1 + a1*dt;
        
  Remarks:
    This is the best choice in the rare case when all forces are
    independent of time. The canonical example when being when
    the only force is the force of gravity and no collisions occur.
    Note that forces whose formulae involve position or velocity are 
    almost never independent of time.  In particular, if your system
    has drag, damping or spring forces, then this is probably
    the worst choice for an integrator.
  */
  static bool IntegrateEuler(
    class ON_ParticleSystem::ON_IntegrateContext& context
    );

  /*
  Description:
    Updates context.m_state.m_position and context.m_state.m_velocity
    using Semi-implicit Euler integration.

  Parameters:
    context - [in / out]
      The returned position and velocity are incremented
      using the following calculation.
        x1 = current position
        v1 = current velocity
        a1 = (current accumulated force)/(current mass)
        // incremented position and velocity
        v = v1 + a1*dt;
        x = x1 + v*dt + 0.5*a1*dt*dt;        
  */
  static bool IntegrateSemiImplicitEuler(
    class ON_ParticleSystem::ON_IntegrateContext& context
    );

  /*
  Description:
    Updates context.m_state.m_position using Verlet integration.

  Parameters:
    context - [in / out]
      The returned position is incremented
      using the following calculation.
        x0 = previous position
        x1 = current position
        a1 = (current accumulated force)/(current mass)
        // incremented position
        x = 2*x1 - x0 + a1*dt*dt;
  Remarks:
    - The Verlet algorithm cannot be used when the force 
      accumulation step depends on velocities.   If you 
      have damped springs or forces like global drag, you
      must use some other integration algorithm.
    - Because the calculation requires a valid previous state,
      the initial iteration must be performed by another
      integrator. If context.m_particle->m_previous_state.m_time
      is ON_UNSET_VALUE, then ON_ParticleSystem::IntegrateEuler 
      is called to get the starting values.
  */
  static bool IntegrateVerlet(
    class ON_ParticleSystem::ON_IntegrateContext& context
    );

  /*
  Description:
    Updates context.m_state.m_position and context.m_state.m_velocity
    using Stormer-Verlet integration.

  Parameters:
    context - [in / out]
      The returned position is incremented
      using the following calculation.
        x1 = current position
        v1 = current velocity
        a1 = (current accumulated force)/(current mass)
        // incremented position
        v = v1 + 0.5* a1*dt;
        x = x1 + v*dt;
  Remarks:
    The velocity Verlet algorithm uses IntegrateVelocityVerlet1
    as the first stemBecause the calculation requires a valid previous state,
    the initial iteration must be performed by another
    integrator. If context.m_particle->m_previous_state.m_time
    is ON_UNSET_VALUE, then IntegrateEuler is called.
  */
  static bool IntegrateVelocityVerlet1(
    class ON_ParticleSystem::ON_IntegrateContext& context
    );

  ON_BoundingBox BoundingBox() const;

private:
  class ON_ParticleEx : public ON_Particle
  {
  public:
    ON_ParticleEx(
      unsigned int particle_id,
      const ON_ParticleState& initial_state
      );

    // bucket information
    class ON_ParticleEx* m_bucket_prev;
    class ON_ParticleEx* m_bucket_next;
    class ON_ParticleBucket* m_bucket;
  };

  class ON_ParticleBucket
  {
  public:
    // bounding box for this bucket
    ON_BoundingBox m_bbox;

    // The particles in this bucket are stored as a linked list
    // using ON_ParticleEx.m_bucket_prev and ON_ParticleEx.m_bucket_next
    // to traverse the list.
    class ON_ParticleEx* m_particle_list;
  };

  unsigned int m_particle_id_source;
  unsigned int m_force_id_source;

  // built-in unary forces
  // gravitational force = m*G, where m = particle's mass
  ON_3dVector m_global_gravity; // gravitational force = mass*m_global_gravity

  // drag force = -(k[0] + k[1]*|v| + k[2]*|v|^2)*v, where v = particle's velocity
  double m_global_drag[3];

  ON_FixedSizePool m_particle_pool;
  ON_FixedSizePool m_unary_force_pool;
  ON_FixedSizePool m_nary_force_pool;
  ON_FixedSizePool m_bucket_pool;
  ON_RTree m_bucket_tree;
  ON_FixedSizePool m_state_pool;

  ON_BoundingBox m_bbox;
  ON_BoundingBox m_prev_bbox;

  double m_time;
  double m_prev_time;

  // Each call to ON_ParticleSystem::IncrementTime() updates
  // m_maximum_increment to point at the particle that moved
  // the largest distance.
  const ON_Particle* m_maximum_incremental_change;
};



#endif

#endif
